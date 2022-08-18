#ifndef _include_units_units_h
#define _include_units_units_h

#include <string_view>

#include <units/details/power.h>
#include <units/details/quantity.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

#define units_set_name(_unit_, _name_) template <> constexpr inline auto ::units::_details::_unit::unit_name<_unit_> = std::string_view(#_name_)
#define units_set_symbol(_unit_, _symbol_) template <> constexpr inline auto ::units::_details::_unit::unit_symbol<_unit_> = std::string_view(#_symbol_)

#define units_add_base_unit(_id_, _name_, _symbol_) \
  using _name_ = ::units::_details::base_unit<_id_>; \
  using _symbol_ = _name_; \
  units_set_name(_name_, _name_); \
  units_set_symbol(_name_, _symbol_);

namespace units {

  // - imports
  using _details::ratio;

  using _details::power;
  using _details::inverse;
  using _details::squared;
  using _details::inverse_squared;

  using _details::make_unit;

  using _details::quantity_t;

  // - base units
  units_add_base_unit(0, meter, m);
  units_add_base_unit(1, second, s);
  units_add_base_unit(2, kilogram, kg);
  units_add_base_unit(3, ampere, A);
  units_add_base_unit(4, kelvin, K);
  units_add_base_unit(5, mole, mol);
  units_add_base_unit(6, candela, cd);
  units_add_base_unit(7, radian, rad);
  units_add_base_unit(8, steradian, sr);

  // - multiples of base units
  using nanometer = make_unit<std::nano, m>;

  // - derived units
  using hertz   = make_unit<inverse<second>>;
  using newton  = make_unit<kilogram, meter, inverse_squared<second>>;
  using pascal  = make_unit<newton, inverse_squared<meter>>;
  using joule   = make_unit<newton, meter>;
  using watt    = make_unit<joule, inverse<second>>;
  using coulomb = make_unit<ampere, second>;
  using volt    = make_unit<joule, inverse<coulomb>>;
  using farad   = make_unit<coulomb, inverse<volt>>;
  using ohm     = make_unit<volt, inverse<ampere>>;
}

#endif