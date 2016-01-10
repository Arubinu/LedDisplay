LedDisplay
====================

Projet Arduino regroupant 6 panneaux de 16x16 leds faisant défiler une phrase.

![Résultat](/small.jpg?raw=true "Résultat")

## Ajouter le sketch dans Arduino

Sous Windows:
 - Rendez-vous dans Documents > Arduino, déposez le dossier LedDisplay.
 - Dans ce même dossier allez dans "librairies" et déposez Timer ainsi que LedMatrix.

## Utilisation

Connectez votre Arduino Mega à l'ordinateur.
Dans l'application Arduino, sélectionner le sketch LedDisplay.

Il vous fait maintenant, soit précisez vos propres pins (ligne 9 à 14), soit relié les panneaux 16x16 comme suit :
 - 9, 8, 7, 6, 5, 4, 3, 2
 - 23, 25, 27, 29, 31, 33, 35, 37
 - 39, 41, 43, 45, 47, 49, 51, 53
 - A0, A1, A2, A3, A4, A5, A6, A7
 - A8, A9, A10, A11, A12, A13, A14, A15

Dans l'ordre: D, C, B, A, G, DI, CLK, LAT

Il ne vous reste plus qu'à transférer le sketch sur votre board.
Pour envoyer une phrase à l'arduino, ouvre le moniteur série et envoyez directement ce que vous souhaitez afficher.

## Notes

Cet exemple fonctionne seulement sur Arduino Mega (car il demande un nombre important de pins (mais il est possible de passer par des shifts register pour palier le problème).
Les calculs étants très important, le panneau scintille...
