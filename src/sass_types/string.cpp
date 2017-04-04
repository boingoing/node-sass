#include "string.h"
#include "../create_string.h"

namespace SassTypes
{
  String::String(napi_env env, Sass_Value* v) : SassValueWrapper(env, v) {}

  Sass_Value* String::construct(napi_env env, const std::vector<napi_value> raw_val, Sass_Value **out) {
    char const* value = "";

    if (raw_val.size() >= 1) {
      napi_valuetype t;
      CHECK_NAPI_RESULT(napi_typeof(env, raw_val[0], &t));

      if (t != napi_string) {
        return fail("First argument should be a string.", out);
      }

      value = create_string(env, raw_val[0]);
    }

    return *out = sass_make_string(value);
  }

  napi_value String::getConstructor(napi_env env, napi_callback cb) {
    napi_value ctor;
    napi_property_descriptor descriptors[] = {
      { "getValue", GetValue },
      { "setValue", SetValue },
    };

    CHECK_NAPI_RESULT(napi_define_class(env, get_constructor_name(), cb, nullptr, 2, descriptors, &ctor));
    return ctor;
  }

  void String::GetValue(napi_env env, napi_callback_info info) {
    CommonGetString(env, info, sass_string_get_value);
  }

  void String::SetValue(napi_env env, napi_callback_info info) {
    CommonSetString(env, info, sass_string_set_value);
  }
}
