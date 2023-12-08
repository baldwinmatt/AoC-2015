#include "aoc/helpers.h"

#include <stack>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  int64_t part1 = 0;
  int64_t part2 = 0;
  int64_t red_parts = 0;
  size_t start = SIZE_MAX;
  size_t len = 0;

  int64_t sum = 0;

  std::stack<int64_t>doc;

  const bool is_obj = !f.empty() && f[0] == '{';
  int red_depth = 0;

  for (size_t i = 0; i < f.size(); i++) {
    const auto c = f[i];

    if (c == '{') {
      if (red_depth) { red_depth++; }
      doc.push(sum);
      sum = 0;
      continue;
    } else if (c == '}') {
      const auto val = doc.top();
      doc.pop();
      part1 += val;
      const bool is_red = !!red_depth;
      red_parts += (val * is_red);
      DEBUG_PRINT("Val: " << val << ", is_red: " << is_red);
      if (red_depth) { red_depth--; }
      if (is_red && !red_depth) { DEBUG_PRINT("Red Done"); }
    } else if (!red_depth && c == ':') {
      const auto r = f.substr(i + 1);
      if (aoc::starts_with(r, std::string_view("\"red\""))) {
        DEBUG_PRINT("Red object");
        red_depth = 1;
      }
    } else if (c == '-' || aoc::is_numeric(c)) {
      if (start == SIZE_MAX) {
        start =  i;
        len = 0;
      }
      len++;
    } else if (len) {
      const auto part = f.substr(start, len);
      if (aoc::is_numeric(part)) {
        const auto val = aoc::stoi(part);
        sum += val;
      }
      len = 0;
      start = SIZE_MAX;
    }
  }

  // Account for the fact the object is enclosed in an array
  if (!is_obj && sum) {
    part1 += sum;
    sum = 0;
  }

  part2 = part1 - red_parts;

  assert(doc.empty());
  assert(sum == 0);

  aoc::print_results(part1, part2);

  return 0;
}

