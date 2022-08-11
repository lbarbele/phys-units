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

  using intm_t = std::intmax_t;

  template <intm_t Num, intm_t Den>
  using ratio_t = typename ratio<Num, Den>::type;
}

#endif