#pragma once
#include "common.h"

enum Professia
{
    STROITEL,
    TRAKTORIST,
    PROGRAMIST,
    TESTER,
    DOKTOR,
    MEDRABOTNIK,
    VODITEL,
    MANAGER, 
    HZ_KTO
};

inline std::string Prof2String(Professia prof)
{
    switch (prof)
    {
    case STROITEL:
        return "Stroitel";
    case TESTER:
        return "Tester";
    case TRAKTORIST:
        return "Traktorist";
    case PROGRAMIST:
        return "Programmist";
    case DOKTOR:
        return "Doktor";
    case MEDRABOTNIK:
        return "Medrabotnik";
    case VODITEL:
        return "Voditel";
    case MANAGER:
        return "Manager";
    default:
        return "хз кто";
    }
}

inline Professia String2Prof(std::string prof)
{
    if (prof.compare("Stroitel") == 0)
        return STROITEL;
    else if (prof.compare("Traktorist") == 0)
        return TRAKTORIST;
    else if (prof.compare("Tester") == 0)
        return TESTER;
    else if (prof.compare("Programmist") == 0)
        return PROGRAMIST;
    else if (prof.compare("Doktor") == 0)
        return DOKTOR;
    else if (prof.compare("Medrabotnik") == 0)
        return MEDRABOTNIK;
    else if (prof.compare("Voditel") == 0)
        return VODITEL;
    else if (prof.compare("Manager") == 0)
        return MANAGER;
    else 
        return HZ_KTO;
}

/// ♥️ ///