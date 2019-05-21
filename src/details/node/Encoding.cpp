#include <demangler/details/node/Encoding.hh>

#include <cassert>
#include <stdexcept>

#include <demangler/details/CVQualifiers.hh>
#include <demangler/details/Utils.hh>
#include <demangler/details/node/BareFunctionType.hh>
#include <demangler/details/node/Name.hh>
#include <demangler/details/node/NestedName.hh>

namespace demangler
{
namespace details
{
namespace node
{
namespace
{
bool nameHasReturnType(Name const& name)
{
  auto const& lastnode = *name.getNode(name.getNodeCount() - 1);
  return lastnode.getType() == Node::Type::TemplateArgs ||
         (lastnode.getType() == Node::Type::NestedName &&
          static_cast<NestedName const&>(lastnode).willHaveReturnType());
}
}
Encoding::Encoding() noexcept : Node{Type::Encoding}
{
}

Encoding::Encoding(clone_tag, Encoding const& b) : Node{clone_tag{}, b}
{
}

std::ostream& Encoding::print(PrintOptions const& opt, std::ostream& out) const
{
  assert(this->getNodeCount() != 0);
  auto const printCVQuals = [&](size_t node_index, char const* prefix = "") {
    auto const cvquals =
        static_cast<Name const&>(*this->getNode(node_index)).getCVQuals();
    if (!cvquals.empty())
    {
      out << prefix;
      printCVRefQualifiers(out, cvquals);
    }
  };

  switch (this->getNodeCount())
  {
    // 1 node -> Name
    case 1:
      this->getNode(0)->print(opt, out);
      printCVQuals(0);
      break;
    // 2 nodes -> Name + Args (function)
    case 2:
      this->getNode(0)->print(opt, out);
      this->getNode(1)->print(opt, out);
      printCVQuals(0, opt.add_parameters ? "" : "()");
      break;
    // 3 nodes -> Return type + Name + Args (function)
    case 3:
      if (opt.add_parameters)
      {
        this->getNode(0)->print(opt, out);
        out << ' ';
      }
      this->getNode(1)->print(opt, out);
      this->getNode(2)->print(opt, out);
      printCVQuals(1, opt.add_parameters ? "" : "()");
      break;
    // No other possible case.
  }
  return out;
}

std::unique_ptr<Node> Encoding::deepClone() const
{
  return std::make_unique<Encoding>(clone_tag{}, *this);
}

std::unique_ptr<Encoding> Encoding::parse(State& s, bool in_expr)
{
  auto ret = std::make_unique<Encoding>();
  auto name = Name::parse(s);
  if (!in_expr && !s.empty())
  {
    auto const has_return_type = nameHasReturnType(*name);
    auto bft = BareFunctionType::parse(s, has_return_type);
    if (has_return_type)
      ret->addNode(bft->retrieveReturnType());
    ret->addNode(std::move(name));
    ret->addNode(std::move(bft));
  }
  else
    ret->addNode(std::move(name));
  return ret;
}
}
}
}
