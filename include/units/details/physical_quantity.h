#ifndef _include_units_details_fundamental_quantity_h
#define _include_units_details_fundamental_quantity_h

#include <units/details/traits.h>

namespace units::_details {

  template <unsigned index>
  struct fundamental_quantity
  : tags::fundamental_quantity,
    props::indexed<index> {};

}

#endif