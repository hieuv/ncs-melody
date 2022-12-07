#include <music.h>;
#include <sound_gen.h>

const music_notedef_t god_rest_ye_gentlemen_melody[] = {
	{NOTE(N_C, 3), 4, 80},
	{NOTE(N_C, 3), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_Eb, 3), 4, 100},{NOTE(N_D, 3), 4, 100},{NOTE(N_C, 3), 4, 100},{NOTE(N_Bb, 2), 4, 100},
	{NOTE(N_C, 3), 4, 100},{NOTE(N_D, 3), 4, 100},{NOTE(N_Eb, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_G, 3), 12, 100},{NOTE(N_C, 3), 4, 100},

	{NOTE(N_C, 3), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_Eb, 3), 4, 100},{NOTE(N_D, 3), 4, 100},{NOTE(N_C, 3), 4, 100},{NOTE(N_Bb, 2), 4, 100},
	{NOTE(N_C, 3), 4, 100},{NOTE(N_D, 3), 4, 100},{NOTE(N_Eb, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_G, 3), 12, 100},{NOTE(N_G, 3), 4, 100},
	{NOTE(N_Ab, 3), 4, 100},{NOTE(N_F, 3), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_Ab, 3), 4, 100},
	{NOTE(N_Bb, 3), 4, 100},{NOTE(N_C, 4), 4, 100},{NOTE(N_G, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_Eb, 3), 4, 100},{NOTE(N_C, 3), 4, 100},{NOTE(N_D, 3), 4, 100},{NOTE(N_Eb, 3), 4, 100},
	{NOTE(N_F, 3), 8, 100},{NOTE(N_Eb, 3), 4, 100},{NOTE(N_F, 3), 4, 100},
	{NOTE(N_G, 3), 4, 100},{NOTE(N_Ab, 3), 4, 100},{NOTE(N_Bb, 3), 4, 100},{NOTE(N_C, 4), 4, 100},
	{NOTE(N_G, 3), 4, 100},{NOTE(N_F, 3), 4, 100},{NOTE(N_Eb, 3), 4, 100},{NOTE(N_D, 3), 4, 100},
	{NOTE(N_C, 3), 16, 100},{NOTE_END, 0, 0},
};

const music_notedef_t god_rest_ye_gentlemen_bass[] = {
	{NOTE_PAUSE, 4, 80},
	{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_C, 2), 4, 100},
	{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_G, 1), 4, 100},
	{NOTE(N_Ab, 1), 4, 100},{NOTE(N_G, 1), 4, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_C, 2), 4, 100},
	{NOTE(N_B, 1), 4, 100},{NOTE(N_G, 1), 4, 100},{NOTE(N_A, 1), 4, 100},{NOTE(N_B, 1), 4, 100},
	{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_C, 2), 4, 100},
	{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_G, 1), 4, 100},
	{NOTE(N_Ab, 1), 4, 100},{NOTE(N_G, 1), 4, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_Eb, 2), 4, 100},
	{NOTE(N_G, 2), 8, 100},{NOTE(N_G, 1), 8, 100},
	{NOTE(N_F, 1), 8, 100},{NOTE(N_F, 1), 8, 100},{NOTE(N_Eb, 1), 8, 100},{NOTE(N_Eb, 1), 8, 100},
	{NOTE(N_C, 1), 8, 100},{NOTE(N_C, 1), 8, 100},{NOTE(N_D, 1), 8, 100},{NOTE(N_Eb, 1), 4, 100},{NOTE(N_F, 1), 4, 100},
	{NOTE(N_G, 1), 8, 100},{NOTE(N_G, 1), 8, 100},
	{NOTE(N_Bb, 1), 4, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_D, 2), 4, 100},{NOTE(N_G, 1), 4, 100},
	{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 1), 8, 100},
	{NOTE_END, 0, 0},
};


music_notelist_t god_rest_ye_gentlemen_melodylist = {
	.note_list = god_rest_ye_gentlemen_melody,
	.instrument = 0,
	.note_offset = 24,
};

music_notelist_t god_rest_ye_gentlemen_basslist = {
	.note_list = god_rest_ye_gentlemen_bass,
	.instrument = 1,
	.note_offset = -12, 
};

const music_songdef_t song_god_rest_ye_gentlemen = {
	.max_amp = 120,
	.note_lists[1] = &god_rest_ye_gentlemen_melodylist,
	.note_lists[0] = &god_rest_ye_gentlemen_basslist,
	.num_note_lists = 2,
	.speed = 60,
};

int music_play_song(music_songdef_t *song)
{
	int time_index = 0;
	int active_lists;
	int note_list_indexes[MAX_NOTE_LISTS] = {0};
	music_notedef_t *current_note[MAX_NOTE_LISTS], *note;
	printk("Playing song...\n");
	for(int i = 0; i < song->num_note_lists; i++) {
		song->note_lists[i]->active = true;
		song->note_lists[i]->index = 0;
	}
	do {
		active_lists = 0;
		for(int nl = 0; nl < song->num_note_lists; nl++) {
			if(song->note_lists[nl]->active) {
				if(song->note_lists[nl]->index == 0 || song->note_lists[nl]->current_note_lifetime >= current_note[nl]->duration) {
					// Fetch a new note
					note = current_note[nl] = &(song->note_lists[nl]->note_list[song->note_lists[nl]->index++]);
					if(note->note == NOTE_END) {
						song->note_lists[nl]->active = false;
					} else {
						printk("\nFetched note: note %i, dur %i amp %i, instr %i ", note->note, note->duration, note->amp, song->note_lists[nl]->instrument);
						if(note->note != NOTE_PAUSE) {
							sg_play_note(note->note + song->note_lists[nl]->note_offset, (float)note->amp / (float)song->max_amp, song->note_lists[nl]->instrument);
						}
						song->note_lists[nl]->current_note_lifetime = 1;
					}
				} else {
					song->note_lists[nl]->current_note_lifetime++;
				}
				active_lists++;
			}
		}
		k_msleep(song->speed);
	} while(active_lists > 0);
	printk("Song completed\n");
}