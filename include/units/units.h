#ifndef _include_units_units_h
#define _include_units_units_h

#include <units/framework.h>

namespace units {

  // - base units

  // * the SI seven
  units_add_base_unit(0,     meter,   m);
  units_add_base_unit(1,    second,   s);
  units_add_base_unit(2,  kilogram,  kg);
  units_add_base_unit(3,    ampere,   A);
  units_add_base_unit(4,    kelvin,   K);
  units_add_base_unit(5,      mole, mol);
  units_add_base_unit(6,   candela,  cd);

  // * solid angle
  units_add_base_unit(7, steradian,  sr);

  // * decay count is used to define the becquerel
  units_add_base_unit(8,     decay, dec);

  // - the SI derived units
  
  units_add_derived_unit(    hertz,  Hz, make_unit<inverse<second>>);
  units_add_derived_unit(   newton,   N, make_unit<kilogram, meter, inverse_squared<second>>);
  units_add_derived_unit(   pascal,  Pa, make_unit<newton, inverse_squared<meter>>);
  units_add_derived_unit(    joule,   J, make_unit<newton, meter>);
  units_add_derived_unit(     watt,   W, make_unit<joule, inverse<second>>);
  units_add_derived_unit(  coulomb,   C, make_unit<ampere, second>);
  units_add_derived_unit(     volt,   V, make_unit<joule, inverse<coulomb>>);
  units_add_derived_unit(    farad,   F, make_unit<coulomb, inverse<volt>>);
  units_add_derived_unit(      ohm, Ohm, make_unit<volt, inverse<ampere>>);
  units_add_derived_unit(  siemens,   S, make_unit<inverse<ohm>>);
  units_add_derived_unit(    weber,  Wb, make_unit<volt, second>);
  units_add_derived_unit(    tesla,   T, make_unit<weber, inverse_squared<meter>>);
  units_add_derived_unit(    henry,   H, make_unit<weber, inverse<ampere>>);
  units_add_derived_unit(    lumen,  lm, make_unit<candela, steradian>);
  units_add_derived_unit(      lux,  lx, make_unit<lumen, inverse_squared<meter>>);
  units_add_derived_unit(becquerel,  Bq, make_unit<decay, inverse<second>>);
  units_add_derived_unit(    katal, kat, make_unit<mole, inverse<second>>);

  // - derived units

  // * units of length

  // multiples of meter
  units_set_all_prefixes(meter, m);
  units_add_derived_unit(angstrom, Ang, make_unit<ratio<100>, picometer>);
  using fermi = femtometer;
  using micron = micrometer;

  // imperial units
  units_add_derived_unit(      foot,  ft, make_unit<ratio<3048, 10000>, meter>);
  units_add_derived_unit(      thou,  th, make_unit<ratio<1, 12000>, foot>);
  units_add_derived_unit(barleycorn,  Bc, make_unit<ratio<1, 36>, foot>);
  units_add_derived_unit(      yard,  yd, make_unit<ratio<3>, foot>);
  units_add_derived_unit(     chain,  ch, make_unit<ratio<66>, foot>);
  units_add_derived_unit(   furlong, fur, make_unit<ratio<660>, foot>);
  units_add_derived_unit(      mile,  mi, make_unit<ratio<5280>, foot>);
  units_add_derived_unit(    league, lea, make_unit<ratio<15840>, foot>);

  // * units of time

  units_set_all_prefixes(second, s);

  units_add_derived_unit(minute, min, make_unit<ratio<60>, second>);
  units_add_derived_unit(hour, h, make_unit<ratio<60>, minute>);
  units_add_derived_unit(day, d, make_unit<ratio<24>, hour>);
  units_add_derived_unit(year, yr, make_unit<ratio<365>, day>);

  // * units of mass

  units_add_derived_unit(gram, g, make_unit<ratio<1, 1000>, kilogram>);
  units_set_prefixes(gram, g, micro, milli, centi, deci);
}

#endif