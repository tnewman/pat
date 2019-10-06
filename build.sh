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
pipenv run py.test
python3 setup.py build
python3 setup.py sdist
cd ..

