#include <demangler/details/Demangler.hh>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <stdexcept>

namespace demangler
{
namespace details
{
Demangler::Demangler(std::string const& psymbol, bool params, bool templates)
  : symbol{psymbol},
    remaining{this->symbol},
    add_params{params},
    add_templates{templates}
{
  this->demangle();
}

std::string const& Demangler::getDemangled() const noexcept
{
  return this->demangled;
}

void Demangler::demangle()
{
  if (this->remaining.size() < 3 ||
      (this->remaining[0] != '_' || this->remaining[1] != 'Z'))
  {
    this->demangled = symbol;
    return;
  }
  this->advance(2);
  if (this->nextChar() == 'T')
    this->nodeSpecialName();
  else
    this->nodeEncoding();
  return;
}

void Demangler::nodeSpecialName()
{
  // TODO(ethiraric): nodeSpecialName
  throw std::runtime_error("Demangling special names is not supported yet");
}

void Demangler::nodeEncoding()
{
  auto const name = this->nodeSourceName();
  this->demangled.append(name.begin(), name.end());
  if (this->remaining.empty())
    return;
  this->nodeBareFunctionType();
}

void Demangler::nodeBareFunctionType()
{
  assert(!this->empty());
  auto params = this->nodeType();
  if (!params.empty()) // If params is empty, function type is void (no param).
    while (!this->empty())
      params += ", " + this->nodeType();
  if (this->add_params)
    this->demangled += '(' + params + ')';
}

std::string Demangler::nodeType()
{
  assert(!this->empty());
  if (this->nextIsSubstitution())
    return this->nodeSubstitution();
  if (std::isdigit(this->nextChar()))
  {
    auto const name = this->nodeSourceName();
    this->substitutions.emplace_back(name);
    return gsl::to_string(name);
  }
  switch (this->nextChar())
  {
#define CHAR_BUILTIN_TYPE(c, str) \
  case c:                         \
    this->advance(1);             \
    return str;                   \
    break;
    CHAR_BUILTIN_TYPE('v', "");
    CHAR_BUILTIN_TYPE('w', "wchar_t");
    CHAR_BUILTIN_TYPE('b', "bool");
    CHAR_BUILTIN_TYPE('c', "char");
    CHAR_BUILTIN_TYPE('a', "signed char");
    CHAR_BUILTIN_TYPE('h', "unsigned char");
    CHAR_BUILTIN_TYPE('s', "short");
    CHAR_BUILTIN_TYPE('t', "unsigned short");
    CHAR_BUILTIN_TYPE('i', "int");
    CHAR_BUILTIN_TYPE('j', "unsigned int");
    CHAR_BUILTIN_TYPE('l', "long");
    CHAR_BUILTIN_TYPE('m', "unsigned long");
    CHAR_BUILTIN_TYPE('x', "long long");
    CHAR_BUILTIN_TYPE('y', "unsigned long long");
    CHAR_BUILTIN_TYPE('n', "__int128");
    CHAR_BUILTIN_TYPE('o', "unsigned __int128");
    CHAR_BUILTIN_TYPE('f', "float");
    CHAR_BUILTIN_TYPE('d', "double");
    CHAR_BUILTIN_TYPE('e', "long double");
    CHAR_BUILTIN_TYPE('g', "__float128");
    CHAR_BUILTIN_TYPE('z', "...");
  case 'u':
  {
    this->advance(1);
    auto const vendorname = this->nodeSourceName();
    return gsl::to_string(vendorname);
    break;
  }
  case 'D':
    this->advance(1);
    switch (this->nextChar())
    {
      CHAR_BUILTIN_TYPE('d', "decimal64");
      CHAR_BUILTIN_TYPE('e', "decimal128");
      CHAR_BUILTIN_TYPE('f', "decimal32");
      CHAR_BUILTIN_TYPE('h', "half");
      CHAR_BUILTIN_TYPE('i', "char32_t");
      CHAR_BUILTIN_TYPE('s', "char16_t");
      CHAR_BUILTIN_TYPE('a', "auto");
      CHAR_BUILTIN_TYPE('c', "decltype(auto)");
      CHAR_BUILTIN_TYPE('n', "decltype(nullptr)");
    default:
      throw std::runtime_error("Unknown type: D" + this->remainingStr());
    }
    break;
  }
#undef CHAR_BUILTIN_TYPE

  throw std::runtime_error("Unknown type: " + this->remainingStr());
}

gsl::cstring_span<> Demangler::nodeSourceName()
{
  if (this->remaining.empty())
    throw std::runtime_error("Extracting empty source name");
  if (!std::isdigit(this->remaining[0]))
    throw std::runtime_error("Source name does not start with a digit: " +
                             this->remainingStr());
  auto len = nodeNumber();
  if (len > this->remaining.size())
    throw std::runtime_error("Source name does not fit in symbol: " +
                             this->remainingStr());
  auto ret = this->remaining.subspan(0, len);
  this->remaining = this->remaining.subspan(len);
  return ret;
}

int Demangler::nodeNumber()
{
  auto i = 0u;
  auto ret = 0u;

  if (this->remaining.empty())
    throw std::runtime_error("Extracting empty decimal");
  if (!std::isdigit(this->remaining[0]))
    throw std::runtime_error("Extracting decimal which is not decimal: " +
                             this->remainingStr());
  while (i < (unsigned)this->remaining.size() &&
         std::isdigit(this->remaining[i]))
  {
    ret = ret * 10 + this->remaining[i] - '0';
    ++i;
  }
  this->advance(i);
  return ret;
}

int Demangler::nodeSeqId()
{
  static auto const digits =
      gsl::cstring_span<>{"0123456789ABCDEFGHIJKLMNOPQRSTUVVWXYZ"};
  this->advance(1);
  auto const posit =
      std::find(this->remaining.begin(), this->remaining.end(), '_');
  if (posit == this->remaining.end())
    throw std::runtime_error("Unfinished seq-id: " + this->remainingStr());
  auto const len = std::distance(this->remaining.begin(), posit);
  if (len == 0)
  {
    this->advance(1);
    return 0;
  }
  auto ret = 0;
  for (auto i = 0; i < len; ++i)
  {
    auto const digitit =
        std::find(digits.begin(), digits.end(), this->remaining[i]);
    if (digitit == digits.end())
      throw std::runtime_error("Invalid digit in seq-id: " +
                               this->remainingStr());
    ret = ret * digits.size() + std::distance(digits.begin(), digitit);
  }
  this->advance(len + 1);
  return ret + 1;
}

bool Demangler::nextIsSubstitution() const noexcept
{
  return this->remaining[0] == 'S';
}

std::string Demangler::nodeSubstitution()
{
  assert(!this->empty());
  assert(this->nextIsSubstitution());
  if (this->remaining.size() < 2)
    throw std::runtime_error("Trying to demangle too short substitution.");
  if (std::isdigit(this->remaining[1]) || this->remaining[1] == '_')
    return this->nodeSubstitutionSeqId();
  return this->nodeSubstitutionSx();
}

std::string Demangler::nodeSubstitutionSx()
{
  if (this->empty())
    throw std::runtime_error("Trying to demangle empty preset substitution");
  // Preset substitution is at least 2 chars.
  if (this->remaining.size() < 2)
    throw std::runtime_error(
        "Trying to demangle too short preset substitution.");
  if (this->remaining[0] != 'S')
    return "";
  switch (this->remaining[1])
  {
#define CHAR_PRESET_SUBSTITUTION(c, str) \
  case c:                                \
    this->advance(2);                    \
    return str;                          \
    break
    // clang-format off
    CHAR_PRESET_SUBSTITUTION('t', "std");
    CHAR_PRESET_SUBSTITUTION('a', "std::allocator");
    CHAR_PRESET_SUBSTITUTION('b', "std::basic_string");
    CHAR_PRESET_SUBSTITUTION('s', "std::basic_string<char, std::char_traits<char>, std::allocator<char>>");
    CHAR_PRESET_SUBSTITUTION('i', "std::basic_istream<char, std::char_traits<char>>");
    CHAR_PRESET_SUBSTITUTION('o', "std::basic_ostream<char, std::char_traits<char>>");
    CHAR_PRESET_SUBSTITUTION('d', "std::basic_iostream<char, std::char_traits<char>>");
    // clang-format on
  }
  throw std::runtime_error("Invalid preset substitution: " +
                           this->remainingStr());
}

std::string Demangler::nodeSubstitutionSeqId()
{
  auto const seqid = [&]() {
    try
    {
      return this->nodeSeqId();
    }
    catch (std::runtime_error&)
    {
      throw std::runtime_error("Failed to extract seq-id for substitution: " +
                               this->remainingStr());
    }
  }();
  if ((unsigned)seqid >= this->substitutions.size())
    throw std::runtime_error(
        "Invalid substitution seqid: " + std::to_string(seqid) + " (got " +
        std::to_string(this->substitutions.size()) + " so far)");
  return gsl::to_string(this->substitutions[seqid]);
}

void Demangler::advance(unsigned int n) noexcept
{
  assert(n <= (unsigned)(this->remaining.size()));
  this->remaining = this->remaining.subspan(n);
}

char Demangler::nextChar() const noexcept
{
  return this->remaining[0];
}

bool Demangler::empty() const noexcept
{
  return this->remaining.empty();
}

std::string Demangler::remainingStr() const
{
  return gsl::to_string(this->remaining);
}
}
}
