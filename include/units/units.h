#ifndef _include_units_units_h
#define _include_units_units_h

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

namespace units {

  // - base units
  using _details::base_unit;

  using meter     = base_unit<0>;
  using second    = base_unit<1>;
  using kilogram  = base_unit<2>;
  using ampere    = base_unit<3>;
  using kelvin    = base_unit<4>;
  using mole      = base_unit<5>;
  using candela   = base_unit<6>;
  using radian    = base_unit<7>;
  using steradian = base_unit<8>;
}

#endif