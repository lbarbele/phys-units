#ifndef _include_units_details_quantity_h
#define _include_units_details_quantity_h

#include <units/details/power.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

namespace units::_details {

  template <class U>
  struct quantity {

    static_assert(
      traits::is_unit_v<U> || traits::is_base_unit_v<U>,
      "quantity<U> requires a unit");

  private:
    double m_value;

  public:
    using unit = make_unit<U>;
    using type = quantity<unit>;

    constexpr quantity(const double value = 0) : m_value(value) {};

    constexpr inline const double& get_value() const {return m_value;}

    // - unit (type) conversion

    template <class... Us>
    constexpr inline auto convert() const {
      // unit to which this quantity is being converted to
      using other_unit = make_unit<Us...>;
      // assert that this' unit is compatible with the target unit
      static_assert(traits::is_compatible_unit_v<unit, other_unit>, "Cannot perform conversion between units that are not compatible");
      // compute the conversion factor as a ratio
      using conversion_factor = ratio_divide<typename unit::factor, typename other_unit::factor>;
      // compute the quantity's value in the other unit
      const double new_value = m_value * double(conversion_factor::num)/double(conversion_factor::den);
      // return a quantity in the new units
      return quantity<other_unit>(new_value);
    }

    template <class V>
    constexpr operator quantity<V>() const {
      return convert<V>();
    }

    // dimensionless quantities are implicitly convertible to double
    template <class T, std::enable_if_t<std::is_same_v<double, T> && std::is_same_v<unit, make_unit<>>, bool> = true>
    constexpr operator T() const {
      return m_value;
    }

    // - assignment operations

    // simple assignment to quantity of compatible unit
    template <class V>
    constexpr inline type& operator=(const quantity<V>& other) {
      m_value = type(other).get_value();
      return *this;
    }

    // addition assignemnt with quantity of compatible unit
    template <class V>
    constexpr inline type& operator+=(const quantity<V>& other) {
      m_value += type(other).get_value();
      return *this;
    }

    // subtraction assignment with quantity of compatible unit
    template <class V>
    constexpr inline type& operator-=(const quantity<V>& other) {
      m_value -= type(other).get_value();
      return *this;
    }

    // multiplication assignment by dimensionless quantity
    template <class T>
    constexpr inline std::enable_if_t<std::is_convertible_v<T, double>, type&> operator*=(const T& m) {
      m_value *= double(m);
      return *this;
    }

    // division assignment by dimensionless quantity
    template <class T>
    constexpr inline std::enable_if_t<std::is_convertible_v<T, double>, type&> operator/=(const T& m) {
      m_value /= double(m);
      return *this;
    }

    // - increment/decrement operations

    constexpr inline type& operator++() {
      ++m_value;
      return *this;
    }

    constexpr inline type& operator--() {
      --m_value;
      return *this;
    }

    constexpr inline type operator++(int) {
      return type(m_value++);
    }

    constexpr inline type operator--(int) {
      return type(m_value--);
    }

    // - arithmetic operations

    // unary plus
    constexpr inline type operator+() const {
      return type(+get_value());
    };

    // unary minus
    constexpr inline type operator-() const {
      return type(-get_value());
    };

    // addition
    template <class V>
    constexpr inline type operator+(const quantity<V>& other) const {
      return type(get_value() + type(other).get_value());
    }

    // subtraction
    template <class V>
    constexpr inline type operator-(const quantity<V>& other) const {
      return type(get_value() - type(other).get_value());
    }

    // multiplication
    template <class V>
    constexpr inline auto operator*(const quantity<V>& other) const {
      using resulting_unit = make_unit<unit, typename quantity<V>::unit>;
      return quantity<resulting_unit>(get_value() * other.get_value());
    }

    // division
    template <class V>
    constexpr inline auto operator/(const quantity<V>& other) const {
      using resulting_unit = make_unit<unit, inverse<typename quantity<V>::unit>>;
      return quantity<resulting_unit>(get_value() / other.get_value());
    }

    // multiplication by dimensionless quantity
    template <class T>
    constexpr inline std::enable_if_t<std::is_convertible_v<T, double>, type> operator*(const T& m) {
      return type(get_value() * double(m));
    }

    // division by dimensionless quantity
    template <class T>
    constexpr inline std::enable_if_t<std::is_convertible_v<T, double>, type> operator/(const T& m) {
      return type(get_value() / double(m));
    }

    // - comparison

    template <class V>
    constexpr inline bool operator==(const quantity<V>& other) const {
      return get_value() == type(other).get_value();
    };

    template <class V>
    constexpr inline bool operator!=(const quantity<V>& other) const {
      return get_value() != type(other).get_value();
    };

    template <class V>
    constexpr inline bool operator<(const quantity<V>& other) const {
      return get_value() < type(other).get_value();
    };

    template <class V>
    constexpr inline bool operator>(const quantity<V>& other) const {
      return get_value() > type(other).get_value();
    };

    template <class V>
    constexpr inline bool operator<=(const quantity<V>& other) const {
      return get_value() <= type(other).get_value();
    };

    template <class V>
    constexpr inline bool operator>=(const quantity<V>& other) const {
      return get_value() >= type(other).get_value();
    };

  };

  // multiplication by dimensionless from lhs
  template <class T, class U>
  constexpr inline auto operator*(const T& m, const quantity<U>& q)
  -> std::enable_if_t< std::is_convertible_v<T, double>, quantity<U> > {
    return quantity<U>(q.get_value() * double(m));
  };

  template <class... Us>
  using quantity_t = quantity< make_unit<Us...> >;

}

#endif
