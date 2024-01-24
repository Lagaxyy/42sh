#!/bin/bash

# Script avec if, cd et else
if [ -d "/chemin/vers/le/repertoire" ]; then
    echo "Le répertoire existe."

    # Changement de répertoire
    cd /chemin/vers/le/repertoire

    # Affiche le répertoire courant après le changement
    echo "Répertoire courant : $(pwd)"
else
    echo "Le répertoire n'existe pas."
fi
