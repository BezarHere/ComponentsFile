#pragma once

typedef const char *const c_cstr;

#define FORCEINLINE __forceinline
#define CINLINE __forceinline constexpr

CINLINE bool is_capitalized(const char c)
{
  return 'A' <= c && c <= 'Z';
}

CINLINE bool is_digit(const char c)
{
  return '0' <= c && c <= '9';
}

CINLINE bool is_hex_digit(const char c)
{
  return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') ||  ('A' <= c && c <= 'F');
}

CINLINE bool is_bin_digit(const char c)
{
  return c == '0' || c == '1';
}

CINLINE bool is_letter(const char c)
{
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

CINLINE bool is_whitespace(const char c)
{
  return c == '\n' || c == ' ' || c == '\t';
}

CINLINE bool is_valid_identifier_continution(const char c)
{
  return c == '_' ||  is_letter(c) || is_digit(c);
}

CINLINE uint8_t get_hex_digit_value(const char c)
{
  if (is_digit(c))
    return c - '0';
   return 10 + c - (c >= 'a' ? 'a' : 'A');
}

CINLINE bool get_bin_digit_value(const char c)
{
  return c - '0';
}

template <typename _T>
CINLINE _T parse_base10(c_cstr s, const size_t n)
{
  constexpr uint64_t POW10TABLE[ 20 ]
  {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
    10000000000000,
    100000000000000,
    1000000000000000,
    10000000000000000,
    100000000000000000,
    1000000000000000000,
    10000000000000000000,
  };
  _T i{};
  for (size_t j{}; j < n; j++)
    i += _T(s[ j ] - '0') * POW10TABLE[ j ];
  return i;
}

template <typename _T>
CINLINE _T parse_base16(c_cstr s, const size_t n)
{
  _T i{};
  for (size_t j{}; j < n; j++)
  {
    const char c = s[ j ];
    if (!is_hex_digit(c))
      bite::raise("invalid hex digit");
    i += _T(get_hex_digit_value(c)) << (j << 2);
  }
  return i;
}

template <typename _T>
CINLINE _T parse_base2(c_cstr s, const size_t n)
{
  _T i{};
  for (size_t j{}; j < n; j++)
  {
    const char c = s[ j ];
    if (!is_hex_digit(c))
      bite::raise("invalid binary digit");
    i += _T(get_bin_digit_value(c)) << j;
  }
  return i;
}

class StringReader final
{
public:
  StringReader(c_cstr p_cstr, const size_t l)
    : m_length{ l }, m_cstr{ p_cstr }, m_index{} {}

  StringReader(const std::string &str)
    : m_length{ str.length() }, m_cstr{ str.c_str() }, m_index{} {}

  FORCEINLINE _NODISCARD char peek_next() const noexcept
  {
    return m_cstr[ m_index + 1 ];
  }

  FORCEINLINE _NODISCARD char peek() const noexcept
  {
    return m_cstr[ m_index ];
  }

  FORCEINLINE _NODISCARD char read() noexcept
  {
    return m_cstr[ m_index++ ];
  }

  FORCEINLINE _NODISCARD wchar_t read_wide() noexcept
  {
    wchar_t w = ((const wchar_t *)m_cstr)[ m_index ];
    m_index += 2;
    return w;
  }

  FORCEINLINE _NODISCARD std::string read(size_t len)
  {
    if (!len)
      return std::string("[[ZLEN]]");

    if (m_index + len >= m_length)
      len = (m_length - m_index) - 1;

    std::string s( m_cstr + m_index, len );
    m_index += len;
    return std::move(s);
  }

  FORCEINLINE _NODISCARD std::string str(const size_t from, size_t len)
  {
    if (!len)
      return std::string("[[ZLEN]]");

    if (from >= m_length)
      return std::string("[[OFB]]");

    if (from + len >= m_length)
      len = (m_length - from) - 1;
    const auto s = std::string(m_cstr + from, len);
    return s;
  }

  FORCEINLINE _NODISCARD bool empty() const noexcept
  {
    return m_index >= m_length;
  }

  FORCEINLINE operator bool() const noexcept
  {
    return m_index < m_length;
  }

  FORCEINLINE void advance() noexcept
  {
    m_index++;
  }

  FORCEINLINE _NODISCARD bool march() noexcept
  {
    return ++m_index < m_length;
  }

  FORCEINLINE void retreat() noexcept
  {
    m_index--;
  }

  // just index - 1
  FORCEINLINE _NODISCARD size_t get_anchor() const noexcept
  {
    return m_index - 1;
  }

  FORCEINLINE _NODISCARD size_t get_index() const noexcept
  {
    return m_index;
  }

  FORCEINLINE _NODISCARD c_cstr begin() const noexcept
  {
    return m_cstr;
  }

  FORCEINLINE _NODISCARD c_cstr current() const noexcept
  {
    return m_cstr + m_index;
  }

  FORCEINLINE _NODISCARD c_cstr end() const noexcept
  {
    return m_cstr + m_length;
  }

  FORCEINLINE _NODISCARD c_cstr data() const noexcept
  {
    return m_cstr;
  }


private:
  size_t m_index;
  const size_t m_length;
  c_cstr m_cstr;
};


enum class NumberBase
{
  Hex,
  Decimal,
  Binary
};
