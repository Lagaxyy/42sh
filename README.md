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

## Clean le repo

Les commandes pour supprimer tous les fichiers produits sont:
```
make distclean
./rm_autotools_files.sh
```
NB: après avoir fait un make check, faire un make clean pour supprimer les
fichiers de coverage

## Autres commandes utiles

- En cas d'erreurs hasardeuses/incompréhensibles dans un fichier,
lancer cette commande sur le fichier en question (enlève le carriage return
ajouté par Windows)
```
sed -i -e 's/\r$//' <nom du fichier>
```
