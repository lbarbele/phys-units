#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <string_view>

#include <units/details/traits.h>

namespace units::_details::_unit {
  template <class Q>
  struct base_unit : props::indexed<Q::index> {
    static_assert(traits::is_fundamental_quantity_v<Q>,
    "Base units can only be defined in terms of a single fundamental quantity");

    using quantity = Q;
  };
}

namespace units::_details {
  using _unit::base_unit;
}

#endif