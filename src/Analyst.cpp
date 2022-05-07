////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "../head/Analyst.h"

Analyst::Analyst(const Image& img) {

    nbElem = img.getSize();
    zones = nbElem; // Il y a, au départ, autant de parties que de pixels.
    pImg = &img;
    pixelsPerColor = initZero();
    zonesPerColor = pixelsPerColor; // Toutes les couleurs ont 0 pixel dans la partition à ce stade.
    
    initPart();

    UnionZones();
}

Analyst::~Analyst() {

    pixelsPerColor.clear();
    zonesPerColor.clear();
    part.clear(); // Le vecteur est vidé de ses éléments mais prend toujours de la place en mémoire !

    pixelsPerColor.shrink_to_fit();
    zonesPerColor.shrink_to_fit();
    part.shrink_to_fit(); // Le vecteur ne peut être désalloué mais son espace mémoire attribué est désormais de 0.

    pImg = NULL; // On ne veut pas supprimer l'image mais seulement désallouer le pointeur.

    delete pImg;
}

vector <int> Analyst::initZero() {

    vector <int> v;

    v.resize(Color::nbColors());

    vector <int>::iterator it;

    for(it = v.begin(); it != v.end(); ++it) {

        *it = 0;
    }

    return v;
}

void Analyst::initPart() {

    part.resize(nbElem);

    for (int k = 0; k < nbElem; ++k) {

        part[k] = new list <int>;

        part[k]->push_front(k); // Chaque liste chaînée, à sa position k, contient l'unique élément k.

        int i = pImg->toCoordinate(k).first;
        int j = pImg->toCoordinate(k).second;

        Color col = pImg->getPixel(i, j); // On obtient la couleur du pixel k de coordonnées (i,j).

        ++pixelsPerColor[col.toInt()];
        ++zonesPerColor[col.toInt()]; // Il y a, à ce stade, autant de parties d'une couleur que de pixels de cette couleur.
    }
}

void Analyst::UnionZones() {

    for (int i = 0; i < pImg->getHeight(); ++i) {

        for (int j = 0; j < pImg->getWidth(); ++j) {

            Union(i, j, i+1, j); // Fusionne le pixel de coordonnées (i,j) et son voisin de droite si nécessaire.
            Union(i, j, i, j+1); // Fusionne le pixel de coordonnées (i,j) et son voisin du dessous si nécessaire.
        }
    }
}

void Analyst::Union(int i1, int j1, int i2, int j2) {

    // Les pixels sont voisins, appartiennent tous deux à l'image et à des zones différentes.
    if (pImg->areConsecutivePixels(i1, j1, i2, j2) && !belongToTheSameZone(i1, i2, j1, j2)) {

        Color col = pImg->getPixel(i1, j1);
        Color col2 = pImg->getPixel(i2, j2);

        // Les pixels sont de même couleur.
        if (col == col2) {

            int k1 = pImg->toIndex(i1, j1);
            int k2 = pImg->toIndex(i2, j2);

            Union(k1, k2); // Fusion de la liste contenant l'élément k1 et de celle contenant k2.

            // La fusion de deux parties entraîne la décrémentation du nombre de parties.
            --zones;
            --zonesPerColor[col.toInt()];
        }
    }
}

void Analyst::Union(int i, int j) {

    // Pour des raisons d'optimisation, on insère toujours la liste la plus petite dans la liste la plus grande.
    if (part[i]->size() < part[j]->size()) {

        return Union(j, i);
    }

    // La liste contenant j est détruite après avoir été vidée de ses éléments au début de la liste contenant i. 
    list <int>::iterator it = part[i]->begin();
    part[i]->splice(it, *part[j]);

    // Chaque élément de l'ancienne liste contenant j pointe désormais sur la liste contenant i.
    for (list <int>::iterator jt = part[i]->begin(); jt != it; ++jt) {

        part[*jt] = part[i];
    }
}

int Analyst::Find(const int i, const int j) {

    return Find(pImg->toIndex(i,j));
}

int Analyst::Find(const int k) {

    return part[k]->front(); // Le représentant du pixel k est le premier élément de la liste le contenant.
}

bool Analyst::belongToTheSameZone(int i1, int i2, int j1, int j2) {

    // Deux pixels de même zone font partie de la même liste, et ont donc le même représentant (premier élément).
    return Find(i1, j1) == Find(i2, j2);
}

int Analyst::nbZones() const {

    return zones;
}

int Analyst::nbPixelsOfColor(Color col) const {

    return pixelsPerColor[col.toInt()];
}

int Analyst::nbZonesOfColor(Color col) const {

    return zonesPerColor[col.toInt()];
}

Image Analyst::fillZone(int i, int j, Color col) {

    // Rien n'est à faire dans le cas où la zone est déjà de la bonne couleur.
    if (pImg->getPixel(i, j) == col) {

        return *pImg;
    }

    // Création d'une nouvelle image par copie de l'actuelle.
    Image img = Image(*pImg);

    int k = img.toIndex(i, j);

    // Pour chaque élément apartenant à la même zone que le pixel k de coordonnées (i,j),
    // le pixel correspondant est colorié de la couleur col.
    for (list <int>::const_iterator it = part[k]->begin(); it != part[k]->end(); ++it) {

        i = img.toCoordinate(*it).first;
        j = img.toCoordinate(*it).second;

        img.setPixel(i, j, col);
    }

    return img;
}

set <int> Analyst::zoneOfPixel(int i, int j) {

    set <int> s;

    int k = pImg->toIndex(i,j);

    // Chaque élément de la liste contenant le pixel k, de coordonnées (i,j), est inséré dans l'ensemble s.
    for (list <int>::const_iterator it = part[k]->begin(); it != part[k]->end(); ++it) {

        s.insert(*it);
    }

    return s;
}