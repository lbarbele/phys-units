#ifndef _include_units_details_tuple_h
#define _include_units_details_tuple_h

#include <algorithm>
#include <tuple>

#include <units/details/power.h>

namespace units::_details {
  // - tuple
  using std::tuple;

  // - tuple access element by index
  using std::tuple_element;
  using std::tuple_element_t;

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

  // - remove first indexed type matching index
  template <unsigned i, class A, class B = tuple<>> struct tuple_remove_first_index;

  template <unsigned i, class T, class... Ts, class... Us>
  struct tuple_remove_first_index<i, tuple<T, Ts...>, tuple<Us...>> {
    using type = std::conditional_t<
      power<T>::base::index == i,
      tuple<Us..., Ts...>,
      typename tuple_remove_first_index<i, tuple<Ts...>, tuple<Us..., T>>::type
    >;

    using removed_type = std::conditional_t<
      power<T>::base::index == i,
      T,
      typename tuple_remove_first_index<i, tuple<Ts...>, tuple<Us..., T>>::removed_type
    >;
  };

  template <unsigned i, class... Us>
  struct tuple_remove_first_index<i, tuple<>, tuple<Us...>> {
    using type = tuple<Us...>;
    using removed_type = void;
  };

  template <unsigned i, class T>
  using tuple_remove_first_index_t = typename tuple_remove_first_index<i, T>::type;

  // - sort powers of indexed bases
  template <class T, class U = tuple<>> struct tuple_sort_indexed;

  template <class... Us>
  struct tuple_sort_indexed<tuple<>, tuple<Us...>> {
    using type = tuple<Us...>;
  };

  template <class T, class... Ts, class... Us>
  struct tuple_sort_indexed<tuple<T, Ts...>, tuple<Us...>> {
    static constexpr unsigned min = std::min({power_t<T>::base::index, power_t<Ts>::base::index...});
    using next = typename tuple_remove_first_index<min, tuple<T, Ts...>>::removed_type;
    using remaining = tuple_remove_first_index_t<min, tuple<T, Ts...>>;
    using type = typename tuple_sort_indexed<remaining, tuple<Us..., next>>::type;
  };

  template <class T>
  using tuple_sort_indexed_t = typename tuple_sort_indexed<T>::type;
}

#endif