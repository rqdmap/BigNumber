#!/usr/bin/python3
import math

a = int(input())
# print(a * 256)

b = input()
c = int(input())

if b == "+":
    print(math.floor(a + c))
if b == "-":
    print(math.floor(a - c))
if b == "*":
    print(math.floor(a * c))
if b == "/":
    print(math.floor(a / c))
if b == "%":
    print(math.floor(a % c))
if b == "&":
    print(math.floor(a & c))
if b == "|":
    print(math.floor(a | c))
if b == "^":
    print(math.floor(a ^ c))

