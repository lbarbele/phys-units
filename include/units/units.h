#ifndef _include_units_units_h
#define _include_units_units_h

#include <string_view>

#include <units/details/power.h>
#include <units/details/quantity.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

namespace units {

  // - imports
  using _details::ratio;
  using _details::power;

  using namespace _details::prefixes;
  using namespace _details::common_powers;

  using _details::base_unit;
  using _details::make_unit;

  // - macro definitions
# define units_set_name(_unit_, _name_, _symbol_) \
    template <> constexpr inline auto ::units::_details::_unit::unit_name<_unit_> = std::string_view(#_name_); \
    template <> constexpr inline auto ::units::_details::_unit::unit_symbol<_unit_> = std::string_view(#_symbol_);

# define units_add_base_unit(_id_, _name_, _symbol_) \
    using _name_   = ::units::_details::base_unit<_id_>; \
    using _symbol_ = _name_; \
    units_set_name(_name_, _name_, _symbol_); \

# define units_add_derived_unit(_name_, _symbol_, ...) \
    using _name_   = __VA_ARGS__; \
    using _symbol_ = _name_; \
    units_set_name(_name_, _name_, _symbol_); \

# define units_add_prefixes(_unit_, _symbol_) \
    units_add_derived_unit(femto ## _unit_,  f ## _symbol_, ::units::_details::make_unit<::units::_details::femto, _unit_>) \
    units_add_derived_unit( pico ## _unit_,  p ## _symbol_, ::units::_details::make_unit<::units::_details::pico,  _unit_>) \
    units_add_derived_unit( nano ## _unit_,  n ## _symbol_, ::units::_details::make_unit<::units::_details::nano,  _unit_>) \
    units_add_derived_unit(micro ## _unit_,  u ## _symbol_, ::units::_details::make_unit<::units::_details::micro, _unit_>) \
    units_add_derived_unit(milli ## _unit_,  m ## _symbol_, ::units::_details::make_unit<::units::_details::milli, _unit_>) \
    units_add_derived_unit(centi ## _unit_,  c ## _symbol_, ::units::_details::make_unit<::units::_details::centi, _unit_>) \
    units_add_derived_unit( deci ## _unit_,  d ## _symbol_, ::units::_details::make_unit<::units::_details::deci,  _unit_>) \
    units_add_derived_unit( deca ## _unit_, da ## _symbol_, ::units::_details::make_unit<::units::_details::deca,  _unit_>) \
    units_add_derived_unit(hecto ## _unit_,  h ## _symbol_, ::units::_details::make_unit<::units::_details::hecto, _unit_>) \
    units_add_derived_unit( kilo ## _unit_,  k ## _symbol_, ::units::_details::make_unit<::units::_details::kilo,  _unit_>) \
    units_add_derived_unit( mega ## _unit_,  M ## _symbol_, ::units::_details::make_unit<::units::_details::mega,  _unit_>) \
    units_add_derived_unit( giga ## _unit_,  G ## _symbol_, ::units::_details::make_unit<::units::_details::giga,  _unit_>) \
    units_add_derived_unit( tera ## _unit_,  T ## _symbol_, ::units::_details::make_unit<::units::_details::tera,  _unit_>) \
    units_add_derived_unit( peta ## _unit_,  P ## _symbol_, ::units::_details::make_unit<::units::_details::peta,  _unit_>) \
    units_add_derived_unit(  exa ## _unit_,  E ## _symbol_, ::units::_details::make_unit<::units::_details::exa,   _unit_>)

# define units_add_base_unit_with_prefixes(_id_, _name_, _symbol_) \
    units_add_base_unit(_id_, _name_, _symbol_) \
    units_add_prefixes(_name_, _symbol_)

# define units_add_derived_unit_with_prefixes(_name_, _symbol_, ...) \
    units_add_derived_unit(_name_, _symbol_, __VA_ARGS__) \
    units_add_prefixes(_name_, _symbol_)

  // - define base units
  units_add_base_unit(0,     meter,   m);
  units_add_base_unit(1,    second,   s);
  units_add_base_unit(2,  kilogram,  kg); // -> define below
  units_add_base_unit(3,    ampere,   A);
  units_add_base_unit(4,    kelvin,   K);
  units_add_base_unit(5,      mole, mol);
  units_add_base_unit(6,   candela,  cd);
  units_add_base_unit(7,    radian, rad);
  units_add_base_unit(8, steradian,  sr);

  // - define SI derived units
  units_add_derived_unit(  hertz,  Hz, make_unit<inverse<second>>);
  units_add_derived_unit( newton,   N, make_unit<kilogram, meter, inverse_squared<second>>);
  units_add_derived_unit( pascal,  Pa, make_unit<newton, inverse_squared<meter>>);
  units_add_derived_unit(  joule,   J, make_unit<newton, meter>);
  units_add_derived_unit(   watt,   W, make_unit<joule, inverse<second>>);
  units_add_derived_unit(coulomb,   C, make_unit<ampere, second>);
  units_add_derived_unit(   volt,   V, make_unit<joule, inverse<coulomb>>);
  units_add_derived_unit(  farad,   F, make_unit<coulomb, inverse<volt>>);
  units_add_derived_unit(    ohm, Ohm, make_unit<volt, inverse<ampere>>);
  units_add_derived_unit(siemens,   S, make_unit<inverse<ohm>>);
  units_add_derived_unit(  weber,  Wb, make_unit<volt, second>);
  units_add_derived_unit(  tesla,   T, make_unit<weber, inverse_squared<m>>);
  units_add_derived_unit(  henry,   H, make_unit<weber, inverse<ampere>>);
  units_add_derived_unit(  lumen,  lm, make_unit<candela, steradian>);
  units_add_derived_unit(    lux,  lx, make_unit<lumen, inverse_squared<meter>>);
  units_add_derived_unit(  katal, kat, make_unit<mole, inverse<second>>);
}

#endif