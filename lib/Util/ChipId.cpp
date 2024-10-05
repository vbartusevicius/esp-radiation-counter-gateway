#include "ChipId.h"

String ChipId::getChipId()
{
    uint32_t chipId = ESP.getChipId();
    String chipIdString = String(chipId, HEX);
    
    chipIdString.toUpperCase();
    return chipIdString;
}