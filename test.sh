#!/bin/bash

try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > ./tmp.s
  gcc -o ./tmp ./tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$expected expected, but got $actual"
  fi
}

try 0 "0;"
try 42 "42;"
try 41 " 12 + 34 - 5   ;"
try 47 "5  + 6 * 7;"
try 15 "5 * (9 - 6);"
try 4 "(3 + 5) / 2;"
try 3 "-(-3);"
try 5 "-10 + 15;"
try 2 "-(3-5);"
try 1 "1 == 1;"
try 0 "1 != 1;"
try 1 "2 >= 1;"
try 0 "10 > 100;"
try 1 "(10 + (20 * 20)) < 10 * 10 * 10;"
try 11 "a = 10; a + 1;"
try 30 "num_1 = 10; num_2 = 20; num_1 + num_2;"

echo "OK"
