declare function play(audioPath: string): Promise<void>;
declare function playSync(audioPath: string): void;
declare function skip(): Promise<void>;
declare function skipSync(): void;
declare function pause(): Promise<void>;
declare function pauseSync(): void;
declare function resume(): Promise<void>;
declare function resumeSync(): void;
declare const _default: {
    play: typeof play;
    playSync: typeof playSync;
    skip: typeof skip;
    skipSync: typeof skipSync;
    pause: typeof pause;
    pauseSync: typeof pauseSync;
    resume: typeof resume;
    resumeSync: typeof resumeSync;
};
export default _default;
//# sourceMappingURL=nodepat.d.ts.map