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

## Lancer les tests

Les commandes pour lancer les tests (unitaires pour l'instant) sont:
```
autoreconf --install
./configure
make check
```

## Voir le coverage des tests unitaires

Les commandes pour voir le coverage sont:
```
autoreconf --install
./configure
make cov
```
Les fichiers _report.html_ sont les fichiers de bilan du coverage

## Clean le repo

Les commandes pour supprimer tous les fichiers produits sont:
```
make distclean
./rm_autotools_files.sh
```

## Autres commandes utiles

- En cas d'erreurs hasardeuses/incompréhensibles dans un fichier,
lancer cette commande sur le fichier en question (enlève le carriage return
ajouté par Windows)
```
sed -i -e 's/\r$//' <nom du fichier>
```
