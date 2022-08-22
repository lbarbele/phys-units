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
    // * assert type is a quantity
    template <class T>
    struct is_quantity : std::false_type {};

    template <class U, class V>
    struct is_quantity<quantity<U, V>> : std::true_type {};

    template <class T>
    constexpr inline bool is_quantity_v = is_quantity<T>::value;

    // * assert type is quantity and value is integral
    template <class T>
    struct is_integral_quantity : std::false_type {};

    template <req::unit U, std::integral V>
    struct is_integral_quantity<quantity<U, V>> : std::true_type {};

    template <class Q>
    constexpr inline bool is_integral_quantity_v = is_integral_quantity<Q>::value;

    // * assert type is a quantity and value is floating point
    template <class T>
    struct is_floating_point_quantity : std::false_type {};

    template <req::unit U, std::floating_point V>
    struct is_floating_point_quantity<quantity<U, V>> : std::true_type {};

    template <class Q>
    constexpr inline bool is_floating_point_quantity_v = is_floating_point_quantity<Q>::value;

    // * assert quantity is dimensionless
    template <class T>
    struct is_dimensionless_quantity : std::false_type {};

    template <req::dimensionless_unit U, std::floating_point V>
    struct is_dimensionless_quantity<quantity<U, V>> : std::true_type {};

    template <class Q>
    constexpr inline bool is_dimensionless_quantity_v = is_dimensionless_quantity<Q>::value;
  }

  // - concept of a quantity

  namespace req {
    template <class T>
    concept quantity = traits::is_quantity_v<T>;

    template <class T, class U>
    concept quantity_compatible = traits::is_compatible_unit_v<typename T::unit_type, typename U::unit_type>;

    template <class Q>
    concept integral_quantity = traits::is_integral_quantity_v<Q>;

    template <class Q>
    concept floating_point_quantity = traits::is_floating_point_quantity_v<Q>;

    template <class Q>
    concept dimensionless_quantity = traits::is_dimensionless_quantity_v<Q>;
  }

  // - definition of the quantity class

  template <req::unit Unit, req::arithmetic ValueType = double>
  struct quantity {
  public:
    using type = quantity;
    using unit_type = std::decay_t<Unit>;
    using value_type = std::decay_t<ValueType>;

  private:
    value_type m_value;

  public:
    explicit constexpr quantity(const value_type& value = 0) : m_value(value) {};

    // * access to raw underlying value

    constexpr auto& get_value() {
      return m_value;
    }

    constexpr const auto& get_value() const {
      return m_value;
    }

    constexpr type& set_value(const req::arithmetic auto value) {
      m_value = value;
      return *this;
    }

    constexpr type set_value(const req::arithmetic auto value) const {
      return type(value);
    }

    // * unit (type) conversion

    template <req::unit_compatible<unit_type> U, req::arithmetic V = value_type>
    constexpr auto convert() const {
      using factor = ratio_divide<typename unit_type::factor, typename U::factor>;
      return quantity<U, V>(V(m_value) * V(factor::num)/V(factor::den));
    }

    template <req::unit_compatible<unit_type> U, req::arithmetic V>
    constexpr operator quantity<U, V>() const {
      return convert<U, V>();
    }

    template <std::constructible_from<value_type> T>
    requires req::dimensionless_unit<unit_type>
    constexpr operator T() const {
      return T(m_value);
    };

    // * assignment operations for quantities of compatible units

    // simple assignment
    constexpr type& operator=(const req::quantity auto q) {
      return set_value(type(q).get_value());
    }

    // addition assignment
    constexpr type& operator+=(const req::quantity auto q) {
      return set_value(get_value() + type(q).get_value());
    }

    // subtraction assignment
    constexpr type& operator-=(const req::quantity auto q) {
      return set_value(get_value() - type(q).get_value());
    }

    // multiplication assignment by dimensionless object
    constexpr type& operator*=(const std::convertible_to<value_type> auto x) {
      return set_value(get_value() * value_type(x));
    }

    // division assignment by dimensionless object
    constexpr type& operator/=(const std::convertible_to<value_type> auto x) {
      return set_value(get_value() / value_type(x));
    }

    // * assignment operations for dimensionless quantities

    // simple assignment
    constexpr type& operator=(const std::convertible_to<value_type> auto& x)
    requires req::dimensionless_unit<unit_type> {
      return set_value(value_type(x));
    }

    // addition assignemnt
    constexpr type& operator+=(const std::convertible_to<value_type> auto& x)
    requires req::dimensionless_unit<unit_type> {
      return set_value(get_value() + value_type(x));
    }

    // subtraction assignment
    constexpr type& operator-=(const std::convertible_to<value_type> auto& x)
    requires req::dimensionless_unit<unit_type> {
      return set_value(get_value() - value_type(x));
    }

    // * increment/decrement operations

    constexpr type& operator++() {
      return set_value(++m_value);
    }

    constexpr type& operator--() {
      return set_value(--m_value);
    }

    constexpr type operator++(int) {
      return type(m_value++);
    }

    constexpr type operator--(int) {
      return type(m_value--);
    }

    // * arithmetic operations

    // unary plus
    constexpr type operator+() const {
      return type(+get_value());
    };

    // unary minus
    constexpr type operator-() const {
      return type(-get_value());
    };

    // addition with quantity of compatible unit
    constexpr auto operator+(const req::quantity_compatible<type> auto q) const {
      const auto value = get_value() + q.template convert<unit_type>().get_value();
      return quantity<unit_type, decltype(value)>(value);
    }

    // subtraction with quantity of compatible unit
    constexpr auto operator-(const req::quantity_compatible<type> auto q) const {
      const auto value = get_value() - q.template convert<unit_type>().get_value();
      return quantity<unit_type, decltype(value)>(value);
    }

    // multiplication by another quantity
    constexpr auto operator*(const req::quantity auto q) const {
      using resulting_unit = make_unit<unit_type, typename decltype(q)::unit_type>;
      const auto value = get_value() * q.get_value();
      return quantity<resulting_unit, decltype(value)>(value);
    }

    // division by another quantity
    constexpr auto operator/(const req::quantity auto q) const {
      using resulting_unit = make_unit<unit_type, inverse<typename decltype(q)::unit_type>>;
      const auto value = get_value() / q.get_value();
      return quantity<resulting_unit, decltype(value)>(value);
    }

    // multiplication by arithmetic type
    constexpr auto operator*(const req::arithmetic auto x) const {
      return quantity<unit_type, decltype(get_value()*x)>(get_value()*x);
    }

    // division by arithmetic type
    constexpr auto operator/(const req::arithmetic auto x) const {
      return quantity<unit_type, decltype(get_value()/x)>(get_value()/x);
    }

    // modulo operation by integral type (onlu for integral quantities)
    constexpr auto operator%(const std::integral auto i) const
    requires req::integral_quantity<type> {
      return decltype(*this/i)(get_value()%i);
    }

    // modulo operation by another integral quantity
    constexpr auto operator%(const req::integral_quantity auto q) const
    requires req::integral_quantity<type> {
      return decltype(*this/q)(get_value() % q.get_value());
    }

    // * comparison

    constexpr bool operator==(const req::quantity auto q) const {
      return get_value() == type(q).get_value();
    }

    constexpr bool operator!=(const req::quantity auto q) const {
      return get_value() != type(q).get_value();
    }

    constexpr bool operator<(const req::quantity auto q) const {
      return get_value() < type(q).get_value();
    }

    constexpr bool operator>(const req::quantity auto q) const {
      return get_value() > type(q).get_value();
    }

    constexpr bool operator<=(const req::quantity auto q) const {
      return get_value() <= type(q).get_value();
    }

    constexpr bool operator>=(const req::quantity auto q) const {
      return get_value() >= type(q).get_value();
    }

  };

  // - multiplication by arithmetic type from lhs

  constexpr auto operator*(const req::arithmetic auto x, const req::quantity auto q) {
    return q * x;
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
