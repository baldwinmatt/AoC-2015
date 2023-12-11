#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  const auto RotateChar = [](const char c) {
    switch (c) {
      case 'z':
        return 'a';
      case 'h':
      case 'n':
      case 'k':
        return static_cast<char>(c + 2);
      default:
        return static_cast<char>(c + 1);
    }
  };

  const auto RotateString = [](std::string& s) {
    ssize_t i = s.size() - 1;
    s[i] = RotateChar(s[i]);

    while (i >= 0 && s[i] == 'a') {
        i--;
        s[i] = RotateChar(s[i]);
    }
  };

  const auto IsGoodPassword = [](const std::string& s) {
    bool straight = false;
    for(size_t i = 2; !straight && i < s.size(); i++) {
        straight = (s[i - 2] + 1 == s[i - 1] && s[i - 1] + 1 == s[i]);
    }
    if (!straight) {
        return false;
    }
    int pc = 0;
    for(size_t i = 1; i < s.size(); i++) {
        if (s[i - 1] == s[i]) {
            if (pc == 1) {
                return true;
            } else {
                pc++;
                i++;
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

  std::string pwd{f.data(), f.size()};

  do {
    RotateString(pwd);
  } while (!IsGoodPassword(pwd));

  std::string part1 = pwd;

  do {
    RotateString(pwd);
  } while (!IsGoodPassword(pwd));

  std::string part2 = pwd;

  aoc::print_results(part1, part2);  

  return 0;
}

