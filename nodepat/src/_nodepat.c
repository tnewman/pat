#define NAPI_VERSION 3
#include <node_api.h>
#include <pat/pat.h>
#include <pat/pat_error.h>
#include <stdlib.h>

#define AUDIO_PATH_LENGTH 1024

typedef enum NodePATTask {
    PAT_PLAY,
    PAT_SKIP,
    PAT_PAUSE,
    PAT_RESUME
} NodePATTask;

typedef struct _PATPlayData {
    NodePATTask task;
    PATError status;
    napi_deferred deferred;
    void* data;
} _PATPlayData;

static napi_status napi_set_named_function(napi_env env, napi_value exports, char* function_name, napi_callback function,
    void* data);

static napi_value _nodepat_play(napi_env env, napi_callback_info info);

static napi_value _nodepat_skip(napi_env env, napi_callback_info info);

static napi_value _nodepat_pause(napi_env env, napi_callback_info info);

static napi_value _nodepat_resume(napi_env env, napi_callback_info info);

static napi_value _nodepat_queue_async_work(napi_env env, NodePATTask task, void* data);

static void _nodepat_execute(napi_env env, void* data);

static void _nodepat_complete(napi_env env, napi_status status, void* data);

napi_value Init(const napi_env env, const napi_value exports) {
    PATError pat_status = pat_init();

    if (pat_status != PAT_SUCCESS) {
        napi_value error;
        napi_value error_str;

        if (napi_create_string_utf8(env, pat_error_to_string(pat_status), NAPI_AUTO_LENGTH, &error_str)
            != napi_ok) {
            return NULL;
        }

        if (napi_create_error(env, NULL, error_str, &error) != napi_ok) {
            return NULL;
        }

        napi_throw(env, error);

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

static napi_value _nodepat_play(napi_env env, napi_callback_info info) {
    napi_status status;

    size_t argc = 1;
    napi_value argv[1];

    char* audio_path = malloc(AUDIO_PATH_LENGTH);

    if (audio_path == NULL) {
        goto error;
    }

    status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, "TypeError", "String expected.");
        goto error;
    }

    status = napi_get_value_string_utf8(env, argv[0], audio_path, AUDIO_PATH_LENGTH, NULL);

    if (status != napi_ok) {
        napi_throw_type_error(env, NULL, "String expected.");
        goto error;
    }

    napi_value promise = _nodepat_queue_async_work(env, PAT_PLAY, (void*) audio_path);

    if (promise == NULL) {
        goto error;
    }

    return promise;

    error:
        if (audio_path != NULL) {
            free(audio_path);
        }

        return NULL;
}

static napi_value _nodepat_skip(napi_env env, napi_callback_info info) {
    return _nodepat_queue_async_work(env, PAT_SKIP, NULL);
}

static napi_value _nodepat_pause(napi_env env, napi_callback_info info) {
    return _nodepat_queue_async_work(env, PAT_PAUSE, NULL);
}

static napi_value _nodepat_resume(napi_env env, napi_callback_info info) {
    return _nodepat_queue_async_work(env, PAT_RESUME, NULL);
}

static napi_value _nodepat_queue_async_work(napi_env env, NodePATTask task, void* data) {
    napi_status status;

    _PATPlayData* pat_play_data = malloc(sizeof(_PATPlayData));

    if(!pat_play_data) {
        goto error;
    }

    pat_play_data->task = task;
    pat_play_data->data = data;

    napi_value undefined;

    status = napi_get_undefined(env, &undefined);

    if (status != napi_ok) {
        goto error;
    }

    napi_value promise;

    status = napi_create_promise(env, &pat_play_data->deferred, &promise);

    if (status != napi_ok) {
        goto error;
    }

    napi_async_work work;

    status = napi_create_async_work(env, NULL, undefined, _nodepat_execute, _nodepat_complete, pat_play_data, &work);

    if (status != napi_ok) {
        goto error;
    }

    status = napi_queue_async_work(env, work);

    if (status != napi_ok) {
        goto error;
    }

    return promise;

    error:
        if (pat_play_data != NULL) {
            free(pat_play_data);
        }

        return NULL;
}

static void _nodepat_execute(napi_env env, void* data) {
    _PATPlayData* pat_play_data = (_PATPlayData*) data;

    switch (pat_play_data->task) {
        case PAT_PLAY:
            pat_play_data->status = pat_play((char*) pat_play_data->data);
            break;
        case PAT_SKIP:
            pat_play_data->status = pat_skip();
            break;
        case PAT_PAUSE:
            pat_play_data->status = pat_pause();
            break;
        case PAT_RESUME:
            pat_play_data->status = pat_resume();
            break;
        default:
            pat_play_data->status = PAT_UNKNOWN_ERROR;
            break;
    }
}

static void _nodepat_complete(napi_env env, napi_status status, void* data) {
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
        if (pat_play_data->data != NULL) {
            free(pat_play_data->data);
        }

        if (pat_play_data != NULL) {
            free(pat_play_data);
        }
}
