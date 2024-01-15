<h1 align="center">Fractalium</h1>

<p align="center">
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/mandelbrot1.png" alt="Mandelbrot" />
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/mandelbrot2.png" alt="Mandelbrot" />
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/julia1.png" alt="Julia" />
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/julia2.png" alt="Julia" />
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/bs1.png" alt="Burning Ship" />
  <img width="250px" height="250px" src="https://github.com/Sudo-Rahman/Fractalium/blob/main/documentation/image/bs2.png" alt="Burning Ship" />
</p>

# Calcul Distribué de l'Ensemble de Mandelbrot avec C++ MPI

## Aperçu

Ce projet implémente la génération de l'ensemble de Mandelbrot en utilisant
une approche de calcul distribué en C++ MPI. L'ensemble de Mandelbrot est un
fractal célèbre en mathématiques et cette implémentation permet le calcul parallèle de l'ensemble en utilisant une
architecture distribuée.

## Fonctionnalités

- **Calcul Distribué :** Utilise C++ MPI pour la répartition du calcul sur plusieurs nœuds d'un cluster.

- **Génération de fractal :** Calcul et visualise des fractales célèbres telles que l'ensemble de Mandelbrot,
  l'ensemble de Julia et le Burning Ship.

- **Paramètres Configurables :** Configurez facilement la résolution, le niveau de zoom et d'autres paramètres pour
  explorer différentes parties de chaque fractale avec une grande précision et une grande vitesse de calcul grâce à la
  distribution.

## Prérequis

- C++
- Boost
- Boost MPI
- Git
- Qt >=5

## Compatibilité

<img src="https://img.shields.io/badge/OS-MacOS-informational?style=flat&logo=apple&logoColor=white&color=2bbc8a" alt="MacOS" />
<img src="https://img.shields.io/badge/OS-Linux-informational?style=flat&logo=linux&logoColor=white&color=2bbc8a" alt="Linux" />
<img src="https://img.shields.io/badge/OS-Windows-informational?style=flat&logo=windows&logoColor=white&color=2bbc8a" alt="Windows" />

## Démarrage

1. **Cloner le Dépôt :**

```bash
git clone https://github.com/Sudo-Rahman/Fractalium.git
```

2. **Compilation :**

```bash
mkdir build
```

```bash
cd build
```

```bash
cmake ..
```

```bash
make
```

3. **Exécution :**

Pour éxecuter en local :

```bash
mpirun -np nombre_de_threads ./Fractalium
```

Pour éxecuter sur un cluster :

```bash
mpirun -hostfile <fichier_hosts> ./Fractalium
```

## Démonstration

https://github.com/Sudo-Rahman/Fractalium/assets/85690773/1ca63c2c-5538-4362-8027-4b41f98bddbc

## Instantané

Importer l'instatané sample.fractalium dans le programme et visualiser pas moins de 10 zoom sur l'ensemble de Mandelbrot.

## Auteurs

Ce projet a été réalisé par:

- [Sudo-Rahman](https://github.com/Sudo-Rahman)
- [Maxime-Cllt](https://github.com/Maxime-Cllt)


