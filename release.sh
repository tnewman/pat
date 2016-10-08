#!/bin/bash
echo "======================================="
echo "| PAT Audio Technician Release Script |"
echo "======================================="

if [[ -n $(git status -s) ]]; then
    echo "Uncommitted files! Aborting release!"
    exit -1
fi

VERSION=`cat VERSION`

echo "Do you want to release with this version number: $VERSION (yes/no)?"
read confirm

if [ $confirm == "yes" ]; then
    git tag $VERSION

    if [ $? -ne 0 ]; then
        echo "Failed to create Git tag $VERSION! Aborting release!"
        exit -1
    fi

    git push origin $VERSION

    if [ $? -ne 0]; then
        echo "Failed to push $VERSION to Git! Aborting release!"
        
        git tag -d $VERSION
        git push --delete origin $VERSION
        
        exit -1
    fi

    python3 setup.py sdist upload

    if [ $? -ne 0]; then
        echo "Failed to upload $VERSION to PyPi! Aborting release!"
        exit -1
    fi

    echo "$VERSION Released Successfully!"
    exit 0
else
    echo "Release cancelled!"
    exit -1
fi
