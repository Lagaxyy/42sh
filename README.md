# 42sh

## Produire le binaire

Les commandes pour générer le binaire sont:
```
autoreconf --install
./configure
make
```

Le binaire se trouvera dans le dossier src/

## Clean le repo

Les commandes pour supprimer tous les fichiers produits sont:
```
make distclean
./rm_autotools_files.sh
```
