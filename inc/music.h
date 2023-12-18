#ifndef __MUSIC_H
#define __MUSIC_H

#include <zephyr/kernel.h>
#include <sound_gen.h>

#define N_C 0
#define N_CS 1
#define N_Db 1
#define N_D 2
#define N_Eb 3
#define N_E 4
#define N_F 5
#define N_Gb 6
#define N_G 7
#define N_Ab 8
#define N_A 9
#define N_Bb 10
#define N_B 11

#define NOTE(a,b) (a+b*12)
#define NOTE_PAUSE 	100001
#define NOTE_END	100002
#define NOTE_REP	100003

#define MAX_NOTE_LISTS 4

typedef struct {
	int note;
	int duration;
	int amp;
} music_notedef_t;

typedef struct {
	int octave_prev;
	int length_prev;
	int amp_prev;
	const char* str_pointer;
} str_decode_context_t;

typedef struct {
	int index;
	bool active;
	bool mute;
	int current_note_lifetime;
	sg_instrument_t *instrument;
	int note_offset;
	int repeat_counter;
	str_decode_context_t str_decode_context;
	const char *note_string;
} music_notelist_t;

typedef struct {
	int speed;
	int max_amp;
	music_notelist_t note_lists[MAX_NOTE_LISTS];
	int num_note_lists;
} music_songdef_t;


extern music_songdef_t song_god_rest_ye_gentlemen;

int music_play_song(music_songdef_t *song);

#endif
