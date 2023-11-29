<img
width="50px"
height="50px"
src="https://github.com/Maxime-Cllt/Mandelbrot/assets/98154358/6121b166-82fa-4081-b953-7d542dac000e"
alt="French" />

# Calcul Distribué de l'Ensemble de Mandelbrot avec C++ MPI

## Aperçu

Ce projet implémente la génération de l'ensemble de Mandelbrot en utilisant
une approche de calcul distribué en C++ MPI. L'ensemble de Mandelbrot est un
fractal célèbre en mathématiques et cette implémentation permet le calcul parallèle de l'ensemble en utilisant une
architecture distribuée.

## Fonctionnalités

- **Calcul Distribué :** Utilise C++ MPI pour la répartition du calcul sur plusieurs nœuds.

- **Génération de l'Ensemble de Mandelbrot :** Calcule et visualise l'ensemble de Mandelbrot en parallèle.

- **Paramètres Configurables :** Configurez facilement la résolution, le niveau de zoom et d'autres paramètres pour
  explorer différentes parties de l'ensemble de Mandelbrot.

## Prérequis

- C++
- Boost
- MPI
- Git

## Démarrage

1. **Cloner le Dépôt :**

```bash
git clone https://github.com/Sudo-Rahman/Fractalium.git
```
2. **Compiler :**

   Il faut d'abord compiler les fichiers grâce au Cmake

```bash
make
```

3. **Exécution :**

   Cette commande va lancer le client qui va se connecter au serveur et lancer les calculs.

```bash
mpirun -n 4 Fractalium  
```

<br>

<img
width="50px"
height="50px"
src="https://github.com/Maxime-Cllt/Mandelbrot/assets/98154358/8f443a15-4d8d-48a4-8277-499c38165633"
alt="English" />

# Distributed Calculation of the Mandelbrot Set with C++ MPI

## Overview

This project implements the generation of the Mandelbrot set using a distributed computing approach with C++ MPI. The
Mandelbrot set is a famous fractal in mathematics, and this implementation allows parallel computation of the set using
a distributed architecture.

## Features

- **Distributed Calculation:** Uses C++ MPI for distributing the computation across multiple nodes.

- **Mandelbrot Set Generation:** Computes and visualizes the Mandelbrot set in parallel.

- **Configurable Parameters:** Easily configure resolution, zoom level, and other parameters to explore different parts
  of the Mandelbrot set.

## Prerequisites

- C++
- Boost
- MPI
- Git

## Getting Started

1. **Clone the Repository:**

```bash
git clone https://github.com/Sudo-Rahman/Fractalium.git
```

2. **Compile :**

First, compile the files with the Cmake.

```bash
make
```

3. **Execution :**

This command will launch the binary that initiates the calculations.

```bash
mpirun -n 4 Fractalium  
```

