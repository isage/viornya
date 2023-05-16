#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include "Base/Singleton.h"

namespace Ironhead
{

class Settings
{
public:
    static Settings *getInstance();

    bool save();
    bool load();

    std::string& musicPath();
    void setMusicPath(const std::string& path);


protected:
    friend class Base::Singleton<Settings>;
    Settings();
    ~Settings();

    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

private:
    // [sound]
    std::string _musicPath = "";
};

} // Ironhead

#endif // SETTINGS_H
