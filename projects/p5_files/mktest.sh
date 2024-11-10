#!/bin/bash


if [ "$#" -ne 1 ]; then
    echo "Usage: ./mktest.sh test#";
    exit
fi


touch p5_tests/$1.a
touch p5_tests/$1.err
touch p5_tests/$1.err.expected
