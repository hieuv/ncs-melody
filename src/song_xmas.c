#include <songs.h>

const char boot_tune[] = "G3-3-100,A-4,B-4,C-4,B-4,A-4,end";

music_songdef_t song_short_dbg = {
	.max_amp = 120,
	.note_lists[0].note_string = boot_tune,
	.note_lists[0].instrument = &instr_lead1,
	.note_lists[0].note_offset = 0,
	.num_note_lists = 1,
	.speed = 300,
};

const char santa_claus_is_coming_to_town_track_1[] = 
	"G3-2-100,E-1,F-1,G-2,G-3,G-1,A-1,B-1,C4-2,C-2,2,E3-1,F-1,G-2,G-2,G-2,A-1,G-1,F-2,F-2,P-2,E-2,G-2,C-2,E-2,D-2,F-4,B2-2,G3-8,end";
const char santa_claus_is_coming_to_town_track_2[] = 
	"B2-2-100,C3-4,E2-4,F-4,A-4,C3-4,Bb2-4,A-4,Ab-4,G-4,A-4,F-4,G-4,C-8,end";

music_songdef_t song_santa_claus_is_coming_to_town = {
	.max_amp = 120,
	.note_lists[0].note_string = santa_claus_is_coming_to_town_track_1,
	.note_lists[0].instrument = &instr_lead1,
	.note_lists[0].note_offset = 0,
	.note_lists[1].note_string = santa_claus_is_coming_to_town_track_2,
	.note_lists[1].instrument = &instr_lead1,
	.note_lists[1].note_offset = 0,
	.num_note_lists = 2,
	.speed = 200,
};

const char paa_laaven_sitter_nissen_track_1[] = 
	"D2-1-100,G-1,Gb-1,G-1,A-1,B-1,A-1,B-1,C3-1,E-2,D-2,D-3,B2-1,D3-2,C-2,C-3,A2-1,C3-2,B2-2,B-3,D-1," \
    "G-1,Gb-1,G-1,A-1,B-1,A-1,B-1,C3-1,E-2,D-2,D-3,B2-1,D3-2,C-2,C-1,Gb2-1,Gb-1,Gb-1,G-2,B-2,G-2,G-2," \
    "E3-1,D-1,C-1,B2-1,D3-1,C-1,B2-1,A-1,G-4,E-2,G-1,G-1,F-4,D-2,A-1,A-1,G-4,E-2,G-2," \
    "E3-1,D-1,C-1,B2-1,D3-1,C-1,B2-1,A-1,G-4,E-2,G-1,G-1,A-2,D3-2,C-1,B2-1,A-1,B-1,C3-2,C3-2,C3-2,end";
const char paa_laaven_sitter_nissen_track_2[] = 
	"P-1-100,G1-4,D-4,G-4,D-4,D-4,A-2,D-2,G-4,D-4,G-4,D-4,G-4,E-4,A-4,D-2,A-2,G-2,D-2,G-4," \
	"C-4,F-4,E-4,C-2,Cs-2,D-4,G-4,C2-4,G1-4,C-4,F-4,C-4,G-2,C-2,D-4,G-4,G-2,G-2,G-2,end";

music_songdef_t song_paa_laaven_sitter_nissen = {
	.max_amp = 120,
	.note_lists[0].note_string = paa_laaven_sitter_nissen_track_1,
	.note_lists[0].instrument = &instr_lead1,
	.note_lists[0].note_offset = 0,
	.note_lists[1].note_string = paa_laaven_sitter_nissen_track_2,
	.note_lists[1].instrument = &instr_lead1,
	.note_lists[1].note_offset = 0,
	.num_note_lists = 2,
	.speed = 200,
};
