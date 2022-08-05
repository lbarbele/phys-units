#ifndef _include_units_details_ratio_h
#define _include_units_details_ratio_h

#include <cstdint>
#include <ratio>

namespace units::_details {
  using std::ratio;

  using std::ratio_add;
  using std::ratio_subtract;
  using std::ratio_multiply;
  using std::ratio_divide;

  template <std::intmax_t Num, std::intmax_t Den>
  using ratio_tp = typename ratio<Num, Den>::type;
}

#endif