# Simulateur de Réseau en C


## Description

Ce projet est un simulateur de réseau écrit en C réalisé par une équipe de 3 développeurs. Il permet de modéliser un réseau en utilisant des adresses, des trames et une topologie définie dans un fichier texte `.mylan`. Le programme inclut la simulation d’algorithmes de routage et de prévention de boucles, comme le **STP (Spanning Tree Protocol)**.

## Structure du projet

* `main.c` : Point d’entrée du programme, contient l’algorithme principal de simulation.
* `reseaux.c` / `reseaux.h` : Gestion des réseaux.
* `addresses.c` / `addresses.h` : Gestion des adresses et des identifiants de nœuds.
* `trames.c` / `trames.h` : Gestion des trames.
* `mylan.txt` : Fichier texte contenant la définition du réseau à simuler (nœuds, liens, etc.).

## Fonctionnalités

* Chargement d’un réseau depuis un fichier `.mylan`.
* Gestion des adresses et des trames dans le réseau.
* Simulation d’algorithmes réseau, comme le STP, pour éviter les boucles.
* Affichage de l’état du réseau et des transmissions de trames.

## Compilation et exécution

Pour compiler le projet :

```bash
make
```

Pour exécuter le simulateur :

```bash
./main mylan.txt
```

## Capture d'écrans
### Menu : 
<img width="951" height="341" alt="image" src="https://github.com/user-attachments/assets/6dd11e28-9eda-4cbf-a3da-e936c59506ae" />

### Affichage d'une station et d'un switch : 
<img width="469" height="849" alt="image" src="https://github.com/user-attachments/assets/5fb8b5d0-6185-40f3-9fe0-4497742a5d08" />

### Envoi d'une trame : 
<img width="891" height="198" alt="image" src="https://github.com/user-attachments/assets/06153c96-ca4b-4f89-a04d-5c8f16b11b36" />






