#include "Storage.h"
#include "Parameter.h"

Storage::Storage()
{
    prefs.begin("esp_rad_cnt");
}

void Storage::saveParameter(char* name, String& value)
{
    prefs.putString(name, value);
}

String Storage::getParameter(char* name, String defaultValue)
{
    return prefs.getString(name, defaultValue);
}

bool Storage::isEmpty()
{
    for (auto &param : Parameter::REQUIRED_PARAMS) {
        if (!prefs.isKey(param)) {
            return true;
        }
    }

    return false;
}

void Storage::reset()
{
    prefs.clear();
}
