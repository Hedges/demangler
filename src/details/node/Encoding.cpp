#include <demangler/details/node/Encoding.hh>

#include <cassert>
#include <stdexcept>

#include <demangler/details/Utils.hh>
#include <demangler/details/node/BareFunctionType.hh>
#include <demangler/details/node/Name.hh>

namespace demangler
{
namespace details
{
namespace node
{
Encoding::Encoding() noexcept : Node{Type::Encoding}
{
}

std::ostream& Encoding::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
    this->getNode(i)->print(opt, out);
  return out;
}

std::unique_ptr<Encoding> Encoding::parse(State& s)
{
  auto ret = std::make_unique<Encoding>();
  ret->addNode(Name::parse(s));
  if (!s.empty())
    ret->addNode(BareFunctionType::parse(s));
  return ret;
}
}
}
}
