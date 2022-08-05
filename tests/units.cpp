#include <type_traits>

#include <units/units.h>

int main(void) {
  
  using namespace units::_details;

  // - ratios
  using ratio_a = ratio<1, 2>;
  using ratio_b = ratio<3, 7>;

  static_assert(traits::is_ratio_v<ratio_a>);
  static_assert(traits::is_ratio_v<ratio_b>);

  static_assert(std::is_same_v<ratio_a, ratio_t<3, 6>>);
  static_assert(std::is_same_v<ratio_b, ratio_t<6, 14>>);

  static_assert(std::is_same_v<ratio_add<ratio_a, ratio_b>, ratio<13, 14>>);
  static_assert(std::is_same_v<ratio_subtract<ratio_a, ratio_b>, ratio<1, 14>>);
  static_assert(std::is_same_v<ratio_multiply<ratio_a, ratio_b>, ratio<3, 14>>);
  static_assert(std::is_same_v<ratio_divide<ratio_a, ratio_b>, ratio<7, 6>>);

  // - powers
  struct my_base {};

  using power_a = power_t<my_base, 4, 6>;
  using power_b = power<my_base, 2, 3>;
  using power_c = power_t<power_a, 3, 5>;
  using power_d = power_t<power_b, 9, 15>;
  using power_e = power_multiply<power_a, power_c>;
  using power_f = power_multiply<my_base, my_base>;
  using power_g = power_multiply<my_base, power_e>;

  static_assert(std::is_same_v<typename power_a::base, my_base>);
  static_assert(std::is_same_v<typename power_a::exponent, ratio<2, 3>>);
  static_assert(std::is_same_v<power_a, power_b>);

  static_assert(std::is_same_v<typename power_c::base, my_base>);
  static_assert(std::is_same_v<typename power_c::exponent, ratio<2, 5>>);
  static_assert(std::is_same_v<power_c, power_d>);

  static_assert(std::is_same_v<typename power_e::base, my_base>);
  static_assert(std::is_same_v<typename power_e::exponent, ratio<16, 15>>);

  static_assert(std::is_same_v<typename power_f::base, my_base>);
  static_assert(std::is_same_v<typename power_f::exponent, ratio<2>>);

  static_assert(std::is_same_v<typename power_g::base, my_base>);
  static_assert(std::is_same_v<typename power_g::exponent, ratio<31, 15>>);

  return 0;
}