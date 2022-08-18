#ifndef _include_units_details_string_h
#define _include_units_details_string_h

#include <array>
#include <string_view>

namespace units::_details { 

  // - alias for the standard string view
  using string = std::string_view;

  // - compile-time single-char strings
  namespace _string {
    template <char c> constexpr inline char character = c;
  }

  template <char c> constexpr inline string schar = string(&_string::character<c>, 1);

  // - compile-time string joiner
  template <const string&... strings>
  struct join {
    static constexpr auto as_array = []{
      constexpr std::size_t len = (strings.size() + ... + 0);
      std::array<char, len> arr{};
      auto append = [i=0, &arr](const auto& s) mutable {for (auto c : s) arr[i++] = c;};
      (append(strings),...);
      return arr;
    }();

    static constexpr auto value = string(as_array.data(), as_array.size());
  };

  template <const string&... strings>
  constexpr inline auto join_v = join<strings...>::value;

  // - join with spaces in between
  template <const string&... strings> struct spaced_join;

  template <const string& a>
  struct spaced_join<a> {
    static constexpr string value = a;
  };

  template <const string& a, const string& b>
  struct spaced_join<a, b> {
    static constexpr string value = join_v<a, schar<' '>, b>;
  };

  template <const string& a, const string& b, const string&... strings>
  struct spaced_join<a, b, strings...> {
    static constexpr string value = spaced_join<spaced_join<a, b>::value, strings...>::value;
  };

  template <const string& a, const string&... strings>
  static constexpr inline string spaced_join_v = spaced_join<a, strings...>::value;

  // - compile time conversion from unsigned to string
  template <std::intmax_t i, typename = void> struct to_string {
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

  template <std::intmax_t i>
  struct to_string<i, std::enable_if_t<(i < 0)>> {
    static constexpr string value = join_v<schar<'-'>, to_string<-i>::value>;
  };

  template <std::intmax_t i>
  constexpr inline auto to_string_v = to_string<i>::value;

}

#endif