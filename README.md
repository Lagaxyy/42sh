# 42sh

## Produire le binaire

Les commandes pour générer le binaire sont:
```
autoreconf --install
./configure
make
```
Le binaire se trouvera dans le dossier _src/_

## Ajouter des flags

Exemple: ajout de flags pour debug et voir les memory leaks
```
autoreconf --install
./configure CFLAGS='-g -fsanitize=address' LDFLAGS='-fsanitize=address'
make
```

Les commandes pour supprimer tous les fichiers produits sont:
```
make distclean
./rm_autotools_files.sh
```
