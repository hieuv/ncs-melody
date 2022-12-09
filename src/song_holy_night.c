#include <songs.h>

const char holy_night_1[] = "G3-3-100,A-1,G-2,E-6,G-3,A-1,G-2,E-6," \
                            "D4-4,D-2,B3-6,C4-4,C-2,G3-6,"\
                            "A-4,A-2,C4-3,B3-1,A-2,G-3,A-1,G-2,E-6,A-4,A-2,C4-3,B3-1,A-2,G-3,A-1,G-2,E-6," \
                            "D4-3,D-1,D-2,F,D,B3,C4-6,E-6,C-3,G3-1,E-2,G-3,F-1,D-2,C-12,";

const char holy_night_2[] = "E3-3-100,F-1,E-2,C-6,E-3,F-1,E-2,C-6," \
                            "F3-4,F-2,D-6,E4-4,E-2,C3-6,"\
                            "F-4,F-2,A4-3,G3-1,F-2,E-3,F-1,E-2,C-6,F-4,F-2,A4-3,G3-1,F-2,E-3,F-1,E-2,C-6," \
                            "F-3,F-1,F-2,D,G,F,E4-6,C-6,E-3,E-1,C-2,B2-3,C3-1,D-2,E-12";

const char holy_night_bass[] = "C2-1-100,G1,C2,E,G,G1,C2,G1,C2,E,G,E," \
                            "C2-1-100,G1,C2,E,G,G1,C2,G1,C2,E,G,E," \
                            "D,E,F,A,D3,C,B2,A,C3,G2,F,E,G,C,E,F,D,E,G,C,B1,A,G,C2,";


music_songdef_t song_holy_night = {
	.max_amp = 120,
	.note_lists[0].note_string = holy_night_1,
	.note_lists[0].instrument = &instr_lead1_long,
	.note_lists[0].note_offset = 12,
    //.note_lists[0].mute=true,
    //.note_lists[1].mute=true,

	.note_lists[1].note_string = holy_night_2,
	.note_lists[1].instrument = &instr_lead1_long,
	.note_lists[1].note_offset = 12,
	.note_lists[2].note_string = holy_night_bass,
	.note_lists[2].instrument = &instr_bass1,
	.note_lists[2].note_offset = 0,
	.num_note_lists = 3,
	.speed = 300,
};