#include <demangler/details/State.hh>

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
void State::advance(unsigned int n) noexcept
{
  this->symbol = this->symbol.subspan(n);
}

std::string State::toString() const
{
  return gsl::to_string(this->symbol);
}

bool State::empty() const noexcept
{
  return this->symbol.empty();
}

char State::nextChar() const noexcept
{
  return this->symbol[0];
}

bool State::hasNext(char c) const noexcept
{
  return (!this->empty()) && this->nextChar() == c;
}

bool State::hasNext(gsl::cstring_span<> needle) const noexcept
{
  return this->symbol.size() >= needle.size() &&
         std::equal(needle.begin(), needle.end(), symbol.begin());
}

gsl::cstring_span<>::index_type State::charsRemaining() const noexcept
{
  return this->symbol.size();
}

char State::peekChar(unsigned int idx) const noexcept
{
  return this->symbol[idx];
}

void State::registerUserSubstitution(Node const* node)
{
  this->user_substitutions.emplace_back(node);
}

Node const* State::getUserSubstitution(std::size_t index) const
{
  if (index >= this->user_substitutions.size())
    throw std::runtime_error("Invalid substitution index (got " +
                             std::to_string(this->user_substitutions.size()) +
                             " so far): " + this->toString());
  return this->user_substitutions[index];
}

void State::dumpSubstitutions() const
{
  std::cout << "Substitutions" << std::endl;
  auto i = 0;
  for (auto const& node : this->user_substitutions)
  {
    std::cout << "\t#" << i++ << ": ";
    node->print({true, true}, std::cout);
    std::cout << std::endl;
  }
}

Node* State::addOrphanedSubstitution(std::unique_ptr<Node>&& orphan)
{
  this->orphaned_substitutions.emplace_back(std::move(orphan));
  return this->orphaned_substitutions.back().get();
}
}
}
