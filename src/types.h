// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <assert.h>
#include <node_api.h>

#include <cstring>
#include <optional>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "src/template_util.h"

namespace ki {

template<typename T, typename Enable = void>
struct TypeBridge {};

template<typename T, typename Enable = void>
struct Type {};

template<>
struct Type<napi_value> {
  static constexpr const char* name = "Value";
  static inline napi_status ToNode(napi_env env,
                                   napi_value value,
                                   napi_value* result) {
    *result = value;
    return napi_ok;
  }
  static inline std::optional<napi_value> FromNode(napi_env env,
                                                   napi_value value) {
    return value;
  }
};

template<>
struct Type<std::nullptr_t> {
  static constexpr const char* name = "Null";
  static inline napi_status ToNode(napi_env env,
                                   std::nullptr_t value,
                                   napi_value* result) {
    return napi_get_null(env, result);
  }
};

template<>
struct Type<void*> {
  static constexpr const char* name = "Buffer";
  static inline napi_status ToNode(napi_env env,
                                   void* value,
                                   napi_value* result) {
    void* data;
    napi_status s = napi_create_buffer(env, sizeof(void*), &data, result);
    if (s != napi_ok)
      return s;
    std::memcpy(data, &value, sizeof(void*));
    return napi_ok;
  }
};

template<>
struct Type<uint8_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   uint8_t value,
                                   napi_value* result) {
    return napi_create_uint32(env, value, result);
  }
  static inline std::optional<uint8_t> FromNode(napi_env env,
                                                napi_value value) {
    uint32_t val;
    if (napi_get_value_uint32(env, value, &val) != napi_ok)
      return std::nullopt;
    return static_cast<uint8_t>(val);
  }
};

template<>
struct Type<uint16_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   uint16_t value,
                                   napi_value* result) {
    return napi_create_uint32(env, value, result);
  }
  static inline std::optional<uint16_t> FromNode(napi_env env,
                                                 napi_value value) {
    uint32_t val;
    if (napi_get_value_uint32(env, value, &val) != napi_ok)
      return std::nullopt;
    return static_cast<uint16_t>(val);
  }
};

template<>
struct Type<int8_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   int8_t value,
                                   napi_value* result) {
    return napi_create_int32(env, value, result);
  }
  static inline std::optional<int8_t> FromNode(napi_env env, napi_value value) {
    int32_t val;
    if (napi_get_value_int32(env, value, &val) != napi_ok)
      return std::nullopt;
    return static_cast<int8_t>(val);
  }
};

template<>
struct Type<int16_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   int16_t value,
                                   napi_value* result) {
    return napi_create_int32(env, value, result);
  }
  static inline std::optional<int16_t> FromNode(napi_env env,
                                                napi_value value) {
    int32_t val;
    if (napi_get_value_int32(env, value, &val) != napi_ok)
      return std::nullopt;
    return static_cast<int16_t>(val);
  }
};

template<>
struct Type<int32_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   int32_t value,
                                   napi_value* result) {
    return napi_create_int32(env, value, result);
  }
  static inline std::optional<int32_t> FromNode(napi_env env,
                                                napi_value value) {
    int32_t result;
    if (napi_get_value_int32(env, value, &result) == napi_ok)
      return result;
    return std::nullopt;
  }
};

template<>
struct Type<uint32_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   uint32_t value,
                                   napi_value* result) {
    return napi_create_uint32(env, value, result);
  }
  static inline std::optional<uint32_t> FromNode(napi_env env,
                                                 napi_value value) {
    uint32_t result;
    if (napi_get_value_uint32(env, value, &result) == napi_ok)
      return result;
    return std::nullopt;
  }
};

template<>
struct Type<int64_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   int64_t value,
                                   napi_value* result) {
    return napi_create_int64(env, value, result);
  }
  static inline std::optional<int64_t> FromNode(napi_env env,
                                                napi_value value) {
    int64_t result;
    if (napi_get_value_int64(env, value, &result) == napi_ok)
      return result;
    return std::nullopt;
  }
};

template<>
struct Type<uint64_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   uint64_t value,
                                   napi_value* result) {
    return napi_create_bigint_uint64(env, value, result);
  }
  static inline std::optional<uint64_t> FromNode(napi_env env,
                                                 napi_value value) {
    uint64_t result;
    bool lossless;
    if (napi_get_value_bigint_uint64(env, value, &result, &lossless) == napi_ok)
      return result;
    return std::nullopt;
  }
};

#if defined(__APPLE__)
template<>
struct Type<size_t> {
  static constexpr const char* name = "Integer";
  static inline napi_status ToNode(napi_env env,
                                   size_t value,
                                   napi_value* result) {
    return napi_create_int64(env, value, result);
  }
};
#endif

template<>
struct Type<float> {
  static constexpr const char* name = "Number";
  static inline napi_status ToNode(napi_env env,
                                   float value,
                                   napi_value* result) {
    return napi_create_double(env, value, result);
  }
  static inline std::optional<float> FromNode(napi_env env,
                                              napi_value value) {
    double intermediate;
    if (napi_get_value_double(env, value, &intermediate) == napi_ok)
      return static_cast<float>(intermediate);
    return std::nullopt;
  }
};

template<>
struct Type<double> {
  static constexpr const char* name = "Number";
  static inline napi_status ToNode(napi_env env,
                                   double value,
                                   napi_value* result) {
    return napi_create_double(env, value, result);
  }
  static inline std::optional<double> FromNode(napi_env env,
                                               napi_value value) {
    double result;
    if (napi_get_value_double(env, value, &result) == napi_ok)
      return result;
    return std::nullopt;
  }
};

template<>
struct Type<bool> {
  static constexpr const char* name = "Boolean";
  static inline napi_status ToNode(napi_env env,
                                   bool value,
                                   napi_value* result) {
    return napi_get_boolean(env, value, result);
  }
  static inline std::optional<bool> FromNode(napi_env env,
                                             napi_value value) {
    bool result;
    if (napi_get_value_bool(env, value, &result) == napi_ok)
      return result;
    return std::nullopt;
  }
};

template<>
struct Type<std::string> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const std::string& value,
                                   napi_value* result) {
    return napi_create_string_utf8(env, value.c_str(), value.length(), result);
  }
  static std::optional<std::string> FromNode(napi_env env, napi_value value) {
    size_t length;
    if (napi_get_value_string_utf8(env, value, nullptr, 0, &length) != napi_ok)
      return std::nullopt;
    std::string out;
    if (length > 0) {
      out.reserve(length + 1);
      out.resize(length);
      if (napi_get_value_string_utf8(env, value, &out.front(), out.capacity(),
                                     nullptr) != napi_ok) {
        return std::nullopt;
      }
    }
    return out;
  }
};

template<>
struct Type<std::u16string> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const std::u16string& value,
                                   napi_value* result) {
    return napi_create_string_utf16(env, value.c_str(), value.length(), result);
  }
  static std::optional<std::u16string> FromNode(napi_env env,
                                                napi_value value) {
    size_t length;
    if (napi_get_value_string_utf16(env, value, nullptr, 0, &length) != napi_ok)
      return std::nullopt;
    std::u16string out;
    if (length > 0) {
      out.reserve(length + 1);
      out.resize(length);
      if (napi_get_value_string_utf16(env, value, &out.front(), out.capacity(),
                                      nullptr) != napi_ok) {
        return std::nullopt;
      }
    }
    return out;
  }
};

template<>
struct Type<const char*> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char* value,
                                   napi_value* result) {
    return napi_create_string_utf8(env, value, NAPI_AUTO_LENGTH, result);
  }
};

template<>
struct Type<char*> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char* value,
                                   napi_value* result) {
    return napi_create_string_utf8(env, value, NAPI_AUTO_LENGTH, result);
  }
};

template<size_t n>
struct Type<char[n]> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char* value,
                                   napi_value* result) {
    return napi_create_string_utf8(env, value, n - 1, result);
  }
};

template<>
struct Type<const char16_t*> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char16_t* value,
                                   napi_value* result) {
    return napi_create_string_utf16(env, value, NAPI_AUTO_LENGTH, result);
  }
};

template<>
struct Type<char16_t*> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char16_t* value,
                                   napi_value* result) {
    return napi_create_string_utf16(env, value, NAPI_AUTO_LENGTH, result);
  }
};

template<size_t n>
struct Type<char16_t[n]> {
  static constexpr const char* name = "String";
  static inline napi_status ToNode(napi_env env,
                                   const char16_t* value,
                                   napi_value* result) {
    return napi_create_string_utf16(env, value, n - 1, result);
  }
};

template<>
struct Type<napi_env> {
  static constexpr const char* name = "Environment";
};

// Optimized helper for creating symbols.
template<size_t n>
struct SymbolHolder {
  const char* str;
};

template<size_t n>
inline SymbolHolder<n> Symbol(const char (&value)[n]) {
  SymbolHolder<n> holder;
  holder.str = value;
  return holder;
}

template<size_t n>
struct Type<SymbolHolder<n>> {
  static constexpr const char* name = "Symbol";
  static inline napi_status ToNode(napi_env env,
                                   SymbolHolder<n> value,
                                   napi_value* result) {
    napi_value symbol;
    napi_status s = Type<char[n]>::ToNode(env, value.str, &symbol);
    if (s != napi_ok)
      return s;
    return napi_create_symbol(env, symbol, result);
  }
};

// Some builtin types.
inline napi_value Global(napi_env env) {
  napi_value result;
  napi_status s = napi_get_global(env, &result);
  assert(s == napi_ok);
  return result;
}

inline napi_value Undefined(napi_env env) {
  napi_value result;
  napi_status s = napi_get_undefined(env, &result);
  assert(s == napi_ok);
  return result;
}

inline napi_value Null(napi_env env) {
  napi_value result;
  napi_status s = napi_get_null(env, &result);
  assert(s == napi_ok);
  return result;
}

// Object helpers.
inline napi_value CreateObject(napi_env env) {
  napi_value value = nullptr;
  napi_status s = napi_create_object(env, &value);
  assert(s == napi_ok);
  return value;
}

inline bool IsArray(napi_env env, napi_value value) {
  bool result = false;
  napi_is_array(env, value, &result);
  return result;
}

inline bool IsType(napi_env env, napi_value value, napi_valuetype target) {
  napi_valuetype type;
  napi_status s = napi_typeof(env, value, &type);
  return s == napi_ok && type == target;
}

// Function helpers.
template<typename T>
inline napi_status ConvertToNode(napi_env env, const T& value,
                                 napi_value* result) {
  return Type<T>::ToNode(env, value, result);
}

template<typename T>
inline napi_status ConvertToNode(napi_env env, T&& value,
                                 napi_value* result) {
  return Type<std::decay_t<T>>::ToNode(env, std::forward<T>(value), result);
}

// Optimized version for string iterals.
template<size_t n>
inline napi_status ConvertToNode(napi_env env, const char (&value)[n],
                                 napi_value* result) {
  return Type<char[n]>::ToNode(env, value, result);
}

template<size_t n>
inline napi_status ConvertToNode(napi_env env, const char16_t (&value)[n],
                                 napi_value* result) {
  return Type<char16_t[n]>::ToNode(env, value, result);
}

// Convert from In to Out and ignore error.
template<typename Out, typename In>
inline napi_value ConvertIgnoringStatus(napi_env env, In&& value) {
  napi_value result = nullptr;
  napi_status s = Type<Out>::ToNode(env, std::forward<In>(value), &result);
  // Return undefined on error.
  if (s != napi_ok)
    return Undefined(env);
  return result;
}

template<typename T>
inline napi_value ToNode(napi_env env, const T& value) {
  return ConvertIgnoringStatus<T>(env, value);
}

template<typename T>
inline napi_value ToNode(napi_env env, T&& value) {
  return ConvertIgnoringStatus<std::decay_t<T>>(env, std::forward<T>(value));
}

template<typename T>
inline napi_value ToNode(napi_env env, std::optional<T>&& value) {
  if (!value)
    return Undefined(env);
  return ToNode(env, *value);
}

template<size_t n>
inline napi_value ToNode(napi_env env, const char (&value)[n]) {
  return ConvertIgnoringStatus<char[n]>(env, value);
}

template<size_t n>
inline napi_value ToNode(napi_env env, const char16_t (&value)[n]) {
  return ConvertIgnoringStatus<char16_t[n]>(env, value);
}

// Keep compatibility with the old API.
template<typename T>
inline bool FromNode(napi_env env, napi_value value, T* out) {
  std::optional<T> result = Type<T>::FromNode(env, value);
  if (!result)
    return false;
  *out = std::move(*result);
  return true;
}

// The modern version of FromNode.
template<typename T>
inline std::optional<T> FromNode(napi_env env, napi_value value) {
  return Type<T>::FromNode(env, value);
}

// Define converters for some frequently used types.

template<typename T>
struct Type<std::optional<T>> {
  static constexpr const char* name = Type<T>::name;
  // There is no converter defined, so we can implement optional argument by
  // using std::optional<T> as parameter.
};

template<typename T>
struct Type<std::vector<T>> {
  static constexpr const char* name = "Array";
  static napi_status ToNode(napi_env env,
                            const std::vector<T>& vec,
                            napi_value* result) {
    napi_status s = napi_create_array_with_length(env, vec.size(), result);
    if (s != napi_ok) return s;
    for (size_t i = 0; i < vec.size(); ++i) {
      napi_value el;
      s = ConvertToNode(env, vec[i], &el);
      if (s != napi_ok) return s;
      s = napi_set_element(env, *result, i, el);
      if (s != napi_ok) return s;
    }
    return napi_ok;
  }
  static std::optional<std::vector<T>> FromNode(napi_env env,
                                                napi_value value) {
    if (!IsArray(env, value))
      return std::nullopt;
    uint32_t length;
    if (napi_get_array_length(env, value, &length) != napi_ok)
      return std::nullopt;
    std::vector<T> result;
    result.reserve(length);
    for (uint32_t i = 0; i < length; ++i) {
      napi_value el = nullptr;
      if (napi_get_element(env, value, i, &el) != napi_ok)
        return std::nullopt;
      std::optional<T> out = Type<T>::FromNode(env, el);
      if (!out)
        return std::nullopt;
      result.push_back(std::move(*out));
    }
    return result;
  }
};

template<typename T>
struct Type<std::set<T>> {
  static constexpr const char* name = "Array";
  static napi_status ToNode(napi_env env,
                            const std::set<T>& vec,
                            napi_value* result) {
    napi_status s = napi_create_array_with_length(env, vec.size(), result);
    if (s != napi_ok) return s;
    int i = 0;
    for (const auto& element : vec) {
      napi_value el;
      s = ConvertToNode(env, element, &el);
      if (s != napi_ok) return s;
      s = napi_set_element(env, *result, i++, el);
      if (s != napi_ok) return s;
    }
    return napi_ok;
  }
  static std::optional<std::set<T>> FromNode(napi_env env,
                                             napi_value value) {
    if (!IsArray(env, value))
      return std::nullopt;
    uint32_t length;
    if (napi_get_array_length(env, value, &length) != napi_ok)
      return std::nullopt;
    std::set<T> result;
    result.reserve(length);
    for (uint32_t i = 0; i < length; ++i) {
      napi_value el;
      if (napi_get_element(env, value, i, &el) != napi_ok)
        return std::nullopt;
      std::optional<T> element = Type<T>::FromNode(env, el);
      if (!element)
        return std::nullopt;
      result.insert(std::move(*element));
    }
    return result;
  }
};

template<typename K, typename V>
struct Type<std::map<K, V>> {
  static constexpr const char* name = "Object";
  static napi_status ToNode(napi_env env,
                            const std::map<K, V>& dict,
                            napi_value* result) {
    napi_status s = napi_create_object(env, result);
    if (s == napi_ok) {
      for (const auto& it : dict) {
        napi_value key, value;
        s = ConvertToNode(env, it.first, &key);
        if (s != napi_ok) break;
        s = ConvertToNode(env, it.second, &value);
        if (s != napi_ok) break;
        s = napi_set_property(env, *result, key, value);
        if (s != napi_ok) break;
      }
    }
    return s;
  }
  static std::optional<std::map<K, V>> FromNode(napi_env env,
                                                napi_value object) {
    napi_value property_names;
    if (napi_get_property_names(env, object, &property_names) != napi_ok)
      return std::nullopt;
    std::optional<std::vector<napi_value>> keys =
        Type<std::vector<napi_value>>::FromNode(env, property_names);
    if (!keys)
      return std::nullopt;
    std::map<K, V> result;
    result.reserve(keys->size());
    for (napi_value key : *keys) {
      std::optional<K> k = Type<K>::FromNode(env, key);
      if (!k)
        return std::nullopt;
      napi_value value;
      if (napi_get_property(env, object, key, &value) != napi_ok)
        return std::nullopt;
      std::optional<V> v = Type<V>::FromNode(env, value);
      if (!v)
        return std::nullopt;
      result.emplace(std::move(*k), std::move(*v));
    }
    return result;
  }
};

template<typename... ArgTypes>
struct Type<std::tuple<ArgTypes...>> {
  using V = std::tuple<ArgTypes...>;

  static constexpr const char* name = "Tuple";
  static napi_status ToNode(napi_env env, const V& tup, napi_value* result) {
    constexpr size_t length = sizeof...(ArgTypes);
    napi_value arr;
    napi_status s = napi_create_array_with_length(env, length, &arr);
    if (s != napi_ok)
      return s;
    s = SetVar(env, arr, tup);
    if (s != napi_ok)
      return s;
    *result = arr;
    return napi_ok;
  }
  static std::optional<V> FromNode(napi_env env, napi_value value) {
    if (!IsArray(env, value))
      return std::nullopt;
    uint32_t length;
    if (napi_get_array_length(env, value, &length) != napi_ok)
      return std::nullopt;
    if (length != sizeof...(ArgTypes))
      return std::nullopt;
    V result;
    if (!GetVar(env, value, &result))
      return std::nullopt;
    return result;
  }

 private:
  template<std::size_t I = 0>
  static napi_status SetVar(napi_env env, napi_value arr, const V& tup) {
    napi_value value;
    napi_status s = ConvertToNode(env, std::get<I>(tup), &value);
    if (s != napi_ok)
      return s;
    s = napi_set_element(env, arr, I, value);
    if (s != napi_ok)
      return s;
    if constexpr (I + 1 >= sizeof...(ArgTypes))
      return napi_ok;
    else
      return SetVar<I + 1>(env, arr, tup);
  }

  template<std::size_t I = 0>
  static bool GetVar(napi_env env, napi_value arr, V* tup) {
    napi_value value;
    if (napi_get_element(env, arr, I, &value) != napi_ok)
      return false;
    using T = std::tuple_element_t<I, V>;
    std::optional<T> result = Type<T>::FromNode(env, value);
    if (!result)
      return false;
    std::get<I>(*tup) = std::move(*result);
    if constexpr (I + 1 >= sizeof...(ArgTypes))
      return true;
    else
      return GetVar<I + 1>(env, arr, tup);
  }
};

// Converter for converting std::variant between js and C++.
template<typename... ArgTypes>
struct Type<std::variant<ArgTypes...>> {
  using V = std::variant<ArgTypes...>;

  static constexpr const char* name = "Variant";
  static napi_status ToNode(napi_env env, const V& var, napi_value* result) {
    napi_status s = napi_generic_failure;
    std::visit([env, result, &s](const auto& arg) {
      s = ConvertToNode(env, arg, result);
    }, var);
    return s;
  }
  static inline std::optional<V> FromNode(napi_env env, napi_value value) {
    return GetVar(env, value);
  }

 private:
  template<std::size_t I = 0>
  static std::optional<V> GetVar(napi_env env, napi_value value) {
    if constexpr (I < std::variant_size_v<V>) {
      using T = std::variant_alternative_t<I, V>;
      std::optional<T> result = Type<T>::FromNode(env, value);
      return result ? std::move(*result) : GetVar<I + 1>(env, value);
    }
    return std::nullopt;
  }
};

// The monostate is a special type for std::variant, when a variant includes it,
// we consider the type accepting undefined/null.
template<>
struct Type<std::monostate> {
  static constexpr const char* name = "";  // no name for monostate
  static napi_status ToNode(napi_env env, std::monostate, napi_value* result) {
    return napi_get_undefined(env, result);
  }
  static inline std::optional<std::monostate> FromNode(napi_env env,
                                                       napi_value value) {
    napi_valuetype type;
    if (napi_typeof(env, value, &type) != napi_ok)
      return std::nullopt;
    if (type == napi_boolean || type == napi_null)
      return std::monostate();
    return std::nullopt;
  }
};

}  // namespace ki

#endif  // SRC_TYPES_H_
