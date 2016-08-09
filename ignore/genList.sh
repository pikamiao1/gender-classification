#!/bin/bash -eu

DIR=$1
DST=$DIR/newlists.txt

if [ -e "$DST" ]; then 
   rm $DST
fi

i=1
for file in $DIR/*; do
   s=$(basename "$file")
   echo ${s%.*} >> $DST
done
