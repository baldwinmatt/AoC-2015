#include "aoc/helpers.h"

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  struct Ingredient {
    int64_t capacity{0};
    int64_t durability{0};
    int64_t flavor{0};
    int64_t texture{0};
    int64_t calories{0};
  };

  using Ingredients = std::vector<Ingredient>;

  const auto LoadIngredients = [](std::string_view f) {
    Ingredients ingredients;

    std::string_view line;
    while (aoc::getline(f, line)) {
      int idx = 0;
      ingredients.push_back({});
      auto& i = ingredients.back();
      std::string_view token;
      while (aoc::getline(line, token, " ,")) {
        if (idx == 2) {
          i.capacity = aoc::stoi(token);
        } else if (idx == 4) {
          i.durability = aoc::stoi(token);
        } else if (idx == 6) {
          i.flavor = aoc::stoi(token);
        } else if (idx == 8) {
          i.texture = aoc::stoi(token);
        } else if (idx == 10) {
          i.calories = aoc::stoi(token);
        }
        idx++;
      }
    }

    return ingredients;
  };

  const auto Score = [](const Ingredients& ingredients, int64_t calorie_target = -1) {
    constexpr int64_t max = 100;
    int64_t max_score = 0;
    for (int64_t a = 0; a <= max; ++a) {
      for (int64_t b = 0; b <= max - a; ++b) {
        for (int64_t c = 0; c <= max - a - b; ++c) {
          const int64_t d = max - a - b - c;

          const int64_t capacity = ingredients[0].capacity * a + ingredients[1].capacity * b + ingredients[2].capacity * c + ingredients[3].capacity * d;
          const int64_t durability = ingredients[0].durability * a + ingredients[1].durability * b + ingredients[2].durability * c + ingredients[3].durability * d;
          const int64_t flavor = ingredients[0].flavor * a + ingredients[1].flavor * b + ingredients[2].flavor * c + ingredients[3].flavor * d;
          const int64_t texture = ingredients[0].texture * a + ingredients[1].texture * b + ingredients[2].texture * c + ingredients[3].texture * d;
          const int64_t calories = ingredients[0].calories * a + ingredients[1].calories * b + ingredients[2].calories * c + ingredients[3].calories * d;
          if (capacity < 0 || durability < 0 || flavor < 0 || texture < 0) {
            continue;
          }
          const int64_t score = capacity * durability * flavor * texture;

          
          if (score > max_score && (calorie_target == calories || calorie_target == -1)) {
            DEBUG_PRINT("score: " << score << " calories: " << calories);
            max_score = score;
          }
        }
      }
    }

    return max_score;
  };
}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  const auto ingredients = LoadIngredients(f);

  const auto part1 = Score(ingredients, -1);
  const auto part2 = Score(ingredients, 500);

  aoc::print_results(part1, part2);

  return 0;
}

