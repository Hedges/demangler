#include <demangler/demangler.hh>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>
#include <vector>

#include <gsl/string_span>

namespace demangler
{
namespace
{
struct State
{
  State(std::string const& psymbol, bool params, bool templates) noexcept
    : symbol{psymbol},
      remaining{this->symbol},
      add_params{params},
      add_templates{templates},
      demangled{}
  {
  }

  void skipChars(unsigned int n) noexcept
  {
    assert(n <= (unsigned)(this->remaining.size()));
    this->remaining = this->remaining.subspan(n);
  }

  char nextChar() const noexcept
  {
    return this->remaining[0];
  }

  bool empty() const noexcept
  {
    return this->remaining.empty();
  }

  std::string const& symbol;
  gsl::cstring_span<> remaining;
  bool add_params;
  bool add_templates;
  std::string demangled;
  std::vector<std::string> substitutions;
};

int extractDecimal(gsl::cstring_span<>& symbol)
{
  auto i = 0u;
  auto ret = 0u;

  if (symbol.empty())
    throw std::runtime_error("Extracting empty decimal");
  if (!std::isdigit(symbol[0]))
    throw std::runtime_error("Extracting decimal which is not decimal: " +
                             gsl::to_string(symbol));
  while (i < (unsigned)symbol.size() && std::isdigit(symbol[i]))
  {
    ret = ret * 10 + symbol[i] - '0';
    ++i;
  }
  symbol = symbol.subspan(i);
  return ret;
}

int extractSeqid(State& s)
{
  static auto const digits =
      gsl::cstring_span<>{"0123456789ABCDEFGHIJKLMNOPQRSTUVVWXYZ"};
  auto& symbol = s.remaining;
  auto const posit = std::find(symbol.begin(), symbol.end(), '_');
  if (posit == symbol.end())
    throw std::runtime_error("Unfinished seq-id: " + gsl::to_string(symbol));
  auto const len = std::distance(symbol.begin(), posit);
  if (len == 0)
  {
    s.skipChars(1);
    return 0;
  }
  auto ret = 0;
  for (auto i = 0; i < len; ++i)
  {
    auto const digitit = std::find(digits.begin(), digits.end(), symbol[i]);
    if (digitit == digits.end())
      throw std::runtime_error("Invalid digit in seq-id: " +
                               gsl::to_string(symbol));
    ret = ret * digits.size() + std::distance(digits.begin(), digitit);
  }
  s.skipChars(len + 1);
  return ret;
}

bool isSubstitution(gsl::cstring_span<> symbol)
{
  return symbol[0] == 'S';
}

std::string demanglePresetSubstitution(State& state)
{
  auto& symbol = state.remaining;
  if (symbol.empty())
    throw std::runtime_error("Trying to demangle empty preset substitution");
  // Preset substitution is at least 2 chars.
  if (symbol.size() < 2)
    throw std::runtime_error(
        "Trying to demangle too short preset substitution.");
  if (symbol[0] != 'S')
    return "";
  switch (symbol[1])
  {
#define CHAR_PRESET_SUBSTITUTION(c, str) \
  case c:                                \
    symbol = symbol.subspan(2);          \
    return str;                          \
    break
    // clang-format off
    CHAR_PRESET_SUBSTITUTION('t', "std");
    CHAR_PRESET_SUBSTITUTION('a', "std::allocator");
    CHAR_PRESET_SUBSTITUTION('b', "std::basic_string");
    CHAR_PRESET_SUBSTITUTION('s', "std::basic_string<char, std::char_traits<char>, std::allocator<char>>");
    CHAR_PRESET_SUBSTITUTION('i', "std::basic_istream<char, std::char_traits<char>>");
    CHAR_PRESET_SUBSTITUTION('o', "std::basic_ostream<char, std::char_traits<char>>");
    CHAR_PRESET_SUBSTITUTION('d', "std::basic_iostream<char, std::char_traits<char>>");
    // clang-format on
  }
  throw std::runtime_error("Invalid preset substitution: " +
                           gsl::to_string(symbol));
}

std::string demangleSeqSubstitution(State& state)
{
  auto const seqid = [&]() {
    try
    {
      return extractSeqid(state);
    }
    catch (std::runtime_error&)
    {
      throw std::runtime_error("Failed to extract seq-id for substitution: " +
                               gsl::to_string(state.remaining));
    }
  }();
  if ((unsigned)seqid >= state.substitutions.size())
    throw std::runtime_error(
        "Invalid substitution seqid: " + std::to_string(seqid) + " (got " +
        std::to_string(state.substitutions.size()) + " so far)");
  return state.substitutions[seqid];
}

std::string demangleSubstitution(State& state)
{
  assert(!state.empty());
  assert(isSubstitution(state.remaining));
  if (state.remaining.size() < 2)
    throw std::runtime_error("Trying to demangle too short substitution.");
  if (std::isdigit(state.remaining[1]))
    return demangleSeqSubstitution(state);
  return demanglePresetSubstitution(state);
}

gsl::cstring_span<> extractSourceName(gsl::cstring_span<>& symbol)
{
  auto i = 0u;
  auto len = 0u;

  if (symbol.empty())
    throw std::runtime_error("Extracting empty source name");
  if (!std::isdigit(symbol[0]))
    throw std::runtime_error("Source name does not start with a digit: " +
                             gsl::to_string(symbol));
  while (i < (unsigned)symbol.size() && std::isdigit(symbol[i]))
  {
    len = len * 10 + symbol[i] - '0';
    ++i;
  }
  if (i + len > (unsigned)(symbol.size()))
    throw std::runtime_error("Source name does not fit in symbol: " +
                             gsl::to_string(symbol));
  auto ret = symbol.subspan(i, len);
  symbol = symbol.subspan(i + len);
  return ret;
}

void demangleSpecialName(State& s)
{
  // TODO(ethiraric): demangleSpecialName
  (void)(s);
  throw std::runtime_error("Demangling special names is not supported yet");
}

std::string demangleType(State& s)
{
  assert(!s.empty());
  if (isSubstitution(s.remaining))
    return demangleSubstitution(s);
  switch (s.nextChar())
  {
#define CHAR_BUILTIN_TYPE(c, str) \
  case c:                         \
    s.skipChars(1);               \
    return str;                   \
    break;
    CHAR_BUILTIN_TYPE('v', "");
    CHAR_BUILTIN_TYPE('w', "wchar_t");
    CHAR_BUILTIN_TYPE('b', "bool");
    CHAR_BUILTIN_TYPE('c', "char");
    CHAR_BUILTIN_TYPE('a', "signed char");
    CHAR_BUILTIN_TYPE('h', "unsigned char");
    CHAR_BUILTIN_TYPE('s', "short");
    CHAR_BUILTIN_TYPE('t', "unsigned short");
    CHAR_BUILTIN_TYPE('i', "int");
    CHAR_BUILTIN_TYPE('j', "unsigned int");
    CHAR_BUILTIN_TYPE('l', "long");
    CHAR_BUILTIN_TYPE('m', "unsigned long");
    CHAR_BUILTIN_TYPE('x', "long long");
    CHAR_BUILTIN_TYPE('y', "unsigned long long");
    CHAR_BUILTIN_TYPE('n', "__int128");
    CHAR_BUILTIN_TYPE('o', "unsigned __int128");
    CHAR_BUILTIN_TYPE('f', "float");
    CHAR_BUILTIN_TYPE('d', "double");
    CHAR_BUILTIN_TYPE('e', "long double");
    CHAR_BUILTIN_TYPE('g', "__float128");
    CHAR_BUILTIN_TYPE('z', "...");
  case 'u':
  {
    s.skipChars(1);
    auto const vendorname = extractSourceName(s.remaining);
    return gsl::to_string(vendorname);
    break;
  }
  case 'D':
    s.skipChars(1);
    switch (s.nextChar())
    {
      CHAR_BUILTIN_TYPE('d', "decimal64");
      CHAR_BUILTIN_TYPE('e', "decimal128");
      CHAR_BUILTIN_TYPE('f', "decimal32");
      CHAR_BUILTIN_TYPE('h', "half");
      CHAR_BUILTIN_TYPE('i', "char32_t");
      CHAR_BUILTIN_TYPE('s', "char16_t");
      CHAR_BUILTIN_TYPE('a', "auto");
      CHAR_BUILTIN_TYPE('c', "decltype(auto)");
      CHAR_BUILTIN_TYPE('n', "decltype(nullptr)");
    default:
      throw std::runtime_error("Unknown type: D" + gsl::to_string(s.remaining));
    }
    break;
  }
#undef CHAR_BUILTIN_TYPE

  throw std::runtime_error("Unknown type: " + gsl::to_string(s.remaining));
}

void demangleBareFunctionType(State& s)
{
  assert(!s.empty());
  auto params = demangleType(s);
  if (!params.empty()) // If params is empty, function type is void (no param).
    while (!s.empty())
      params += ", " + demangleType(s);
  if (s.add_params)
    s.demangled += '(' + params + ')';
}

void demangleData(State& s)
{
  auto const name = extractSourceName(s.remaining);
  s.demangled.append(name.begin(), name.end());
  if (s.remaining.empty())
    return;
  demangleBareFunctionType(s);
}

std::string demangle_internal(State s)
{
  if (s.remaining.size() < 3 ||
      (s.remaining[0] != '_' || s.remaining[1] != 'Z'))
    return s.symbol;
  s.skipChars(2);
  if (s.nextChar() == 'T')
    demangleSpecialName(s);
  else
    demangleData(s);
  return s.demangled;
}
}

std::string demangle(std::string const& symbol)
{
  return demangle_internal({symbol, true, true});
}

std::string demangle(std::string const& symbol, NoParamTag)
{
  return demangle_internal({symbol, false, true});
}

std::string demangle(std::string const& symbol, NoTemplateParamTag)
{
  return demangle_internal({symbol, true, false});
}

std::string demangle(std::string const& symbol, MinimalTag)
{
  return demangle_internal({symbol, false, false});
}
}
