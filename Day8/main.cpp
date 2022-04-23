#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  enum class State {
    Initial,
    Literal,
    Slash,
    Hex,
    Hex0,
    Last,
  };

  const auto DecodeString = [](std::string_view s) {
#ifndef NDEBUG
    std::string out;
#endif
    size_t len = 0;
    State i = State::Initial;

    char d = 0;

    for (const auto& c : s) {
      switch (i) {
        case State::Initial:
          if (c != '"') {
            throw std::runtime_error("Bad Input 1");
          }
          i = State::Literal;
          break;
        case State::Literal:
          if (c == '"') {
            i = State::Last;
          } else if (c != '\\') {
            DEBUG(out.append(&c, 1));
            len++;
          } else {
            i = State::Slash;
          }
          break;
        case State::Slash:
          if (c == '"' || c == '\\') {
            DEBUG(out.append(&c, 1));
            len++;
            i = State::Literal;
          } else if (c == 'x') {
            i = State::Hex;
            d = 0;
          } else {
            throw std::runtime_error("Bad Input 2");
          }
          break;
        case State::Hex:
        case State::Hex0:
          if (c >= '0' && c <= '9') {
            d <<= 4;
            d |= (c - '0');
          } else if (c >= 'A' && c <= 'F') {
            d <<= 4;
            d |= ((c - 'A') + 10);
          } else if (c >= 'a' && c <= 'f') {
            d <<= 4;
            d |= ((c - 'a') + 10);
          } else {
            throw std::runtime_error("Bad Input 3");
          }

          if (i == State::Hex) {
            i = State::Hex0;
          } else {
            len++;
            DEBUG(out.append(&d, 1));
            i = State::Literal;
          }
          break;
        case State::Last:
          throw std::runtime_error("Bad Input 4");
      }
    }

    DEBUG_PRINT("In: " << s << " Out: " << out);

    return len;
  };

  const auto EncodeString = [](std::string_view s) {
#ifndef NDEBUG
    std::string out{"\""};
#endif
    size_t len = 1;

    for (const auto& c : s) {
        switch (c) {
          case '"':
            DEBUG(out.append("\\\""));
            len+=2;
            break;
          case '\\':
            DEBUG(out.append("\\\\"));
            len+=2;
            break;
          default:
            DEBUG(out.append(&c, 1));
            len++;
            break;
        }
    }

    DEBUG(out.append("\"", 1));
    len++;
    DEBUG_PRINT("In: " << s << " Out: " << out);

    return len;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::string_view line;
  size_t memory = 0;
  size_t chars = 0;
  size_t enc_chars = 0;
  while (aoc::getline(f, line)) {
    memory += line.size();
    const auto out = DecodeString(line);
    const auto enc = EncodeString(line);
    chars += out;
    enc_chars += enc;
  }

  size_t part1 = memory - chars;
  size_t part2 = enc_chars - memory;
  aoc::print_results(part1, part2);

  return 0;
}

