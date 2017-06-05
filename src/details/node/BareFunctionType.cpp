#include <demangler/details/node/BareFunctionType.hh>

#include <cassert>
#include <stdexcept>

#include <demangler/details/node/Type.hh>

namespace demangler
{
namespace details
{
namespace node
{
BareFunctionType::BareFunctionType() noexcept : Node{Type::BareFunctionType}
{
}

std::ostream& BareFunctionType::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  out << '(';
  for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
  {
    if (i)
      out << ", ";
    this->getNode(i)->print(opt, out);
  }
  out << ')';
  return out;
}

std::unique_ptr<BareFunctionType> BareFunctionType::parse(State &s)
{
  auto ret = std::make_unique<BareFunctionType>();
  while (!s.empty())
  {
    auto type = node::Type::parse(s);
    ret->addNode(std::move(type));
  }
  return ret;
}
}
}
}
