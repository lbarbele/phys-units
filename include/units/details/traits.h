#ifndef _include_units_details_traits_h
#define _include_units_details_traits_h

#include <cstdint>
#include <type_traits>
#include <ratio>

namespace units::_details::tags {
  struct power {};
}

namespace units::_details::traits {
  // create void from pack
  template <class... Ts> using void_t = void;

  // check if class can represent a ratio (contains ::num and ::den members)
  template <class T, class Enabler = void>
  struct is_ratio : std::false_type {};

  template <class T>
  struct is_ratio<T, void_t<decltype(T::num), decltype(T::den)>> : std::true_type {};

  template <class T>
  constexpr inline bool is_ratio_v = is_ratio<T>::value;

  // check if class is a power
  template <class T> struct is_power : std::bool_constant<std::is_base_of_v<tags::power, T>> {};
  template <class T> constexpr inline bool is_power_v = is_power<T>::value;

}

#endif