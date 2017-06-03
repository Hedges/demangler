#include <demangler/demangler.hh>

namespace demangler
{
std::string demangle(std::string const& symbol)
{
  return symbol;
}

std::string demangle(std::string const& symbol, NoParamTag)
{
  return demangle(symbol);
}

std::string demangle(std::string const& symbol, NoTemplateParamTag)
{
  return demangle(symbol);
}

std::string demangle(std::string const& symbol, MinimalTag)
{
  return demangle(symbol);
}
}
