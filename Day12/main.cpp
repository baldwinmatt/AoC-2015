#include "aoc/helpers.h"

#include <stack>
#include <memory>
#include <map>
#include <vector>

namespace {
  using MappedFileSource = aoc::MappedFileSource<char>;

  enum class State {
    START = 0,
    OBJECT_START = 1, 
    OBJECT_KEY = 2,
    OBJECT_VALUE = 3,
    OBJECT_VALUE_STRING = 4,
    ARRAY_START = 5,
    ARRAY_ELEMENT = 6,
    END
  };

  enum class JsonType {
    NONE = -1,
    OBJECT = 0,
    ARRAY = 1,
    STRING = 2,
    NUMBER = 3,
  };

  struct JsonObject {

    JsonType type{JsonType::NONE};

    std::string_view value{};
    int64_t number{0};
    std::vector<JsonObject> array;
    std::map<std::string_view, JsonObject> object;

    bool is_red{false};

    JsonObject() = default;

    const std::string_view& get_value() const {
      if (type != JsonType::STRING) {
        throw std::runtime_error("Not a string");
      }
      return value;
    }

    int64_t get_number() const {
      if (type != JsonType::NUMBER) {
        throw std::runtime_error("Not a number");
      }
      return number;
    }

    const std::vector<JsonObject>& get_array() const {
      if (type != JsonType::ARRAY) {
        throw std::runtime_error("Not an array");
      }
      return array;
    }

    const std::map<std::string_view, JsonObject>& get_object() const {
      if (type != JsonType::OBJECT) {
        throw std::runtime_error("Not an object");
      }
      return object;
    }

    std::ostream& print(std::ostream& os) const {
      if (is_red) {
        return os;
      }

      switch (type) {
        case JsonType::NONE:
          os << "NONE";
          break;
        case JsonType::OBJECT:
          os << "{";
          for (auto& kv : object) {
            os << kv.first << ":";
            kv.second.print(os);
            os << ",";
          }
          os << "}";
          break;
        case JsonType::ARRAY:
          os << "[";
          for (auto& e : array) {
            e.print(os);
            os << ",";
          }
          os << "]";
          break;
        case JsonType::STRING:
          os << value;
          break;
        case JsonType::NUMBER:
          os << number;
          break;
      }
      return os;
    }

    int64_t sum(bool no_red = false) const {
      if (no_red && is_red) {
        return 0;
      }
      switch (type) {
        case JsonType::NONE:
        case JsonType::STRING:
          return 0;
        case JsonType::NUMBER:
          return number;
        case JsonType::ARRAY: {
          int64_t sum = 0;
          for (auto& e : array) {
            if (!no_red || !e.is_red) {
              sum += e.sum(no_red);
            }
          }
          return sum;
        }
        case JsonType::OBJECT: {
          int64_t sum = 0;
          for (auto& e : object) {
            if (!no_red || !e.second.is_red) {
              sum += e.second.sum(no_red);
            }
          }
          return sum;
        }
      }
      return 0;
    }
  };

  std::ostream& operator<<(std::ostream& os, const JsonObject& v) {
    return v.print(os);
  }

  JsonObject parseJsonObject(std::string_view& f);
  JsonObject parseJsonArray(std::string_view& f);

  std::pair<std::string_view, JsonObject> parseJsonKeyValue(std::string_view& f) {
    assert(!f.empty());

    std::string_view key;
    JsonObject value;

    if (f.front() == '"') {
      f.remove_prefix(1);
      // string, look for end
      size_t len = 0;
      while (f[len] != '"') {
        len++;
      }
      key = f.substr(0, len);
      f.remove_prefix(len + 1);
    } else {
      size_t len = 0;
      while (f[len] != ':') {
        len++;
      }
      key = f.substr(0, len);
      f.remove_prefix(len);
    }

    DEBUG_PRINT("Key: " << key);

    assert(f.front() == ':');
    f.remove_prefix(1);
    if (f.front() == '{') {
      value = parseJsonObject(f);
    } else if (f.front() == '[') {
      value = parseJsonArray(f);
    } else {
      size_t len = 0;
      while (f[len] != '}' && f[len] != ',') {
        len++;
      }
      value.type = JsonType::STRING;
      value.value = f.substr(0, len);
      f.remove_prefix(len);
      if (value.value.front() == '"') {
        value.value.remove_prefix(1);
      }
      if (value.value.back() == '"') {
        value.value.remove_suffix(1);
      }
      if (aoc::is_numeric(value.value)) {
        value.type = JsonType::NUMBER;
        value.number = aoc::stoi(value.value);
      }
    }

    if (f.front() == ',') {
      f.remove_prefix(1);
    }
    DEBUG_PRINT("Value: " << value);

    return {key, value};
  }

  JsonObject parseJsonObject(std::string_view& f) {
    assert(f.front() == '{');
    f.remove_prefix(1);
    JsonObject j;
    j.type = JsonType::OBJECT;
    while (f.front() != '}') {
      const auto [key, value] = parseJsonKeyValue(f);
      j.object[key] = value;

      if (value.type == JsonType::STRING && value.value == "red") {
        j.is_red = true;
      }
    }
    f.remove_prefix(1);
    if (f.front() == ',') {
      f.remove_prefix(1);
    }
    return j;
  }

  JsonObject parseJsonPrimitive(std::string_view& f) {
    JsonObject j;
    if (f.front() == '"') {
      f.remove_prefix(1);
      // string, look for end
      size_t len = 0;
      while (f[len] != '"') {
        len++;
      }
      j.type = JsonType::STRING;
      j.value = f.substr(0, len);
      f.remove_prefix(len + 1);
    } else {
      size_t len = 0;
      while (f[len] != ',' && f[len] != ']') {
        len++;
      }
      j.type = JsonType::STRING;
      j.value = f.substr(0, len);
      f.remove_prefix(len);
      if (aoc::is_numeric(j.value)) {
        j.type = JsonType::NUMBER;
        j.number = aoc::stoi(j.value);
      }
    }
    if (f.front() == ',') {
      f.remove_prefix(1);
    }

    return j;
  }

  JsonObject parseJsonArray(std::string_view& f) {
    assert(f.front() == '[');
    f.remove_prefix(1);
    JsonObject j;
    j.type = JsonType::ARRAY;
    while (f.front() != ']') {
      if (f.front() == '{') {
        j.array.push_back(parseJsonObject(f));
        DEBUG_PRINT("Pushed object: " << j.array.back());
      } else if (f.front() == '[') {
        j.array.push_back(parseJsonArray(f));
        DEBUG_PRINT("Pushed object: " << j.array.back());
      } else {
        j.array.push_back(parseJsonPrimitive(f));
        DEBUG_PRINT("Pushed object: " << j.array.back());
      }
    }
    f.remove_prefix(1);
    if (f.front() == ',') {
      f.remove_prefix(1);
    }
    return j;
  }

  JsonObject parseJson(std::string_view f) {
    JsonObject obj;

    switch (f.front()) {
      case '{':
        obj = parseJsonObject(f);
        break;
      case '[':
        obj = parseJsonArray(f);
        break;
    }

    return obj;
  }

}

int main(int argc, char** argv) {
  aoc::AutoTimer t;

  std::unique_ptr<MappedFileSource>m(new MappedFileSource(argc, argv));
  std::string_view f(m->data(), m->size());

  int64_t part1 = 0;
  int64_t part2 = 0;

  const auto json = parseJson(f);
  DEBUG_PRINT("Parsed json: " << json);

  {
    aoc::AutoTimer t1{"Part 1"};

    part1 = json.sum();
  }

  {
    aoc::AutoTimer t2{"Part 2"};

    part2 = json.sum(true);
  }

  aoc::print_results(part1, part2);

  return 0;
}

