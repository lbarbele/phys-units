#ifndef _include_units_details_traits_h
#define _include_units_details_traits_h

#include <concepts>
#include <cstdint>
#include <type_traits>
#include <ratio>

namespace units::_details::concepts {
  template <class T>
  concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;
}

#endif