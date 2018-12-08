"use strict";

const ffi = require('ffi');
const ref = require('ref');

const PATPtrType = ref.refType('void');
const PATPtrPtrType = ref.refType(PATPtrType);

const libpat = ffi.Library(__dirname + '/../libpat/build/bin/libpat', {
    'pat_open': ['int', [PATPtrPtrType]],
    'pat_play_async': ['int', [PATPtrType, 'string', 'pointer', 'Object']],
    'pat_skip_async': ['int', [PATPtrType, 'pointer', 'Object']],
    'pat_pause_async': ['int', [PATPtrType, 'pointer', 'Object']],
    'pat_resume_async': ['int', [PATPtrType, 'pointer', 'Object']],
    'pat_close': ['void', [PATPtrType]],
    'pat_error_to_string': ['string', ['int']]
});

const PAT_SUCCESS = 0;
const PAT_AUDIO_DEVICE_ERROR = 1;
const PAT_DEMUX_ERROR = 2;
const PAT_DECODE_ERROR = 3;
const PAT_FILE_OPEN_ERROR = 4;
const PAT_INTERRUPTED_ERROR = 5;
const PAT_MEMORY_ERROR = 6;
const PAT_RESAMPLE_ERROR = 7;
const PAT_TERMINATED_ERROR = 8;
const PAT_UNKNOWN_ERROR = 9;

let patPtr = open();

let libpatCallback = ffi.Callback('void', ['int', 'Object'], (patError, promise) => {
    if(patError === PAT_SUCCESS) {
        promise.resolve();
    } else {
        promise.reject(new Error(libpat.pat_error_to_string(patError)));
    }
});

function open() {
    const patPtrPtr = ref.alloc(PATPtrPtrType);

    let error = libpat.pat_open(patPtrPtr);
    checkStatus(error);

    return patPtrPtr.deref();
}

function play(audioPath) {
    return new Promise((resolve, reject) => {
        let status = libpat.pat_play_async(patPtr, audioPath, libpatCallback, {resolve, reject});

        if(status !== PAT_SUCCESS) {
            reject(new Error(libpat.pat_error_to_string(status)))
        }
    });
}

function skip() {
    return new Promise((resolve, reject) => {
        libpat.pat_skip_async(patPtr, libpatCallback, {resolve, reject});
    });
}

function pause() {
    return new Promise((resolve, reject) => {
        libpat.pat_pause_async(patPtr, libpatCallback, {resolve, reject});
    });
}

function resume() {
    return new Promise((resolve, reject) => {
        libpat.pat_resume_async(patPtr, libpatCallback, {resolve, reject});
    });
}

function checkStatus(error) {
    if (error !== PAT_SUCCESS) {
        throw new Error(libpat.pat_error_to_string(error));
    }
}

process.on('exit', () => {
    libpat.pat_close(patPtr);
});

exports.play = play;
exports.skip = skip;
exports.pause = pause;
exports.resume = resume;
