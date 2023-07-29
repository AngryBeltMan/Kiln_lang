#!/bin/sh
REPO="git@github.com:AngryBeltMan/Kiln_lang.git"
git add .

while getopts ":m:d" opt; do
    case $opt in
        m)
            echo "commiting with message"
            git commit -m "$OPTARG"
            ;;
        d)
            echo "commiting with default message"
            git commit -m "default"
            ;;
    esac
done
git push $REPO
