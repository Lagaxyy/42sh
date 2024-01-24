#!/bin/bash

# Script avec while et if
count=1

while [ $count -le 5 ]; do
    if [ $count -eq 3 ]; then
        echo "Compte atteint : $count"
    fi
    ((count++))
done
