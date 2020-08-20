#ifndef ERROR_H
#define ERROR_H

#include <system_error>

#define ERRORS_QUERY    NoMatchesFound = 1, \
                        InvalidSearchArgument


enum class KeyBindErrors{
    ERRORS_QUERY,
    KeyAlreadyBound,
    InvalidEvent
};

namespace std
{
  template <>
    struct is_error_code_enum<KeyBindErrors> : true_type {};
}
 
std::error_code make_error_code(KeyBindErrors);

#endif