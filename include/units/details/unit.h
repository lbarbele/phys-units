#ifndef _include_units_details_unit_h
#define _include_units_details_unit_h

#include <array>
#include <string_view>
#include <tuple>

#include <units/details/power.h>
#include <units/details/ratio.h>
#include <units/details/string.h>
#include <units/details/traits.h>

namespace units::_details {

  // - preliminary, private implementation declarations and aliases

  namespace _unit {
    // * the indexed unit (represents some fundamental unit type)
    template <unsigned id>
    struct indexed;

    // * alias to the power of a indexed
    template <unsigned id, intm_t num = 1, intm_t den = 1>
    using indexed_power = power<indexed<id>, num, den>;

    // * unit multiplicator
    template <class, class, class>
    struct multiply;
    
    // * unit creator
    template <class... Ts>
    struct create;
  }

  // - forward declarations and aliases

  template <concepts::reduced_ratio R, concepts::reduced_power... Ps>
  struct unit;

  template <unsigned id>
  using base_unit = unit<one, _unit::indexed_power<id>>;

  template <class... Ts>
  using make_unit = typename _unit::create<unit<one>, Ts...>::type;

  template <class A, class B>
  using unit_multiply = typename _unit::multiply<A, B, unit<one>>::type;

  // - traits

  namespace traits {
    // * assert type is a unit
    template <class T>
    struct is_unit : std::false_type {};

    template <concepts::reduced_ratio R, concepts::reduced_power... Ps>
    struct is_unit<unit<R, Ps...>> : std::true_type {};

    template <class T>
    static constexpr inline bool is_unit_v = is_unit<T>::value;

    // * assert unit is dimensionless
    template <class T>
    struct is_dimensionless_unit : std::false_type {};

    template <concepts::reduced_ratio R>
    struct is_dimensionless_unit<unit<R>> : std::true_type {};

    template <class T>
    constexpr inline bool is_dimensionless_unit_v = is_dimensionless_unit<T>::value;

    // * assert type is base unit
    template <class T>
    struct is_base_unit : std::false_type {};

    template <unsigned id>
    struct is_base_unit<base_unit<id>> : std::true_type {};

    template <class T>
    constexpr inline bool is_base_unit_v = is_base_unit<T>::value;

    // * assert units are compatible (that is, different up to a factor)
    template <class A, class B>
    struct is_compatible_unit : std::false_type {};

    template <class A, class B>
    requires is_unit_v<A> && is_unit_v<B> && std::is_same_v<typename A::powers, typename B::powers>
    struct is_compatible_unit<A, B> : std::true_type {};

    template <class A, class B>
    constexpr inline bool is_compatible_unit_v = is_compatible_unit<A, B>::value;
  }

  // - concepts

  namespace concepts {
    // * unit types
    template <class T>
    concept unit = traits::is_unit_v<T>;

    // * dimensionless units
    template <class T>
    concept dimensionless_unit = traits::is_dimensionless_unit_v<T>;

    // * base units
    template <class T>
    concept base_unit = traits::is_base_unit_v<T>;

    // * derived units
    template <class T>
    concept compound_unit = (traits::is_unit_v<T> && !traits::is_base_unit_v<T> && !traits::is_dimensionless_unit_v<T>);

    // * compatible units
    template <class A, class B>
    concept unit_compatible = traits::is_compatible_unit_v<A, B>;
  }

  // - implementation of a indexed base unit

  namespace _unit {
    template <unsigned id>
    struct indexed : base_unit<id> {
      static constexpr inline auto index = id;
    };
  }

  // - units nomenclature helper template variables

  namespace _unit {
    template <concepts::unit U>
    constexpr inline sstr symbol = "?";

    template <concepts::reduced_ratio R>
    constexpr inline sstr ratiostr = (R::num%R::den != 0)?
      ("(" + stringify(R::num) + "/" + stringify(R::den) + ")") :
      stringify(R::num);

    template <concepts::reduced_power P>
    constexpr inline sstr smbpowstr = std::is_same_v<one, typename P::exponent>?
      symbol<base_unit<P::base::index>> :
      symbol<base_unit<P::base::index>> + "^" + ratiostr<typename P::exponent>;
  }

  // - definition of the unit class

  template <concepts::reduced_ratio R, concepts::reduced_power... Ps>
  struct unit {
    static_assert(((Ps::exponent::num != 0) && ...));

    using type = unit;
    using factor = R;
    using powers = std::tuple<Ps...>;

    static constexpr sstr symbol =
      (_unit::symbol<type> != "?") ? _unit::symbol<type> :
      (std::is_same_v<factor, one>) ? ((_unit::smbpowstr<Ps> + " ") + ...) :
      "· " + _unit::ratiostr<factor> + " " + unit<one, Ps...>::symbol;
  };

  // - implementation of the product between units

  namespace _unit {
    template <class Ra, class Pa, class... Pas, class Rb, class Pb, class... Pbs, class... Pcs>
    requires (Pa::base::index == Pb::base::index)
    struct multiply<unit<Ra, Pa, Pas...>, unit<Rb, Pb, Pbs...>, unit<one, Pcs...>>
    : multiply<unit<Ra, Pas...>, unit<Rb, Pbs...>, unit<one, Pcs..., power_multiply<Pa, Pb>>> {};

    template <class Ra, class Pa, class... Pas, class Rb, class Pb, class... Pbs, class... Pcs>
    requires (Pa::base::index < Pb::base::index)
    struct multiply<unit<Ra, Pa, Pas...>, unit<Rb, Pb, Pbs...>, unit<one, Pcs...>>
    : multiply<unit<Ra, Pas...>, unit<Rb, Pb, Pbs...>, unit<one, Pcs..., Pa>> {};

    template <class Ra, class Pa, class... Pas, class Rb, class Pb, class... Pbs, class... Pcs>
    requires (Pa::base::index > Pb::base::index)
    struct multiply<unit<Ra, Pa, Pas...>, unit<Rb, Pb, Pbs...>, unit<one, Pcs...>>
    : multiply<unit<Ra, Pa, Pas...>, unit<Rb, Pbs...>, unit<one, Pcs..., Pb>> {};

    template <class Ra, class... Pas, class Rb, class... Pbs, class... Pcs>
    requires (sizeof...(Pas) == 0 || sizeof...(Pbs) == 0)
    struct multiply<unit<Ra, Pas...>, unit<Rb, Pbs...>, unit<one, Pcs...>> {
      using type = unit<ratio_multiply<Ra, Rb>, Pcs..., Pas..., Pbs...>;
    };
  }

  // - implementation of the unit creator 

  namespace _unit {
    // next parameter is a ratio
    template <class Ra, class... Ps, concepts::ratio Rb, class... Ts>
    struct create<unit<Ra, Ps...>, Rb, Ts...>
    : create<unit<ratio_multiply<Ra, Rb>, Ps...>, Ts...> {};

    // next parameter is a unit
    template <concepts::unit Ua, concepts::unit Ub, class... Ts>
    struct create<Ua, Ub, Ts...>
    : create<unit_multiply<Ua, Ub>, Ts...> {};

    // next parameter is an integral, non-zero power of a unit (simply unpack it)
    template <concepts::unit U, concepts::ratio R, class... Ps, intm_t num, intm_t den, class... Ts>
    requires (num%den == 0 && num/den != 0)
    struct create<U, power<unit<R, Ps...>, num, den>, Ts...>
    : create<U, unit<ratio_power<R, num/den>, power_t<Ps, num/den>...>, Ts...> {};

    // base case
    template <concepts::unit U>
    struct create<U> {
      using type = U;
    };
  }

}

#endif