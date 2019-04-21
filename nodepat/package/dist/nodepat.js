"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var ffi_1 = require("ffi");
var ref_1 = require("ref");
var PATPtrType = ref_1.refType('void');
var PATPtrPtrType = ref_1.refType(PATPtrType);
var libpat = ffi_1.Library(__dirname + '/libpat/build/bin/libpat', {
    'pat_open': ['int', [PATPtrPtrType]],
    'pat_play': ['int', [PATPtrType, 'string']],
    'pat_skip': ['int', [PATPtrType]],
    'pat_pause': ['int', [PATPtrType]],
    'pat_resume': ['int', [PATPtrType]],
    'pat_close': ['void', [PATPtrType]],
    'pat_error_to_string': ['string', ['int']]
});
var PAT_SUCCESS = 0;
var patPtr = open();
function open() {
    var patPtrPtr = ref_1.alloc(PATPtrPtrType);
    var status = libpat.pat_open(patPtrPtr);
    checkPatStatus(status);
    return patPtrPtr.deref();
}
function play(audioPath) {
    return new Promise(function (resolve, reject) {
        libpat.pat_play.async(patPtr, audioPath, function (err, res) { return audioOperationCallback(err, res, resolve, reject); });
    });
}
function playSync(audioPath) {
    checkPatStatus(libpat.pat_play(patPtr, audioPath));
}
function skip() {
    return new Promise(function (resolve, reject) {
        libpat.pat_skip.async(patPtr, function (err, res) { return audioOperationCallback(err, res, resolve, reject); });
    });
}
function skipSync() {
    checkPatStatus(libpat.pat_skip(patPtr));
}
function pause() {
    return new Promise(function (resolve, reject) {
        libpat.pat_pause.async(patPtr, function (err, res) { return audioOperationCallback(err, res, resolve, reject); });
    });
}
function pauseSync() {
    checkPatStatus(libpat.pat_pause(patPtr));
}
function resume() {
    return new Promise(function (resolve, reject) {
        libpat.pat_resume.async(patPtr, function (err, res) { return audioOperationCallback(err, res, resolve, reject); });
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
    }
    else if (res !== PAT_SUCCESS) {
        reject(new Error(libpat.pat_error_to_string(res)));
    }
    else {
        resolve();
    }
}
process.on('exit', function () {
    libpat.pat_close(patPtr);
});
exports.default = ({
    play: play,
    playSync: playSync,
    skip: skip,
    skipSync: skipSync,
    pause: pause,
    pauseSync: pauseSync,
    resume: resume,
    resumeSync: resumeSync,
});
