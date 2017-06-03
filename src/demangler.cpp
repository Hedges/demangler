#include <demangler/demangler.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

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

  std::string const& symbol;
  gsl::cstring_span<> remaining;
  bool add_params;
  bool add_templates;
  std::string demangled;
};

gsl::cstring_span<> extractSourceName(gsl::cstring_span<>& symbol)
{
  auto i = 0u;
  auto len = 0u;

  if (symbol.empty())
    throw std::runtime_error("Extracting empty source name");
  if (!std::isdigit(symbol[0]))
    throw std::runtime_error("Source name does not start with a digit");
  while (i < (unsigned)symbol.size() && std::isdigit(symbol[i]))
  {
    len = len * 10 + symbol[i] - '0';
    ++i;
  }
  if (i + len >= (unsigned)(symbol.size()))
    throw std::runtime_error("Source name does not fit in symbol");
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

void demangleBareFunctionType(State& s)
{
  assert(!s.remaining.empty());
  auto params = std::string{};
  if (s.nextChar() == 'v')
    ;
  else
    throw std::runtime_error("Failed to parse bare function type");
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
