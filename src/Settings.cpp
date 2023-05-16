#include <fstream>
#include <string>
#include <stdexcept>

#include "ResourceManager.h"
#include "Utils/Logger.h"
#include "Utils/Ini/File.h"
#include "Utils/Ini/Section.h"
#include "Utils/Ini/Writer.h"
#include "Settings.h"

namespace Ironhead
{

Settings *Settings::getInstance()
{
  return Base::Singleton<Settings>::get();
}


Settings::Settings()
{
    if (!load())
    {
        save();
    }
}

Settings::~Settings()
{
}

bool Settings::save()
{
    std::string configFile = ResourceManager::getInstance()->getPrefPath("config.ini");
    std::ofstream stream(configFile);

    LOG_DEBUG("Saving config to {}", configFile);

    if (!stream)
    {
        LOG_WARNING("Cannot open config file at `{}`", configFile);
        return false;
    }

    Utils::Ini::File file;

    auto path = file.section("path");
    path->setPropertyString("last_path", _musicPath);

    Utils::Ini::Writer writer(file);
    writer.write(stream);

    return true;
}


bool Settings::load()
{
    std::string configFile = ResourceManager::getInstance()->getPrefPath("config.ini");
    std::ifstream stream(configFile);

    LOG_DEBUG("Loading config from {}", configFile);

    if (!stream)
    {
        LOG_WARNING("Cannot open config file at `{}`", configFile);
        return false;
    }

    Utils::Ini::Parser parser(stream);
    auto file = parser.parse();

    auto path = file->section("path");
    if (path)
    {
        _musicPath = path->propertyString("last_path", _musicPath);
    }

    return true;
}


void Settings::setMusicPath(const std::string& path)
{
    _musicPath = path;
}

std::string& Settings::musicPath()
{
    return _musicPath;
}

}
