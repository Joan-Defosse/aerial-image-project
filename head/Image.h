////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_H
#define IMAGE_H

#include <utility>
#include <string>
#include <vector>
#include "Color.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
/// This est une image rectangulaire colorée.
////////////////////////////////////////////////////////////////////////////////
class Image {
  
public:

  /// Crée une image rectangulaire noire de dimensions w*h pixels.
  /// w est la largeur de l'image (width), h est sa hauteur (height).
  Image(int w, int h);

  /// Constructeur qui fait de this une copie de img. 
  Image(const Image& img);

  /// Destructeur par défaut.
  ~Image();

  /// Interdit l'affectation d'image.
  Image& operator=(const Image& img) = delete;

  /// Retourne la largeur (width) de this.
  int getWidth() const;

  /// Retourne la hauteur (height) de this.
  int getHeight() const;

  /// Retourne le nombre de pixels de this.
  int getSize() const;

  /// Retourne la couleur du pixel de la ligne i et de la colonne j.
  /// Précondition : 0 <= i < height et 0 <= j < width.
  Color getPixel(int i, int j) const;

  /// Insère la couleur col dans le pixel de coordonnées (i,j).
  /// Précondition : 0 <= i < height et 0 <= j < width.
  void setPixel(int i, int j, Color col);

  /// Retourne le numéro k du pixel de coordonnées (i, j).
  int toIndex(int i, int j) const;

  /// Retourne les coordonnées (i,j) du pixel numéro k.
  /// Dans le réusltat p, p.first est la ligne i et p.second est la colonne j. 
  pair <int, int> toCoordinate(int k) const;

  /// Remplit this de la couleur col.
  void fill(Color col);

  /// Remplit un rectangle, partie de this, de la couleur col.
  /// (i1, j1) est le coin supérieur gauche.
  /// (i2, j2) est le coin inférieur gauche.
  /// Précondition : (i1,j1) et (i2,j2) sont des coordonnées valides.
  void fillRectangle(int i1, int j1, int i2, int j2, Color c);
  
  /// Génère une image au format SVG d'après un nom de fichier sans extension.
  /// Le nom du fichier génér est 'filename.svg'.
  /// Chaque pixel est représenté par un carré de côté pixelSize.
  /// Renvoie une exception runtime_error si une erreur survient.
  void writeSVG(const string& filename, int pixelSize) const;

  /// Sauvegarde this dans un fichier texte d'un format spécifique :
  ///   - la largeur puis la hauteur de this en première ligne;
  ///   - suivies des lignes de this de manière à ce que chaque numéro
  ///     corresponde à la couleur de son pixel.
  /// Un exemple de sortie serait :
  ///   5 3
  ///   10014
  ///   01233
  ///   23101
  /// Le fichier en sortie est nommé 'filename.aip'.
  /// Renvoie une exception runtime_error si une erreur survient.
  void writeAIP(const string& filename) const;

  /// Crée une image à partir d'un fichier AIP.
  /// Le nom du fichier doit être donné sans son extension.
  /// Renvoie une exception runtime_error si une erreur survient.
  static Image readAIP(const string& filename);

  /// Retourne vrai si this et img sont égales.
  bool operator==(const Image& img) const;

  /// Retourne vrai si this et img sont différentes.
  bool operator!=(const Image& img) const;

  /// Retourne vrai si (i1, j1) et (i2, j2) sont deux pixels consécutifs et qui appartiennent à this.
  bool areConsecutivePixels(int i1, int j1, int i2, int j2) const;

private:

  /// height est la hauteur de this et width en est la largeur.
  int height, width;

  /// pixelTab est un tableau 2D de couleurs, représentant les pixels de l'image. 
  vector <vector <Color>> pixelTab;

  ////////////////////////////////////////////////////////////////////////////////
  
  /// Teste si (i,j) sont les coordonnées d'un pixel de this.
  bool isValidCoordinate(int i, int j) const;
};

/// Génère une image de largeur w et de hauteur h tout en attribuant des couleurs aléatoires aux pixels de this.
Image makeRandomImage(int w, int h);

#endif
