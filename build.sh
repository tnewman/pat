#!/bin/bash
set -e

echo "Building libpat"
rm -rf libpat/build
mkdir -p libpat/build
cd libpat/build
touch .gitkeep
cmake .. -G Ninja -DBUILD_PAT_PLAY=true
cmake --build .
cd ../..

echo "Building nodepat"
cd nodepat
npm install
npm test
npm pack
cd ..

echo "Building pypat"
cd pypat
pipenv sync --dev
pipenv run python setup.py build_ext --inplace
pipenv run py.test
pipenv run python setup.py sdist
cd ..

