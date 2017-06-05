#ifndef DEMANGLER_DETAILS_PARSER_HH_
#define DEMANGLER_DETAILS_PARSER_HH_

#include <memory>

#include <demangler/details/Node.hh>

namespace demangler
{
namespace details
{
class Parser
{
public:
  Parser() noexcept = default;
  Parser(Parser const& b) noexcept = default;
  Parser(Parser&& b) noexcept = default;
  ~Parser() noexcept = default;

  Parser& operator=(Parser const& rhs) noexcept = default;
  Parser& operator=(Parser&& rhs) noexcept = default;

  std::unique_ptr<Node> parse(std::string const& symbol);

private:
};
}
}

#endif /* !DEMANGLER_DETAILS_PARSER_HH_ */
