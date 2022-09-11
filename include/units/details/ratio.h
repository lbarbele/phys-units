#ifndef _include_units_details_ratio_h
#define _include_units_details_ratio_h

#include <algorithm>
#include <array>
#include <concepts>
#include <initializer_list>
#include <numeric>
#include <string_view>
#include <type_traits>

namespace units::_details {

  // - alias for the maximum representable integer type

  using intm_t = __int128_t;

  // * also create a literal so numbers can be written down

  template <char... cs>
  constexpr __int128_t operator""_imax() {
    constexpr std::array<char, sizeof...(cs)> chars = {cs...};

    constexpr auto get_num = [](char c){
      constexpr std::string_view str = "0123456789";
      return (c == '\'') ? 0 : str.find_first_of(c);
    };

    static_assert(std::all_of(chars.begin(), chars.end(), [&](char c){return get_num(c) < 10;}));

    __int128_t ret = 0;
    for (const auto c : chars) {
      if (c != '\'')
        ret = 10*ret + get_num(c);
    }
    return ret;
  }

  // * extend the concept of integral type to support intm_t

  namespace concepts {
    template <class T>
    concept integral = std::integral<T> || std::same_as<T, intm_t>;
  }

  // * implementation of a gcd function supporting the intm_t type

  constexpr auto gcd(concepts::integral auto a, concepts::integral auto b, concepts::integral auto... is)
  -> std::common_type_t<decltype(a), decltype(b)> {
    if constexpr (sizeof...(is) == 0) {
      if (a == 0 && b == 0)
        return 0;

      a = (a<0)? -a : a;
      b = (b<0)? -b : b;

      if (b == 0)
        return a;

      if (a == 0)
        return b;

      return gcd(b%a, a);
    } else {
      return gcd(gcd(a, b), is...);
    }
  }

  // - forward declarations and aliases

  template <intm_t n, intm_t d = 1> struct ratio;

  template <intm_t n, intm_t d = 1>
  using ratio_t = typename ratio<n, d>::type;

  inline namespace prefixes {
    using yocto = ratio<1, 1'000'000'000'000'000'000'000'000_imax>;
    using zepto = ratio<1, 1'000'000'000'000'000'000'000_imax>;
    using atto  = ratio<1, 1'000'000'000'000'000'000>;
    using femto = ratio<1, 1'000'000'000'000'000>;
    using pico  = ratio<1, 1'000'000'000'000>;
    using nano  = ratio<1, 1'000'000'000>;
    using micro = ratio<1, 1'000'000>;
    using milli = ratio<1, 1'000>;
    using centi = ratio<1, 100>;
    using deci  = ratio<1, 10>;
    using one   = ratio<1, 1>;
    using deca  = ratio<10, 1>;
    using hecto = ratio<100, 1>;
    using kilo  = ratio<1'000, 1>;
    using mega  = ratio<1'000'000, 1>;
    using giga  = ratio<1'000'000'000, 1>;
    using tera  = ratio<1'000'000'000'000, 1>;
    using peta  = ratio<1'000'000'000'000'000, 1>;
    using exa   = ratio<1'000'000'000'000'000'000, 1>;
    using zetta = ratio<1'000'000'000'000'000'000'000_imax, 1>;
    using yotta = ratio<1'000'000'000'000'000'000'000'000_imax, 1>;
  }

  // - traits

  namespace traits {
    // * assert type is a ratio
    template <class T>
    struct is_ratio : std::false_type {};

    template <intm_t num, intm_t den>
    struct is_ratio<ratio<num, den>> : std::true_type {};

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

  // * ratio sum
  template <concepts::ratio A, concepts::ratio B>
  using ratio_add = ratio_t<(A::num*B::den + A::den*B::num), A::den * B::den>;

  // * ratio subtraction
  template <concepts::ratio A, concepts::ratio B>
  using ratio_subtract = ratio_t<(A::num*B::den - A::den*B::num), A::den * B::den>;

  // * ratio multiplication
  namespace _ratio {
    template <concepts::ratio A, concepts::ratio B>
    struct multiply {
      using a = ratio_t<A::num, B::den>;
      using b = ratio_t<B::num, A::den>;
      using type = ratio_t<a::num*b::num, a::den*b::den>;
    };
  }

  template <concepts::ratio A, concepts::ratio B>
  using ratio_multiply = typename _ratio::multiply<A, B>::type;

  // * ratio division
  template <concepts::ratio A, concepts::ratio B>
  using ratio_divide = ratio_multiply<A, ratio<B::den, B::num>>;

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
  struct ratio {
    static_assert(d != 0, "Denominator cannot be zero.");

    static constexpr intm_t div = gcd(n, d);
    static constexpr intm_t num = (d > 0)? n/div : -n/div;
    static constexpr intm_t den = (d > 0)? d/div : -d/div;

    using type = ratio<num, den>;
    using invert = ratio<den, num>;

    template <std::floating_point T>
    static constexpr T value{static_cast<T>(num)/static_cast<T>(den)};
  };
}

#endif