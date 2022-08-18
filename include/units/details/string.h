#ifndef _include_units_details_string_h
#define _include_units_details_string_h

#include <array>
#include <string_view>

namespace units::_details {

  // - alias for the standard string view
  using string = std::string_view;

  // - compile-time string joiner
  template <const string&... strings>
  struct join {
    static constexpr auto as_array = []{
      constexpr std::size_t len = (strings.size() + ... + 0);
      std::array<char, len> arr{};
      auto append = [i=0, &arr](const auto& s) mutable {
        for (auto c : s) arr[i++] = c;
      };
      (append(strings),...);
      return arr;
    }();

    static constexpr auto value = string(as_array.data(), as_array.size());
  };

  template <const string&... strings>
  constexpr inline auto join_v = join<strings...>::value;

  // - compile time conversion from unsigned to string
  template <unsigned i> struct to_string {
    static constexpr string value = join_v<to_string<i/10>::value, to_string<i%10>::value>;
  };

  template <> struct to_string<0> {static constexpr string value = "0";};
  template <> struct to_string<1> {static constexpr string value = "1";};
  template <> struct to_string<2> {static constexpr string value = "2";};
  template <> struct to_string<3> {static constexpr string value = "3";};
  template <> struct to_string<4> {static constexpr string value = "4";};
  template <> struct to_string<5> {static constexpr string value = "5";};
  template <> struct to_string<6> {static constexpr string value = "6";};
  template <> struct to_string<7> {static constexpr string value = "7";};
  template <> struct to_string<8> {static constexpr string value = "8";};
  template <> struct to_string<9> {static constexpr string value = "9";};

  template <unsigned i>
  constexpr inline auto to_string_v = to_string<i>::value;

}

#endif