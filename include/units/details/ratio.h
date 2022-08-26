#ifndef _include_units_details_ratio_h
#define _include_units_details_ratio_h

#include <concepts>
#include <cstdint>
#include <ratio>
#include <type_traits>

namespace units::_details {

  // - alias for the maximum representable integer type

  using intm_t = std::intmax_t;

  // - forward declarations and aliases

  template <intm_t n, intm_t d = 1> struct ratio;

  template <intm_t n, intm_t d = 1>
  using ratio_t = typename ratio<n, d>::type;

  inline namespace prefixes {
    using atto  = ratio<1, 1000000000000000000>;
    using femto = ratio<1, 1000000000000000>;
    using pico  = ratio<1, 1000000000000>;
    using nano  = ratio<1, 1000000000>;
    using micro = ratio<1, 1000000>;
    using milli = ratio<1, 1000>;
    using centi = ratio<1, 100>;
    using deci  = ratio<1, 10>;
    using one   = ratio<1, 1>;
    using deca  = ratio<10, 1>;
    using hecto = ratio<100, 1>;
    using kilo  = ratio<1000, 1>;
    using mega  = ratio<1000000, 1>;
    using giga  = ratio<1000000000, 1>;
    using tera  = ratio<1000000000000, 1>;
    using peta  = ratio<1000000000000000, 1>;
    using exa   = ratio<1000000000000000000, 1>;
  }

  // - traits

  namespace traits {
    // * assert type is a ratio
    template <class T>
    struct is_ratio : std::false_type {};

    template <class T>
    requires std::is_integral_v<decltype(T::num)> && std::is_integral_v<decltype(T::den)>
    struct is_ratio<T> : std::true_type {};

    template <class T>
    constexpr inline bool is_ratio_v = is_ratio<T>::value;

    // * assert ratio is in reduced form
    template <class T>
    struct is_reduced_ratio : std::false_type {};

    template <class T>
    requires traits::is_ratio_v<T> && std::is_same_v<T, typename T::type>
    struct is_reduced_ratio<T> : std::true_type {};

    template <class T>
    constexpr inline bool is_reduced_ratio_v = is_reduced_ratio<T>::value;
  }

  // - concepts

  namespace concepts {
    // * types that can represent ratios
    template <class T>
    concept ratio = traits::is_ratio_v<T>;

    // * ratio is already reduced
    template <class T>
    concept reduced_ratio = traits::is_reduced_ratio_v<T>;

    // * ratio represents integral number
    template <class T>
    concept integral_ratio = traits::is_ratio_v<T> && (T::num%T::den == 0);
  }

  // - ratio operations

  // * actual implementation of the basic operations
  namespace _ratio {
    template <template<concepts::ratio, concepts::ratio> class Op, concepts::ratio A, concepts::ratio B>
    struct operation {
      using result = Op<A, B>;
      using type = typename ratio<result::num, result::den>::type;
    };
  }

  // * ratio sum
  template <concepts::ratio A, concepts::ratio B>
  using ratio_add = typename _ratio::operation<std::ratio_add, A, B>::type;

  // * ratio subtraction
  template <concepts::ratio A, concepts::ratio B>
  using ratio_subtract = typename _ratio::operation<std::ratio_subtract, A, B>::type;

  // * ratio multiplication
  template <concepts::ratio A, concepts::ratio B>
  using ratio_multiply = typename _ratio::operation<std::ratio_multiply, A, B>::type;

  // * ratio division
  template <concepts::ratio A, concepts::ratio B>
  using ratio_divide = typename _ratio::operation<std::ratio_divide, A, B>::type;

  // * implementation of the ratio power
  namespace _ratio {
    // * ratio power with integer exponent
    template <concepts::ratio R, intm_t exponent>
    struct rpower;

    // null exponent
    template <concepts::ratio R>
    struct rpower<R, 0> {
      using type = ratio<1, 1>;
    };

    // unity exponent
    template <concepts::ratio R>
    struct rpower<R, 1> {
      using type = typename R::type;
    };

    // negative exponent
    template <concepts::ratio R, intm_t exponent>
    requires (exponent < 0)
    struct rpower<R, exponent> {
      using type = typename rpower<typename R::invert, -exponent>::type;
    };

    // even, positive exponent
    template <concepts::ratio R, intm_t exponent>
    requires (exponent > 1 && exponent%2 == 0)
    struct rpower<R, exponent> {
      using half_p = typename rpower<R, exponent/2>::type;
      using type = ratio_multiply<half_p, half_p>;
    };

    // odd, positive exponent
    template <concepts::ratio R, intm_t exponent>
    requires (exponent > 1 && exponent%2 != 0)
    struct rpower<R, exponent> {
      using type = ratio_multiply<R, typename rpower<R, exponent-1>::type>;
    };
  }

  // * integer power of a ratio
  template <concepts::ratio R, intm_t exponent>
  using ratio_power = typename _ratio::rpower<R, exponent>::type;

  // - definition of the ratio struct

  template <intm_t n, intm_t d>
  struct ratio : std::ratio<n, d> {
    using std::ratio<n, d>::num;
    using std::ratio<n, d>::den;
    
    using type = ratio<num, den>;
    using invert = ratio<den, num>;

    template <concepts::ratio R> using add = ratio_add<type, R>;
    template <concepts::ratio R> using subtract = ratio_subtract<type, R>;
    template <concepts::ratio R> using multiply = ratio_multiply<type, R>;
    template <concepts::ratio R> using divide = ratio_divide<type, R>;
    template <intm_t exponent> using pow = ratio_power<type, exponent>;
    
    template <std::floating_point T> static constexpr T value{static_cast<T>(num)/static_cast<T>(den)};
  };
}

#endif