#ifndef _include_units_framework_h
#define _include_units_framework_h

#include <units/details/macros.h>
#include <units/details/power.h>
#include <units/details/quantity.h>
#include <units/details/ratio.h>
#include <units/details/unit.h>

namespace units {

  // * ratios, operations, and predefined prefixes

  using _details::ratio;
  using _details::ratio_add;
  using _details::ratio_subtract;
  using _details::ratio_multiply;
  using _details::ratio_divide;
  using _details::ratio_power;
  using namespace _details::prefixes;

  // * powers, power multiplication, and common powers

  using _details::power;
  using _details::power_multiply;
  using namespace _details::common_powers;

  // * unit, base unit, and unit-creation helpers

  using _details::unit;
  using _details::base_unit;
  using _details::make_unit;
  using _details::unit_multiply;
  using _details::unit_divide;

  // * quantities

  using _details::quantity;

  // * literals (defined along with units)

  namespace literals {};

  // * traits and concepts

  namespace concepts = _details::concepts;
  namespace traits = _details::traits;
}

#endif