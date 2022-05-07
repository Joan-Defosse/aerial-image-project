////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iostream>
#include "../head/FireSimulator.h"

using namespace std;

int main(void) {

  /* Tests sur les différentes façon de construire une image. */
  Image readImg = Image::readAIP("images/amazonie_0");
  Image copyImg(readImg);
  Image randomImg = makeRandomImage(10,10);

  /* Vérification du bon fonctionnement de la méthode writeAIP
  * sur les images précédemment créées. */
  readImg.writeAIP("images/readTest");
  copyImg.writeAIP("images/copyTest");
  randomImg.writeAIP("images/randomTest");

  /* Vérification du bon fonctionnement de la méthode writeSVG
  * sur les images précédemment créées. */
  readImg.writeSVG("svg/readTest", 20);
  copyImg.writeSVG("svg/copyTest", 20);
  randomImg.writeSVG("svg/randomTest", 20);

  /* Prépare la simulation d'un incendie dans la zone de forêt
  * du premier pixel de l'image. */
  FireSimulator f(readImg, 0, 0);

  /* Simule 7 étapes d'un incendie, crée 8 fichiers image'i'.aip
  * et retourne les 8 images correspondantes. */
  vector <Image> tab = f.runSimulator(7);


  /* La suite transforme en fichiers .svg les images de la simulation recueillies. */
  
  int i = 0;

  for (Image sim : tab) {

    string name = ("svg/image" + to_string(i));
    sim.writeSVG(name, 20);
    ++i;
  }

  cout << "Fin du programme !\nVous retrouverez les images de la simulation dans le dossier svg.\n";

  return 0;
}
