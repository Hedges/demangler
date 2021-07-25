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
  bool first = true;
  for (auto i = size_t{0}; i < this->getNodeCount(); ++i)
  {
    auto const& node = *this->getNode(i);
    if (!node.isEmpty())
    {
      if (first)
        first = false;
      else
        out << ", ";
      node.print(opt, out);
    }
  }
  out << ')';
  return out;
}

std::unique_ptr<Node> BareFunctionType::deepClone() const
{
  return std::make_unique<BareFunctionType>(clone_tag{}, *this);
}

std::unique_ptr<BareFunctionType> BareFunctionType::parse(State& s,
                                                          bool phas_return_type,
                                                          bool in_argument)
{
  assert(!s.empty());
  auto ret = std::make_unique<BareFunctionType>();
  if (phas_return_type)
    ret->return_type = node::Type::parse(s);
  if (!in_argument)
    while (!s.empty() && s.nextChar() != 'E')
    {
      if (s.nextChar() == '@')
      {
        s.advance(s.symbol.length());
      }
      else
      {
        ret->addNode(node::Type::parse(s));
      }
    }
  else
  {
    while (!s.empty() && s.nextChar() != 'E')
    {
      if (s.nextChar() == '@')
      {
        s.advance(s.symbol.length());
      }
      else
      {
        ret->addNode(node::Type::parse(s));
      }
    }
    if (s.empty())
      throw std::runtime_error("Unfinished BareFunctionType");
    s.advance(1);
  }
  ret->has_return_type = phas_return_type;
  return ret;
}

bool BareFunctionType::hasReturnType() const noexcept
{
  return this->has_return_type;
}

std::unique_ptr<node::Type> BareFunctionType::retrieveReturnType() noexcept
{
  if (!this->hasReturnType())
    return nullptr;
  this->has_return_type = false;
  return std::move(this->return_type);
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
