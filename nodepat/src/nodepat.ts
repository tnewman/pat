import { Library } from 'ffi';
import { alloc, refType } from 'ref';

const PATPtrType = refType('void');
const PATPtrPtrType = refType(PATPtrType);

const libpat = Library(__dirname + '/libpat/build/bin/libpat', {
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
    const patPtrPtr: any = alloc(PATPtrPtrType);

    let status = libpat.pat_open(patPtrPtr);
    checkPatStatus(status);

    return patPtrPtr.deref();
}

function play(audioPath: string): Promise<void> {
    return new Promise((resolve, reject) => {
        libpat.pat_play.async(patPtr, audioPath, (err: Error, res: number) => audioOperationCallback(err, res, resolve, reject));
    });
}

function playSync(audioPath: string): void {
    checkPatStatus(libpat.pat_play(patPtr, audioPath));
}

function skip(): Promise<void> {
    return new Promise((resolve, reject) => {
        libpat.pat_skip.async(patPtr, (err: Error, res: number) => audioOperationCallback(err, res, resolve, reject));
    });
}

function skipSync(): void {
    checkPatStatus(libpat.pat_skip(patPtr));
}

function pause(): Promise<void> {
    return new Promise((resolve, reject) => {
        libpat.pat_pause.async(patPtr, (err: Error, res: number) => audioOperationCallback(err, res, resolve, reject));
    });
}

function pauseSync(): void {
    checkPatStatus(libpat.pat_pause(patPtr));
}

function resume(): Promise<void> {
    return new Promise((resolve, reject) => {
        libpat.pat_resume.async(patPtr, (err: Error, res: number) => audioOperationCallback(err, res, resolve, reject));
    });
}

function resumeSync(): void {
    checkPatStatus(libpat.pat_resume(patPtr));
}

function checkPatStatus(error: number): void {
    if (error !== PAT_SUCCESS) {
        throw new Error(libpat.pat_error_to_string(error));
    }
}

function audioOperationCallback(err: Error, res: number, resolve: () => void, reject: (error: Error) => void) {
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

export default ({
    play,
    playSync,
    skip,
    skipSync,
    pause,
    pauseSync,
    resume,
    resumeSync,
});
