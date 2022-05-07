////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#ifndef FIRE_SIMULATOR_H
#define FIRE_SIMULATOR_H

#include <vector>
#include <set>
#include "Image.h"

// Représente un feu sur le pixel k, allumé lors de l'étape lightTime.
struct Fire {

    // Pixel incendié.
    int k;

    // Naissance de l'incendie sur ce pixel.
    int lightTime;

    // Définition de l'opérateur '<' sur le type Fire. Nécessaire pour
    // pouvoir utiliser le type std::set <Fire>.
    bool operator<(const Fire other) const {

        return k < other.k;
    }
};

////////////////////////////////////////////////////////////////////////////////
/// This simule l'expérience d'un feu de forêt.
///
/// À partir d'un pixel de forêt d'une image, un feu est simulé dans la zone
/// de forêt de ce pixel.
////////////////////////////////////////////////////////////////////////////////
class FireSimulator {

public:

    // Prépare les données pour une simulation d'incendie sur l'image img, dans la zone
    // de forêt du pixel k.
    FireSimulator(Image& img, int k);
    
    // Prépare les données pour une simulation d'incendie sur l'image img, dans la zone
    // de forêt du pixel de coordonnées (i,j).
    FireSimulator(Image& img, int i, int j);

    // Destructeur, désalloue la mémoire.
    ~FireSimulator();

    // Fais avancer la simulation de n étapes.
    // Produit les fichier .aip correspondant à chaque étape.
    // Retourne les images de chacune des étapes effectuées.
    vector <Image> runSimulator(int n);

    // Fais avancer la simulation d'une étape.
    void nextStage();

    // Retourne l'image de la simulation à l'étape courante. 
    Image getImage();

    // Retourne l'étape courante.
    int getTime();

private:

    // Repère temporel sur l'état de la simulation. Commence à 0 et s'incrémente à chaque étape.
    int experienceTime;

    // Une copie modifiable de l'image de départ de la simulation.
    Image* currentImg;

    // Définit la zone de forêt dans laquelle l'incendie se déclare. Il ne peut se propager en dehors.
    set <int> limitZone;

    // Définit l'ensembe des pixels de cendres, issues d'un incendie qui s'est éteint. Un feu ne peut pas s'y déclarer.
    set <int> dustZone;

    // Définit la zone incendiée. Chaque pixel dans cette zone y est pour une durée temporaire (3 temps).
    set <Fire> fireZone;

    ////////////////////////////////////////////////////////////////////////////////

    // Détermine aléatoirement l'emplacement du départ de feu parmi les pixels de limitZone.
    // L'ajoute à fireZone. Appelée une fois au début de l'expérience.
    void lightFire();

    // Vérifie quels feux doivent être éteints, les retire de fireZone et les ajoute à dustzone.
    // Appelée à chaque stade de l'expérience à partir du 3e temps. 
    void extinguishFire();

    // Étend la zone de feu en définissant aléatoirement quels pixels, parmi
    // ceux présents dans riskZone, sont ajoutés à fireZone.
    void spreadFire(vector <int> & riskZone);

    // Incrémente experienceTime et actualise l'image courante.
    void runTime();

    // Actualise l'image courante à partir de fireZone et dustZone.
    void refreshImage();

    // Vrai si le pixel de coordonnées (i2, j2) est une zone de forêt qui touche une zone de flammes.
    bool isUnsafe(int i1, int j1, int i2, int j2);

    // Liste de tous les pixels de forêt en contact avec un pixel de flammes.
    vector <int> unsafeList();
};

#endif
