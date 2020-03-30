"use strict";

const fs = require('fs-extra');
const path = require('path');
const config = require('./config');

if(fs.existsSync(config.LIBPAT_DIR)) {
    if (fs.existsSync(config.LIBPAT_NODEPAT_DIR)) {
        fs.removeSync(config.LIBPAT_NODEPAT_DIR);
    }

    let symlinkPath = path.relative(config.NODEPAT_SRC_DIR, config.LIBPAT_DIR);

    fs.ensureSymlinkSync(symlinkPath, config.LIBPAT_NODEPAT_DIR);
}
