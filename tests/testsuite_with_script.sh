#!/bin/sh

# Emplacement de votre shell 42sh
YOUR_SHELL="../src/42sh"

# Emplacement du shell par défaut (par exemple, Bash)
DEFAULT_SHELL="/bin/sh"

# Répertoire de travail pour les tests
TEST_DIR="d"

# Répertoire contenant les fichiers de test
TEST_FILES_DIR="fichiers_test"

# Liste des fichiers à tester
TEST_FILES=("$TEST_FILES_DIR"/*)

# couleur
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

TEST_SUCCESS=0
TEST_ECHEC=0

# Création du répertoire de tests
mkdir -p "$TEST_DIR"

# Boucle sur chaque fichier à tester
for file in "${TEST_FILES[@]}"; do
    # Exécution de la commande avec votre shell
    echo "Test:    $file"
    $YOUR_SHELL < "$file" > "$TEST_DIR/42sh_output.txt"
    EXITCODE_YOUR_SHELL=$?

    # Exécution de la commande avec le shell par défaut
    $DEFAULT_SHELL < "$file" > "$TEST_DIR/default_shell_output.txt"
    EXITCODE_DEFAULT_SHELL=$?

    # Comparaison des résultats
    diff "$TEST_DIR/42sh_output.txt" "$TEST_DIR/default_shell_output.txt"

    if [ $? -eq 0 ] && [ $EXITCODE_YOUR_SHELL -eq $EXITCODE_DEFAULT_SHELL ]; then
        echo -e "${GREEN}Test passed${NC}"
        TEST_SUCCESS=$((TEST_SUCCESS + 1))
    else
        echo -e "${RED}Test failed${NC}"
        TEST_ECHEC=$((TEST_ECHEC + 1))
    fi

    # Separation visuelle
    echo "-------------------------------------------------------------------"
done

# Separation visuelle
echo ""

# Nettoyage du répertoire de tests
rm -r "$TEST_DIR"

echo -e "${GREEN}Tests passed: $TEST_SUCCESS${NC}"
echo -e "${RED}Tests failed: $TEST_ECHEC${NC}"
