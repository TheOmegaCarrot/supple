supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/all_of.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/any_of.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/chunk.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/count_if.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/for_each_both.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/for_each_in_subtuple.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/for_each_reverse.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/make_cref_tuple.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/none_of.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/resolve_refs.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/reverse.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/rotate_left.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/rotate_right.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/test_for_each.cpp)
supple_add_test(${CMAKE_CURRENT_SOURCE_DIR}/zip.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/alternating_split)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/back)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/back_n)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/convert)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/elem_swap)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/erase)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/front)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/front_n)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/insert)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/interleave)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pop_back)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pop_front)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/push_back)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/push_back_as)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/push_front)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/push_front_as)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/reorder)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/replace)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/split)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/subtuple)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/transform)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/type_transform)
