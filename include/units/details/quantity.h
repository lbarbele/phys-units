#ifndef _include_units_details_quantity_h
#define _include_units_details_quantity_h

#include <ostream>
#include <string>
#include <concepts>

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

namespace units::_details {

  // - forward declare the quantity struct

  template <req::unit U, req::arithmetic V> struct quantity;

  // - traits regarting quantities

  namespace traits {
    template <class T>
    struct is_quantity : std::false_type {};

    template <class U, class V>
    struct is_quantity<quantity<U, V>> : std::true_type {};

    template <class T>
    constexpr inline bool is_quantity_v = is_quantity<T>::value;
  }

  // - concept of a quantity

  namespace req {
    template <class T>
    concept quantity = traits::is_quantity_v<T>;
  }

  // - definition of the quantity class

  template <req::unit Unit, req::arithmetic ValueType = double>
  struct quantity {
  public:
    using type = quantity;
    using unit_type = Unit;
    using value_type = ValueType;

  private:
    value_type m_value;

  public:
    explicit constexpr quantity(const value_type& value = 0) : m_value(value) {};

    // * access to raw underlying value

    constexpr const value_type& get_value() const {
      return m_value;
    }

    constexpr type& set_value(const req::arithmetic auto& value) {
      m_value = value;
      return *this;
    }

    // * unit (type) conversion

    template <req::compatible_units<unit_type> U, req::arithmetic V>
    constexpr auto convert() const {
      using factor = ratio_divide<typename unit_type::factor, typename U::factor>;
      return quantity<U, V>(V(m_value) * V(factor::num)/V(factor::den));
    }

    template <req::compatible_units<unit_type> U, req::arithmetic V>
    constexpr operator quantity<U, V>() const {
      return convert<U, V>();
    }

    template <std::constructible_from<value_type> T>
    requires req::dimensionless_unit<unit_type>
    constexpr operator T() const {
      return T(m_value);
    };

    // - assignment operations for quantities of compatible units

    // simple assignment
    constexpr type& operator=(const req::quantity auto& other) {
      return set_value(type(other).get_value());
    }

    // addition assignment
    constexpr type& operator+=(const req::quantity auto& other) {
      m_value += type(other).get_value();
      return *this;
    }

    // subtraction assignment
    constexpr type& operator-=(const req::quantity auto& other) {
      m_value -= type(other).get_value();
      return *this;
    }

    // multiplication assignment by dimensionless object
    constexpr type& operator*=(const std::convertible_to<value_type> auto& m) {
      m_value *= value_type(m);
      return *this;
    }

    // division assignment by dimensionless object
    constexpr type& operator/=(const std::convertible_to<value_type> auto& m) {
      m_value /= value_type(m);
      return *this;
    }

    // - assignment operations for dimensionless quantities

    // simple assignment
    constexpr type& operator=(const req::arithmetic auto& value)
    requires req::dimensionless_unit<unit_type> {
      return set_value(value);
    }

    // addition assignemnt
    constexpr type& operator+=(const req::arithmetic auto& value)
    requires req::dimensionless_unit<unit_type> {
      m_value += value;
      return *this;
    }

    // subtraction assignment
    constexpr type& operator-=(const req::arithmetic auto& value)
    requires req::dimensionless_unit<unit_type> {
      m_value -= value;
      return *this;
    }

    // - increment/decrement operations

    constexpr type& operator++() {
      ++m_value;
      return *this;
    }

    constexpr type& operator--() {
      --m_value;
      return *this;
    }

    constexpr type operator++(int) {
      return type(m_value++);
    }

    constexpr type operator--(int) {
      return type(m_value--);
    }

    // - arithmetic operations

    // unary plus
    constexpr type operator+() const {
      return type(+get_value());
    };

    // unary minus
    constexpr type operator-() const {
      return type(-get_value());
    };

    // addition
    template <req::compatible_units<unit_type> U, class V>
    constexpr auto operator+(const quantity<U, V>& other) const {
      using res_t = quantity<unit_type, decltype(value_type{}+V{})>;
      auto value = res_t(*this).get_value() + res_t(other).get_value();
      return res_t(value);
    }

    // subtraction
    template <req::compatible_units<unit_type> U, class V>
    constexpr auto operator-(const quantity<U, V>& other) const {
      using res_t = quantity<unit_type, decltype(value_type{}-V{})>;
      auto value = res_t(*this).get_value() - res_t(other).get_value();
      return res_t(value);
    }

    // multiplication
    template <class U, class V>
    constexpr auto operator*(const quantity<U, V>& other) const {
      auto value = get_value() * other.get_value();
      using res_t = quantity<make_unit<unit_type, U>, decltype(value)>;
      return res_t(value);
    }

    // division
    template <class U, class V>
    constexpr auto operator/(const quantity<U, V>& other) const {
      auto value = get_value() / other.get_value();
      using res_t = quantity<make_unit<unit_type, inverse<U>>, decltype(value)>;
      return res_t(value);
    }

    // multiplication by dimensionless quantity
    constexpr auto operator*(const req::arithmetic auto& x) {
      auto value = get_value() * x;
      return quantity<unit_type, decltype(value)>(value);
    }

    // division by dimensionless quantity
    constexpr auto operator/(const req::arithmetic auto& x) {
      auto value = get_value() / x;
      return quantity<unit_type, decltype(value)>(value);
    }

    // - comparison

    constexpr bool operator==(const req::quantity auto& other) const {
      return get_value() == type(other).get_value();
    }

    constexpr bool operator!=(const req::quantity auto& other) const {
      return get_value() != type(other).get_value();
    }

    constexpr bool operator<(const req::quantity auto& other) const {
      return get_value() < type(other).get_value();
    }

    constexpr bool operator>(const req::quantity auto& other) const {
      return get_value() > type(other).get_value();
    }

    constexpr bool operator<=(const req::quantity auto& other) const {
      return get_value() <= type(other).get_value();
    }

    constexpr bool operator>=(const req::quantity auto& other) const {
      return get_value() >= type(other).get_value();
    }

  };

  // - multiplication by dimensionless from lhs
  constexpr auto operator*(const req::arithmetic auto& x, const req::quantity auto& q) {
    auto value = q.get_value() * x;
    using unit_type = typename std::decay_t<decltype(q)>::unit_type;
    using value_type = decltype(value);
    return quantity<unit_type, value_type>(value);
  };

  // - print function
  template<class CharT, class Traits, class U, class V>
  std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    const quantity<U, V>& q
  ) {
    return os << q.get_value() << " " << U::symbol;
  }

}

#endif
