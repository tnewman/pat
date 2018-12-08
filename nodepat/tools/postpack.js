"use strict";

const fs = require('fs-extra');
const config = require('./config');

if(fs.existsSync(config.LIBPAT_DIR)) {
    if (fs.existsSync(config.LIBPAT_NODEPAT_DIR)) {
        fs.removeSync(config.LIBPAT_NODEPAT_DIR);
    }

    fs.ensureSymlinkSync(config.LIBPAT_DIR, config.LIBPAT_NODEPAT_DIR);
}
