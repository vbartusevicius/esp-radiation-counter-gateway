#include "ChipId.h"

String ChipId::getChipId()
{
    uint64_t chipId = ESP.getEfuseMac();
    
    uint32_t lowerChipId = (uint32_t)(chipId);
    String chipIdString = String(lowerChipId, HEX);
    
    chipIdString.toUpperCase();
    return chipIdString;
}