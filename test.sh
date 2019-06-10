#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./dest/9cc "$input" > ./dest/tmp.s
  gcc -o ./dest/tmp ./dest/tmp.s
  ./dest/tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
  fi
}

try 0 0
try 42 42
try 41 " 12 + 34 - 5   "
try 47 "5  + 6 * 7"
try 15 "5 * (9 - 6)"
try 4 "(3 + 5) / 2"

echo "OK"
