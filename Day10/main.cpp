#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  const auto Iterate = [](const std::string_view s) {
    std::string out;
    
    for (size_t i = 0; i < s.size();) {
      char c = s[i];
      const auto n = s.find_first_not_of(c, i);
      const auto len = (n == std::string::npos ? s.size() - i : n - i);
      out.append(std::to_string(len));
      out.append(&c, 1);
      i += len;
    }

    return out;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::string in{f.data(), f.size()};

  DEBUG(
    std::string t{"1"};
    for (size_t i = 0; i < 5; i++) {
      t = Iterate(t);
      DEBUG_PRINT((i + 1) << ": " << t);
    }
  );

  DEBUG_PRINT(0 << ": " << in);
  for (size_t i = 0; i < 40; i++) {
    in = Iterate(in);
    DEBUG_PRINT((i + 1) << ": " << in);
  }
  const size_t part1 = in.size();

  for (size_t i = 40; i < 50; i++) {
    in = Iterate(in);
    DEBUG_PRINT((i + 1) << ": " << in);
  }
  const size_t part2 = in.size();

  aoc::print_results(part1, part2);

  return 0;
}

