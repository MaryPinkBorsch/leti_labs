#pragma once

#include "common.h"

enum Obrazovanie
{
  NACHALNOE,
  SREDNEE,
  SREDNESPECIALNOE,
  VISSHEE,
  XZ_CHTO
};

inline std::string Obraz2String(Obrazovanie obraz)
{
  switch (obraz)
  {
  case NACHALNOE:
    return "Nachalnoe";
  case SREDNEE:
    return "Srednee";
  case SREDNESPECIALNOE:
    return "Srednespecialnoe";
  case VISSHEE:
    return "Visshee";

  default:
    return "хз chto";
  }
}

inline Obrazovanie String2Obraz(std::string obraz)
{
  if (obraz.compare("Nachalnoe") == 0)
    return NACHALNOE;
  else if (obraz.compare("Srednee") == 0)
    return SREDNEE;
  else if (obraz.compare("Srednespecialnoe") == 0)
    return SREDNESPECIALNOE;
  else if (obraz.compare("Visshee") == 0)
    return VISSHEE;
 
  else
    return XZ_CHTO;
}
