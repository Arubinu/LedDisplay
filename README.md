LedDisplay
====================

Projet Arduino regroupant 6 panneaux de 16x16 leds faisant défiler une phrase.

![Résultat](/small.jpg?raw=true "Résultat")

## Ajouter le sketch dans Arduino

Sous Windows:
 - Rendez-vous dans Documents > Arduino, déposez le dossier LedDisplay.
 - Dans ce même dossier allez dans "librairies" et déposez Timer, DateTime, ainsi que LedMatrix.

Sous Linux:
 - Rendez-vous dans Dossier personnel > Arduino, déposez le dossier LedDisplay.
 - Dans ce même dossier allez dans "librairies" et déposez Timer, DateTime, ainsi que LedMatrix.

## Utilisation

Connectez votre Arduino à l'ordinateur.
Dans l'application Arduino, sélectionner le sketch LedDisplay.

Il vous fait maintenant, soit précisez vos propres pins (dans LedDisplay.h l25 à l30), soit relié les panneaux 16x16 comme suit :
 - 14, 15, 16, 17, 18, 19, 20, 21
 - 9, 8, 7, 6, 5, 4, 3, 2
 - 23, 25, 27, 29, 31, 33, 35, 37
 - 39, 41, 43, 45, 47, 49, 51, 53
 - A0, A1, A2, A3, A4, A5, A6, A7
 - A8, A9, A10, A11, A12, A13, A14, A15

Dans l'ordre: D, C, B, A, G, DI, CLK, LAT.
Vous pouvez bien entendu choisir un nombre moins important d'afficheur en fonction de votre matériel.

Il ne vous reste plus qu'à transférer le sketch sur votre board.
Pour envoyer une phrase à l'arduino, ouvre le moniteur série et envoyez directement ce que vous souhaitez afficher.

## Fonctionnalitées supplémentaires

En envoyer "date=1384504920" vous activerez l'affichage de la date à laquelle l'école 42 ( à Paris ) a ouverte ses portes.
Vous pouvez préciser un autre timestamp UTC ( via par exemple www.timestamp.fr ).
Toutefois, il faut obligatoirement 10 chiffres ( ou changer son comportement dans le fichier LedDisplay.h ).

## Notes

La librairie LedMatrix a été modifié pour aléger le temps de calcul.
Cette modification est activée grace à la variable Mapping de celle-ci.
Elle n'est disponible que sur les Arduinos possédant les ATmega 168, 168P, 328P, 1280, et 2560.
Pour une compatibilitée supplémentaire, n'hésitez pas à me contacter.

Les calculs étants très important pour 5 afficheurs, le panneau peut scintiller...