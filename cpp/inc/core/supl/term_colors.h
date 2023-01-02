/* {{{ doc */
/**
 * @file term_colors.h
 *
 * @brief Aliases to ANSI color codes
 *
 * @details This is not portable.
 */
/* }}} */

#ifndef SUPPLE_CORE_TERMCOLORS_H
#define SUPPLE_CORE_TERMCOLORS_H

#include <string_view>

// Make doxygen ignore this file
/// @cond

namespace supl {

constexpr inline std::string_view FG_BLACK = "\033[1;30m";
constexpr inline std::string_view FG_RED = "\033[1;31m";
constexpr inline std::string_view FG_GREEN = "\033[1;32m";
constexpr inline std::string_view FG_YELLOW = "\033[1;33m";
constexpr inline std::string_view FG_BLUE = "\033[1;34m";
constexpr inline std::string_view FG_MAGENTA = "\033[1;35m";
constexpr inline std::string_view FG_CYAN = "\033[1;36m";
constexpr inline std::string_view FG_WHITE = "\033[1;37m";
constexpr inline std::string_view BG_BLACK = "\033[1;40m";
constexpr inline std::string_view BG_RED = "\033[1;41m";
constexpr inline std::string_view BG_GREEN = "\033[1;42m";
constexpr inline std::string_view BG_YELLOW = "\033[1;43m";
constexpr inline std::string_view BG_BLUE = "\033[1;44m";
constexpr inline std::string_view BG_MAGENTA = "\033[1;45m";
constexpr inline std::string_view BG_CYAN = "\033[1;46m";
constexpr inline std::string_view BG_WHITE = "\033[1;47m";
constexpr inline std::string_view RESET = "\033[1;0m";

}  // namespace supl

#endif
