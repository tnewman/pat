"use strict";

const ffi = require('ffi');
const ref = require('ref');

const PATPtrType = ref.refType('void');
const PATPtrPtrType = ref.refType(PATPtrType);

const libpat = ffi.Library(__dirname + '/libpat/build/bin/libpat', {
    'pat_open': ['int', [PATPtrPtrType]],
    'pat_play': ['int', [PATPtrType, 'string']],
    'pat_skip': ['int', [PATPtrType]],
    'pat_pause': ['int', [PATPtrType]],
    'pat_resume': ['int', [PATPtrType]],
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

function open() {
    const patPtrPtr = ref.alloc(PATPtrPtrType);

    let status = libpat.pat_open(patPtrPtr);
    checkPatStatus(status);

    return patPtrPtr.deref();
}

function play(audioPath) {
    return new Promise((resolve, reject) => {
        libpat.pat_play.async(patPtr, audioPath, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function skip() {
    return new Promise((resolve, reject) => {
        libpat.pat_skip.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function pause() {
    return new Promise((resolve, reject) => {
        libpat.pat_pause.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function resume() {
    return new Promise((resolve, reject) => {
        libpat.pat_resume.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function checkPatStatus(error) {
    if (error !== PAT_SUCCESS) {
        throw new Error(libpat.pat_error_to_string(error));
    }
}

function audioOperationCallback(err, res, resolve, reject) {
    if (err) {
        reject(err);
    } else if (res !== PAT_SUCCESS) {
        reject(new Error(libpat.pat_error_to_string(res)));
    } else {
        resolve();
    }
}

process.on('exit', () => {
    libpat.pat_close(patPtr);
});

exports.play = play;
exports.skip = skip;
exports.pause = pause;
exports.resume = resume;
