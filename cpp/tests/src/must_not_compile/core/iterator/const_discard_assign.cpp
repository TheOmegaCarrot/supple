#include <vector>

#include "supl/iterators.hpp"

auto main() -> int
{
    const std::vector<int> cvec {1, 2, 3, 4};
    supl::iterator<int> itr {};

    // bad assignment: would discard const
    itr = cvec.begin();
}
