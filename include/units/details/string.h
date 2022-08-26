#ifndef _include_units_details_string_h
#define _include_units_details_string_h

#include <algorithm>
#include <array>
#include <string_view>

namespace units::_details {

  // - forward declarations/aliases

  template <std::size_t max_length>
  struct static_string;
  
  using sstr = static_string<256>;

  // - definition of the static_string class

  template <std::size_t L>
  struct static_string {
  public:
    static constexpr std::size_t max_length = L;

    using type = static_string;

  private:
    std::array<char, max_length+1> m_data;
    std::size_t m_size = 0;

  public:
    constexpr static_string(const char* c = nullptr) {
      m_data.fill(0);

      while(c && c[m_size] != 0 && m_size < max_length) {
        m_data[m_size] = c[m_size];
        ++m_size;
      }
    }

    constexpr static_string(const char c) {
      m_data.fill(0);
      m_data[0] = c;
    }

    constexpr static_string(const std::string_view& s)
    : static_string(s.data())
    {}

    constexpr auto data() const {return m_data.data();}
    constexpr auto size() const {return m_size;}
    constexpr auto begin() const {return m_data.begin();}
    constexpr auto end() const {return m_data.begin() + size();}

    constexpr void fill(const char c) {
      m_data.fill(c);
      m_size = (c == 0)? 0 : max_length;
    }

    // * addition assignment

    constexpr auto operator+=(const char* other) {
      std::size_t i = 0;
      while(m_size < max_length && other[i] != 0) m_data[m_size++] = other[i++];
      return *this;
    }

    constexpr auto operator+=(const auto& other)
    requires std::is_same_v<decltype(other.data()), const char*> {
      return operator+=(other.data());
    }

    // * equal comparison

    constexpr bool operator==(const type other) const {
      if (size() != other.size()) {
        return false;
      }

      for (std::size_t i = 0; i < size(); ++i) {
        if (m_data[i] != other.m_data[i]) {
          return false;
        }
      }

      return true;
    }

    constexpr bool operator==(const char* other) const {
      return *this == type(other);
    }

    // * concatenation

    constexpr auto operator+(const char* other) const {
      auto ret = *this;
      ret += other;
      return ret;
    }

    constexpr auto operator+(const auto& other) const
    requires std::is_same_v<decltype(other.data()), const char*> {
      return *this + other.data();
    }

    constexpr auto operator+(const char c) const {
      return *this + type(c);
    }

    // * conversion to const char*

    constexpr operator const char*() const {
      return data();
    }

  };

  // - addition from lhs

  template <std::size_t len>
  constexpr auto operator+(const char* c, const static_string<len>& s) {
    return static_string<len>(c) + s;
  }

  template <std::size_t len>
  constexpr auto operator+(const std::string_view& c, const static_string<len>& s) {
    return static_string<len>(c) + s;
  }

  template <std::size_t len>
  constexpr auto operator+(const std::string& c, const static_string<len>& s) {
    return static_string<len>(c) + s;
  }
  
  // - stringify: convert integral type value to a static_string

  constexpr sstr stringify(const std::integral auto i) {
    if (i < 0) {
      return "-" + stringify(-i);
    } 
    
    switch (i) {
      case 0: return "0";
      case 1: return "1";
      case 2: return "2";
      case 3: return "3";
      case 4: return "4";
      case 5: return "5";
      case 6: return "6";
      case 7: return "7";
      case 8: return "8";
      case 9: return "9";
      default: return stringify(i/10) + stringify(i%10);
    }
  }
}

#endif