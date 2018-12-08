#!/bin/bash
set -e
if [ $# -ne 1 ]
then
    echo "Usage release.sh version"
    exit 1
fi

VERSION=$1

echo "Releasing Version $VERSION"

echo "Substituting Version $VERSION"
cd nodepat
npm version $VERSION --allow-same-version
cd ..
echo "VERSION='$VERSION'" > pypat/version.py

echo "Tagging $VERSION"
git tag -a v$VERSION -m v$VERSION
git push origin v$VERSION

./build.sh

echo "Publishing $VERSION"

echo "Publishing nodepat"
cd nodepat
npm login
npm publish
cd ..

echo "Publishing pypat"
cd pypat
twine upload dist/pypat-$VERSION.tar.gz
cd ..
