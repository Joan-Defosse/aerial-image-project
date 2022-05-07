////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#ifndef ANALYST_H
#define ANALYST_H

#include <set>
#include <list>
#include "Image.h"

////////////////////////////////////////////////////////////////////////////////
/// This est une analyse d'image sous forme de partition.
///
/// Une analyse permet de mettre en évidence les différentes zones d'une image.
/// Une zone est un groupe de pixels de même couleurs reliés entre eux.
////////////////////////////////////////////////////////////////////////////////
class Analyst {
  
public:

  /// Démarre l'analyse d'une image donnée.
  Analyst(const Image& img);

  /// Interdit la copie d'analyses.
  Analyst(const Analyst&) = delete;

  /// Interdit l'affectation d'analyses.
  Analyst& operator=(const Analyst&) = delete;

  /// Destructeur par défaut.
  ~Analyst();

  /// Teste si les pixels de coordonnées (i1, j1) et (i2, j2) de l'image
  /// analysée font partie d'une même zone.
  bool belongToTheSameZone(int i1, int j1, int i2, int j2);

  /// Retourne le nombre de pixels d'une couleur donnée dans l'image analysée.
  int nbPixelsOfColor(Color c) const;

  /// Retourne le nombre de zones d'une couleur donnée dans l'image analysée.
  int nbZonesOfColor(Color c) const;

  /// Retourne le nombre de zones de l'image analysée.
  int nbZones() const;

  /// Crée une nouvelle image à partir de celle en analyse, en remplissant la zone du pixel
  /// de coordonnées (i,j) d'une nouvelle couleur en entrée.
  Image fillZone(int i, int j, Color c);

  /// Retourne les clés de tous les pixels qui appartiennent à la même zone que celui de coordonnées (i, j).
  set <int> zoneOfPixel(int i, int j);

private:

  // Ce pointeur permet de garder une trace de l'image analysée.
  const Image* pImg;

  // Voici la partition des pixels en zones représentées par chaque sous liste du tableau.
  vector <list <int>*> part;

  // Un tableau dont chaque case contient le nombre d'occurences de la couleur d'identifiant
  // le numéro de la case. Ex : si le nombre 0 représente la couleur Black, alors la case 0 du
  // tableau contient le nombre de pixels noirs (Black) de l'image analysée.
  vector <int> pixelsPerColor;
  
  // Un tableau dont chaque case contient le nombre de zones de la couleur d'identifiant
  // le numéro de la case. Ex : si le nombre 0 représente la couleur Black, alors la case 0 du
  // tableau contient le nombre de zones noires (Black) de l'image analysée.
  vector <int> zonesPerColor;

  // Le nombre de pixels de l'image.
  int nbElem;

  // Le nombre de zones de l'image.
  int zones;

  ////////////////////////////////////////////////////////////////////////////////

  // Crée un tableau de taille nbColors et le remplit de 0.
  vector <int> initZero();

  // Initialise la Partition en créant une partie pour chaque pixel.
  void initPart();

  // Finalise la Partition en fusionnant les parties des pixels de même zone.
  void UnionZones();

  // Fusionne les parties des pixels de coordonnées
  // (i1, j1) et (i2, j2) s'ils sont consécutifs et de même couleur.
  void Union(int i1, int j1, int i2, int j2);

  // Fusionne les parties des pixels i et j.
  void Union(int i, int j);

  // Retourne le représentant du pixel de coordonnées (i, j).
  int Find (const int i, const int j);

  // Retourne le représentant du pixel k.
  int Find(const int i);
};

#endif
