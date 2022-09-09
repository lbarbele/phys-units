#ifndef _include_units_angular_h
#define _include_units_angular_h

#include <cmath>

#include <units/framework.h>

namespace units {

  // - angular units

  // * define the radian, the base unit of angle
  units_add_base_unit(9,    radian, rad);

  // * define other angular units
  units_add_derived_unit(pi, pi, make_unit<ratio<14885392687, 4738167652>, radian>);
  units_add_derived_unit(degree, deg, make_unit<ratio<1, 180>, pi>);
  units_add_derived_unit(cycle, cyc, make_unit<ratio<2>, pi>);
  units_add_derived_unit(arcminute, arcmin, make_unit<ratio<1, 60>, degree>);
  units_add_derived_unit(arcsecond, arcsec, make_unit<ratio<1, 60>, arcminute>);

  // - trigonometric functions

  namespace _details::_math {

    // * trigonometric functions can only be computed using units of angle
    auto sin(const concepts::quantity_compatible<radian_t<>> auto q)
    {return std::sin(q/radian_t{1});}

    auto cos(const concepts::quantity_compatible<radian_t<>> auto q)
    {return std::cos(q/radian_t{1});}

    auto tan(const concepts::quantity_compatible<radian_t<>> auto q)
    {return std::tan(q/radian_t{1});}

    template <concepts::unit_compatible<radian> Unit = radian>
    auto asin(const concepts::arithmetic auto x) {
      const auto rad_value = radian_t{std::asin(x)};
      if constexpr (std::is_same_v<Unit, radian>)
        return rad_value;
      else
        return rad_value.template convert<Unit>();
    }

    template <concepts::unit_compatible<radian> Unit = radian>
    auto acos(const concepts::arithmetic auto x){
      const auto rad_value = radian_t{std::acos(x)};
      if constexpr (std::is_same_v<Unit, radian>)
        return rad_value;
      else
        return rad_value.template convert<Unit>();
    }

    template <concepts::unit_compatible<radian> Unit = radian>
    auto atan(const concepts::arithmetic auto x){
      const auto rad_value = radian_t{std::atan(x)};
      if constexpr (std::is_same_v<Unit, radian>)
        return rad_value;
      else
        return rad_value.template convert<Unit>();
    }

    template <concepts::unit_compatible<radian> Unit = radian>
    auto atan2(const concepts::quantity auto y, const concepts::quantity auto x)
    requires concepts::quantity_compatible<decltype(y), decltype(x)> {
      const auto rad_value = radian_t(std::atan2(y.get_value(), decltype(y)(x).get_value()));
      if constexpr (std::is_same_v<Unit, radian>)
        return rad_value;
      else
        return rad_value.template convert<Unit>();
    }
  }
}

#endif