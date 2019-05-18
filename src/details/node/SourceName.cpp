#include <demangler/details/node/SourceName.hh>

#include <cctype>
#include <stdexcept>

#include <demangler/details/Utils.hh>

namespace demangler
{
namespace details
{
namespace node
{
SourceName::SourceName() noexcept : Node{Type::SourceName}
{
}

SourceName::SourceName(string_type pname) noexcept
  : Node{Type::SourceName}, name{pname}
{
}

SourceName::SourceName(clone_tag, SourceName const& b)
  : Node{clone_tag{}, b}, name{b.name}
{
}

std::ostream& SourceName::print(PrintOptions const&, std::ostream& out) const
{
  printStringSpan(out, this->name);
  return out;
}

std::unique_ptr<Node> SourceName::deepClone() const
{
  return std::make_unique<SourceName>(clone_tag{}, *this);
}

std::unique_ptr<SourceName> SourceName::parse(State& s)
{
  if (!std::isdigit(s.symbol[0]))
    throw std::runtime_error("Expected number for source-name: " +
                             s.toString());
  auto length = 0;
  auto i = 0;
  while (i < s.symbol.size() && std::isdigit(s.symbol[i]))
  {
    length = length * 10 + s.symbol[i] - '0';
    ++i;
  }
  if (s.symbol.size() < length + i)
    throw std::runtime_error("source-name does not fit: " + s.toString());
  auto ret = std::make_unique<SourceName>();
  ret->name = s.symbol.subspan(i, length);
  s.advance(static_cast<unsigned int>(i + length));
  return ret;
}

SourceName::string_type SourceName::getName() const noexcept
{
  return this->name;
}
}
}
}
