#ifndef _include_units_units_h
#define _include_units_units_h

#include <units/details/physical_quantity.h>
#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

namespace units {

  // - fundamental quantities
  using _details::fundamental_quantity;

  using length              = fundamental_quantity<0>;
  using time                = fundamental_quantity<1>;
  using mass                = fundamental_quantity<2>;
  using electric_current    = fundamental_quantity<3>;
  using temperature         = fundamental_quantity<4>;
  using amount_of_substance = fundamental_quantity<5>;
  using luminous_intensity  = fundamental_quantity<6>;
  using angle               = fundamental_quantity<7>;
  using solid_angle         = fundamental_quantity<8>;

  // - base units
  using _details::base_unit;

  using meter     = base_unit<length>;
  using second    = base_unit<time>;
  using kilogram  = base_unit<mass>;
  using ampere    = base_unit<electric_current>;
  using kelvin    = base_unit<temperature>;
  using mole      = base_unit<amount_of_substance>;
  using candela   = base_unit<luminous_intensity>;
  using radian    = base_unit<angle>;
  using steradian = base_unit<solid_angle>;
}

#endif