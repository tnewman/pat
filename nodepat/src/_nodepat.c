#define NAPI_VERSION 3
#include <node_api.h>
#include <pat/pat.h>
#include <pat/pat_error.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define AUDIO_PATH_LENGTH 1024

static napi_status napi_set_named_function(napi_env env, napi_value exports, char* function_name, napi_callback function,
    void* data);

static void _nodepat_close(void* args);

static napi_value _nodepat_play(napi_env env, napi_callback_info info);

static napi_value _nodepat_skip(napi_env env, napi_callback_info info);

static napi_value _nodepat_pause(napi_env env, napi_callback_info info);

static napi_value _nodepat_resume(napi_env env, napi_callback_info info);

static PAT* pat;

napi_value Init(const napi_env env, const napi_value exports) {
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

typedef struct _PATPlayData {
    PATError status;
    napi_deferred deferred;
    char audio_path[AUDIO_PATH_LENGTH];
} _PATPlayData;

void _nodepat_play_execute(napi_env env, void* data) {
    _PATPlayData* pat_play_data = (_PATPlayData*) data;

    pat_play_data->status = pat_play(pat, pat_play_data->audio_path);
}

void _nodepat_play_complete(napi_env env, napi_status status, void* data) {
    _PATPlayData* pat_play_data = (_PATPlayData*) data;

    if (status != napi_ok) {
        goto cleanup;
    }

    if (pat_play_data->status == PAT_SUCCESS) {
        napi_value undefined;

        status = napi_get_undefined(env, &undefined);

        if (status != napi_ok) {
            goto cleanup;
        }

        status = napi_resolve_deferred(env, pat_play_data->deferred, undefined);

        if (status != napi_ok) {
            goto cleanup;
        }
    } else {
        napi_value error;
        napi_value error_str;

        status = napi_create_string_utf8(env, pat_error_to_string(pat_play_data->status), NAPI_AUTO_LENGTH, &error_str);

        if (status != napi_ok) {
            goto cleanup;
        }

        status = napi_create_error(env, NULL, error_str, &error);

        if (status != napi_ok) {
            goto cleanup;
        }

        status = napi_reject_deferred(env, pat_play_data->deferred, error);

        if (status != napi_ok) {
            goto cleanup;
        }
    }

    cleanup:
        free(pat_play_data);
}

static napi_value _nodepat_play(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value result = NULL;

    size_t argc = 1;
    napi_value argv[1];

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, "TypeError", "String expected.");
        goto error;
    }

    _PATPlayData* data = malloc(sizeof(_PATPlayData));

    if(!data) {
        goto error;
    }

    status = napi_get_value_string_utf8(env, argv[0], data->audio_path, AUDIO_PATH_LENGTH, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, NULL, "String expected.");
        goto error;
    }

    napi_value promise;

    status = napi_create_promise(env, &data->deferred, &promise);

    if (status != napi_ok) {
        goto error;
    }

    napi_async_work work;

    status = napi_create_async_work(env, NULL, "PAT Play", _nodepat_play_execute, _nodepat_play_complete, data, &work);

    if (status != napi_ok) {
        goto error;
    }

    status = napi_queue_async_work(env, work);

    if (status != napi_ok) {
        goto error;
    }

    return result;

    error:
        if (data != NULL) {
            free(data);
        }

        return NULL;
}

static napi_value _nodepat_skip(napi_env env, napi_callback_info info) {
    return NULL;
}

static napi_value _nodepat_pause(napi_env env, napi_callback_info info) {
    return NULL;
}

static napi_value _nodepat_resume(napi_env env, napi_callback_info info) {
    return NULL;
}
