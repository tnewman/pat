#define NAPI_VERSION 3
#include <node_api.h>
#include <pat/pat.h>
#include <pat/pat_error.h>

static napi_status napi_set_named_function(napi_env env, napi_value exports, char* function_name, napi_callback function,
    void* data);

static void _nodepat_close(void* args);

static napi_value _nodepat_play(napi_env env, napi_callback_info info);

static napi_value _nodepat_skip(napi_env env, napi_callback_info info);

static napi_value _nodepat_pause(napi_env env, napi_callback_info info);

static napi_value _nodepat_resume(napi_env env, napi_callback_info info);

static PAT* pat;

napi_value Init(const napi_env env, const napi_value exports) {
    napi_status napi_status;

    PATError pat_status = pat_open(&pat);

    if(pat_status != PAT_SUCCESS) {
        return NULL;
    }

    if (napi_set_named_function(env, exports, "_play", _nodepat_play, NULL) != napi_ok) {
        return NULL;
    }

    if (napi_set_named_function(env, exports, "_skip", _nodepat_skip, NULL) != napi_ok) {
        return NULL;
    }

    if (napi_set_named_function(env, exports, "_pause", _nodepat_pause, NULL) != napi_ok) {
        return NULL;
    }

    if (napi_set_named_function(env, exports, "_resume", _nodepat_resume, NULL) != napi_ok) {
        return NULL;
    }

    if (napi_add_env_cleanup_hook(env, _nodepat_close, NULL) != napi_ok) {
        return NULL;
    }

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

static napi_status napi_set_named_function(napi_env env, napi_value exports, char* function_name, napi_callback function,
    void* data) {

    napi_status status;
    napi_value callback;

    status = napi_create_function(env, function_name, NAPI_AUTO_LENGTH, function, data, &callback);

    if (status != napi_ok) {
        return status;
    }

    return napi_set_named_property(env, exports, function_name, callback);
}

static void _nodepat_close(void* args) {
    pat_close(pat);
}

static napi_value _nodepat_play(napi_env env, napi_callback_info info) {
    const size_t ARG_LENGTH = 1;

    napi_status status;

    size_t argc = ARG_LENGTH;
    napi_value argv[ARG_LENGTH];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, "TypeError", "String expected.");
        return NULL;
    }

    const size_t AUDIO_PATH_LENGTH = 1024;
    char audio_path[AUDIO_PATH_LENGTH];

    status = napi_get_value_string_utf8(env, argv[0], audio_path, AUDIO_PATH_LENGTH, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, "TypeError", "String expected.");
        return NULL;
    }

    pat_play(pat, audio_path);

    return NULL;
}

static napi_value _nodepat_skip(napi_env env, napi_callback_info info) {
    return napi_ok;
}

static napi_value _nodepat_pause(napi_env env, napi_callback_info info) {
    return napi_ok;
}

static napi_value _nodepat_resume(napi_env env, napi_callback_info info) {
    return napi_ok;
}
