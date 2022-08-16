#ifndef _include_units_details_name_h
#define _include_units_details_name_h

#include <string_view>

namespace units::_details {

  template <class T> constexpr inline std::string_view name = "?";
  template <class T> constexpr inline std::string_view plural = "?";
  template <class T> constexpr inline std::string_view abbrev = "?";

}

#endif