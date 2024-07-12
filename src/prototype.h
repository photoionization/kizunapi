// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#ifndef SRC_PROTOTYPE_H_
#define SRC_PROTOTYPE_H_

#include "src/prototype_internal.h"

namespace ki {

// Push a constructor to JS.
template<typename T>
struct Class {};

template<typename T>
struct Type<Class<T>> {
  static inline napi_status ToNode(napi_env env,
                                   Class<T>,
                                   napi_value* result) {
    napi_value constructor = internal::InheritanceChain<T>::Get(env);
    if (!constructor)
      return napi_generic_failure;
    *result = constructor;
    return napi_ok;
  }
};

// Helper to create a new class wrapping raw ptr.
// The Constructor/Destructor of Type<T> will NOT be called.
template<typename T>
napi_status ManagePointerInJSWrapper(napi_env env, T* ptr, napi_value* result) {
  InstanceData* instance_data = InstanceData::Get(env);
  if (internal::CanCachePointer<T>::value) {
    // Check if there is already a JS object created.
    if (instance_data->GetWeakRef<T>(ptr, result))
      return napi_ok;
  }
  // Create a JS object with "new Class(external)".
  napi_value object = internal::CreateInstance<T>(env);
  if (!object)
    return napi_generic_failure;
  // Wrap the |ptr| into JS object.
  auto* data = internal::Wrap<T>::Do(ptr);
  using DataType = decltype(data);
  napi_status s = napi_wrap(env, object, data,
                            [](napi_env env, void* data, void* ptr) {
    if (internal::CanCachePointer<T>::value) {
      // If the weak ref has already been removed, do not run finalizer.
      if (!InstanceData::Get(env)->DeleteWeakRef<T>(ptr))
        return;
    }
    internal::Finalize<T>::Do(static_cast<DataType>(data));
  }, ptr, nullptr);
  if (s != napi_ok) {
    internal::Finalize<T>::Do(data);
    return s;
  }
  // Save weak reference.
  if (internal::CanCachePointer<T>::value)
    instance_data->AddWeakRef<T>(ptr, object);
  *result = object;
  return napi_ok;
}

// Default converter for pointers.
template<typename T>
struct Type<T*, std::enable_if_t<!std::is_const_v<T> &&
                                 std::is_class_v<T> &&
                                 std::is_class_v<Type<T>>>> {
  static constexpr const char* name = Type<T>::name;

 private:
  // Mark the direct ToNode/FromNode methods as private to force users to use
  // helper functions.
  template<typename U>
  friend napi_status ConvertToNode(napi_env env, U&& value, napi_value* result);
  template<typename U>
  friend std::optional<U> FromNodeTo(napi_env env, napi_value value);

  static inline napi_status ToNode(napi_env env, T* ptr, napi_value* result) {
    static_assert(internal::HasWrap<T>::value &&
                  internal::HasFinalize<T>::value,
                  "Converting pointer to JavaScript requires "
                  "TypeBridge<T>::Wrap and TypeBridge<T>::Finalize being "
                  "defined.");
    if (!ptr)
      return napi_get_null(env, result);
    return ManagePointerInJSWrapper(env, ptr, result);
  }

  static inline std::optional<T*> FromNode(napi_env env, napi_value value) {
    void* result;
    if (napi_unwrap(env, value, &result) != napi_ok)
      return std::nullopt;
    if (internal::CanCachePointer<T>::value) {
      // Check if the pointer has been released by user.
      napi_value obj;
      if (!InstanceData::Get(env)->GetWeakRef<T>(result, &obj))
        return std::nullopt;
    }
    if (!internal::IsInstanceOf<T>(env, value))
      return std::nullopt;
    T* ptr = internal::Unwrap<T>::Do(result);
    if (!ptr)
      return std::nullopt;
    return ptr;
  }
};

// Default converter for const pointers.
template<typename T>
struct Type<T*, std::enable_if_t<std::is_const_v<T> &&
                                 std::is_class_v<T> &&
                                 std::is_class_v<Type<T>>>> {
  static constexpr const char* name = Type<std::decay_t<T>>::name;
  static inline napi_status ToNode(napi_env env, T* ptr, napi_value* result) {
    return ConvertToNode(env, const_cast<std::decay_t<T>*>(ptr), result);
  }
};

// For classes that want to allow converting values instead of just pointers
// between JS and C++, they can inherite this class for automatic convertion
// implemented via copying.
template<typename T>
struct AllowPassByValue {
  static inline napi_status ToNode(napi_env env, T value, napi_value* result) {
    return ManagePointerInJSWrapper(env, new T(std::move(value)), result);
  }
  static inline std::optional<T> FromNode(napi_env env, napi_value value) {
    std::optional<T*> ptr = ki::FromNodeTo<T*>(env, value);
    if (!ptr)
      return std::nullopt;
    return *ptr.value();
  }
};

}  // namespace ki

#endif  // SRC_PROTOTYPE_H_
