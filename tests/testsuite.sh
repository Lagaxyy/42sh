#!/bin/bash

# Emplacement de votre shell 42sh
YOUR_SHELL="../src/42sh"

# Emplacement du shell par défaut (par exemple, Bash)
DEFAULT_SHELL="/bin/sh"

# Répertoire de travail pour les tests
TEST_DIR="d"

# Liste des commandes à tester
COMMANDS=(
    # simple_commands
    "echo Hello, World!;"
    "echo -nE Hello World! \\n;"
    "echo -eEn Hello World! \\n"
    "echo -n -E -e Hello World! \\n"
    "echo -n -E -e Hello World! \\\\n"
    "echo -Ee -n Hello World! \\n;"
    "echo -Ee -n Hello World! \\\\n;"
    "echo Hello World! \\\\\\\\n;"
    "echo -e Hello World! \\\\\\\\n"
    "echo ;"
    "ls /bin"
    "ls connaispas"
    "existepas"
    "true"
    "false"
    "true true false"
    "false true"

    # command_list
    # "echo Command 1; echo Command 2; echo Command 3"

    # if_command
    # "if true; then echo Condition is true.; fi"
)

# couleur
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

TEST_SUCCESS=0
TEST_ECHEC=0


# Création du répertoire de tests
mkdir -p "$TEST_DIR"

# Boucle sur chaque commande à tester
for cmd in "${COMMANDS[@]}"; do
    # Exécution de la commande avec votre shell
    echo -e "Running command with 42sh:"
    echo ">    $cmd"
    $YOUR_SHELL -c "$cmd" > "$TEST_DIR/42sh_output.txt"
    EXITCODE_YOUR_SHELL=$?

    # Exécution de la commande avec le shell par défaut
    echo -e "Running command with default shell:"
    echo ">    $cmd"
    $DEFAULT_SHELL -c "$cmd" > "$TEST_DIR/default_shell_output.txt"
    EXITCODE_DEFAULT_SHELL=$?

    # Comparaison des résultats
    diff "$TEST_DIR/42sh_output.txt" "$TEST_DIR/default_shell_output.txt"

    if [ $? -eq 0 ] && [ $EXITCODE_YOUR_SHELL -eq $EXITCODE_DEFAULT_SHELL ]; then
        # echo -e "${GREEN}Test passed: $cmd ${NC}"
        echo -e "${GREEN}Test passed${NC}"
        TEST_SUCCESS=$((TEST_SUCCESS + 1))
    else
        # echo -e "${RED}Test failed: $cmd ${NC}"
        echo -e "${RED}Test failed${NC}"
        TEST_ECHEC=$((TEST_ECHEC + 1))
    fi

    # Separation visuelle
    echo ""
done

# Separation visuelle
echo ""

# Nettoyage du répertoire de tests
rm -r "$TEST_DIR"

echo -e "${GREEN}Test passed: $TEST_SUCCESS ${NC}"
echo -e "${RED}Test failed: $TEST_ECHEC ${NC}"
