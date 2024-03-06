// Compatibility sound functions

#include <libspectrum.h>
#include <externs.h>

extern int retro_audio_freq;

int sound_lowlevel_init(const char *device, int *freqptr, int *stereoptr)
{
   (void)device;
   *freqptr = retro_audio_freq;
   return 0;
}

void sound_lowlevel_end(void)
{
}

void sound_lowlevel_frame(libspectrum_signed_word *data, int len)
{
   audio_cb( data, (size_t)len / 2 );
   some_audio = 1;
}
