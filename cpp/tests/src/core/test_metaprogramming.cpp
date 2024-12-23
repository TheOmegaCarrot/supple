#include <array>
#include <cstdint>
#include <forward_list>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <type_traits>
#include <vector>

#include "supl/metaprogramming.hpp"
#include "supl/type_list.hpp"

// all tests happen at compile time anyway
// if this TU compiles, all tests pass

///////////////////////////////////////////// make_reverse_index_sequence

// {{{
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<75>,
              std::index_sequence<
                75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61,
                60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,
                45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
                30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
                15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<74>,
              std::index_sequence<
                74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60,
                59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,
                44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,
                14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<73>,
              std::index_sequence<
                73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59,
                58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44,
                43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,
                28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<72>,
              std::index_sequence<
                72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58,
                57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,
                42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,
                27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<71>,
              std::index_sequence<
                71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57,
                56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,
                41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,
                26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<70>,
              std::index_sequence<
                70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56,
                55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
                40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,
                25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
                10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<69>,
              std::index_sequence<
                69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55,
                54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,
                39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
                9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<68>,
              std::index_sequence<
                68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54,
                53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39,
                38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,
                23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
                8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<67>,
              std::index_sequence<
                67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53,
                52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38,
                37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
                22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
                7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<66>,
              std::index_sequence<
                66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52,
                51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37,
                36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,
                21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
                5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<65>,
              std::index_sequence<
                65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
                50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36,
                35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
                20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<64>,
              std::index_sequence<
                64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50,
                49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35,
                34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
                3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<63>,
              std::index_sequence<
                63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,
                48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34,
                33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
                18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<62>,
              std::index_sequence<
                62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48,
                47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,
                32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18,
                17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<61>,
              std::index_sequence<
                61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47,
                46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
                31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,
                16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<60>,
              std::index_sequence<
                60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,
                45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
                30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
                15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<59>,
              std::index_sequence<
                59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,
                44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,
                14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<58>,
              std::index_sequence<
                58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44,
                43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,
                28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<57>,
              std::index_sequence<
                57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,
                42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,
                27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<56>,
              std::index_sequence<
                56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42,
                41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,
                26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<55>,
              std::index_sequence<
                55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
                40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,
                25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
                10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<54>,
              std::index_sequence<
                54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,
                39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
                9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<53>,
              std::index_sequence<
                53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39,
                38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,
                23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
                8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<52>,
              std::index_sequence<
                52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38,
                37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
                22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
                7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<51>,
              std::index_sequence<
                51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37,
                36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,
                21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
                5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<50>,
              std::index_sequence<
                50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36,
                35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
                20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<49>,
              std::index_sequence<
                49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35,
                34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
                3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<48>,
              std::index_sequence<
                48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34,
                33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
                18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<47>,
              std::index_sequence<
                47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,
                32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18,
                17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<46>,
              std::index_sequence<
                46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
                31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,
                16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<45>,
              std::index_sequence<
                45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
                30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
                15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<44>,
              std::index_sequence<
                44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,
                14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<43>,
              std::index_sequence<
                43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,
                28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<42>,
              std::index_sequence<
                42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,
                27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<41>,
              std::index_sequence<
                41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,
                26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<40>,
              std::index_sequence<
                40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,
                25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
                10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<39>,
              std::index_sequence<
                39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25,
                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
                9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<38>,
              std::index_sequence<
                38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,
                23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
                8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<37>,
              std::index_sequence<
                37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,
                22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
                7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<36>,
              std::index_sequence<
                36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,
                21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
                5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<35>,
              std::index_sequence<
                35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
                20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<34>,
              std::index_sequence<
                34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
                3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<33>,
              std::index_sequence<
                33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
                18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<32>,
              std::index_sequence<
                32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18,
                17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<31>,
              std::index_sequence<
                31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,
                16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<30>,
              std::index_sequence<
                30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,
                15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<29>,
              std::index_sequence<
                29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,
                14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<28>,
              std::index_sequence<
                28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14,
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<27>,
              std::index_sequence<
                27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,
                12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<26>,
              std::index_sequence<
                26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<25>,
              std::index_sequence<
                25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
                10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<24>,
              std::index_sequence<
                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
                9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<23>,
              std::index_sequence<
                23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
                8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<22>,
              std::index_sequence<
                22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8,
                7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<21>,
              std::index_sequence<
                21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,
                5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<20>,
              std::index_sequence<
                20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5,
                4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<19>,
              std::index_sequence<
                19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,
                3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<18>,
              std::index_sequence<
                18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<17>,
              std::index_sequence<
                17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
                1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<16>,
              std::index_sequence<
                16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<15>,
              std::index_sequence<
                15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<14>,
              std::index_sequence<
                14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<13>,
              std::index_sequence<
                13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<12>,
              std::index_sequence<12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<11>,
              std::index_sequence<11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<10>,
              std::index_sequence<10, 9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<9>,
              std::index_sequence<9, 8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<8>,
              std::index_sequence<8, 7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<7>,
              std::index_sequence<7, 6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<6>,
              std::index_sequence<6, 5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<5>,
              std::index_sequence<5, 4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<4>,
              std::index_sequence<4, 3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<3>,
              std::index_sequence<3, 2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<2>,
              std::index_sequence<2, 1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<1>,
              std::index_sequence<1>>);
static_assert(std::is_same_v<
              supl::make_reverse_index_sequence<0>,
              std::index_sequence<>>);
// }}}

///////////////////////////////////////////// sum_type

static_assert(
  std::is_same_v<supl::sum_type_t<int, int>, int>, "int + int == int"
);
static_assert(
  std::is_same_v<supl::sum_type_t<short, short>, int>,
  "short + short == int"
);
static_assert(
  std::is_same_v<supl::sum_type_t<double, double>, double>,
  "double + double == double"
);
static_assert(
  std::is_same_v<supl::sum_type_t<double, int>, double>,
  "double + int == double"
);
static_assert(
  std::is_same_v<supl::sum_type_t<double, float>, double>,
  "double + float == double"
);
static_assert(
  std::is_same_v<supl::sum_type_t<int, short>, int>, "int + short == int"
);
static_assert(
  std::is_same_v<supl::sum_type_t<double, int>, double>,
  "double + int == double"
);
static_assert(
  std::is_same_v<supl::sum_type_t<char, int>, int>, "char + int == int"
);
static_assert(
  std::is_same_v<supl::sum_type_t<short, short, short, short, int>, int>,
  "short + short + short + short + int == int"
);

///////////////////////////////////////////// index_type_for

template <typename T>
struct parameterized_test_for_index_type_for
{
    static_assert(std::is_same_v<
                  supl::index_type_for_max_t<
                    std::numeric_limits<T>::max() - 1>,
                  T>);
};

template struct parameterized_test_for_index_type_for<unsigned char>;
template struct parameterized_test_for_index_type_for<unsigned short>;
template struct parameterized_test_for_index_type_for<unsigned int>;
template struct parameterized_test_for_index_type_for<unsigned long>;

///////////////////////////////////////////// size_of

static_assert(supl::size_of<int>::value == sizeof(int));
static_assert(supl::size_of_v<int> == sizeof(int));

///////////////////////////////////////////// align_of

static_assert(supl::align_of<int>::value == alignof(int));
static_assert(supl::align_of_v<int> == alignof(int));

///////////////////////////////////////////// remove_cvref

static_assert(std::is_same_v<int, supl::remove_cvref_t<int>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<const int>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<volatile int>>);
static_assert(std::is_same_v<
              int, supl::remove_cvref_t<const volatile int>>);

static_assert(std::is_same_v<int, supl::remove_cvref_t<int&>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<const int&>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<volatile int&>>);
static_assert(std::is_same_v<
              int, supl::remove_cvref_t<const volatile int&>>);

static_assert(std::is_same_v<int, supl::remove_cvref_t<int&&>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<const int&&>>);
static_assert(std::is_same_v<int, supl::remove_cvref_t<volatile int&&>>);
static_assert(std::is_same_v<
              int, supl::remove_cvref_t<const volatile int&&>>);

static_assert(std::is_same_v<
              // NOLINTNEXTLINE(*-avoid-c-arrays)
              int[5],
              // NOLINTNEXTLINE(*-avoid-c-arrays)
              supl::remove_cvref_t<const int[5]>>);

///////////////////////////////////////////// is_type_in_pack

static_assert(supl::is_type_in_pack_v<int, char, bool, int>);
static_assert(supl::is_type_in_pack_v<int, int, bool, int>);
static_assert(not supl::is_type_in_pack_v<
              int, char, bool, char, char, char, float, double>);
static_assert(supl::is_type_in_pack_v<int, int, int, int>);
static_assert(supl::is_type_in_pack_v<int, int>);
static_assert(not supl::is_type_in_pack_v<int, char>);

///////////////////////////////////////////// peel_first

static_assert(std::is_same_v<
              supl::peel_first_t<int, char, char, char>, int>);
static_assert(std::is_same_v<supl::peel_first_t<int, int, int, int>, int>);
static_assert(std::is_same_v<
              supl::peel_first_t<int, char, int, int>, int>);
static_assert(std::is_same_v<
              supl::peel_first_t<char, int, int, int>, char>);

///////////////////////////////////////////// peel_last

static_assert(std::is_same_v<
              supl::peel_last_t<char, char, char, int>, int>);
static_assert(std::is_same_v<supl::peel_last_t<int, int, int, int>, int>);
static_assert(std::is_same_v<supl::peel_last_t<int, char, int, int>, int>);
static_assert(std::is_same_v<
              supl::peel_last_t<int, int, int, char>, char>);

///////////////////////////////////////////// is_pack_uniform

static_assert(supl::is_pack_uniform_v<int, int, int, int, int>);
static_assert(not supl::is_pack_uniform_v<int, char, int, int, int>);
static_assert(not supl::is_pack_uniform_v<int, int, int, int, char>);
static_assert(not supl::is_pack_uniform_v<int, int, int, char, int, int>);
static_assert(not supl::is_pack_uniform_v<int, bool, bool, bool, bool>);
static_assert(not supl::is_pack_uniform_v<bool, bool, int, bool, bool>);
static_assert(not supl::is_pack_uniform_v<bool, bool, bool, bool, int>);
static_assert(supl::is_pack_uniform_v<int, int>);
static_assert(not supl::is_pack_uniform_v<int, char>);
static_assert(supl::is_pack_uniform_v<int>);

///////////////////////////////////////////// is_pack_only

static_assert(supl::is_pack_only_v<double, double, double, double>);
static_assert(not supl::is_pack_only_v<double, double, double, int>);
static_assert(supl::is_pack_only_v<int, int, int, int>);
static_assert(not supl::is_pack_only_v<int, double, double, double>);
static_assert(not supl::is_pack_only_v<
              std::string, std::string_view, std::string_view,
              std::string_view>);
static_assert(not supl::is_pack_only_v<double>);
static_assert(not supl::is_pack_only_v<void>);

///////////////////////////////////////////// is_iterable

static_assert(supl::is_iterable_v<std::vector<int>>);
static_assert(supl::is_iterable_v<std::list<int>>);
static_assert(supl::is_iterable_v<std::array<int, 5>>);
// NOLINTNEXTLINE(*-avoid-c-arrays)
static_assert(supl::is_iterable_v<int[5]>);
static_assert(not supl::is_iterable_v<int>);
static_assert(not supl::is_iterable_v<char>);
static_assert(not supl::is_iterable_v<bool>);
static_assert(not supl::is_iterable_v<std::tuple<int, char, bool>>);
static_assert(not supl::is_iterable_v<std::pair<int, char>>);

///////////////////////////////////////////// is_iterator

static_assert(not supl::is_iterator_v<int>);
static_assert(supl::is_iterator_v<std::vector<int>::iterator>);
static_assert(supl::is_iterator_v<std::array<int, 5>::iterator>);
static_assert(supl::is_iterator_v<int*>);
static_assert(supl::is_iterator_v<const int*>);

///////////////////////////////////////////// is_forward

static_assert(supl::is_forward_v<std::forward_list<int>::iterator>);
static_assert(supl::is_forward_v<std::vector<int>::iterator>);
static_assert(supl::is_forward_v<std::list<int>::iterator>);
static_assert(supl::is_forward_v<std::deque<int>::iterator>);

///////////////////////////////////////////// is_bidirectional

static_assert(not supl::is_bidirectional_v<
              std::forward_list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::vector<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::list<int>::iterator>);
static_assert(supl::is_bidirectional_v<std::deque<int>::iterator>);

///////////////////////////////////////////// is_random_access

static_assert(not supl::is_random_access_v<
              std::forward_list<int>::iterator>);
static_assert(supl::is_random_access_v<std::vector<int>::iterator>);
static_assert(not supl::is_random_access_v<std::list<int>::iterator>);
static_assert(supl::is_random_access_v<std::deque<int>::iterator>);

///////////////////////////////////////////// is_tuple

static_assert(supl::is_tuple_v<std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<
              const volatile std::tuple<int, char, double>>);
static_assert(supl::is_tuple_v<std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<
              const volatile std::tuple<int, char, double>&>);
static_assert(supl::is_tuple_v<std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<
              const volatile std::tuple<int, char, double>&&>);
static_assert(supl::is_tuple_v<std::tuple<int, char>>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>>);
static_assert(supl::is_tuple_v<std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>&>);
static_assert(supl::is_tuple_v<std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<const std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<volatile std::tuple<int, char>&&>);
static_assert(supl::is_tuple_v<const volatile std::tuple<int, char>&&>);
static_assert(not supl::is_tuple_v<std::pair<int, char>>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>>);
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>>);
static_assert(not supl::is_tuple_v<std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>&>);
static_assert(not supl::is_tuple_v<std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<const std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<volatile std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<const volatile std::pair<int, char>&&>);
static_assert(not supl::is_tuple_v<int>);

///////////////////////////////////////////// is_pair

static_assert(supl::is_pair_v<std::pair<int, char>>);
static_assert(supl::is_pair_v<const std::pair<int, char>>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>>);
static_assert(supl::is_pair_v<std::pair<int, char>&>);
static_assert(supl::is_pair_v<const std::pair<int, char>&>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>&>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>&>);
static_assert(supl::is_pair_v<std::pair<int, char>&&>);
static_assert(supl::is_pair_v<const std::pair<int, char>&&>);
static_assert(supl::is_pair_v<volatile std::pair<int, char>&&>);
static_assert(supl::is_pair_v<const volatile std::pair<int, char>&&>);
static_assert(not supl::is_pair_v<std::tuple<int, char>>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>>);
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>>);
static_assert(not supl::is_pair_v<std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>&>);
static_assert(not supl::is_pair_v<std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<const std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<volatile std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<const volatile std::tuple<int, char>&&>);
static_assert(not supl::is_pair_v<int>);

///////////////////////////////////////////// is_smart_pointer

static_assert(supl::is_smart_pointer_v<std::unique_ptr<int>>);
static_assert(supl::is_smart_pointer_v<std::shared_ptr<int>>);
static_assert(supl::is_smart_pointer_v<std::weak_ptr<int>>);
static_assert(supl::is_smart_pointer_v<std::unique_ptr<int, void()>>);
static_assert(not supl::is_smart_pointer_v<int>);
static_assert(not supl::is_smart_pointer_v<std::vector<int>>);

///////////////////////////////////////////// is_printable

static_assert(supl::is_printable_v<int>);
static_assert(supl::is_printable_v<char>);
static_assert(supl::is_printable_v<bool>);
static_assert(supl::is_printable_v<std::string>);
static_assert(not supl::is_printable_v<std::vector<int>>);
static_assert(not supl::is_printable_v<std::list<std::string>>);

///////////////////////////////////////////// are_equality_comparable

static_assert(supl::are_equality_comparable_v<int, int>);
static_assert(supl::are_equality_comparable_v<int, char>);
static_assert(supl::are_equality_comparable_v<int, float>);
static_assert(supl::are_equality_comparable_v<int, double>);
static_assert(supl::are_equality_comparable_v<int, bool>);
static_assert(supl::are_equality_comparable_v<int, char>);
static_assert(not supl::are_equality_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_inequality_comparable

static_assert(supl::are_inequality_comparable_v<int, int>);
static_assert(supl::are_inequality_comparable_v<int, char>);
static_assert(supl::are_inequality_comparable_v<int, float>);
static_assert(supl::are_inequality_comparable_v<int, double>);
static_assert(supl::are_inequality_comparable_v<int, bool>);
static_assert(supl::are_inequality_comparable_v<int, char>);
static_assert(not supl::are_inequality_comparable_v<
              int, std::vector<int>>);

///////////////////////////////////////////// are_less_comparable

static_assert(supl::are_less_comparable_v<int, int>);
static_assert(supl::are_less_comparable_v<int, char>);
static_assert(supl::are_less_comparable_v<int, float>);
static_assert(supl::are_less_comparable_v<int, double>);
static_assert(supl::are_less_comparable_v<int, bool>);
static_assert(supl::are_less_comparable_v<int, char>);
static_assert(not supl::are_less_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_less_eq_comparable

static_assert(supl::are_less_eq_comparable_v<int, int>);
static_assert(supl::are_less_eq_comparable_v<int, char>);
static_assert(supl::are_less_eq_comparable_v<int, float>);
static_assert(supl::are_less_eq_comparable_v<int, double>);
static_assert(supl::are_less_eq_comparable_v<int, bool>);
static_assert(supl::are_less_eq_comparable_v<int, char>);
static_assert(not supl::are_less_eq_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_greater_comparable

static_assert(supl::are_greater_comparable_v<int, int>);
static_assert(supl::are_greater_comparable_v<int, char>);
static_assert(supl::are_greater_comparable_v<int, float>);
static_assert(supl::are_greater_comparable_v<int, double>);
static_assert(supl::are_greater_comparable_v<int, bool>);
static_assert(supl::are_greater_comparable_v<int, char>);
static_assert(not supl::are_greater_comparable_v<int, std::vector<int>>);

///////////////////////////////////////////// are_greater_eq_comparable

static_assert(supl::are_greater_eq_comparable_v<int, int>);
static_assert(supl::are_greater_eq_comparable_v<int, char>);
static_assert(supl::are_greater_eq_comparable_v<int, float>);
static_assert(supl::are_greater_eq_comparable_v<int, double>);
static_assert(supl::are_greater_eq_comparable_v<int, bool>);
static_assert(supl::are_greater_eq_comparable_v<int, char>);
static_assert(not supl::are_greater_eq_comparable_v<
              int, std::vector<int>>);

///////////////////////////////////////////// make_const_ref

static_assert(std::is_same_v<supl::make_const_ref_t<int>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const int>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<volatile int>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const volatile int>, const int&>);
static_assert(std::is_same_v<supl::make_const_ref_t<int&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const int&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<volatile int&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const volatile int&>, const int&>);
static_assert(std::is_same_v<supl::make_const_ref_t<int&&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const int&&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<volatile int&&>, const int&>);
static_assert(std::is_same_v<
              supl::make_const_ref_t<const volatile int&&>, const int&>);

///////////////////////////////////////////// is_same_as

static_assert(supl::tl::none_of_v<
              supl::tl::type_list<int, char, bool>,
              supl::is_same_as<void>::func>);

static_assert(supl::tl::any_of_v<
              supl::tl::type_list<int, char, bool>,
              supl::is_same_as<char>::func>);

static_assert(not supl::tl::all_of_v<
              supl::tl::type_list<int, char, bool>,
              supl::is_same_as<char>::func>);

static_assert(supl::is_same_as<int>::template func<int>::value);
static_assert(supl::is_same_as<int>::template func_v<int>);
static_assert(not supl::is_same_as<void>::template func<int>::value);
static_assert(not supl::is_same_as<void>::template func_v<int>);
static_assert(not supl::is_same_as<int>::template func<void>::value);
static_assert(not supl::is_same_as<int>::template func_v<void>);

///////////////////////////////////////////// conjunction_compose

static_assert(supl::tl::any_of_v<
              supl::tl::type_list<int, char, const bool>,
              supl::conjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(not supl::tl::any_of_v<
              supl::tl::type_list<int, char, bool, const double>,
              supl::conjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(not supl::tl::all_of_v<
              supl::tl::type_list<int, char, const bool>,
              supl::conjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(supl::tl::all_of_v<
              supl::tl::type_list<const int, const char, const bool>,
              supl::conjunction_compose<
                std::is_integral, std::is_const>::func>);

///////////////////////////////////////////// disjunction_compose

static_assert(supl::tl::any_of_v<
              supl::tl::type_list<int, char, const bool>,
              supl::disjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(supl::tl::any_of_v<
              supl::tl::type_list<const double, void, std::nullptr_t>,
              supl::disjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(supl::tl::all_of_v<
              supl::tl::type_list<int, const double>,
              supl::disjunction_compose<
                std::is_integral, std::is_const>::func>);

static_assert(supl::tl::all_of_v<
              supl::tl::type_list<const int, char, bool, const void>,
              supl::disjunction_compose<
                std::is_integral, std::is_const>::func>);

///////////////////////////////////////////// binary_partial_apply

static_assert(supl::binary_partial_apply<
              std::is_same, int>::template func_v<int>);

static_assert(
  supl::binary_partial_apply<std::is_same, int>::template func<int>::value
);

static_assert(not supl::binary_partial_apply<
              std::is_same, void>::template func_v<int>);

static_assert(not supl::binary_partial_apply<
              std::is_same, void>::template func<int>::value);

static_assert(not supl::binary_partial_apply<
              std::is_same, int>::template func_v<void>);

static_assert(not supl::binary_partial_apply<
              std::is_same, int>::template func<void>::value);

template <typename T, typename U>
struct identity_or_void : supl::type_identity<void>
{
};

template <typename T>
struct identity_or_void<T, T> : supl::type_identity<T>
{
};

static_assert(std::is_same_v<
              supl::binary_partial_apply<
                identity_or_void, int>::template func<int>::type,
              int>);

static_assert(std::is_same_v<
              supl::binary_partial_apply<
                identity_or_void, int>::template func_t<int>,
              int>);

static_assert(std::is_same_v<
              supl::binary_partial_apply<
                identity_or_void, char>::template func<int>::type,
              void>);

static_assert(std::is_same_v<
              supl::binary_partial_apply<
                identity_or_void, char>::template func_t<int>,
              void>);

///////////////////////////////////////////// sequential_apply

static_assert(std::is_same_v<
              supl::sequential_apply_t<int, std::add_const>, const int>);

static_assert(std::is_same_v<
              supl::sequential_apply_t<
                int, std::add_const, std::add_pointer, std::add_const>,
              const int* const>);

static_assert(std::is_same_v<
              supl::sequential_apply_t<
                char, std::add_const, std::add_pointer, std::add_volatile,
                std::add_lvalue_reference>,
              const char* volatile&>);

static_assert(supl::sequential_apply_v<
              bool, std::add_const, std::is_const>);

static_assert(supl::sequential_apply_v<
              int, std::add_const, std::add_lvalue_reference,
              supl::binary_partial_apply<std::is_same, const int&>::func>);

static_assert(! supl::sequential_apply_v<
              int, std::add_const,
              supl::binary_partial_apply<std::is_same, const int&>::func>);

///////////////////////////////////////////// sequential_applicator

static_assert(std::is_same_v<
              supl::sequential_applicator<
                supl::remove_cvref, std::add_const,
                std::add_lvalue_reference>::template func_t<int>,
              const int&>);

static_assert(std::is_same_v<
              supl::sequential_applicator<
                supl::remove_cvref, std::add_const,
                std::add_lvalue_reference>::template func_t<const int&&>,
              const int&>);

static_assert(std::is_same_v<
              supl::sequential_applicator<
                supl::remove_cvref, std::add_const,
                std::add_lvalue_reference>::template func_t<volatile int>,
              const int&>);

static_assert(std::is_same_v<
              supl::sequential_applicator<
                std::decay, std::remove_pointer, std::decay,
                std::add_volatile, std::add_const, std::add_pointer,
                std::add_const, std::add_lvalue_reference>::
                template func_t<volatile char* const>,
              const volatile char* const&>);

///////////////////////////////////////////// apply_if

static_assert(std::is_same_v<
              const int, supl::apply_if_t<true, std::add_const, int>>);

static_assert(std::is_same_v<
              int, supl::apply_if_t<false, std::add_const, int>>);

///////////////////////////////////////////// least_of

static_assert(std::is_same_v<
              std::uint16_t, supl::least_of_t<
                               supl::size_of, std::less, std::uint16_t,
                               std::uint32_t, std::uint64_t>>);
static_assert(std::is_same_v<
              std::uint16_t, supl::least_of_t<
                               supl::size_of, std::less, std::uint64_t,
                               std::uint32_t, std::uint16_t>>);

static_assert(std::is_same_v<
              std::uint16_t, supl::least_of_t<
                               supl::size_of, std::less, std::uint32_t,
                               std::uint64_t, std::uint16_t>>);

static_assert(std::is_same_v<
              std::uint16_t, supl::least_of_t<
                               supl::size_of, std::less, std::uint16_t,
                               std::uint64_t, std::uint32_t>>);

static_assert(std::is_same_v<
              std::uint64_t, supl::least_of_t<
                               supl::size_of, std::greater, std::uint16_t,
                               std::uint32_t, std::uint64_t>>);
static_assert(std::is_same_v<
              std::uint64_t, supl::least_of_t<
                               supl::size_of, std::greater, std::uint64_t,
                               std::uint32_t, std::uint16_t>>);

static_assert(std::is_same_v<
              std::uint64_t, supl::least_of_t<
                               supl::size_of, std::greater, std::uint32_t,
                               std::uint64_t, std::uint16_t>>);

static_assert(std::is_same_v<
              std::uint64_t, supl::least_of_t<
                               supl::size_of, std::greater, std::uint16_t,
                               std::uint64_t, std::uint32_t>>);

auto main() -> int
{
}
