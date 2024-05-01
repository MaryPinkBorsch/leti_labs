#pragma once
#include "common.h"
enum Gorod 
{
    MOSCOW,
    SAINT_PETERSBURG,
    NOVGOROD,
    NOVOSIBIRSK,
    XZ_GDE
};


inline std::string Gorod2String(Gorod gorod)
{
  switch (gorod)
  {
  case MOSCOW:
    return "Moscow";
  case SAINT_PETERSBURG:
    return "Saint-Petersburg";
  case NOVGOROD:
    return "Novgorod";
  case NOVOSIBIRSK:
    return "Novosibirsk";

  default:
    return "хз gde";
  }
}

inline Gorod String2gorod(std::string gorod)
{
  if (gorod.compare("Moscow") == 0)
    return MOSCOW;
  else if (gorod.compare("Saint-Petersburg") == 0)
    return SAINT_PETERSBURG;
  else if (gorod.compare("Novgorod") == 0)
    return NOVGOROD;
  else if (gorod.compare("Novosibirsk") == 0)
    return NOVOSIBIRSK;
 
  else
    return XZ_GDE;
}
