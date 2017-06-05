#include <demangler/demangler.hh>

#include <sstream>

#include <demangler/details/Demangler.hh>
#include <demangler/details/Parser.hh>

namespace demangler
{
std::string demangle(std::string const& symbol)
{
  details::Parser p;
  auto ast = p.parse(symbol);
  if (!ast)
    return symbol;
  auto ss = std::stringstream{};
  details::PrintOptions o;
  ast->print(o, ss);
  return ss.str();
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
