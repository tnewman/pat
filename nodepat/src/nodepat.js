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

function playSync(audioPath) {
    checkPatStatus(libpat.pat_play(patPtr, audioPath));
}

function skip() {
    return new Promise((resolve, reject) => {
        libpat.pat_skip.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function skipSync() {
    checkPatStatus(libpat.pat_skip(patPtr));
}

function pause() {
    return new Promise((resolve, reject) => {
        libpat.pat_pause.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function pauseSync() {
    checkPatStatus(libpat.pat_pause(patPtr));
}

function resume() {
    return new Promise((resolve, reject) => {
        libpat.pat_resume.async(patPtr, (err, res) => audioOperationCallback(err, res, resolve, reject));
    });
}

function resumeSync() {
    checkPatStatus(libpat.pat_resume(patPtr));
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
exports.playSync = playSync;
exports.skip = skip;
exports.skipSync = skipSync;
exports.pause = pause;
exports.pauseSync = pauseSync;
exports.resume = resume;
exports.resumeSync = resumeSync;

