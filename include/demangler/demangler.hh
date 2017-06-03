#ifndef DEMANGLER_DEMANGLER_HH_
#define DEMANGLER_DEMANGLER_HH_

#include <string>

namespace demangler
{
struct NoParamTag
{
};
struct NoTemplateParamTag
{
};
struct MinimalTag
{
};

std::string demangle(std::string const& symbol);
std::string demangle(std::string const& symbol, NoParamTag);
std::string demangle(std::string const& symbol, NoTemplateParamTag);
std::string demangle(std::string const& symbol, MinimalTag);
}

#endif /* !DEMANGLER_DEMANGLER_HH_ */
