#!/bin/bash
make filecopy

./filecopy test_inputs/test_short.txt test_outputs/test_short.txt
./filecopy test_inputs/test_medium.txt test_outputs/test_medium.txt
./filecopy test_inputs/test_long.txt test_outputs/test_long.txt

cmp test_inputs/test_short.txt test_outputs/test_short.txt
cmp test_inputs/test_medium.txt test_outputs/test_medium.txt
cmp test_inputs/test_long.txt test_outputs/test_long.txt


