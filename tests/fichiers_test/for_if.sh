#!/bin/bash

# Script avec for et if
for number in {1..5}; do
    if [ $number -eq 3 ]; then
        echo "Nombre trouvé : $number"
    fi
done
