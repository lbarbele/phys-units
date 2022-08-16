#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/tuple.h>

namespace units::_details::_unit {

  // - base unit

  template <unsigned index>
  struct base_unit : tags::base_unit, props::indexed<index> {};

  // - forward-declare the general unit struct

  template <class S, class... Ts> struct unit;

  // - unit creator

  template <class... Ts> struct make_unit;

  // next parameter is raw base unit
  template <intm_t num, intm_t den, class... Ts, unsigned idx, class... Us>
  struct make_unit< ratio<num, den>, tuple<Ts...>, base_unit<idx>, Us... >
  : make_unit< ratio<num, den>, tuple<Ts..., power_t<base_unit<idx>>>, Us... > {};

  // next parameter is scale factor (a ratio)
  template <intm_t na, intm_t da, class... Ts, intm_t nb, intm_t db, class... Us>
  struct make_unit<ratio<na, da>, tuple<Ts...>, ratio<nb, db>, Us...>
  : make_unit<ratio_multiply<ratio<na, da>, ratio<nb, db>>, tuple<Ts...>, Us...> {};

  // next parameter is another unit (not a base unit)
  template <intm_t num, intm_t den, class... Ts, class S, class... Us, class... Vs>
  struct make_unit<ratio<num, den>, tuple<Ts...>, unit<S, Us...>, Vs...>
  : make_unit< ratio_multiply<ratio<num, den>, S>, tuple<Ts..., Us...>, Vs... > {};

  // next parameter is power of raw base unit
  template <intm_t na, intm_t da, class... Ts, unsigned idx, intm_t nb, intm_t db, class... Us>
  struct make_unit< ratio<na, da>, tuple<Ts...>, power< base_unit<idx>, nb, db >, Us... >
  : make_unit< ratio<na, da>, tuple<Ts..., power_t< base_unit<idx>, nb, db > >, Us... > {
    // ! for consistency, we also disallow powers of raw base units here (see below)
    static_assert(db == 1, "Fractional powers of base units are not allowed in the creation of units");
  };

  // next parameter is power of unit
  template <intm_t na, intm_t da, class... Ts, class S, class... Us, intm_t exp_num, intm_t exp_den, class... Vs>
  struct make_unit< ratio<na, da>, tuple<Ts...>, power< unit<S, Us...>, exp_num, exp_den>, Vs...>
  : make_unit< ratio<na, da>, tuple<Ts...>, ratio_power<S, exp_num>, power_t<Us, exp_num>..., Vs... > {
    // ! warning: only integer powers are supported
    // ! the denominator of the exponent is not even considered above
    static_assert(exp_den == 1, "Fractional powers of units are not allowed in the creation of units");
  };

  // base case: scale factor and tuple of powers of base units
  template <intm_t num, intm_t den, class... Ts>
  struct make_unit< ratio<num, den>, tuple<Ts...> > {
    using merged = tuple_merge_powers_t<tuple<Ts...>>;
    using sorted = tuple_sort_indexed_t<merged>;
    using unit_args = tuple_concat<tuple<ratio<num, den>>, sorted>;
    using type = tuple_convert_t<unit_args, unit>;
  };

  // - general unit

  template <class S, class... Ts>
  struct unit {
    static_assert(traits::is_ratio_v<S>, "Scale factor must be a ratio.");
    static_assert(
      std::conjunction_v< traits::is_base_unit<typename power_t<Ts>::base>... >,
      "Derived units can only be defined in terms of base units.");
    using factor = S;
    using units_product = tuple<Ts...>;

    template <class... Us> using times = typename make_unit<factor, units_product, Us...>::type;
    template <class... Us> using over = typename make_unit<factor, units_product, inverse<Us>...>::type;
  };
}

namespace units::_details {
  using _unit::base_unit;

  template <class... Ts>
  using make_unit = typename _unit::make_unit<ratio<1, 1>, tuple<>, Ts...>::type;
}

#endif