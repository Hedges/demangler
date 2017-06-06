#include <demangler/demangler.hh>

#include <sstream>

#include <demangler/details/Parser.hh>

namespace demangler
{
namespace
{
std::string demangle(std::string const& symbol,
                     details::PrintOptions const& opt)
{
  details::Parser p;
  auto ast = p.parse(symbol);
  if (!ast)
    return symbol;
  auto ss = std::stringstream{};
  ast->print(opt, ss);
  return ss.str();
}
}
std::string demangle(std::string const& symbol)
{
  return demangle(symbol, {true, true});
}

std::string demangle(std::string const& symbol, NoParamTag)
{
  return demangle(symbol, {false, true});
}

std::string demangle(std::string const& symbol, NoTemplateParamTag)
{
  return demangle(symbol, {true, false});
}

std::string demangle(std::string const& symbol, MinimalTag)
{
  return demangle(symbol, {false, false});
}
}
