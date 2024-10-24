#!/bin/bash


if ["$#" -ne 1]; then
    echo "Usage: ./mktest.sh test#";
    exit
fi


touch p4_tests/$1.a

touch p4_tests/$1.err

touch p4_tests/$1.err.expected

touch p4_tests/$1.out

touch p4_tests/$1.unparse.expected