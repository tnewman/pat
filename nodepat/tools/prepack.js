"use strict";

const fs = require('fs-extra');
const config = require('./config');

if(fs.existsSync(config.LIBPAT_DIR)) {
    removeIfExists(config.LIBPAT_NODEPAT_DIR);

    fs.copySync(config.LIBPAT_DIR, config.LIBPAT_NODEPAT_DIR);

    fs.removeSync(config.LIBPAT_BUILD_DIR);

    removeIfExists(config.LIBPAT_NODEPAT_CMAKE_DEBUG_DIR);
    removeIfExists(config.LIBPAT_NODEPAT_CMAKE_RELEASE_DIR);
    removeIfExists(config.LIBPAT_NODEPAT_IDEA_DIR);

    fs.ensureDirSync(config.LIBPAT_BUILD_DIR);
}

function removeIfExists(directory) {
    if(fs.existsSync(directory)) {
        fs.removeSync(directory);
    }
}
