#!/bin/sh
REPO="git@github.com:AngryBeltMan/Kiln_lang.git"
git add .
while getopts ":a:d" opt; do
    case $opt in
        a)
            git commit -m "$OPTARG"
            ;;
        d)
            git commit -m "default"
            ;;
    esac
done
git push $REPO
