#include "aoc/helpers.h"

#include <set>
#include <map>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  std::set<std::string_view> people;
  std::map<std::pair<std::string_view, std::string_view>, int> happiness;

  std::string_view line;
  while (aoc::getline(f, line)) {
    std::string_view name;
    std::string_view neighbor;
    int change = 1;
    std::string_view part;
    size_t idx = 0;
    while (aoc::getline(line, part, ". ")) {
      if (idx == 0) {
        name = part;
      } else if (idx == 2) {
        if (part == "gain") {
          change = 1;
        } else {
          change = -1;
        }
      } else if (idx == 3) {
        change *= aoc::stoi(part);
      } else if (idx == 10) {
        neighbor = part;
      }
      idx++;
    }
    people.emplace(name);
    people.emplace(neighbor);
    happiness[{name, neighbor}] = change;
  }

  std::vector<std::string_view> people_vec(people.begin(), people.end());

  const auto get_optimal_happiness = [&]() {
    int optimal_happiness = INT_MIN;
    while (std::next_permutation(people_vec.begin(), people_vec.end())) {
      int score = 0;
      for (size_t i = 0; i < people_vec.size(); i++) {
        if (i == 0) {
          score += happiness[{people_vec[i], people_vec.back()}];
          score += happiness[{people_vec[i], people_vec[i + 1]}];
        } else if (i == people_vec.size() - 1) {
          score += happiness[{people_vec[i], people_vec.front()}];
          score += happiness[{people_vec[i], people_vec[i - 1]}];
        } else {
          score += happiness[{people_vec[i], people_vec[i - 1]}];
          score += happiness[{people_vec[i], people_vec[i + 1]}];
        }
      }
      optimal_happiness = std::max(optimal_happiness, score);
    }
    return optimal_happiness;
  };

  const auto part1 = get_optimal_happiness();

  // add ourself to the matrix, with a happiness of 0
  people_vec.emplace_back("me");
  for (const auto& p : people) {  
    happiness[{p, "me"}] = 0;
    happiness[{"me", p}] = 0;
  }

  const auto part2 = get_optimal_happiness();

  aoc::print_results(part1, part2);
  return 0;
}

