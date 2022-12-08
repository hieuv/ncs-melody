#include <songs.h>

const char god_rest_ye_melody[] = "P-30,C3-2-120,C,G,G,F,Eb,D,C,Bb2,C3,D,Eb,F,G-6,C-2," \
								  "C3-2-80,G,G,F,Eb,D,C,Bb2,C3,D,Eb,F,G-6,G-2," \
								  "Ab,F,G,Ab,Bb,C4,G3,F,Eb,C,D,Eb,F-4,Eb-2,F," \
								  "G,Ab,Bb,C4,G3,F,Eb,D,C-8,";
const char god_rest_ye_bass[]   = "P-32," \
								  "C2-4-80,C-2,C,C-4,C-2,C,C-4,C-2,C,G,F,Eb,D," \
								  "C2-4,B1-2,G,C2-4,C,D-2,F,Eb,C,G-4,G," \
								  "F-4,D,Eb,Eb-2,D,C-4,C-2,Eb,G-4,G," \
								  "F-2,G,F,D,Eb,D,C,Eb,G,F,Eb,D,C-4,C1-4,";

const char god_rest_ye_drums[]  = "C0-4-10,C-2-15,C-2-22,C-4-33,C-2-44,C-2-55,C-4-66,C-2-77,C-2-88,C-4-99,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4-90,C-2-70,C-2-50,C-4-36,C-2-15,C-2-5,";

music_songdef_t song_god_rest_ye_gentlemen = {
	.max_amp = 120,
	.note_lists[0].note_string = god_rest_ye_melody,
	.note_lists[0].instrument = &instr_lead1,
	.note_lists[0].note_offset = 12,
	.note_lists[1].note_string = god_rest_ye_bass,
	.note_lists[1].instrument = &instr_bass1,
	.note_lists[1].note_offset = -12,
	.note_lists[2].note_string = god_rest_ye_drums,
	.note_lists[2].instrument = &instr_drum_snare,
	.note_lists[2].note_offset = 0,
	.num_note_lists = 3,
	.speed = 120,
};