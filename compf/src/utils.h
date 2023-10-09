#pragma once

typedef const char *const c_cstr;

#define FORCEINLINE __forceinline
#define CINLINE __forceinline constexpr

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

CINLINE bool is_capitalized(const char c)
{
  return 'A' <= c && c <= 'Z';
}

CINLINE bool is_number(const char c)
{
  return '0' <= c && c <= '9';
}

CINLINE bool is_letter(const char c)
{
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

CINLINE bool is_whitespace(const char c)
{
  return c == '\n' || c == ' ' || c == '\t';
}

template <typename _T>
CINLINE _T parse_integral(c_cstr s, const size_t n)
{
  _T i;
  for (size_t j{}; j < n; j++)
    i += _T(s[ j ] - '0') * POW10TABLE[ j ];
  return i;
}

class StringReader final
{
public:
  StringReader(c_cstr p_cstr, const size_t l)
    : m_length{ l }, m_cstr{ p_cstr }, m_index{} {}

  StringReader(const std::string &str)
    : m_length{ str.length() }, m_cstr{ str.c_str() }, m_index{} {}

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

  FORCEINLINE _NODISCARD std::string &&read(size_t n)
  {
    if (!n)
      return std::string();

    if (m_index + n >= m_length)
      n = (m_length - m_index) - 1;

    std::string s{ m_cstr + m_index, n };
    m_index += n;
    return std::move(s);
  }

  FORCEINLINE _NODISCARD std::string &&read(const size_t i, size_t n)
  {
    if (!n)
      return std::string();

    if (i >= m_length)
      return std::string();

    if (i + n >= m_length)
      n = (m_length - i) - 1;
    return std::string{ m_cstr + i, n };
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

  FORCEINLINE _NODISCARD size_t get_index() const noexcept
  {
    return m_index;
  }

  FORCEINLINE _NODISCARD c_cstr begin() const noexcept
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

