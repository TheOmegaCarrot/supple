#include <array>
#include <cstddef>

#include "supl/utility.hpp"

#include "supl/test_results.hpp"
#include "supl/test_runner.hpp"
#include "supl/test_section.hpp"

namespace detail
{

enum class Logger_Type : char
{
    Small,
    Medium,
    Large
};

class Logging_Base
{
public:

    Logging_Base()                               = default;
    Logging_Base(const Logging_Base&)            = delete;
    Logging_Base(Logging_Base&&)                 = delete;
    Logging_Base& operator=(const Logging_Base&) = delete;
    Logging_Base& operator=(Logging_Base&&)      = delete;
    virtual ~Logging_Base()                      = default;

    virtual Logger_Type kind() const noexcept               = 0;
    virtual std::size_t get_instance_count() const noexcept = 0;
};

class Small_Logger : public Logging_Base
{
public:

    static inline std::size_t instance_count = 0;

    Small_Logger()
    {
        ++instance_count;
    }

    Small_Logger(const Small_Logger&)            = delete;
    Small_Logger(Small_Logger&&)                 = delete;
    Small_Logger& operator=(const Small_Logger&) = delete;
    Small_Logger& operator=(Small_Logger&&)      = delete;

    virtual ~Small_Logger() override
    {
        --instance_count;
    }

    virtual Logger_Type kind() const noexcept override
    {
        return Logger_Type::Small;
    }

    virtual std::size_t get_instance_count() const noexcept override
    {
        return instance_count;
    }
};

class Medium_Logger : public Logging_Base
{
public:

    static inline std::size_t instance_count = 0;

    // filler so that it may fit in some small buffers
    [[maybe_unused]] std::array<char, 16> filler {};

    Medium_Logger()
    {
        ++instance_count;
    }

    Medium_Logger(const Medium_Logger&)            = delete;
    Medium_Logger(Medium_Logger&&)                 = delete;
    Medium_Logger& operator=(const Medium_Logger&) = delete;
    Medium_Logger& operator=(Medium_Logger&&)      = delete;

    virtual ~Medium_Logger() override
    {
        --instance_count;
    }

    virtual Logger_Type kind() const noexcept override
    {
        return Logger_Type::Medium;
    }

    virtual std::size_t get_instance_count() const noexcept override
    {
        return instance_count;
    }
};

class Large_Logger : public Logging_Base
{
private:

    // filler so that it won't fit in small buffer
    [[maybe_unused]] std::array<char, 256> filler {};

public:

    static inline std::size_t instance_count = 0;

    Large_Logger()
    {
        ++instance_count;
    }

    Large_Logger(const Large_Logger&)            = delete;
    Large_Logger(Large_Logger&&)                 = delete;
    Large_Logger& operator=(const Large_Logger&) = delete;
    Large_Logger& operator=(Large_Logger&&)      = delete;

    virtual ~Large_Logger() override
    {
        --instance_count;
    }

    virtual Logger_Type kind() const noexcept override
    {
        return Logger_Type::Large;
    }

    virtual std::size_t get_instance_count() const noexcept override
    {
        return instance_count;
    }
};

}  // namespace detail

template <typename T, std::size_t Size, template <typename> typename ALLOC>
[[nodiscard]] auto object_in_small_buffer(
  const supl::polymorphic_storage<T, Size, ALLOC>& polymorphic_storage
) noexcept -> bool
{
    using storage_t = supl::polymorphic_storage<T, Size, ALLOC>;

    // returned ptr is within the object
    // yes this is a horrible hack
    return (reinterpret_cast<const std::byte*>(polymorphic_storage.ptr())
            - reinterpret_cast<const std::byte*>(&polymorphic_storage))
           <= static_cast<std::ptrdiff_t>(sizeof(storage_t));
}

auto test_small_buffer_small_type() -> supl::test_results
{
    supl::test_results results;

    using namespace detail;

    supl::polymorphic_storage<Logging_Base, 16> storage;

    storage.construct<Small_Logger>();

    results.enforce_equal(storage.ptr(), nullptr, "should be null");

    results.enforce_true(
      object_in_small_buffer(storage), "not in small buffer"
    );

    results.enforce_equal(
      storage.ptr()->get_instance_count(), 1, "wrong instance count"
    );
    results.enforce_equal(
      storage.ptr()->kind(), Logger_Type::Small, "wrong instance count"
    );

    storage.construct<Small_Logger>();

    results.enforce_equal(
      storage.ptr()->get_instance_count(), 1, "wrong instance count"
    );
    results.enforce_equal(
      storage.ptr()->kind(), Logger_Type::Small, "wrong instance count"
    );

    storage.destruct();

    results.enforce_equal(storage.ptr(), nullptr, "should be null");

    return results;
}

auto test_polymorphic_storage() -> supl::test_section
{
    supl::test_section section;

    section.add_test(
      "small buffer small type", &test_small_buffer_small_type
    );

    return section;
}

auto main() -> int
{
    supl::test_runner runner;

    runner.add_section(test_polymorphic_storage());

    return runner.run();
}
