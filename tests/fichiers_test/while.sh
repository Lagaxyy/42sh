#!/bin/bash

count=1

# Boucle while
while [ $count -le 5 ]; do
    echo "Iteration $count"
    ((count++))
done
