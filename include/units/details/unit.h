#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <array>
#include <string_view>

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/string.h>
#include <units/details/traits.h>
#include <units/details/tuple.h>

namespace units::_details {

  namespace _unit {
    // - summary/forward declarations
    template <class... Ts> struct unit;
    template <class... Ts> struct unit_tag;
    template <class... Ts> struct unit_create;
    template <unsigned id> struct unit_indexed;

    // - helper aliases
    using one = ratio<1, 1>;
    
    template <unsigned id, intm_t num = 1, intm_t den = 1>
    using unit_indexed_power = power<unit_indexed<id>, num, den>;
    
    template <unsigned id> 
    using base_unit = unit<one, unit_indexed_power<id>>;

    // - the indexed unit: appends an index to a base unit
    template <unsigned id> struct unit_indexed : props::indexed<id>, base_unit<id> {};

    // - tag for the unit class
    template <intm_t num, intm_t den>
    struct unit_tag<ratio<num, den>>
    : tags::dimensionless_unit {};

    template <unsigned id>
    struct unit_tag<one, unit_indexed_power<id>>
    : tags::base_unit {};

    template <intm_t num, intm_t den, class T, class... Ts>
    struct unit_tag<ratio<num, den>, T, Ts...>
    : tags::derived_unit {};

    // - name/symbol for the unit class
    template <class T> constexpr inline string unit_name = "?";
    template <class T> constexpr inline string unit_symbol = "?";

    // create a string from the multiplicative factor
    template <intm_t num, intm_t den>
    constexpr inline string str_factor =
      (num == 1 && den == 1)? "" :
      (den == 1)? join_v<schar<'*'>, schar<' '>, to_string_v<num>> :
      join_v<schar<'*'>, schar<' '>, schar<'('>, to_string_v<num>, schar<'/'>, to_string_v<den>, schar<')'>>;

    // string for a power using numerador and denominator of exponent
    template <intm_t num, intm_t den>
    constexpr inline string str_pow = 
      (num == 1 && den == 1)? "" :
      (den == 1)? join_v<schar<'^'>, to_string_v<num>> :
      join_v<schar<'^'>, schar<'('>, to_string_v<num>, schar<'/'>, to_string_v<den>, schar<')'>>;

    // string of base unit name raised to given power
    template <unsigned id, intm_t num, intm_t den>
    constexpr inline string str_name_pow = join_v<unit_name<base_unit<id>>, str_pow<num, den>>;

    // string of base unit symbol raised to given power
    template <unsigned id, intm_t num, intm_t den>
    constexpr inline string str_symbol_pow = join_v<unit_symbol<base_unit<id>>, str_pow<num, den>>;

    // - unit class
    template <intm_t num, intm_t den, unsigned... ids, intm_t... nums, intm_t... dens>
    struct unit< ratio<num, den>, unit_indexed_power<ids, nums, dens>... >
    : unit_tag<ratio<num, den>, unit_indexed_power<ids, nums, dens>... > {
      using without_prefix = unit<one, unit_indexed_power<ids, nums, dens>...>;
      using type = unit;
      using factor = ratio<num, den>;
      using powers = tuple<unit_indexed_power<ids, nums, dens>...>;

      static constexpr string name =
        // name for this unit is explicitly defined
        (unit_name<type> != "?")? unit_name<type> :
        // explicit name for the corresponding unit without the ratio
        (unit_name<without_prefix> != "?")? spaced_join_v<str_factor<num, den>, unit_name<without_prefix>> :
        // can only write unit in terms of its base units
        spaced_join_v<str_factor<num, den>, str_name_pow<ids, nums, dens>...>;

      static constexpr string symbol =
        // symbol for this unit is explicitly defined
        (unit_symbol<type> != "?")? unit_symbol<type> :
        // explicit symbol for the corresponding unit without the ratio
        (unit_symbol<without_prefix> != "?")? spaced_join_v<str_factor<num, den>, unit_symbol<without_prefix>> :
        // can only write unit in terms of its base units
        spaced_join_v<str_factor<num, den>, str_symbol_pow<ids, nums, dens>...>;

      static_assert(traits::is_sorted_and_unique_v<ids...>);
      static_assert(((nums != 0) && ...));
      static_assert(((dens != 0) && ...));
    };

    // - unit creator
    // next parameter is a ratio
    template <intm_t na, intm_t da, class... Ts, intm_t nb, intm_t db, class... Us>
    struct unit_create<ratio<na, da>, tuple<Ts...>, ratio<nb, db>, Us...>
    : unit_create< ratio_multiply<ratio<na, da>, ratio<nb, db>>, tuple<Ts...>, Us...> {};

    // next parameter is a unit
    template <intm_t num, intm_t den, class... Ts, class S, class... Us, class... Vs>
    struct unit_create<ratio<num, den>, tuple<Ts...>, unit<S, Us...>, Vs...>
    : unit_create< ratio_multiply<ratio<num, den>, S>, tuple<Ts..., Us...>, Vs... > {};

    // next parameter is (integer) power of unit
    template <intm_t na, intm_t da, class... Ts, class S, class... Us, intm_t exp_num, intm_t exp_den, class... Vs>
    struct unit_create< ratio<na, da>, tuple<Ts...>, power< unit<S, Us...>, exp_num, exp_den>, Vs...>
      : unit_create< ratio<na, da>, tuple<Ts...>, unit<ratio_power<S, exp_num>, power_t<Us, exp_num>...>, Vs... > {
      static_assert(exp_den == 1, "Fractional powers of units are not allowed in the creation of units");
    };

    // base case
    template <intm_t num, intm_t den, class... Ts>
    struct unit_create< ratio<num, den>, tuple<Ts...> > {
      using merged = tuple_merge_powers_t<tuple<Ts...>>;
      using sorted = tuple_sort_indexed_t<merged>;
      using unit_args = tuple_concat<tuple<ratio<num, den>>, sorted>;
      using type = tuple_convert_t<unit_args, unit>;
    };
  }

  // * general unit class
  using _unit::unit;

  // * alias for base units (parametrized by id, which is of type uint)
  using _unit::base_unit;

  // * helper type that creates units
  template <class... Ts>
  using make_unit = typename _unit::unit_create<ratio<1, 1>, tuple<>, Ts...>::type;
}

#endif