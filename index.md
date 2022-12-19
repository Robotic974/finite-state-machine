---
layout: default
title: Implémentation d’une Machine à États Finis
---

# {{ page.title }}

<span class="github"></span> [Code source][src]{: target="_blank" }
{: .repo }

<div class="robotic974"></div>


## Aperçu global

- [Objectif de l'atelier](#purpose)
- [Cartes de développement](#dev-boards)
- [Réalisation du circuit](#wiring)
- [Configuration du projet PlatformIO](#pio)
- [Guide d'implémentation](#guide)
- [Solution et code source complet](#source)


<a name="purpose"></a>

## Objectif de l'atelier

L'objectif de cet atelier est de programmer un petit jeu de réflexe sur une carte Arduino, orchestré par une ***machine à états finis***, que l'on nomme également un ***automate fini***. Chaque joueur dispose d'un bouton poussoir et d'une LED témoin. Le jeu consiste à appuyer sur le bouton pour allumer la LED en étant plus rapide que son adversaire. Le top départ est donné par un flash des deux LEDs, après une période d'attente pendant laquelle les LEDs clignotent alternativement. Si un joueur appuie sur son bouton avant le top départ, il perd la partie. Le vainqueur est signalé par l'allumage de sa LED pendant 2 secondes.

<video autoplay loop muted preload="auto" class="media media-50 round-8 shadow">
    <source src="{{ 'assets/videos/demo.mp4' | relative_url }}" type="video/mp4">
</video>

Démonstration du fonctionnement du jeu de réflexe
{: .caption }


<a name="dev-boards"></a>

## Cartes de développement

J'utilise ici une carte [Arduino Nano][nano], mais vous pouvez tout à fait réaliser le même montage avec une carte [Arduino Uno][uno]. Le raccordement des LEDs et des boutons sur les broches de la carte Arduino est exactement le même.

<div class="boards round-8 shadow">
    <div class="board uno"></div>
    <div class="board nano"></div>
</div>

Cartes Arduino Uno (à gauche) et Nano (à droite)
{: .caption }


<a name="wiring"></a>

## Réalisation du circuit

![Câblage du circuit][wiring]{: .media .round-8 .shadow }

Câblage du circuit
{: .caption }

### Brochage de la carte Arduino

- **A3** : broche de lecture du bouton 1
- **A5** : broche de commande de la LED 1
- **D2** : broche de lecture du bouton 2
- **D4** : broche de commande de la LED 2

Chaque LED est connectée en série avec une résistance de 220 Ω pour limiter l'intensité du courant qui la traverse, et chaque bouton est associé à une résistance de rappel ***pull-down*** de 10 kΩ pour stabiliser la lecture du signal à l'état `LOW` lorsqu'il est relâché.


<a name="pio"></a>

## Configuration du projet PlatformIO

L'organisation de votre projet PlatformIO est *grosso-modo* la suivante :

```sh
finite-state-machine   # le dossier du projet
├── platformio.ini     # le fichier de configuration du projet
└── src                # le dossier des codes sources
    ├── main.cpp       # le programme principal
    └── ...            # les éventuels codes sources annexes
```

Voici les directives à inscrire dans votre fichier de configuration `platformio.ini` :

```ini
[env:game]
platform  = atmelavr
board     = nanoatmega328new  ; pour une carte Arduino Nano récente  [OptiBoot]
; board   = nanoatmega328     ; pour une carte Arduino Nano obsolète [ATmegaBOOT]
; board   = uno               ; pour une carte Arduino Uno
framework = arduino
```


<a name="guide"></a>

## Guide d'implémentation

### Définition des états de l'automate

L'idée générale, pour implémenter un automate fini, est de définir **l'ensemble des états** qui le caractérisent :

```cpp
enum class State : uint8_t {
    RESET,     // (Ré)initialisation de la partie
    GET_READY, // Période d'attente du top départ
    PLAY,      // Phase compétitive du jeu
    STOP       // Suspension de la partie après affichage du vainqueur
};
```

Puis de déclarer une variable `state`, qui va permettre de déterminer l'état **courant** de l'automate :

```cpp
State state;
```

### Architecture globale du programme

Le programme principal s'articule alors de la manière suivante :

```cpp
void reset()    {} // Routine à exécuter dans l'état RESET
void getReady() {} // Routine à exécuter dans l'état GET_READY
void play()     {} // Routine à exécuter dans l'état PLAY
void stop()     {} // Routine à exécuter dans l'état STOP

void setup() {     // Initialisation du programme

    // Affectation de l'état initial de l'automate
    state = State::RESET;

}

void loop() {      // Boucle de contrôle principal du programme

    switch (state) {

        case State::RESET:     reset();    break;
        case State::GET_READY: getReady(); break;
        case State::PLAY:      play();     break;
        case State::STOP:      stop();

    }

}
```

### Interface de contrôle de chaque joueur

Pour simplifier et améliorer la lisibilité du code principal, il peut être judicieux de définir une classe `Player` pour implémenter l'interface entre le joueur et le micro-contrôleur. Voici la déclaration primitive de cette classe, qui doit être inscrite dans un fichier d'en-tête nommé `Player.h` :

```cpp
#pragma once

#include <Arduino.h>

class Player {

    public:

        /**
         * @brief Le constructeur de la classe Player
         * 
         * @param btn_pin la broche de lecture du bouton
         * @param led_pin la broche de commande de la LED
         */
        Player(uint8_t const btn_pin, uint8_t const led_pin);

        /**
         * @brief Lecture du signal provenant du bouton
         */
        void read();

        /**
         * @brief Détermine si le bouton est enfoncé ou pas
         * 
         * @return true  si le bouton est enfoncé
         *         false dans le cas contraire
         */
        bool pressed();

        /**
         * @brief Allume la LED
         */
        void lightOn();

        /**
         * @brief Éteint la LED
         */
        void lightOff();

    private:

        uint8_t btn_pin;   // Broche de lecture du bouton
        uint8_t led_pin;   // Broche de commande de la LED
        uint8_t btn_state; // État courant du bouton (LOW / HIGH)
        uint8_t led_state; // État courant de la LED (LOW / HIGH)

};
```

Il vous appartiendra de compléter éventuellement cette déclaration avec les éléments que vous jugerez utiles au bon fonctionnement du programme. Vous devrez également écrire le fichier d'implémentation `Player.cpp` qui définit précisément le fonctionnement de chaque méthode de la classe.

L'organisation de votre projet devrait alors être la suivante :

```
finite-state-machine
├── platformio.ini
└── src
    ├── main.cpp
    ├── Player.h
    └── Player.cpp
```

### Squelette du programme principal

Sur la base des éléments évoqués en préambule, on peut donc ébaucher le code source du programme principal `main.cpp`. Pour vous mettre le pied à l'étrier, j'ai implémenté les définitions complètes des fonctions `play()` et `loop()` pour vous montrer comment utiliser l'interface publique de la classe `Player` :

```cpp
#include <Arduino.h>
#include "Player.h"

// Définition de l'ensemble des états de l'automate
enum class State : uint8_t {
    RESET,
    GET_READY,
    PLAY,
    STOP
};

// Déclaration de l'état courant de l'automate
State state;

// Définition des interfaces de chaque joueur
Player player1(A3,  A5);
Player player2(DD2, DD4);

/**
 * @brief Routine à exécuter lorsque l'automate est à l'état RESET
 */
void reset() {

}

/**
 * @brief Routine à exécuter lorsque l'automate est à l'état GET_READY
 */
void getReady() {

}

/**
 * @brief Routine à exécuter lorsque l'automate est à l'état PLAY
 */
void play() {

         if (player1.pressed()) { player1.lightOn(); state = State::STOP; }
    else if (player2.pressed()) { player2.lightOn(); state = State::STOP; }

}

/**
 * @brief Routine à exécuter lorsque l'automate est à l'état STOP
 */
void stop() {

}

/**
 * @brief Initialisation du programme
 */
void setup() {

    state = State::RESET;

}

/**
 * @brief Boucle de contrôle principal du programme
 */
void loop() {

    player1.read();
    player2.read();

    switch (state) {

        case State::RESET:     reset();    break;
        case State::GET_READY: getReady(); break;
        case State::PLAY:      play();     break;
        case State::STOP:      stop();

    }

}
```


<a name="source"></a>

## Solution et code source complet

Lorsque vous aurez terminé cet exercice et obtenu un programme fonctionnel, ou si vous vous arrachez les cheveux sans être parvenu au résultat attendu, vous pourrez aller jeter un coup d'oeil sur le code source de la solution que je vous propose.  :wink:

[Accéder au code source de la solution][src]{: .button target="_blank" }
{: .center }


[src]:    https://github.com/Robotic974/finite-state-machine
[nano]:   https://docs.arduino.cc/hardware/nano
[uno]:    https://docs.arduino.cc/hardware/uno-rev3
[wiring]: assets/images/wiring.jpg