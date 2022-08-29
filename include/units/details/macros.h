#ifndef _include_units_details_macros_h
#define _include_units_details_macros_h

#include <type_traits>

// empty type used to assert if macros are being used in the correct namespace
namespace units {
  struct ns_assert;
}

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

// add prefixes to given unit
# define units_add_prefixes(_unit_, _symbol_) units_assert_namespace \
  units_add_derived_unit(femto ## _unit_,  f ## _symbol_, _details::make_unit<_details::femto, _unit_>) \
  units_add_derived_unit( pico ## _unit_,  p ## _symbol_, _details::make_unit<_details::pico,  _unit_>) \
  units_add_derived_unit( nano ## _unit_,  n ## _symbol_, _details::make_unit<_details::nano,  _unit_>) \
  units_add_derived_unit(micro ## _unit_,  u ## _symbol_, _details::make_unit<_details::micro, _unit_>) \
  units_add_derived_unit(milli ## _unit_,  m ## _symbol_, _details::make_unit<_details::milli, _unit_>) \
  units_add_derived_unit(centi ## _unit_,  c ## _symbol_, _details::make_unit<_details::centi, _unit_>) \
  units_add_derived_unit( deci ## _unit_,  d ## _symbol_, _details::make_unit<_details::deci,  _unit_>) \
  units_add_derived_unit( deca ## _unit_, da ## _symbol_, _details::make_unit<_details::deca,  _unit_>) \
  units_add_derived_unit(hecto ## _unit_,  h ## _symbol_, _details::make_unit<_details::hecto, _unit_>) \
  units_add_derived_unit( kilo ## _unit_,  k ## _symbol_, _details::make_unit<_details::kilo,  _unit_>) \
  units_add_derived_unit( mega ## _unit_,  M ## _symbol_, _details::make_unit<_details::mega,  _unit_>) \
  units_add_derived_unit( giga ## _unit_,  G ## _symbol_, _details::make_unit<_details::giga,  _unit_>) \
  units_add_derived_unit( tera ## _unit_,  T ## _symbol_, _details::make_unit<_details::tera,  _unit_>) \
  units_add_derived_unit( peta ## _unit_,  P ## _symbol_, _details::make_unit<_details::peta,  _unit_>) \
  units_add_derived_unit(  exa ## _unit_,  E ## _symbol_, _details::make_unit<_details::exa,   _unit_>)

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