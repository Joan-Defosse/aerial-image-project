# Simulateur d'Images Aériennes

Ce projet a pour but de simuler une suite de vues aériennes d'un feu de forêt généré aléatoirement.

## Compilation

Afin de créer les fichiers exécutables du projet, vous devez disposer du compilateur g++ et effectuer la commande :

- `make` si vous souhaitez compiler `main.cpp` et créer le fichier `main.exe`.

- `g++ src/Color.cpp src/Image.cpp src/Analyst.cpp src/testeval.cpp -o testeval.exe` si le fichier qui vous intéresse est `testeval.cpp`.

## Organisation

Les sources de ce projet sont organisées de la manière suivante :

- `Color.h` définit l'énumération **Couleur** et permet d'associer chaque couleur à un entier.

- `Image.h` définit l'objet **Image**, composé de **Couleurs**, et ses opérations.

- `Analyst.h` définit les méthodes d'analyse sur les objets **Images**, permettant notamment de délimiter des *zones* de **Couleurs**

- `FireSimulator.h` définit les opérations permettant finalement la simulations de feux, la création de suites d'**Images** reliées par un scénario aléatoire répondant à certaines règles.
