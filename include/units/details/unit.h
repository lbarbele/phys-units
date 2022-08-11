#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <units/details/traits.h>

namespace units::_details::_unit {

  // - base unit

  template <unsigned index>
  struct base_unit : tags::base_unit, props::indexed<index> {};
}

namespace units::_details {
  using _unit::base_unit;
}

#endif