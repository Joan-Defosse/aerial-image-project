////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <time.h>
#include "../head/Image.h"

Image::Image(int w, int h) {

     assert(w >= 1 && h >= 1);

     width = w;
     height = h;

     pixelTab.resize(height); // pixelTab étant un vector de taille non définie à l'avance, cette instruction initialise sa taille.

     for (int i = 0; i < height; ++i) {

          pixelTab[i].resize(width); // De même pour chaque sous-vecteur de pixelTab.

          for (int j = 0; j < width; ++j) {

               pixelTab[i][j] = Color(); // Attribue par défaut la couleur noire.
          }
     }
}

Image::Image(const Image& img) {

     width = img.getWidth();
     height = img.getHeight();
     
     pixelTab.resize(height);

     for (int i = 0; i < height; ++i) {

          pixelTab[i].resize(width);

          for (int j = 0; j < width; ++j) {

               pixelTab[i][j] = img.getPixel(i, j);
          }
     }
}

Image::~Image() {

     pixelTab.clear(); // Le vecteur est vidé de ses éléments mais prend toujours de la place en mémoire !
     pixelTab.shrink_to_fit(); // Le vecteur ne peut être désalloué mais son espace mémoire attribué est désormais de 0.
}

bool Image::isValidCoordinate(int i, int j) const {

     return (0 <= i) && (i < height) && (0 <= j) && (j < width);
}

int Image::getWidth() const {

     return width;
}

int Image::getHeight() const {

     return height;
}

int Image::getSize() const {

     return height * width;
}

Color Image::getPixel(int i, int j) const {

     assert(isValidCoordinate(i, j));

     return pixelTab[i][j];
}

void Image::setPixel(int i, int j, Color col) {
     
     assert(isValidCoordinate(i, j));

     pixelTab[i][j] = col;
}

void Image::fill(Color col) {

     for (int i = 0; i < height; ++i) {

          for (int j = 0; j < width; ++j) {

               pixelTab[i][j] = col;
          }
     }
}

void Image::fillRectangle(int i1, int i2, int j1, int j2, Color col) {

     assert(isValidCoordinate(i1, j1) && isValidCoordinate(i2, j2));

     for (int i = i1; i <= i2; ++i) {
          
          for (int j = j1; j <= j2; ++j) {

               pixelTab[i][j] = col;
          }
     }
}

// Le pixel de coordonnées (i, j) est le pixel numéro k = i*w + j.
int Image::toIndex(int i, int j) const {

     assert(isValidCoordinate(i, j));

     return width * i + j;
}

// Le pixel k est le pixel de coordonées (i, j) avec k = i*w + j.
pair <int, int> Image::toCoordinate(int k) const {

     assert(k >= 0);

     int i = k / width;
     int j = k % width;

     return make_pair(i,j);
}

// On compare ici la largeur, la hauteur des deux images, ainsi que chacun de leurs pixels.
bool Image::operator==(const Image &img) const {

     if (width != img.getWidth()) return false;
     if (height != img.getHeight()) return false;

     for (int i = 0; i < height; ++i) {

          for (int j = 0; j < width; ++j) {

               if (pixelTab[i][j] != img.getPixel(i, j)) return false;
          }
     }

     return true;
}

bool Image::operator!=(const Image &img) const {

     if (width != img.getWidth()) return true;
     if (height != img.getHeight()) return true;

     for (int i = 0; i < height; ++i) {

          for (int j= 0; j < width; ++j) {

               if (pixelTab[i][j] != img.getPixel(i, j)) return true;
          }
     }

     return false;
}

// Des pixels consécutifs sont des pixels qui se touchent par un de leurs 4 bords.
bool Image::areConsecutivePixels(int i1, int j1, int i2, int j2) const {

     if (!isValidCoordinate(i1, j1) || !isValidCoordinate(i2, j2)) return false;

     bool sameRow = false;
     bool sameColumn = false;
     bool consecutiveRow = false;
     bool consecutiveColumn = false;

     if (i1 == i2) sameRow = true;
     if (j1 == j2) sameColumn = true;
     if ((i1 - 1) == i2 || (i1 + 1 == i2)) consecutiveRow = true;
     if ((j1 - 1) == j2 || (j1 + 1 == j2)) consecutiveColumn = true;

     return (sameRow && consecutiveColumn) || (sameColumn && consecutiveRow);
}

Image makeRandomImage(int w, int h) {

     srand(time(nullptr)); // Cette instrucion permet d'avoir des valeurs aléatoires différentes à chaque exécution.

     Image img = Image(w,h); // On génère une image noire.

     for (int i = 0; i < img.getHeight(); ++i) {

          for (int j = 0; j < img.getWidth(); ++j) {

               // On fait appel aux méthodes de la classe Color pour choisir aléatoirement la couleur.
               Color col = Color::makeColor(rand() % Color::nbColors());

               img.setPixel(i, j, col);
          }
     }

     return img;
}

Image Image::readAIP(const string& filename) {

     ifstream file; // Objet qui récupèrera le contenu du fichier.aip.
     int w, h; // w et la largeur et h la hauteur de l'image à créer.
     string s; // Cette chaîne va permettre d'intéragir (et de stocker) ligne par ligne avec le contenu de file.
     Color col;

     file.open(filename + ".aip"); // L'objet file est chargé en mémoire et récupère le contenu de filename.aip.

     if (!file) throw runtime_error("error open file (read AIP)");

     file >> w >> h; // On récupère les valeurs numériques de w et h, séparées par un espace sur la première ligne de file.

     Image img = Image(w, h);

     getline(file, s); // Récupère temporairement la première ligne de file dans s, nécessaire pour passer aux lignes suivantes.

     s.resize(img.getWidth()); // Optimise la taille de s pour la suite.

     for (int i = 0; i < img.getHeight(); ++i) {

          getline(file, s); // s récupère toutes les lignes de file une par une.

          for (int j = 0; j < img.getWidth(); ++j) {

               // Pour convertir le caractère récupéré en entier facilement, on soustrait 
               // à sa valeur celle du caractère '0'. On convertit la valeur en couleur.
               col = Color::makeColor(s.at(j) - '0');

               img.setPixel(i, j, col); // Affectation des couleurs de img.
          }
     }

     file.close(); // Destruction de l'objet file.
     s.clear(); // Vide s.
     s.shrink_to_fit(); // Réduit l'empreinte mémoire de s à 0.

     return img;
}

void Image::writeAIP(const string& filename) const {

     ofstream file; // Objet dont le contenu sera déposé dans le fichier .aip.
     file.open(filename + ".aip"); // file est chargé en mémoire et se lie au fichier filename.aip. Le crée s'il n'existe pas.

     if (!file) throw runtime_error("error open file (write AIP)");

     file << getWidth()  // Insère les dimensions de l'image dans la première ligne de file.
          << " " 
          << getHeight()
          << endl;

     // Insère à leurs positions, ligne par ligne et colonne par colonne, les valeurs numériques
     // correspondant aux couleurs de chaque pixel de l'image.
     for (int i = 0; i < getHeight(); ++i) { 

          for (int j = 0; j < getWidth(); ++j) {

               file << getPixel(i,j).toInt();
          }

          file << endl;
     }

     file.close(); // Détruit l'objet file après avoir inséré son contenu dans la cible filename.aip.
}

void Image::writeSVG(const string& filename, int pixelSize) const {

     assert(pixelSize > 0);

     ofstream file;
     file.open(filename + ".svg");

     if (!file) throw runtime_error("error open file (write SVG)");

     file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          << endl
          << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\""
          << pixelSize*getWidth()
          << "\" height=\""
          << pixelSize*getHeight()
          << "\">"
          << endl;

     for (int i = 0; i < getHeight(); ++i) {

          for (int j = 0; j < getWidth(); ++j) {

               file << "<rect width=\""
                    << pixelSize
                    << "\" height=\""
                    << pixelSize
                    << "\" x=\""
                    << pixelSize*(j)
                    << "\" y=\""
                    << pixelSize*(i)
                    << "\" fill=\""
                    << getPixel(i, j)
                    << "\" />"
                    << endl;
          }
     }

     file << "</svg>"
          << endl;

     file.close();
}