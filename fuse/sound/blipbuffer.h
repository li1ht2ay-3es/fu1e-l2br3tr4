/** Sample buffer that resamples from input clock rate to output sample rate */

/* blip_buf $vers */
#ifndef BLIP_BUF_H 
#define BLIP_BUF_H

#ifdef __cplusplus
	extern "C" {
#endif

/** First parameter of most functions is blip_t*, or const blip_t* if nothing
is changed. */
typedef struct blip_t blip_t;
typedef struct blip_buffer_state_t blip_buffer_state_t;

/** Creates new buffer that can hold at most sample_count samples. Sets rates
so that there are blip_max_ratio clocks per sample. Returns pointer to new
buffer, or NULL if insufficient memory. */
blip_t* blip_new( int sample_count );

/** Sets approximate input clock rate and output sample rate. For every
clock_rate input clocks, approximately sample_rate samples are generated. */
void blip_set_rates( blip_t*, double clock_rate, double sample_rate );

enum { /** Maximum clock_rate/sample_rate ratio. For a given sample_rate,
clock_rate must not be greater than sample_rate*blip_max_ratio. */
blip_max_ratio = 1 << 20 };

/** Clears entire buffer. Afterwards, blip_samples_avail() == 0. */
void blip_clear( blip_t* );

/** Adds positive/negative delta into buffer at specified clock time. */
void blip_add_delta( blip_t*, unsigned clock_time, int delta );

/** Same as blip_add_delta(), but uses faster, lower-quality synthesis. */
void blip_add_delta_fast( blip_t*, unsigned clock_time, int delta );

/** Reads and removes at most 'count' samples and writes them to to every other 
element of 'out', allowing easy interleaving of two buffers into a stereo sample
stream. Outputs 16-bit signed samples. Returns number of samples actually read.  */
int blip_read_samples( blip_t*, short out [], int count, int stereo);

/** Length of time frame, in clocks, needed to make sample_count additional
samples available. */
int blip_clocks_needed( const blip_t*, int sample_count );

enum { /** Maximum number of samples that can be generated from one time frame. */
blip_max_frame = 4000 };

/** Makes input clocks before clock_duration available for reading as output
samples. Also begins new time frame at clock_duration, so that clock time 0 in
the new time frame specifies the same clock as clock_duration in the old time
frame specified. Deltas can have been added slightly past clock_duration (up to
however many clocks there are in two output samples). */
void blip_end_frame( blip_t*, unsigned clock_duration );

/** Number of buffered samples available for reading. */
int blip_samples_avail( const blip_t* );

/** Discards samples by moving the write pointer backwards directly,
leaving the audio buffer dirty. */
void blip_discard_samples_dirty(blip_t*, int count);

/* Same as above function except sample is mixed from three blip buffers source */
int blip_mix_samples( blip_t* m1, blip_t* m2, blip_t* m3, short out [], int count);

/** Frees buffer. No effect if NULL is passed. */
void blip_delete( blip_t* );



/* Deprecated */
typedef blip_t blip_buffer_t;

typedef long blip_time_t;

/*  Output samples are 16-bit signed, with a range of -32767 to 32767 */
typedef short blip_sample_t;

typedef struct blip_t Blip_Buffer;
typedef struct blip_synth_t Blip_Synth;


/* Fuse */
Blip_Buffer *new_Blip_Buffer( void );
void delete_Blip_Buffer( Blip_Buffer ** buff );
void blip_buffer_set_clock_rate( Blip_Buffer * buff, long cps );
char *blip_buffer_set_sample_rate( Blip_Buffer * buff, long samples_per_sec, int msec_length );
long blip_buffer_read_samples( Blip_Buffer * buff, blip_sample_t * out, long max_samples, int stereo );
void blip_buffer_end_frame( Blip_Buffer * buff, blip_time_t t );
void blip_buffer_set_bass_freq( Blip_Buffer * buff, int frequency );

Blip_Synth *new_Blip_Synth( void );
void delete_Blip_Synth( Blip_Synth ** synth );
void blip_synth_set_output( Blip_Synth * synth, Blip_Buffer * b );
void blip_synth_set_volume( Blip_Synth * synth, double v );
void blip_synth_set_treble_eq( Blip_Synth * synth, double treble );
void blip_synth_update( Blip_Synth * synth, blip_time_t t, int amp );

#ifdef __cplusplus
	}
#endif

#endif
