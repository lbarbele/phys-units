#ifndef _include_units_units_h
#define _include_units_units_h

#include <string_view>

#include <units/details/power.h>
#include <units/details/quantity.h>
#include <units/details/ratio.h>
#include <units/details/traits.h>
#include <units/details/unit.h>

#define units_add_base_unit(id, name_singular, name_plural, abbreviation) \
  using name_singular = make_unit<base_unit<id>>; \
  using name_plural = name_singular; \
  using abbreviation = name_singular; \
  template<> constexpr inline std::string_view units::_details::name<name_singular> = #name_singular; \
  template<> constexpr inline std::string_view units::_details::plural<name_singular> = #name_plural; \
  template<> constexpr inline std::string_view units::_details::abbrev<name_singular> = #abbreviation;

namespace units {

  using lala = double;

  // - imports
  using _details::ratio;

  using _details::power;
  using _details::inverse;
  using _details::squared;
  using _details::inverse_squared;

  using _details::base_unit;
  using _details::make_unit;

  using _details::quantity_t;

  // - base units
  units_add_base_unit(0, meter, meters, m);
  units_add_base_unit(1, second, seconds, s);
  units_add_base_unit(2, kilogram, kilograms, kg);
  units_add_base_unit(3, ampere, amperes, A);
  units_add_base_unit(4, kelvin, kelvins, K);
  units_add_base_unit(5, mole, moles, mol);
  units_add_base_unit(6, candela, candelas, cd);
  units_add_base_unit(7, radian, radians, rad);
  units_add_base_unit(8, steradian, steradians, sr);

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
  // ...

  using gram = make_unit<ratio<1, 1000>, kilogram>;
  using centimeter = make_unit<ratio<1, 100>, meter>;

  using gram_per_squared_centimeter = make_unit<gram, inverse<centimeter>>;

  using depth_t = quantity_t<gram_per_squared_centimeter>;
}

#endif