#!/bin/bash
set -e

echo "Building libpat"
cd libpat/build
cmake .. -G Ninja && ninja clean && ninja
cd ../..

echo "Building nodepat"
cd nodepat
npm install && npm pack
cd ..

echo "Building pypat"
cd pypat
python3 setup.py bdist
python3 setup.py sdist
cd ..

