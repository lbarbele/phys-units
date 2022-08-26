#ifndef _include_units_details_power_h
#define _include_units_details_power_h

#include <cstdint>

#include <units/details/ratio.h>
#include <units/details/traits.h>

namespace units::_details {

  // - forward declarations and aliases

  template <class T, intm_t num = 1, intm_t den = 1>
  struct power;

  template <class T, intm_t num = 1, intm_t den = 1>
  using power_t = typename power<T, num, den>::type;

  inline namespace common_powers {
    template <class T> using power_null = power_t<T, 0>;
    template <class T> using inverse = power_t<T, -1>;
    template <class T> using squared = power_t<T, 2>;
    template <class T> using inverse_squared = power_t<T, -2>;
  }

  // - traits

  namespace traits {
    // * assert type is a power
    template <class T>
    struct is_power : std::false_type {};

    template <class T, intm_t num, intm_t den>
    struct is_power<power<T, num, den>> : std::true_type {};

    template <class T>
    constexpr inline bool is_power_v = is_power<T>::value;

    // * assert is power and has reduced exponent
    template <class T>
    struct is_reduced_power : std::false_type {};

    template <class T>
    requires is_power_v<T> && std::is_same_v<typename T::exponent, typename T::exponent::type>
    struct is_reduced_power<T> : std::true_type {};

    template <class T>
    constexpr inline bool is_reduced_power_v = is_reduced_power<T>::value;

    // * assert powers share the same base
    template <class T, class U>
    struct is_compatible_power : std::false_type {};

    template <class T, class U>
    requires std::is_same_v<typename power<T>::base, typename power<U>::base>
    struct is_compatible_power<T, U> : std::true_type {};

    template <class T, class U>
    constexpr inline bool is_compatible_power_v = is_compatible_power<T, U>::value;
  }

  // - concepts

  namespace concepts {
    // * type is a power
    template <class T>
    concept power = traits::is_power_v<T>;

    // * type is a power with reduced exponent (ratio)
    template <class T>
    concept reduced_power = traits::is_reduced_power_v<T>;

    // * power bases are compatible
    template <class T, class U>
    concept compatible_power = traits::is_compatible_power_v<T, U>;
  }

  // - definition of the power class

  // * base definition
  template <class T, intm_t num, intm_t den>
  struct power {
    using base = T;
    using exponent = ratio_t<num, den>;
    using type = power<base, exponent::num, exponent::den>;
  };

  // * specialization for the power of a power
  template <concepts::power P, intm_t num, intm_t den>
  struct power<P, num, den> {
    using base = typename P::base;
    using exponent = ratio_multiply<ratio<num, den>, typename P::exponent>;
    using type = power<base, exponent::num, exponent::den>;
  };

  // - multiplication of powers (requires compatible base!)

  namespace _power {
    // * multiplication
    template <class P, concepts::compatible_power<P>... Ps>
    struct multiply;

    template <class P>
    struct multiply<P> {
      using type = typename power<P>::type;
    };

    template <class A, concepts::compatible_power<A> B>
    struct multiply<A, B> {
      using base = typename power<A>::base;
      using exponent = ratio_add<typename power<A>::exponent, typename power<B>::exponent>;
      using type = power<base, exponent::num, exponent::den>;
    };

    template <class T, concepts::compatible_power<T> U, concepts::compatible_power<T>... Us>
    struct multiply<T, U, Us...>
    : multiply<typename multiply<T, U>::type, Us...> {};
  }

  template <class P, concepts::compatible_power<P>... Ps>
  using power_multiply = typename _power::multiply<P, Ps...>::type;

}

#endif