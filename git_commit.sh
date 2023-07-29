#!/bin/sh
REPO="git@github.com:AngryBeltMan/Kiln_lang.git"
ssh git@github.com

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
