#ifndef _include_units_details_tuple_h
#define _include_units_details_tuple_h

#include <tuple>

#include <units/details/power.h>

namespace units::_details {
  // - tuple
  using std::tuple;

  // - tuple concatenation
  template <class... Ts>
  using tuple_concat = decltype(std::tuple_cat<Ts...>(Ts{}...));

  // - move types of this tuple
  template <class From, template<class...> class To> struct tuple_convert;

  template <class... Ts, template<class...> class T>
  struct tuple_convert<tuple<Ts...>, T> {
    using type = T<Ts...>;
  };

  template <class From, template<class...> class To>
  using tuple_convert_t = typename tuple_convert<From, To>::type;

  // - merge all powers of the same base
  template <class From, class To>
  struct tuple_merge_powers;

  template <class... Ts, class U, class... Us>
  struct tuple_merge_powers<tuple<U, Us...>, tuple<Ts...>> {
    using base = typename power_t<U>::base;

    using remaining = tuple_concat<
      std::conditional_t< std::is_same_v<base, typename power_t<Us>::base>, tuple<>, tuple<Us> >... >;

    using merged = power_multiply< U,
      std::conditional_t< std::is_same_v<base, typename power_t<Us>::base>, Us, power_null<base> >... >;

    using type = typename tuple_merge_powers<remaining, tuple<Ts..., merged>>::type;
  };

  template <class... Ts>
  struct tuple_merge_powers<tuple<>, tuple<Ts...>> {
    using type = tuple<Ts...>;
  };

  template <class TupleType>
  using tuple_merge_powers_t = typename tuple_merge_powers<TupleType, tuple<>>::type;
}

#endif