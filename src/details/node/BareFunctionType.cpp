#include <demangler/details/node/BareFunctionType.hh>
#include <iostream>

#include <algorithm>
#include <cassert>
#include <stdexcept>

#include <demangler/details/node/BuiltinType.hh>
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

BareFunctionType::BareFunctionType(clone_tag, BareFunctionType const& b)
  : Node{clone_tag{}, b}
{
}

std::ostream& BareFunctionType::print(PrintOptions const& opt,
                                      std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  if (!opt.add_parameters)
    return out;
  if (this->isVoidFunction())
    return (out << "()");

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

std::unique_ptr<Node> BareFunctionType::deepClone() const
{
  return std::make_unique<BareFunctionType>(clone_tag{}, *this);
}

std::unique_ptr<BareFunctionType> BareFunctionType::parse(State& s)
{
  auto ret = std::make_unique<BareFunctionType>();
  while (!s.empty())
  {
    auto type = node::Type::parse(s);
    ret->addNode(std::move(type));
  }
  return ret;
}

bool BareFunctionType::isVoidFunction() const noexcept
{
  if (this->getNodeCount() == 1)
  {
    // Get the Type node first, then the BuiltinType.
    auto const* node = this->getNode(0)->getNode(0);
    if (node->getType() == Type::BuiltinType)
    {
      auto btype = static_cast<BuiltinType const*>(node)->getTypeName();
      if (btype.size() == 4 && std::equal(btype.begin(), btype.end(), "void"))
        return true;
    }
  }
  return false;
}
}
}
}
