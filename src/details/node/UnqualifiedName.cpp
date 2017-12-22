#include <demangler/details/node/UnqualifiedName.hh>

#include <cassert>
#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/Constructor.hh>
#include <demangler/details/node/OperatorName.hh>
#include <demangler/details/node/SourceName.hh>

namespace demangler
{
namespace details
{
namespace node
{
UnqualifiedName::UnqualifiedName() noexcept : Node{Type::UnqualifiedName}
{
}

std::ostream& UnqualifiedName::print(PrintOptions const& opt,
                                     std::ostream& out) const
{
  assert(this->getNodeCount() == 1);
  this->getNode(0)->print(opt, out);
  return out;
}

std::unique_ptr<UnqualifiedName> UnqualifiedName::parse(State& s,
                                                        string_type ctorname)
{
  auto ret = std::make_unique<UnqualifiedName>();
  if (std::isdigit(s.symbol[0]))
  {
    ret->addNode(SourceName::parse(s));
    return ret;
  }
  else if (s.nextChar() == 'C')
  {
    if (ctorname.empty())
      throw std::runtime_error("Constructor of unknown class: " + s.toString());
    s.advance(1);
    if (!std::isdigit(s.nextChar()))
      throw std::runtime_error("Unknown constructor: C" + s.toString());
    ret->addNode(std::make_unique<Constructor>(ctorname));
    s.advance(1);
    return ret;
  }
  else
  {
    auto operatorname = OperatorName::parse(s);
    if (operatorname)
    {
      ret->addNode(std::move(operatorname));
      return ret;
    }
  }
  throw std::runtime_error("Unknown unqualified-name: " + s.toString());
}
}
}
}
