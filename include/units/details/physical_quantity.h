#ifndef _include_units_details_physical_quantity_h
#define _include_units_details_physical_quantity_h

#include <units/details/power.h>
#include <units/details/traits.h>
#include <units/details/tuple.h>

namespace units::_details::_physical_quantity {

  // - fundamental quantity

  template <unsigned index>
  struct fundamental_quantity
  : tags::fundamental_quantity,
    props::indexed<index> {};

  // alias to be used only in this NS
  template <unsigned i>
  using fq = fundamental_quantity<i>;

  // - general physical quantity

  template <class... Ts>
  struct physical_quantity {
    static_assert(
      std::conjunction_v< traits::is_fundamental_quantity<typename power_t<Ts>::base>... >,
      "Physical quantities can only be specified in terms of fundamental quantities"
    );
  };

  // - physical quantity creator

  template <class Q, class... Ts>
  struct make_physical_quantity {
    static_assert(traits::is_fundamental_quantity_v<Q>,
      "Only fundamental quantities are allowed!");
  };

  // next parameter is raw fundamental quantity
  template <unsigned index, class... Ts, class... Us>
  struct make_physical_quantity< tuple<Ts...>, fq<index>, Us...>
  : make_physical_quantity< tuple< Ts..., power_t<fq<index>> >, Us...> {};

  // next parameter is another physical quantity
  template <class... Ts, class... Us, class... Vs>
  struct make_physical_quantity< tuple<Ts...>, physical_quantity<Us...>, Vs...>
  : make_physical_quantity< tuple< Ts...>, Us..., Vs...> {};

  // next parameter is power of fundamental quantity
  template <class... Ts, unsigned index, std::intmax_t num, std::intmax_t den, class... Us>
  struct make_physical_quantity< tuple< Ts...>, power<fq<index>, num, den>, Us... >
  : make_physical_quantity< tuple< Ts..., power<fq<index>, num, den>>, Us... > {};

  // base case: Ts... is a list of powers of fundamental quantities
  template <class... Ts>
  struct make_physical_quantity<tuple<Ts...>> {
    using merged = tuple_merge_powers_t<tuple<Ts...>>;
    using sorted = tuple_sort_indexed_t<merged>;
    using type = tuple_convert_t<sorted, physical_quantity>;
  };
}

namespace units::_details {
  using _physical_quantity::fundamental_quantity;

  template <class... Ts>
  using physical_quantity = _physical_quantity::physical_quantity<Ts...>;

  template <class... Ts>
  using physical_quantity_t = typename _physical_quantity::make_physical_quantity<tuple<>, Ts...>::type;
};

#endif