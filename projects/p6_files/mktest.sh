#!/bin/bash


if [ "$#" -ne 1 ]; then
    echo "Usage: ./mktest.sh test#";
    exit
fi


touch p6_tests/$1.a
touch p6_tests/$1.err
touch p6_tests/$1.3ac
touch p6_tests/$1.3ac.expected