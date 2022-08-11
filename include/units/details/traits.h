#ifndef _include_units_details_traits_h
#define _include_units_details_traits_h

#include <cstdint>
#include <type_traits>
#include <ratio>

namespace units::_details::tags {
  struct power {};
  struct fundamental_quantity {};
  struct physical_quantity {};
  struct base_unit {};
  struct unit {};
}

namespace units::_details::props {
  template <unsigned i>
  struct indexed {
    static constexpr auto index = i;
  };
}

namespace units::_details::traits {
  // check if type is indexed (conatins ::index static member of integer type)
  template <class T, class Enabler = void>
  struct is_indexed : std::false_type {};

  template <class T>
  struct is_indexed<T, std::void_t<decltype(T::index)>> // : std::true_type {};
  : std::bool_constant<std::is_integral_v<decltype(T::index)>> {};

  template <class T>
  constexpr inline bool is_indexed_v = is_indexed<T>::value;

  // check if class can represent a ratio (contains ::num and ::den members)
  template <class T, class Enabler = void>
  struct is_ratio : std::false_type {};

  template <class T>
  struct is_ratio<T, std::void_t<decltype(T::num), decltype(T::den)>> : std::true_type {};

  template <class T>
  constexpr inline bool is_ratio_v = is_ratio<T>::value;

  // check if class is a power
  template <class T> struct is_power : std::bool_constant<std::is_base_of_v<tags::power, T>> {};
  template <class T> constexpr inline bool is_power_v = is_power<T>::value;

  // check if class represents a fundamental quantity
  template <class T> struct is_fundamental_quantity : std::is_base_of<tags::fundamental_quantity, T> {};
  template <class T> constexpr inline bool is_fundamental_quantity_v = is_fundamental_quantity<T>::value;

  // check if type represents a physical quantity
  template <class T> struct is_physical_quantity : std::is_base_of<tags::physical_quantity, T> {};
  template <class T> constexpr inline bool is_physical_quantity_v = is_physical_quantity<T>::value;

  // check if type represents a base unit
  template <class T> struct is_base_unit : std::is_base_of<tags::base_unit, T> {};
  template <class T> constexpr inline bool is_base_unit_v = is_base_unit<T>::value;
}

#endif