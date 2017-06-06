#ifndef DEMANGLER_DETAILS_NODE_SEQID_HH_
#define DEMANGLER_DETAILS_NODE_SEQID_HH_

#include <gsl/string_span>

#include <demangler/details/State.hh>

namespace demangler
{
namespace details
{
namespace node
{
// This one is not a node strictly speaking, but rather an utility that extracts
// a seq-id.
class SeqId
{
public:
  SeqId() noexcept = default;
  SeqId(SeqId const& b) noexcept = default;
  SeqId(SeqId&& b) noexcept = default;
  ~SeqId() noexcept = default;

  SeqId& operator=(SeqId const& rhs) noexcept = default;
  SeqId& operator=(SeqId&& rhs) noexcept = default;

  static unsigned int parse(State &s);

};
}
}
}

#endif /* !DEMANGLER_DETAILS_NODE_SEQID_HH_ */
