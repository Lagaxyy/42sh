#!/bin/bash

# Boucle for pour afficher les fichiers dans le répertoire courant
for file in $(ls); do
    echo "Fichier : $file"
done
