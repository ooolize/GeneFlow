/*
 * @Description:
 * @Author: lize
 * @Date: 2024-10-23
 * @LastEditors: lize
 */

#pragma once
#include <cstdint>

namespace lz {
namespace GeneFlow {
enum class Result : std::uint8_t {
  SUCCESS = 0,
  FORMAT_ERROR = 1,
  NOT_NUMBER = 2,
  NOT_BOOL = 3,
  NOT_STRING = 4,
  FILE_OPEN_FAILED = 5,
  FAILED,
};

// template <typename, std::size_t>
// struct Field;

#define EXPAND(x) x  // NOLINT  for msvc

#define CYLINDER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N  // NOLINT
#define GET_ARGS_COUNT(...) CYLINDER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define REPEAT_1(f, i, arg) f(i, arg)  // NOLINT
#define REPEAT_2(f, i, arg, ...) f(i, arg) REPEAT_1(f, i + 1, __VA_ARGS__)
#define REPEAT_3(f, i, arg, ...) f(i, arg) REPEAT_2(f, i + 1, __VA_ARGS__)
#define REPEAT_4(f, i, arg, ...) f(i, arg) REPEAT_3(f, i + 1, __VA_ARGS__)
#define REPEAT_5(f, i, arg, ...) f(i, arg) REPEAT_4(f, i + 1, __VA_ARGS__)
#define REPEAT_6(f, i, arg, ...) f(i, arg) REPEAT_5(f, i + 1, __VA_ARGS__)
#define REPEAT_7(f, i, arg, ...) f(i, arg) REPEAT_6(f, i + 1, __VA_ARGS__)
#define REPEAT_8(f, i, arg, ...) f(i, arg) REPEAT_7(f, i + 1, __VA_ARGS__)
#define REPEAT_9(f, i, arg, ...) f(i, arg) REPEAT_8(f, i + 1, __VA_ARGS__)

#define CONCATE(a, b) a##b         // NOLINT
#define PASTE(a, b) CONCATE(a, b)  // NOLINT

#define PARE(...) __VA_ARGS__  // NOLINT
#define PAIR(x) PARE x         // NOLINT
// PARE(double) x;
// PAIR((double)x);

#define EAT(...)
#define STRIP(x) EXPAND(EAT x)  // NOLINT

// 为什么要两步
#define STR(x) #x         // NOLINT
#define STRING(x) STR(x)  // NOLINT

// (double) x
#define EACH_FIELD(i, arg)                \
  PAIR(arg);                              \
  template <typename T>                   \
  struct Field<T, i> {                    \
    T& obj;                               \
    decltype(auto) value() {              \
      return obj.STRIP(arg);              \
    }                                     \
    static constexpr const char* name() { \
      return STRING(STRIP(arg));          \
    }                                     \
  };

#define GENE_DEFINE(st, ...)                                                 \
  struct st {                                                                \
    static constexpr std::size_t fields_count = GET_ARGS_COUNT(__VA_ARGS__); \
    template <typename T, std::size_t N>                                     \
    struct Field;                                                            \
    PASTE(REPEAT_, GET_ARGS_COUNT(__VA_ARGS__))                              \
    (EACH_FIELD, 0, __VA_ARGS__)                                             \
  };

}  // namespace GeneFlow
}  // namespace lz