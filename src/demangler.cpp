#include <demangler/demangler.hh>

#include <demangler/details/Demangler.hh>

namespace demangler
{
std::string demangle(std::string const& symbol)
{
  return details::Demangler({symbol, true, true}).getDemangled();
}

std::string demangle(std::string const& symbol, NoParamTag)
{
  return details::Demangler({symbol, false, true}).getDemangled();
}

std::string demangle(std::string const& symbol, NoTemplateParamTag)
{
  return details::Demangler({symbol, true, false}).getDemangled();
}

std::string demangle(std::string const& symbol, MinimalTag)
{
  return details::Demangler({symbol, false, false}).getDemangled();
}
}
