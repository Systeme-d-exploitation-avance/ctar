# Projet - CTAR

## I. Introduction

Dans le cadre de ce projet, nous avons réalisé un générateur/extracteur d'archive .tar conforme à la norme GNU POSIX 1003.1-1988 "ustar" avec une prise en charge de la compression/decompression gzip via la librairie zlib. 

Ce programme permet de lister, extraire et générer des archives .tar ou .tar.gz.

## II. Organisation du Projet

Le projet est organisé en plusieurs dossiers :

- **build** : contient les fichiers générés lors de la compilation du programme
- **coverage** : contient les fichiers générés lors de l'exécution des tests unitaires via gcov utilisable directement via le makefile (make coverage)
- **doc** : contient la documentation du programme, générée avec Doxygen via le makefile (make doxygen)
- **include** : contient les fichiers d'en-tête du programme
- **src** : contient le code source du programme
- **template** : contient les fichiers de test du programme (archives .tar et .tar.gz)

Ainsi qu'un fichier Makefile à la racine permettant de compiler le programme, de générer la documentation, et d'exécuter les tests unitaires.



## V. Guide d'Utilisation

### V.1. Compilation

Pour compiler le programme, il suffit d'exécuter la commande suivante à la racine du projet :

```sh
make
```

### V.2. Exécution

- **Lister les éléments d'une archive tar ou gz:**
  ```sh
  ./build/ctar -l archive.tar(.gz)
  ```

- **Extraire l'intégralité d'une archive tar ou gz :**
  ```sh
  # sans spécifier le répertoire de travail (répertoire courant par défaut)
  ./build/ctar -e archive.tar(.gz)
  # en spécifiant le répertoire de travail
  ./build/ctar -d myFolder -e archive.tar(.gz)
  ```

- **Générer une archive tar ou gz :**
  ```sh
  # compression implicitement geré via l'extension du fichier
  ./build/ctar -c archive.tar(.gz) fichier1 fichier2 dossier1
  # compression explicite avec le flag -z
  ./build/ctar -zc archive.tar fichier1 fichier2 dossier1
  ```

- **Spécifier le répertoire de travail :**
  ```sh
  ./build/ctar -d myFolder -e archive.tar
  ```

- **Activer le mode verbose :**
  ```sh
  ./build/ctar -v -l archive.tar
  ```

- **Afficher l'aide :**
  ```sh
  ./build/ctar -h
  ```

### V.3. Documentation Doxygen

Pour générer la documentation Doxygen, il suffit d'exécuter la commande suivante à la racine du projet :

```sh
make doxygen
```
Puis d'ouvrir le fichier **doc/html/index.html** dans un navigateur web.

### V.4. Controles de couverture

Pour générer les fichiers de couverture, il suffit d'exécuter la commande suivante à la racine du projet :

```sh
make coverage
```
