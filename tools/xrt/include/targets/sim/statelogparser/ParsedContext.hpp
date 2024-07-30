#pragma once

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>

struct ParsedContext {
  struct Value {
    enum class Type {
      INT,
      INT_VECTOR
    };

    Type type;
    struct {
      uint32_t i;
      std::vector<uint32_t> iv;
    } v;

    inline Value() = default;

    inline Value(uint32_t _i) {
      type = Type::INT;
      v.i = _i;
    }

    inline Value(std::vector<uint32_t> _iv) {
      type = Type::INT_VECTOR;
      v.iv = _iv;
    }
  };
  
  std::unordered_map<std::string, Value> result;
  std::vector<uint32_t> curvector;
  Value curvalue;

  std::function<void(ParsedContext&)> callback;
};

