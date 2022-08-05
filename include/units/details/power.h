#ifndef _include_units_details_power_h
#define _include_units_details_power_h

#include <cstdint>

#include <units/details/ratio.h>
#include <units/details/traits.h>

namespace units::_details::_power {

  // - definition of powers

  // definition of a power of a type
  template <class T, class R = ratio<1>>
  struct power : tags::power {
    static_assert(traits::is_ratio_v<R>, "The exponent of a power must be a ratio.");
    using base = T;
    using exponent = typename R::type;
    using type = power<base, exponent>;
  };

  // specialization for a power of a power
  template <class T, class Ra, class Rb>
  struct power<power<T, Ra>, Rb>
  : power<T, ratio_multiply<Ra, Rb>> {};

  // - multiplication of powers

  // general case
  template <class Pa, class Pb, class...Ps>
  struct power_multiply : power_multiply<typename power_multiply<Pa, Pb>::type, Ps...> {};

  // power multiply as a binary operator
  template <class Pa, class Pb>
  struct power_multiply<Pa, Pb>
  : power_multiply<typename power<Pa>::type, typename power<Pb>::type> {};

  template <class Ta, class Tb, class Ra, class Rb>
  struct power_multiply<power<Ta, Ra>, power<Tb, Rb>> {
    static_assert(std::is_same_v<Ta, Tb>, "Cannot multiply powers of different bases.");
    using type = typename power<Ta, ratio_add<Ra, Rb>>::type;
  };

}

namespace units::_details {
  // alias for a power defined in terms of numerator and denominator
  template <class T, std::intmax_t num = 1, std::intmax_t den = 1>
  using power = _power::power<T, ratio<num, den>>;

  // alias for the power member typedef type (reduced ratio for the exponent)
  template <class T, std::intmax_t num = 1, std::intmax_t den = 1>
  using power_t = typename power<T, num, den>::type;

  // aliases for some common powers
  template <class T> using power_null = power_t<T, 0>;

  // alias for power multiplication
  template <class Pa, class Pb = power_null<Pa>, class... Ps>
  using power_multiply = typename _power::power_multiply<Pa, Pb, Ps...>::type;
}

#endif