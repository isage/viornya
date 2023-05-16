#ifndef SOUND_SOUNDMANAGER_H_
#define SOUND_SOUNDMANAGER_H_

#include "../Base/Singleton.h"

#include <SDL_mixer.h>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

#define SAMPLE_RATE 44100

namespace Ironhead
{
namespace Sound
{

enum class SFX : uint16_t
{
  SND_NULL = 0,
  // drums
  SND_DRUM_BASS = 150,
  SND_DRUM_SNARE = 151,
  SND_DRUM_HICLOSE = 152,
  SND_DRUM_HIOPEN = 153,
  SND_DRUM_TOM = 154,
  SND_DRUM_PERCUSSION = 155
};

class SoundManager
{

public:
  static SoundManager *getInstance();

  bool init();
  void shutdown();

  void music(const std::filesystem::path& path);
  void stop();

  std::filesystem::path& currentSong();

  void fadeMusic();
  void runFade();
  void pause();
  void resume();
  void ff();
  void rwd();
  bool playing() { return _playing;}

protected:
  friend class Base::Singleton<SoundManager>;

  SoundManager();
  ~SoundManager();
  SoundManager(const SoundManager &) = delete;
  SoundManager &operator=(const SoundManager &) = delete;

private:
  std::filesystem::path _currentSong;
  bool _playing = false;
};



} // namespace Sound
} // namespace Ironhead
#endif
