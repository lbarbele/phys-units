#ifndef _include_units_details_macros_h
#define _include_units_details_macros_h

#include <type_traits>

// - empty type used to assert if macros are being used in the correct namespace

namespace units {
  struct ns_assert;
}

// - helper macros
#define units_macro_concat(A, B) A ## B
#define units_macro_expand_concat(A, B) units_macro_concat(A, B)

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, _a, _b, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, _a, _b, __VA_ARGS__)))

#define FOR_EACH_HELPER(macro, _a, _b, _c, ...)                         \
  macro(_a, _b, _c)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, _a, _b, __VA_ARGS__))

#define FOR_EACH_AGAIN() FOR_EACH_HELPER

// - unit definition macros

// macro that asserts units are declared in the correct namespace
#define units_assert_namespace \
  struct ns_assert; \
  static_assert(\
    std::is_same_v<ns_assert, ::units::ns_assert>, \
    "Units macros can only be used within the ::units namespace" );

// macro to define the symbol of a unit, which is for printout
#define units_set_symbol(_unit_, _symbol_) units_assert_namespace \
  template <> constexpr inline auto _details::_unit::symbol<_unit_> = _details::string(#_symbol_);

// macro to create a literal for the given unit (a type)
# define units_set_literal(_unit_, _symbol_) units_assert_namespace \
  namespace literals { \
    constexpr auto operator ""_ ## _symbol_ (long double value) \
    {return _details::quantity<_symbol_>(value);} \
    constexpr auto operator ""_ ## _symbol_ (unsigned long long value) \
    {return _details::quantity<_symbol_>(value);} \
  }

// abbreviations of standard prefixes
#define units_femto_abbrev f
#define units_pico_abbrev  p
#define units_nano_abbrev  n
#define units_micro_abbrev u
#define units_milli_abbrev m
#define units_centi_abbrev c
#define units_deci_abbrev  d
#define units_deca_abbrev  da
#define units_hecto_abbrev h
#define units_kilo_abbrev  k
#define units_mega_abbrev  M
#define units_giga_abbrev  G
#define units_tera_abbrev  T
#define units_peta_abbrev  P
#define units_exa_abbrev   E

// set single prefix to given units with symbol
#define units_set_prefix(_unit_, _symbol_, _prefix_) units_assert_namespace \
  static_assert(std::is_same_v<_unit_, _symbol_>); \
  units_add_derived_unit(_prefix_ ## _unit_, units_macro_expand_concat(units_ ## _prefix_ ## _abbrev, _symbol_), make_unit<_details::_prefix_, _unit_>)

#define units_set_prefixes(_unit_, _symbol_, ...) \
  FOR_EACH(units_set_prefix, _unit_, _symbol_, __VA_ARGS__)

// create aliases for a base unit with given id, then sets its symbol and creates
// a literal operator
# define units_add_base_unit(_id_, _name_, _symbol_) units_assert_namespace \
  using _name_   = _details::base_unit<_id_>; \
  using _symbol_ = _name_; \
  units_set_symbol(_name_, _symbol_); \
  units_set_literal(_name_, _symbol_)

// create aliases for a derived unit, then set its symbol and create a literal operator
# define units_add_derived_unit(_name_, _symbol_, ...) units_assert_namespace \
  using _name_   = __VA_ARGS__; \
  using _symbol_ = _name_; \
  units_set_symbol(_name_, _symbol_); \
  units_set_literal(_name_, _symbol_)

// create base unit, then create prefixes for it
# define units_add_base_unit_with_prefixes(_id_, _name_, _symbol_) units_assert_namespace \
  units_add_base_unit(_id_, _name_, _symbol_) \
  units_add_prefixes(_name_, _symbol_)

// create derived unit, then create prefixes for it
# define units_add_derived_unit_with_prefixes(_name_, _symbol_, ...) units_assert_namespace \
  units_add_derived_unit(_name_, _symbol_, __VA_ARGS__) \
  units_add_prefixes(_name_, _symbol_)


#endif