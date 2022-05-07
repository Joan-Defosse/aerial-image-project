////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "../head/Analyst.h"
#include "../head/FireSimulator.h"

FireSimulator::FireSimulator(Image& img, int i, int j) {

    // Vérification de la couleur du pixel de la zone où démarre l'incendie.
    assert(img.getPixel(i, j) == Color::Green);

    Image cpImg(img);

    currentImg = &img;

    // Analyse de l'image de départ pour comptabiliser et limiter les zones.
    Analyst a(img);

    // Définition de la zone de forêt dans laquelle se déclare et se propage l'incendie.
    limitZone = a.zoneOfPixel(i, j);

    // Le moment de la simulation est initialisé à 0.
    experienceTime = 0;
}

FireSimulator::FireSimulator(Image& img, int k) {

   int i = img.toCoordinate(k).first;
   int j = img.toCoordinate(k).second;

   FireSimulator(img, i, j);
}

FireSimulator::~FireSimulator() {

    limitZone.clear();
    fireZone.clear();
    dustZone.clear();

    delete currentImg;
}

vector <Image> FireSimulator::runSimulator(int n) {

    assert(n >= 0);

    vector <Image> tab;

    // Ajout de l'image courante avant la moindre modification.
    tab.push_back(getImage());

    string name = ("images/image" + to_string(experienceTime));

    tab[0].writeAIP(name);

    // À chaque étape, on ajoute l'image courante et on crée son fichier .aip.
    for (int i = 1; i <= n; ++i) {

        nextStage();

        tab.push_back(getImage());

        name = ("images/image" + to_string(experienceTime));
        tab[i].writeAIP(name);
    }

    return tab;
}

void FireSimulator::nextStage() {

    // Permet l'utilisation de l'aléatoire.
    srand(time(nullptr));

    if (experienceTime == 0) {

        lightFire();

        runTime();
    } 
    
    else {

        if (experienceTime >= 3) {

            extinguishFire();
        }

        vector <int> riskArea = unsafeList();

        spreadFire(riskArea);

        runTime();
    }
}

void FireSimulator::lightFire() {

    assert(experienceTime == 0);

    Fire f;

    // Définition aléatoire de l'indice du départ de feu.
    int r = rand() % limitZone.size();

    set <int>::const_iterator it = limitZone.begin();

    // On se positionne à l'emplacement définit aléatoirement par r
    for (int i = 0; i < r; ++i) {

        ++it;
    }

    f.k = *it;
    f.lightTime = experienceTime;

    // Ajout du départ de feu à la liste des feux.
    fireZone.insert(f);
}

void FireSimulator::extinguishFire() {

    assert(experienceTime >= 3);

    set<Fire>::iterator it;
    
    // Parmi l'ensemble des pixels enflammés, ceux qui le sont
    // depuis au moins 3 temps doivent laisser place à la cendre.
    for(it = fireZone.begin(); it != fireZone.end(); ++it)  {

        if (experienceTime - it->lightTime == 3) {

            set<Fire>::iterator jt = it--; // Nécessaire pour éviter une erreur de segmentation.

            dustZone.insert(jt->k);

            fireZone.erase(jt);
        }
    }

    // Des modifications impliquent un rechargement d'image.
    refreshImage();
}

void FireSimulator::spreadFire(vector <int> & riskZone) {

    // On ne peut allumer plus de feux qu'il y a de zones à risque.
    int max = riskZone.size();

    // S'il n'y a pas de zone à riqsue à risque, on ne fait rien.
    if (max >= 1) {

        // On détermine aléatoirement le nombre de feux à ajouter,
        // qui est au minimum de 1, et au maximum de max.
        int toAdd = (rand() % max) + 1;

        // Tant qu'il y a des pixels à ajouter dans la zone de feu.
        for (int i = 0; i < toAdd; ++i) {
        
            // On détermine aléatoirement la position du nouveau feu.
            int r = rand() % toAdd;

            // Si une case vaut -1, c'est qu'on a déjà ajouté le pixel correspondant.
            while (riskZone[r] == -1) {

                r = rand() % toAdd;
            }

            // Création du nouveau feu.
            Fire f;
            f.k = riskZone[r];
            f.lightTime = experienceTime;

            // Insertion du nouveau feu.
            fireZone.insert(f);

            // On bloque l'accès au pixel pour ne pas l'ajouter une deuxième fois.
            riskZone[r] = -1;
        }
    }
}

void FireSimulator::runTime() {

    // Si le temps avance, c'est qu'il s'est probablement passé
    // suffisamment de choses pour devoir recharger l'image.
    refreshImage();
    ++experienceTime;
}

void FireSimulator::refreshImage() {

    for (Fire f : fireZone) {

        int i = currentImg->toCoordinate(f.k).first;
        int j = currentImg->toCoordinate(f.k).second;

        // Place sur l'image actuelle le pixel en feu.
        currentImg->setPixel(i, j, Color::Red);
    }

    for (int k : dustZone) {

        int i = currentImg->toCoordinate(k).first;
        int j = currentImg->toCoordinate(k).second;

        // Place sur l'image actuelle le pixel éteint.
        currentImg->setPixel(i, j, Color::Black);
    }
}

vector <int> FireSimulator::unsafeList() {

    // Création du tableau d'entiers tab qui contient les pixels en danger.
    // C'est dans cette liste que seront piochés au hasard les pixels qui s'enflammeront
    // lors de la prochaine étape de la simulation.
    vector <int> tab;

    // On parcourt l'ensemble des pixels déjà enflammés.
    for (Fire f : fireZone) {

        int i = currentImg->toCoordinate(f.k).first;
        int j = currentImg->toCoordinate(f.k).second;

        bool riskOnTop = isUnsafe(i,j,i-1,j);
        bool riskOnBottom = isUnsafe(i,j,i+1,j);
        bool riskAtLeft = isUnsafe(i,j,i,j-1);
        bool riskAtRight = isUnsafe(i,j,i,j+1);
        
        // Si les pixels adjacents ont un risque de s'enflammer, parce que de couleur verte, ils sont ajoutés à tab.
        if (riskOnTop) tab.push_back(currentImg->toIndex(i-1,j));
        if (riskOnBottom) tab.push_back(currentImg->toIndex(i+1,j));
        if (riskAtLeft) tab.push_back(currentImg->toIndex(i,j-1));
        if (riskAtRight) tab.push_back(currentImg->toIndex(i,j+1));
    }

    return tab;
}

bool FireSimulator::isUnsafe(int i1, int j1, int i2, int j2) {

    // Si le pixel en feu touche le pixel (i2,j2) ET s'il (i2,j2) appartient bien à l'image.
    if (currentImg->areConsecutivePixels(i1,j1,i2,j2)) {

        // Retourne vrai si le pixel(i2,j2) est une zone de forêt.
        return currentImg->getPixel(i2,j2) == Color::Green;
    }
    
    return false;
}

Image FireSimulator::getImage() {

    return (*currentImg);
}

int FireSimulator::getTime() {

    return experienceTime;
}
