#ifndef _include_units_details_math_h
#define _include_units_details_math_h

#include <cinttypes>
#include <cmath>
#include <concepts>
#include <cstdlib>

#include <units/details/power.h>
#include <units/details/quantity.h>
#include <units/details/traits.h>

namespace units::_details::_math {

  // - basic operations

  // * abs
  using std::abs;

  constexpr auto abs(const concepts::quantity auto q) {
    return q.set_value(q.get_value() >= 0? q.get_value() : -q.get_value());
  }

  // * div (integral quantity)
  using std::div;

  using std::div_t;
  using std::ldiv_t;
  using std::lldiv_t;
  using std::imaxdiv_t;

  template <concepts::integral_quantity Q> struct qdiv_t {Q quot; Q rem;};

  constexpr auto div(const concepts::integral_quantity auto a, const concepts::integral_quantity auto b) {
    return qdiv_t<decltype(a/b)>{a/b, a%b};
  }

  constexpr auto div(const concepts::integral_quantity auto a, const std::integral auto i) {
    return qdiv_t<decltype(a)>{a/i, a%i};
  }

  // * fmod
  using std::fmod;

  auto fmod (const concepts::quantity auto a, const concepts::quantity auto b) {
    using type = decltype(a/b);
    return type(fmod(type(a).get_value(), type(b).get_value()));
  }

  auto fmod (const concepts::quantity auto a, const concepts::arithmetic auto b) {
    return decltype(a)(fmod(a.get_value(), b));
  }

  // * remainder
  using std::remainder;

  auto remainder(const concepts::quantity auto a, const concepts::quantity auto b) {
    using type = decltype(a/b);
    return type(remainder(type(a).get_value(), type(b).get_value()));
  }

  auto remainder (const concepts::quantity auto a, const concepts::arithmetic auto b) {
    return a.set_value(remainder(a.get_value(), b));
  }

  // ! remquo (not implemented)

  // * fused multiply add
  using std::fma;

  constexpr auto fma(const concepts::quantity auto a, const concepts::quantity auto b, const concepts::quantity auto c) {
    return (a*b) + c;
  }

  // * min/max value
  using std::fmax;

  constexpr auto fmax(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)> {
    return a >= b? a : b;
  }

  using std::fmin;

  constexpr auto fmin(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)> {
    return a <= b? a : b;
  }

  // * fdim
  using std::fdim;

  constexpr auto fdim(const concepts::quantity auto a, const concepts::quantity auto b)
  requires concepts::quantity_compatible<decltype(a), decltype(b)> {
    return (a > b)? (a - b) : decltype(a-b)(0);
  }

  // ! nan not implemented

  // - exponential functions (use implicit conversion to value_type)
  using std::exp;
  using std::exp2;
  using std::expm1;
  using std::log;
  using std::log10;
  using std::log2;
  using std::log1p;

  // - power functions
  using std::pow;
  using std::sqrt;
  using std::cbrt;
  using std::hypot;

  // * compile-time integer power of an arithmetic-type object
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

  // * compile-time integer power of a quantity
  template <std::intmax_t p, concepts::unit U, concepts::arithmetic V>
  auto pow(const quantity<U, V> q) {
    const auto value = pow<p>(q.get_value());
    return quantity<make_unit<power<U, p>>, decltype(value)>(value);
  }

  // * hypot function for quantitites
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

  // - trigonometric functions
  using std::sin;
  using std::cos;
  using std::tan;
  using std::asin;
  using std::acos;
  using std::atan;
  using std::atan2;

  // - hyperbolic functions
  using std::sinh;
  using std::cosh;
  using std::tanh;
  using std::asinh;
  using std::acosh;
  using std::atanh;

  // - error and gamma functions
  using std::erf;
  using std::erfc;
  using std::tgamma;
  using std::lgamma;

  // - nearest integer fp operations
  using std::ceil;
  auto ceil(const concepts::quantity auto q) {
    return decltype(q)(ceil(q.get_value()));
  }

  using std::floor;
  auto floor(const concepts::quantity auto q) {
    return decltype(q)(floor(q.get_value()));
  }

  using std::trunc;
  auto trunc(const concepts::quantity auto q) {
    return decltype(q)(trunc(q.get_value()));
  }

  using std::round;
  auto round(const concepts::quantity auto q) {
    return decltype(q)(round(q.get_value()));
  }

  using std::nearbyint;
  auto nearbyint(const concepts::quantity auto q) {
    return decltype(q)(nearbyint(q.get_value()));
  }

  using std::rint;
  auto rint(const concepts::quantity auto q) {
    return decltype(q)(rint(q.get_value()));
  }

  // - fp manipulation
  using std::frexp;
  auto frexp(const concepts::quantity auto q, int* exponent) {
    return decltype(q)(frexp(q.get_value(), exponent));
  }

  using std::ldexp;
  auto ldexp(const concepts::quantity auto q, int exponent) {
    return decltype(q)(ldexp(q.get_value(), exponent));
  }

  using std::modf;
  template <concepts::quantity Q>
  auto modf(const Q q, Q* qiptr) {
    return Q(modf(q.get_value(), qiptr->get_value()));
  }

  using std::scalbn;
  auto scalbn(const concepts::quantity auto q, const std::integral auto exponent) {
    return decltype(q)(scalbn(q.get_value(), exponent));
  }

  using std::ilogb;
  auto ilogb(const concepts::quantity auto q) {
    return ilogb(q.get_value());
  }

  using std::logb;
  auto logb(const concepts::quantity auto q) {
    return logb(q.get_value());
  }

  using std::nextafter;
  using std::nexttoward;

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

  using std::copysign;
  constexpr auto copysign(const concepts::quantity auto mag, const concepts::quantity auto sgn) {
    return sgn.get_value() > 0? abs(mag) : -abs(mag);
  }

  // - classification and comparison
# define units_import(_name_) \
  using std::_name_; \
  auto _name_(const concepts::quantity auto q) {return _name_(q.get_value());}

  using std::fpclassify;
  auto fpclassify(const concepts::quantity auto q) {return fpclassify(q.get_value());}

  using std::isfinite;
  auto isfinite(const concepts::quantity auto q) {return isfinite(q.get_value());}

  using std::isinf;
  auto isinf(const concepts::quantity auto q) {return isinf(q.get_value());}

  using std::isnan;
  auto isnan(const concepts::quantity auto q) {return isnan(q.get_value());}

  using std::isnormal;
  auto isnormal(const concepts::quantity auto q) {return isnormal(q.get_value());}

  using std::signbit;
  auto signbit(const concepts::quantity auto q) {return signbit(q.get_value());}

  using std::isgreater;
  constexpr auto isgreater(const concepts::quantity auto a, const concepts::quantity auto b) {
    return a > b;
  }

  using std::isgreaterequal;
  constexpr auto isgreaterequal(const concepts::quantity auto a, const concepts::quantity auto b) {
    return a >= b;
  }

  using std::isless;
  constexpr auto isless(const concepts::quantity auto a, const concepts::quantity auto b) {
    return a < b;
  }

  using std::islessequal;
  constexpr auto islessequal(const concepts::quantity auto a, const concepts::quantity auto b) {
    return a < b;
  }

  using std::islessgreater;
  constexpr auto islessgreater(const concepts::quantity auto a, const concepts::quantity auto b) {
    return a < b || a > b;
  }

  using std::isunordered;
  constexpr auto isunordered(const concepts::quantity auto a, const concepts::quantity auto b) {
    return isnan(a) || isnan(b);
  }

}

#endif