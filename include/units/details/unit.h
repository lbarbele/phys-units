#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <string_view>

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/tuple.h>

namespace units::_details {

  namespace _unit {

    // - unit names
    template <class T> constexpr inline std::string_view unit_name("?");
    template <class T> constexpr inline std::string_view unit_symbol("?");

    template <class T>
    struct named {
      static constexpr const auto& name = unit_name<T>;
      static constexpr const auto& symbol = unit_symbol<T>;
    };

    // - base unit
    template <unsigned index>
    struct base_unit
    : tags::base_unit, props::indexed<index> {};

    // - general unit
    template <class S, class... Ts>
    struct unit : tags::unit {

      using factor = S;
      using powers = tuple<Ts...>;

      static constexpr const auto& name = unit_name<unit<S, Ts...>>;
      static constexpr const auto& symbol = unit_symbol<unit<S, Ts...>>;

      static_assert(traits::is_ratio_v<S>, "Scale factor must be a ratio.");
      static_assert(std::conjunction_v<traits::is_power<Ts>...>, "Units must be derived in terms of powers.");
      static_assert(std::conjunction_v<traits::is_base_unit<typename power_t<Ts>::base>...>, "Only powers of base units are allowed.");
      static_assert(std::is_same_v<powers, tuple_merge_powers_t<powers>>, "Indices must be unique");
      static_assert(std::is_same_v<powers, tuple_sort_indexed_t<powers>>, "Indices must be ordered");
    };

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
      static_assert(db == 1, "Fractional powers of base units are not allowed in the creation of units");
    };

    // next parameter is power of unit
    template <intm_t na, intm_t da, class... Ts, class S, class... Us, intm_t exp_num, intm_t exp_den, class... Vs>
    struct make_unit< ratio<na, da>, tuple<Ts...>, power< unit<S, Us...>, exp_num, exp_den>, Vs...>
    : make_unit< ratio<na, da>, tuple<Ts...>, ratio_power<S, exp_num>, power_t<Us, exp_num>..., Vs... > {
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

  }

  using _unit::unit;

  template <class... Ts>
  using make_unit = typename _unit::make_unit<ratio<1, 1>, tuple<>, Ts...>::type;

  template <unsigned i>
  using make_base_unit = make_unit<_unit::base_unit<i>>;
}

#endif