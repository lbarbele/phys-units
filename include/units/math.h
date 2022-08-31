#ifndef _include_units_math_h
#define _include_units_math_h

#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <concepts>
#include <cstdlib>

#include <units/details/power.h>
#include <units/details/quantity.h>

// - MACROS: only visible inside this file (they are undef below)

#define units_math_import_function_for_dimensionless(_fname_)      \
  using std::_fname_;                                              \
  auto _fname_(const concepts::dimensionless_quantity auto q)      \
  {return _fname_(q.template convert<make_unit<>>().get_value());}
  
#define units_math_import_function_for_get_value_returning_quantity(_fname_)  \
  using std::_fname_;                                                         \
  auto _fname_(const concepts::quantity auto q)                               \
  {return decltype(q)(_fname_(q.get_value()));}

#define units_math_import_function_for_get_value_returning_value(_fname_)     \
  using std::_fname_;                                                         \
  auto _fname_(const concepts::quantity auto q)                               \
  {return _fname_(q.get_value());}


// - import declarations to the units::math namespace

namespace units::_details::_math {}

namespace units::math {
  using namespace units::_details::_math;
}

// - math functions

namespace units::_details::_math {

  // * basic operations

  using std::fmod;
  using std::remainder;
  using std::remquo;
  using std::fma;
  using std::fmax;
  using std::fmin;
  using std::max;
  using std::min;
  using std::fdim;
  using std::nan;

  // abs
  constexpr auto abs(const concepts::arithmetic auto x)
  {return x != x? x : x >= 0? x : -x;}

  constexpr auto abs(const concepts::quantity auto q)
  {return q.set_value(abs(q.get_value()));}

  // div (for integral quantity)
  using std::div_t;
  using std::ldiv_t;
  using std::lldiv_t;

  template <concepts::integral_quantity Q, concepts::integral_quantity R>
  struct qdiv_t {Q quot; R rem;};

  constexpr auto div(const int a, const int b)
  {return div_t{a/b, a%b};}

  constexpr auto div(const long a, const long b)
  {return ldiv_t{a/b, a%b};}

  constexpr auto div(const long long a, const long long b)
  {return lldiv_t{a/b, a%b};}

  constexpr auto div(const concepts::integral_quantity auto a, const auto b)
  {return qdiv_t(a/b, a%b);}

  // fmod
  auto fmod (const concepts::quantity auto a, const concepts::quantity auto b)
  {return a - decltype(a/b)(std::trunc((a/b).get_value())) * b;}

  auto fmod (const concepts::quantity auto a, const concepts::arithmetic auto b)
  {return decltype(a)(fmod(a.get_value(), b));}

  // remainder
  auto remainder(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a - decltype(a/b)(std::trunc((a/b).get_value())) * b;}

  auto remainder (const concepts::quantity auto a, const concepts::arithmetic auto b)
  {return decltype(a)(fmod(a.get_value(), b));}

  // fused multiply add
  constexpr auto fma(const concepts::quantity auto a, const concepts::quantity auto b, const concepts::quantity auto c)
  {return (a*b) + c;}

  // min/max value
  constexpr auto max(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)>
  {return a >= b? a : decltype(a)(b);}

  // fdim
  constexpr auto fdim(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)>
  {return (a > b)? (a - b) : decltype(a-b)(0);}

  // * exponential functions

  units_math_import_function_for_dimensionless(exp)
  units_math_import_function_for_dimensionless(exp2)
  units_math_import_function_for_dimensionless(expm1)
  units_math_import_function_for_dimensionless(log)
  units_math_import_function_for_dimensionless(log10)
  units_math_import_function_for_dimensionless(log2)
  units_math_import_function_for_dimensionless(log1p)

  // * power functions

  units_math_import_function_for_dimensionless(pow)
  units_math_import_function_for_dimensionless(sqrt)
  units_math_import_function_for_dimensionless(cbrt)
  units_math_import_function_for_dimensionless(hypot)

  // compile-time integer power of an arithmetic-type object
  template <std::intmax_t p>
  auto pow(const concepts::arithmetic auto x) -> std::conditional_t<(p>=0), decltype(x), long double> {
    if constexpr (p == 0)
      return 1;
    else if (p == 1)
      return x;
    else if (p < 0)
      return 1.0L/pow<-p>(x);
    else {
      const auto y = pow<p/2>(x);
      return y * y * (p%2 == 1? x : 1);
    }
  }

  // compile-time integer power of a quantity
  template <std::intmax_t p, concepts::unit U, concepts::arithmetic V>
  auto pow(const quantity<U, V> q) {
    const auto value = pow<p>(q.get_value());
    return quantity<make_unit<power<U, p>>, decltype(value)>(value);
  }

  // hypot function for quantitites
  auto hypot(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)> {
    using unit_type = typename decltype(a+b)::unit_type;
    const auto value = hypot(
      a.template convert<unit_type>().get_value(),
      b.template convert<unit_type>().get_value());
    return quantity<unit_type, decltype(value)>(value);
  }

  auto hypot(const concepts::quantity auto a, const concepts::quantity auto b, const concepts::quantity auto c)
  requires concepts::quantity_compatible<decltype(a), decltype(b), decltype(c)> {
    using unit_type = typename decltype(a+b+c)::unit_type;
    const auto value = hypot(
      a.template convert<unit_type>().get_value(),
      b.template convert<unit_type>().get_value(),
      c.template convert<unit_type>().get_value());
    return quantity<unit_type, decltype(value)>(value);
  }

  // * trigonometric functions (do not allow conversion from dimensionless!)

  using std::sin;
  using std::cos;
  using std::tan;
  using std::asin;
  using std::acos;
  using std::atan;
  using std::atan2;

  // * hyperbolic functions

  units_math_import_function_for_dimensionless(sinh)
  units_math_import_function_for_dimensionless(cosh)
  units_math_import_function_for_dimensionless(tanh)
  units_math_import_function_for_dimensionless(asinh)
  units_math_import_function_for_dimensionless(acosh)
  units_math_import_function_for_dimensionless(atanh)

  // * error and gamma functions

  units_math_import_function_for_dimensionless(erf)
  units_math_import_function_for_dimensionless(erfc)
  units_math_import_function_for_dimensionless(tgamma)
  units_math_import_function_for_dimensionless(lgamma)

  // * nearest integer fp operations

  units_math_import_function_for_get_value_returning_quantity(ceil)
  units_math_import_function_for_get_value_returning_quantity(floor)
  units_math_import_function_for_get_value_returning_quantity(trunc)
  units_math_import_function_for_get_value_returning_quantity(round)
  units_math_import_function_for_get_value_returning_quantity(nearbyint)
  units_math_import_function_for_get_value_returning_quantity(rint)

  // * fp manipulation

  using std::frexp;
  using std::ldexp;
  using std::modf;
  using std::scalbn;
  using std::ilogb;
  using std::logb;
  using std::nextafter;
  using std::nexttoward;
  using std::copysign;

  auto frexp(const concepts::quantity auto q, int* exponent)
  {return decltype(q)(frexp(q.get_value(), exponent));}

  auto ldexp(const concepts::quantity auto q, int exponent)
  {return decltype(q)(ldexp(q.get_value(), exponent));}

  template <concepts::quantity Q>
  auto modf(const Q q, Q* qiptr)
  {return Q(modf(q.get_value(), qiptr->get_value()));}

  auto scalbn(const concepts::quantity auto q, const std::integral auto exponent)
  {return decltype(q)(scalbn(q.get_value(), exponent));}

  auto ilogb(const concepts::quantity auto q)
  {return ilogb(q.get_value());}

  auto logb(const concepts::quantity auto q)
  {return logb(q.get_value());}

  auto nextafter(const concepts::quantity auto from, const concepts::quantity auto to)
  requires concepts::quantity_compatible<decltype(from), decltype(to)> {
    using tp = decltype(from + to);
    return nextafter(tp(from).get_value(), tp(to).get_value());
  }

  auto nexttoward(const concepts::quantity auto from, const concepts::quantity auto to)
  requires concepts::quantity_compatible<decltype(from), decltype(to)> {
    using tp = decltype(from + to);
    return nexttoward(tp(from).get_value(), tp(to).get_value());
  }

  constexpr auto copysign(const concepts::quantity auto mag, const concepts::quantity auto sgn)
  {return sgn.get_value() > 0? abs(mag) : -abs(mag);}

  // * classification and comparison

  units_math_import_function_for_get_value_returning_value(fpclassify)
  units_math_import_function_for_get_value_returning_value(isfinite)
  units_math_import_function_for_get_value_returning_value(isinf)
  units_math_import_function_for_get_value_returning_value(isnan)
  units_math_import_function_for_get_value_returning_value(isnormal)
  units_math_import_function_for_get_value_returning_value(signbit)

  using std::isgreater;
  using std::isgreaterequal;
  using std::isless;
  using std::islessequal;
  using std::islessgreater;
  using std::isunordered;

  constexpr auto isgreater(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a > b;}

  constexpr auto isgreaterequal(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a >= b;}

  constexpr auto isless(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a < b;}

  constexpr auto islessequal(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a < b;}

  constexpr auto islessgreater(const concepts::quantity auto a, const concepts::quantity auto b)
  {return a < b || a > b;}

  constexpr auto isunordered(const concepts::quantity auto a, const concepts::quantity auto b)
  {return isnan(a) || isnan(b);}
}

// - undefine macros

#undef units_math_import_function_for_dimensionless
#undef units_math_import_function_for_get_value_returning_quantity
#undef units_math_import_function_for_get_value_returning_value

#endif