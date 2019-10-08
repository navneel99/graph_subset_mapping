#!/bin/bash
python generate.py $1 $2 $3 $4 > $5
./compile.sh
./run1.sh $5
./minisat "$5.satinput" "$5.satoutput"
./run2.sh $5
cat "$5.mapping"
