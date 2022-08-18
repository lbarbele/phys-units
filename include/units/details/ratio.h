#ifndef _include_units_details_ratio_h
#define _include_units_details_ratio_h

#include <cstdint>
#include <ratio>
#include <type_traits>

namespace units::_details {
  // - shorter alias for int max
  using intm_t = std::intmax_t;

  // - basic ratio
  using std::ratio;

  template <intm_t Num, intm_t Den>
  using ratio_t = typename ratio<Num, Den>::type;

  // - ratio operations
  using std::ratio_add;
  using std::ratio_subtract;
  using std::ratio_multiply;
  using std::ratio_divide;

  // - prefixes
  inline namespace prefixes {
    using std::atto;
    using std::femto;
    using std::pico;
    using std::nano;
    using std::micro;
    using std::milli;
    using std::centi;
    using std::deci;
    using std::deca;
    using std::hecto;
    using std::kilo;
    using std::mega;
    using std::giga;
    using std::tera;
    using std::peta;
    using std::exa;
  }

  // - ratio power implementation
  namespace _ratio {

    template <class R, intm_t exp, typename = void>
    struct ratio_power;

    // exponent is 0 -> ratio becomes 1/1
    template <intm_t num, intm_t den>
    struct ratio_power< ratio<num, den>, 0> {
      using type = ratio<1, 1>;
    };

    // base case: exponent is 1
    template <intm_t num, intm_t den>
    struct ratio_power< ratio<num, den>, 1> {
      using type = ratio_t<num, den>;
    };

    // negative exponent: invert ratio
    template <intm_t num, intm_t den, intm_t exp>
    struct ratio_power< ratio<num, den>, exp, std::enable_if_t<(exp<0)> > {
      using type = typename ratio_power< ratio<den, num>, -exp >::type;
    };

    // positive exponent
    template <intm_t num, intm_t den, intm_t exp>
    struct ratio_power< ratio<num, den>, exp, std::enable_if_t<(exp>0)> > {
      using type = typename ratio_multiply< ratio<num, den>, typename ratio_power<ratio<num, den>, exp-1>::type >::type;
    };
  }

  template <class R, intm_t exp>
  using ratio_power = typename _ratio::ratio_power<R, exp>::type;
}

#endif