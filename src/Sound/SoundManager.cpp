#include "SoundManager.h"

#include "../ResourceManager.h"
#include "../common/misc.h"
#include "../Utils/Logger.h"
#include "Organya.h"
#include "Pixtone.h"

#include <json.hpp>
#include <fstream>
#include <iostream>

#define MUSIC_OFF 0
#define MUSIC_ON 1
#define MUSIC_BOSS_ONLY 2

namespace Ironhead
{
namespace Sound
{

SoundManager::SoundManager() {}
SoundManager::~SoundManager() {}

SoundManager *SoundManager::getInstance()
{
  return Base::Singleton<SoundManager>::get();
}

bool SoundManager::init()
{
  LOG_INFO("Sound system init");
  if (Mix_Init(MIX_INIT_OGG) == -1)
  {
    LOG_ERROR("Unable to init mixer.");
    return false;
  }

#if SDL_MIXER_PATCHLEVEL >= 2
  if (Mix_OpenAudioDevice(SAMPLE_RATE, AUDIO_S16, 2, 2048, NULL, 0) == -1)
  {
    LOG_ERROR("Unable to open audio device.");
    return false;
  }
#else
  if (Mix_OpenAudio(SAMPLE_RATE, AUDIO_S16, 2, 2048) == -1)
  {
    LOG_ERROR("Unable to open audio device.");
    return false;
  }
#endif
  Mix_AllocateChannels(64);

  Pixtone::getInstance()->init();
  Organya::getInstance()->init();

  return true;
}

void SoundManager::shutdown()
{
  Organya::getInstance()->shutdown();
  Pixtone::getInstance()->shutdown();

  Mix_CloseAudio();
  Mix_Quit();
  LOG_INFO("Sound system shutdown");
}

void SoundManager::music(const std::filesystem::path& path)
{
  LOG_DEBUG(" >> music({})", path.string());

  Organya::getInstance()->stop();
  _playing = false;

  if (Organya::getInstance()->load(path.string()))
  {
    Organya::getInstance()->start(0);
    _currentSong = path;
    _playing = true;
  }
}

std::filesystem::path& SoundManager::currentSong()
{
  return _currentSong;
}

void SoundManager::fadeMusic()
{
    Organya::getInstance()->fade();
}

void SoundManager::runFade()
{
    Organya::getInstance()->runFade();
}

void SoundManager::ff()
{
    Organya::getInstance()->ff();
}

void SoundManager::rwd()
{
    Organya::getInstance()->rwd();
}

void SoundManager::pause()
{
  Mix_Pause(-1);
  Organya::getInstance()->pause();
}

void SoundManager::stop()
{
  Organya::getInstance()->stop();
  _playing = false;
}

void SoundManager::resume()
{
  Mix_Resume(-1);
  Organya::getInstance()->resume();
}

} // namespace Sound
} // namespace Ironhead
