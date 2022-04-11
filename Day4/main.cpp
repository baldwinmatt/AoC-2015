#include "aoc/helpers.h"

#include <array>
#include <openssl/md5.h>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  size_t part1 = SIZE_MAX;
  size_t part2 = SIZE_MAX;
  size_t counter = 0;
  std::array<u_int8_t, 16> coin{0};

  while (part1 == SIZE_MAX || part2 == SIZE_MAX) {
    counter++;
    
    std::string key{f.data(), f.size()};
    key.append(std::to_string(counter));

    MD5(reinterpret_cast<const unsigned char *>(key.data()), key.size(), coin.data());

    if (coin[0] == 0 &&
      coin[1] == 0 &&
      coin[2] < 0x10)
    {
      part1 = std::min(counter, part1);

      if (coin[2] == 0) {
        part2 = counter;
      }
    }
  }

  aoc::print_results(part1, part2);  

  return 0;
}

