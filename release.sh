#!/bin/bash
set -e
if [ $# -ne 1 ]
then
    echo "Usage release.sh version"
    exit 1
fi

VERSION=$1

echo "Releasing Version $VERSION"

echo "Tagging $VERSION"
git tag v$VERSION
git push v$VERSION

./build.sh

