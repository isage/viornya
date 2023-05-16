/* Based on */
/* SIMPLE CAVE STORY MUSIC PLAYER (Organya) */
/* Written by Joel Yliluoma -- http://iki.fi/bisqwit/ */
/* https://bisqwit.iki.fi/jutut/kuvat/programming_examples/doukutsu-org/orgplay.cc */

#ifndef SOUND_PIXTONE_H
#define SOUND_PIXTONE_H

#include "../Base/Singleton.h"
#include "../common/basics.h"
#include "SoundManager.h"

#include <SDL_mixer.h>
#include <string>

#define PXT_NO_CHANNELS 4
#define PXENV_NUM_VERTICES 3

#define NUM_RESAMPLED_BUFFERS 16

namespace Ironhead
{
namespace Sound
{

enum
{
  MOD_SINE,
  MOD_TRI,
  MOD_SAWUP,
  MOD_SAWDOWN,
  MOD_SQUARE,
  MOD_NOISE,

  PXT_NO_MODELS
};

typedef struct
{
  int32_t initial;
  struct
  {
    int32_t time, val;
  } p[PXENV_NUM_VERTICES];
  int32_t evaluate(int32_t i) const;
} stPXEnvelope;

typedef struct
{
  bool enabled;
  uint32_t nsamples;
  struct stPXWave
  {
    const int8_t *wave;
    double pitch;
    int32_t level, offset;
  };

  stPXWave carrier;
  stPXWave frequency;
  stPXWave amplitude;
  stPXEnvelope envelope;
  int8_t *buffer;
  void synth();
} stPXChannel;

typedef struct
{
  stPXChannel channels[PXT_NO_CHANNELS];

  int8_t *final_buffer = nullptr;
  uint32_t final_size;
  bool load(const std::string &fname);
  bool render();
  int32_t allocBuf();
  void freeBuf();
} stPXSound;

class Pixtone
{
public:
  static Pixtone *getInstance();
  bool init();
  void shutdown();

protected:
  friend class Base::Singleton<Pixtone>;

  Pixtone();
  ~Pixtone();
  Pixtone(const Pixtone &) = delete;
  Pixtone &operator=(const Pixtone &) = delete;

private:
  bool _inited = false;
};

} // namespace Sound
} // namespace Ironhead
#endif
