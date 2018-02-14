#include <demangler/details/node/Number.hh>

#include <algorithm>
#include <cassert>

#include <gsl/string_span>

namespace demangler
{
namespace details
{
namespace node
{
Number::Number() noexcept : Node{Type::Number}
{
}

Number::Number(clone_tag, Number const& b)
  : Node{clone_tag{}, b}, number{b.number}
{
}

std::ostream& Number::print(PrintOptions const&, std::ostream& out) const
{
  out << this->number;
  return out;
}

std::unique_ptr<Node> Number::deepClone() const
{
  return std::make_unique<Number>(clone_tag{}, *this);
}

std::unique_ptr<Number> Number::parse(State& s)
{
  static auto const digits = gsl::cstring_span<>{"0123456789"};

  auto negative = false;

  if (s.nextChar() == 'n')
  {
    negative = true;
    s.advance(1);
    if (s.empty())
      throw std::runtime_error("Empty negative <number>");
  }

  auto it = std::find(digits.begin(), digits.end(), s.nextChar());
  if (it == digits.end())
    throw std::runtime_error("Invalid <number>");

  auto num = int64_t{0};
  auto i = 0;

  while (it != digits.end())
  {
    num = num * digits.size() + std::distance(digits.begin(), it);
    it = std::find(digits.begin(), digits.end(), s.symbol[++i]);
  }
  s.advance(i);

  auto ret = std::make_unique<Number>();
  ret->number = negative ? -num : num;
  return ret;
}

int64_t Number::getNumber() const noexcept
{
  return this->number;
}
}
}
}
