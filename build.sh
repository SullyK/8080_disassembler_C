#!/bin/bash

#gcc -Wall -Wextra -pedantic -std=c17 -g -o dis dis.c #debug build

gcc -Wall -Wextra -pedantic -std=c17 -O2 -o dis dis.c



time ./dis <<EOF > output.txt
invaders.rom
EOF


echo "Runtime: $runtime seconds"

sed -i '' '1d' output.txt
python3 compare.py > compare.txt

if [ ! -s "compare.txt" ]; then
    echo  "✔ Passed Test" 
else
    echo "☠ Difference's found; failed test"
fi
