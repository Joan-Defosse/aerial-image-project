////////////////////////////////////////////////////////////////////////////////
/// Ce fichier appartient au projet Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// Les sources de AIP sont distribuées sans aucune garantie.
////////////////////////////////////////////////////////////////////////////////

#ifndef COLOR_H
#define COLOR_H

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
/// This est une couleur.
///
/// Les couleurs valides sont définies comme des membres constants statiques : Color::Black,
/// Color::White, Color::Red, Color::Blue, Color::Green.
///
/// Voici un exemple :
///
/// Color c = Color::Blue;
/// int id  = c.toInt();
/// Color d = Color::makeColor(id);
/// if (c == d) { ... }
////////////////////////////////////////////////////////////////////////////////
class Color {
public:
  /// Crée une couleur, noire par défaut.
  Color();

  /// Constructeur de copie par défaut.
  Color(const Color&) = default;

  /// Opérateur d'affectation par défaut.
  Color& operator=(const Color&) = default;

  /// Destructeur par défaut.
  ~Color() = default;

  ///@{
  /// Les couleurs valides.
  static const Color Black;
  static const Color White;
  static const Color Red;
  static const Color Blue;
  static const Color Green;
  ///@}

  /// Associe une couleur à un identifiant unique entre 0 et nbColors()-1
  int toInt() const;

  /// Retourne le nombre de couleurs.
  static int nbColors();

  /// Crée une couleur à partir de son identifiant n avec : 0 <= n < nbColors()
  static Color makeColor(int n);

  /// Test d'égalité.
  bool operator==(const Color& c) const;

  /// Test de différence.
  bool operator!=(const Color& c) const;

private:
  enum Value : uint8_t { _black = 0, _white, _red, _blue, _green, _size };
  Value val_;

  Color(Value val);

  friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

/// Affichage d'une couleur en sortie sur un terminal.
std::ostream& operator<<(std::ostream& os, const Color& c);

#endif
