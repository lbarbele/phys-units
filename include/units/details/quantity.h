#ifndef _include_units_details_quantity_h
#define _include_units_details_quantity_h

#include <ostream>
#include <string>
#include <concepts>

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/unit.h>

namespace units::_details {

  // - helper definition of the arithmetic concept
  namespace concepts {
    template <class T>
    concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;
  }

  // - forward declare the quantity struct

  template <concepts::unit U, concepts::arithmetic V> struct quantity;

  // - traits and concepts

  // * type is quantity

  namespace traits {
    template <class T>
    struct is_quantity : std::false_type {};

    template <class T>
    requires (std::is_const_v<T> || std::is_volatile_v<T>)
    struct is_quantity<T> : is_quantity<std::remove_cv_t<T>> {};

    template <class U, class V>
    struct is_quantity<quantity<U, V>> : std::true_type {};

    template <class T>
    constexpr inline bool is_quantity_v = is_quantity<T>::value;
  }

  namespace concepts {
    template <class T>
    concept quantity = traits::is_quantity_v<T>;
  }

  // * type is quantity holding integral/floating point type

  namespace traits {
    // integral quantity
    template <class Q>
    struct is_integral_quantity : std::false_type {};

    template <concepts::quantity Q>
    struct is_integral_quantity<Q> : std::is_integral<typename Q::value_type> {};

    template <class Q>
    constexpr inline bool is_integral_quantity_v = is_integral_quantity<Q>::value;

    // floating-point quantity
    template <class Q>
    struct is_floating_point_quantity : std::false_type {};

    template <concepts::quantity Q>
    struct is_floating_point_quantity<Q> : std::is_floating_point<typename Q::value_type> {};

    template <class Q>
    constexpr inline bool is_floating_point_quantity_v = is_floating_point_quantity<Q>::value;
  }

  namespace concepts {
    template <class Q>
    concept integral_quantity = traits::is_integral_quantity_v<Q>;

    template <class Q>
    concept floating_point_quantity = traits::is_floating_point_quantity_v<Q>;
  }

  // * type is quantity with dimensionless units (a conversion factor is accepted)

  namespace traits {
    template <class T>
    struct is_dimensionless_quantity : std::false_type {};

    template <concepts::quantity Q>
    struct is_dimensionless_quantity<Q> : is_dimensionless_unit<typename Q::unit_type> {};

    template <class Q>
    constexpr inline bool is_dimensionless_quantity_v = is_dimensionless_quantity<Q>::value;
  }

  namespace concepts {
    template <class Q>
    concept dimensionless_quantity = traits::is_dimensionless_quantity_v<Q>;
  }

  // * quantities are compatible/incompatible (meaning compatible units)

  namespace traits {
    // assert quantities are compatible
    template <class A, class B>
    struct is_compatible_quantity : std::false_type {};

    template <concepts::quantity Qa, concepts::quantity Qb>
    struct is_compatible_quantity<Qa, Qb>
    : is_compatible_unit<typename Qa::unit_type, typename Qb::unit_type> {};

    template <class A, class B>
    constexpr inline bool is_compatible_quantity_v = is_compatible_quantity<A, B>::value;

    // assert two quantities are incompatible
    template <class A, class B>
    struct is_incompatible_quantity : std::false_type {};

    template <concepts::quantity Qa, concepts::quantity Qb>
    struct is_incompatible_quantity<Qa, Qb>
    : std::negation<is_compatible_quantity<Qa, Qb>> {};

    template <class A, class B>
    constexpr inline bool is_incompatible_quantity_v = is_incompatible_quantity<A, B>::value;
  }

  namespace concepts {
    template <class Q, class... Qs>
    concept quantity_compatible = std::conjunction_v<traits::is_compatible_quantity<Q, Qs>...>;

    template <class Q, class... Qs>
    concept quantity_incompatible = std::conjunction_v<traits::is_incompatible_quantity<Q, Qs>...>;
  }

  // - definition of the quantity class

  template <concepts::unit Unit, concepts::arithmetic ValueType = double>
  struct quantity {
  public:
    using type = quantity;
    using unit_type = std::remove_cv_t<Unit>;
    using value_type = std::remove_cv_t<ValueType>;

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

    constexpr type& set_value(const std::convertible_to<value_type> auto value) {
      m_value = value;
      return *this;
    }

    constexpr type set_value(const std::convertible_to<value_type> auto value) const {
      return type(value);
    }

    // * unit (type) conversion

    // when this' value type is fp number, returned quantity has the same value_type
    template <concepts::unit_compatible<unit_type> U>
    requires concepts::floating_point_quantity<type>
    constexpr auto convert() const {
      using factor = ratio_divide<typename unit_type::factor, typename U::factor>;
      return quantity<U, value_type>(m_value * factor::template value<value_type>); 
    }

    // when this' value type is integral, the value type of the returned quantity
    // may be promoted to long double to acomodate for the conversion factor
    template <concepts::unit_compatible<unit_type> U>
    requires concepts::integral_quantity<type>
    constexpr auto convert() const {
      using factor = ratio_divide<typename unit_type::factor, typename U::factor>;
      if constexpr (factor::num%factor::den == 0)
        // in case the remainder of the operation num/den is zero, the returned
        // quantity carries the same value type as this', because the resulting value
        // can be represented by value type
        return quantity<U, value_type>(m_value * factor::num);
      else
        // otherwise, a floating point number is required, so we promote the, originally
        // integral, value_type to long double
        return quantity<U, long double>(m_value * factor::template value<value_type>);
    }

    // explicit type conversion operator. value type is always the requested one
    template <concepts::unit_compatible<unit_type> U, concepts::arithmetic V>
    constexpr operator quantity<U, V>() const {
      using factor = ratio_divide<typename unit_type::factor, typename U::factor>;
      return quantity<U, V>(m_value * factor::template value<V>);
    }

    // dimensionless quantities can be converted to any type constructible from value_type
    template <std::constructible_from<value_type> T>
    requires concepts::dimensionless_quantity<type>
    constexpr operator T() const {
      return T(m_value * unit_type::factor::template value<value_type>());
    };

    // * assignment operations for quantities of compatible units

    // simple assignment
    constexpr type& operator=(const concepts::quantity_compatible<type> auto q) {
      return set_value(q.template convert<unit_type>().get_value());
    }

    // addition assignment
    constexpr type& operator+=(const concepts::quantity_compatible<type> auto q) {
      return set_value(get_value() + q.template convert<unit_type>().get_value());
    }

    // subtraction assignment
    constexpr type& operator-=(const concepts::quantity_compatible<type> auto q) {
      return set_value(get_value() - q.template convert<unit_type>().get_value());
    }

    // multiplication assignment by dimensionless object
    constexpr type& operator*=(const std::convertible_to<value_type> auto& x) {
      return set_value(get_value() * value_type(x));
    }

    // division assignment by dimensionless object
    constexpr type& operator/=(const std::convertible_to<value_type> auto& x) {
      return set_value(get_value() / value_type(x));
    }

    // modulo assignment of integral quantity by integer number
    constexpr type& operator%=(const std::integral auto i)
    requires concepts::integral_quantity<type> {
      return set_value(get_value()%i);
    }

    // * assignment operations for dimensionless quantities

    // simple assignment
    constexpr type& operator=(const std::convertible_to<value_type> auto& x)
    requires concepts::dimensionless_quantity<type> {
      return set_value(value_type(x));
    }

    // addition assignemnt
    constexpr type& operator+=(const std::convertible_to<value_type> auto& x)
    requires concepts::dimensionless_quantity<type> {
      return set_value(get_value() + value_type(x));
    }

    // subtraction assignment
    constexpr type& operator-=(const std::convertible_to<value_type> auto& x)
    requires concepts::dimensionless_quantity<type> {
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
    constexpr auto operator+(const concepts::quantity_compatible<type> auto q) const {
      const auto value = get_value() + q.template convert<unit_type>().get_value();
      return quantity<unit_type, decltype(value)>(value);
    }

    // subtraction with quantity of compatible unit
    constexpr auto operator-(const concepts::quantity_compatible<type> auto q) const {
      const auto value = get_value() - q.template convert<unit_type>().get_value();
      return quantity<unit_type, decltype(value)>(value);
    }

    // multiplication by another quantity
    constexpr auto operator*(const concepts::quantity auto q) const {
      using resulting_unit = unit_multiply<unit_type, typename decltype(q)::unit_type>;
      const auto value = get_value() * q.get_value();
      return quantity<resulting_unit, decltype(value)>(value);
    }

    // division by incompatible quantity
    constexpr auto operator/(const concepts::quantity_incompatible<type> auto q) const {
      // specialization for the division of incompatible quantites. in this case,
      // the resulting unit is given by the division of quantities' units,
      // including the ratio. this means that the result can be multiplied by another
      // quantity of the same unit as q and the unit of this will be recovered
      // first, we determine the resulting unit
      using resulting_unit = unit_divide<unit_type, typename decltype(q)::unit_type>;
      // then, we compute the value
      const auto value = get_value() / q.get_value();
      // next, create a quantity with a value type matching the one deduced for value
      return quantity<resulting_unit, decltype(value)>(value);
    }

    // division by compatible quantity
    constexpr auto operator/(const concepts::quantity_compatible<type> auto q) const {
      // specialization for the division of compatible quantites, whose result is a
      // dimensionless quantity. in this case, we simply return a number that also
      // takes into account the division of the factors carried by each quantities unit.
      // first, we divide the factors carried by the units
      using factor = ratio_divide<typename unit_type::factor, typename decltype(q)::unit_type::factor>;
      // then, proceed to compute the division
      return (get_value() * factor::num) / (q.get_value() * factor::den);
    }

    // multiplication by arithmetic type
    constexpr auto operator*(const concepts::arithmetic auto x) const {
      return quantity<unit_type, decltype(get_value()*x)>(get_value()*x);
    }

    // division by arithmetic type
    constexpr auto operator/(const concepts::arithmetic auto x) const {
      return quantity<unit_type, decltype(get_value()/x)>(get_value()/x);
    }

    // modulo operation by integral type (only for integral quantities)
    constexpr auto operator%(const std::integral auto i) const
    requires concepts::integral_quantity<type> {
      return quantity<unit_type, decltype(get_value()%i)>(get_value()%i);
    }

    // modulo operation by another integral quantity
    constexpr auto operator%(const concepts::integral_quantity auto q) const
    requires concepts::integral_quantity<type> {
      return *this - (*this/q) * q;
    }

    // * comparison

    constexpr bool operator==(const concepts::quantity auto q) const {
      return get_value() == q.template convert<unit_type>().get_value();
    }

    constexpr bool operator!=(const concepts::quantity auto q) const {
      return get_value() != q.template convert<unit_type>().get_value();
    }

    constexpr bool operator<(const concepts::quantity auto q) const {
      return get_value() < q.template convert<unit_type>().get_value();
    }

    constexpr bool operator>(const concepts::quantity auto q) const {
      return get_value() > q.template convert<unit_type>().get_value();
    }

    constexpr bool operator<=(const concepts::quantity auto q) const {
      return get_value() <= q.template convert<unit_type>().get_value();
    }

    constexpr bool operator>=(const concepts::quantity auto q) const {
      return get_value() >= q.template convert<unit_type>().get_value();
    }

  };

  // - multiplication by arithmetic type from lhs

  constexpr auto operator*(const concepts::arithmetic auto x, const concepts::quantity auto q) {
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
