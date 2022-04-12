#include "aoc/helpers.h"

#include <algorithm>
#include <numeric>
#include <set>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  const auto IsBlockListed = [](const std::string_view word) {
    for (size_t i = 1; i < word.size(); i++) {
      switch (word[i - 1]) {
        case 'a':
          if (word[i] == 'b') { return true; }
          break;
        case 'c':
          if (word[i] == 'd') { return true; }
          break;
        case 'p':
          if (word[i] == 'q') { return true; }
          break;
        case 'x':
          if (word[i] == 'y') { return true; }
          break;
        default:
          break;
      }
    }
    return false;
  };

  const auto ContainsDouble = [](const std::string_view word) {
    for (size_t i = 1; i < word.size(); i++) {
      if (word[i] == word[i-1]) { return true; }
    }
    return false;
  };

  const auto CountVowels = [](const std::string_view word) {
    size_t count = 0;

    return std::accumulate(word.cbegin(), word.cend(), count, [](size_t n, const char c) {
      switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
          return n + 1;
      }

      return n;
    });
  };

  const auto ContainsPalandromeTriple = [](const std::string_view word) {
    if (word.size() < 3) { return false; }

    for (size_t i = 1; i < word.size() - 1; i++) {
      const auto c1 = word[i-1];
      const auto c2 = word[i];
      const auto c3 = word[i+1];

      if (c1 == c3 && c2 != c1) { return true; }
    }
    return false;
  };

  const auto ContainsTwoPairs = [](const std::string_view word) {
    if (word.size() < 4) { return false; }

    for (size_t i = 1; i < word.size() - 2; i++) {
      char pair[2] = { word[i - 1], word[i] };

      for (size_t j = i + 1; j < word.size() - 1; j++) {
        if (pair[0] == word[j] && pair[1] == word[j + 1]) {
          return true;
        }
      }
    }

    return false;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::string_view word;
  size_t part1 = 0;
  size_t part2 = 0;
  while (aoc::getline(f, word)) {
    part1 += (ContainsDouble(word) && !IsBlockListed(word) && CountVowels(word) >= 3);
    part2 += (ContainsTwoPairs(word) && ContainsPalandromeTriple(word));
  }

  aoc::print_results(part1, part2);

  return 0;
}

