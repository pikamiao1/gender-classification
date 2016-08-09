#!/bin/bash -eu

DIR=$1

i=1
for file in $DIR/*; do
  new=$(printf "face_%04d.bmp" "$i") #04 pad to length of 4
  mv -- "$file" "$DIR/$new"
  let i=i+1
done
