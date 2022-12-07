#include <music.h>
#include <sound_gen.h>

const music_notedef_t god_rest_ye_gentlemen_melody[] = {
	{NOTE_PAUSE, 64-4, 80},
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
	{NOTE_PAUSE, 64, 80},
	{NOTE(N_D, 2), 8, 100},{NOTE(N_C, 3), 4, 100},{NOTE(N_E, 2), 4, 100},
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

const music_notedef_t god_rest_ye_gentlemen_drum[] = {
		{NOTE(N_C, 2), 8, 100},{NOTE(N_C, 2), 4, 100},{NOTE(N_C, 2), 4, 100},{NOTE_REP, 12, 3},
		{NOTE_END, 0, 0},
};

music_songdef_t song_god_rest_ye_gentlemen = {
	.max_amp = 120,
	.note_lists[1].note_list = god_rest_ye_gentlemen_melody,
	.note_lists[1].instrument = 0,
	.note_lists[1].note_offset = 24,
	.note_lists[0].note_list = god_rest_ye_gentlemen_bass,
	.note_lists[0].instrument = 1,
	.note_lists[0].note_offset = -12,
	.note_lists[2].note_list = god_rest_ye_gentlemen_drum,
	.note_lists[2].instrument = 2,
	.note_lists[2].note_offset = 0,
	.num_note_lists = 3,
	.speed = 60,
};

static void fetch_note(const music_songdef_t *song, music_notelist_t *notelist, const music_notedef_t *note)
{
	if(note->note != NOTE_PAUSE) {
		sg_play_note(note->note + notelist->note_offset, (float)note->amp / (float)song->max_amp, notelist->instrument);
	}
}

int music_play_song(music_songdef_t *song)
{
	int active_lists;
	const music_notedef_t *current_note[MAX_NOTE_LISTS], *note;
	printk("Playing song...\n");
	for(int i = 0; i < song->num_note_lists; i++) {
		song->note_lists[i].active = true;
		song->note_lists[i].index = 0;
		song->note_lists[i].repeat_counter = 0;
	}
	do {
		active_lists = 0;
		for(int nl = 0; nl < song->num_note_lists; nl++) {
			if(song->note_lists[nl].active) {
				if(song->note_lists[nl].index == 0 || song->note_lists[nl].current_note_lifetime >= current_note[nl]->duration) {
					// Fetch a new note
					note = current_note[nl] = &(song->note_lists[nl].note_list[song->note_lists[nl].index++]);
					if(note->note == NOTE_END) {
						song->note_lists[nl].active = false;
					} else if(note->note == NOTE_REP) {
						if(++song->note_lists[nl].repeat_counter <= note->duration) {
							// Do another repeat, by reducing the note_list index
							song->note_lists[nl].index -= note->amp;
							song->note_lists[nl].current_note_lifetime = 1;
							note = current_note[nl] =  &(song->note_lists[nl].note_list[song->note_lists[nl].index-1]);
							fetch_note(song, &(song->note_lists[nl]), note);
						} else {
							// Abort the repeat, and let the index increase as normal
							song->note_lists[nl].repeat_counter = 0;
						}
					} else {
						//printk("\nFetched note: note %i, dur %i amp %i, instr %i ", note->note, note->duration, note->amp, song->note_lists[nl].instrument);
						fetch_note(song, &(song->note_lists[nl]), note);
						song->note_lists[nl].current_note_lifetime = 1;
					}
				} else {
					song->note_lists[nl].current_note_lifetime++;
				}
				active_lists++;
			}
		}
		k_msleep(song->speed);
	} while(active_lists > 0);
	printk("Song completed\n");
	return 0;
}