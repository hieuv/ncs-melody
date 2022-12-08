#include <music.h>
#include <sound_gen.h>
#include <string.h>
#include <ctype.h>

const int str_keywords_index[] = {0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,                              NOTE_PAUSE, NOTE_END};
const char *str_keywords[] = {"c", "cs", "db", "d", "eb", "e", "f", "gb", "g", "ab", "a", "bb", "b",    "p",         "end"};
#define STR_KEYWORD_LIST_LEN (sizeof(str_keywords) / sizeof(str_keywords[0]))

const char god_rest_ye_melody[] = "P-30,C3-2-80,C,G,G,F,Eb,D,C,Bb2,C3,D,Eb,F,G-6,C-2," \
								  "C3-2-80,C,G,G,F,Eb,D,C,Bb2,C3,D,Eb,F,G-6,G-2," \
								  "Ab,F,G,Ab,Bb,C4,G3,F,Eb,C,D,Eb,F-4,Eb-2,F," \
								  "G,Ab,Bb,C4,G3,F,Eb,D,C-8,";
const char god_rest_ye_bass[]   = "P-32," \
								  "C2-4-100,C-2,C,C-4,C-2,C,C-4,C-2,C,G,F,Eb,D" \
								  "C2-4,B1-2,G,C2-4,C,D-2,F,Eb,C,G-4,G,";

const char god_rest_ye_drums[]  = "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2," \
								  "C0-4-100,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,C-4,C-2,C-2,";

music_songdef_t song_god_rest_ye_gentlemen = {
	.max_amp = 120,
	.note_lists[0].note_string = god_rest_ye_melody,
	.note_lists[0].instrument = 1,
	.note_lists[0].note_offset = -12,
	.note_lists[1].note_string = god_rest_ye_bass,
	.note_lists[1].instrument = 0,
	.note_lists[1].note_offset = 24,
	.note_lists[2].note_string = god_rest_ye_drums,
	.note_lists[2].instrument = 2,
	.note_lists[2].note_offset = 0,
	.num_note_lists = 3,
	.speed = 120,
};

static char *parse_number(char *string, int *number)
{
	*number = 0;
	while(isdigit(*string)) {
		*number *= 10;
		*number += (int)(*string - '0');
		string++;
	}
	return string;
}

static char *music_string_parser(str_decode_context_t *context, music_notedef_t *note)
{
	char *string = context->str_pointer;
	char keyword[16];
	int i, k, octave, length, amp;
	
	// Check for end of string
	if(*string == 0) return 0;
	
	// Find the keyword
	for(i = 0; i < 15 && *string != ',' && *string != '-'; i++) {
		keyword[i] = tolower(*string++);
	}
	keyword[i] = 0;

	// Check if there is a octave index after the note
	if(isdigit(keyword[i-1])) {
		context->octave_prev = octave = (int)(keyword[i-1] - '0');
		// Shave the octave digit of the keyword
		keyword[i-1] = 0;
		//printk("[oct %i]", octave);
	} else octave = context->octave_prev;
	//printk("[kwd %s]", keyword);

	// Look for a matching keyword
	for(k = 0; k < STR_KEYWORD_LIST_LEN; k++) {
		if(strcmp(keyword, str_keywords[k]) == 0) {
			break;
		}
	}
	if(str_keywords_index[k] < 12) {
		// Regular note
		note->note = str_keywords_index[k];
	} else if(str_keywords_index[k] == NOTE_END) {
		return 0;
	} else if(str_keywords_index[k] == NOTE_PAUSE) {
		note->note = NOTE_PAUSE;
	}

	// Check if there is a length specifier
	if(*string == '-') {
		string = parse_number(string+1, &length);
		context->length_prev = length;
	} else length = context->length_prev;

	// Check if there is an amp specifier
	if(*string == '-') {
		string = parse_number(string+1, &amp);
		context->amp_prev = amp;
	} else amp = context->amp_prev;
	if(note->note != NOTE_PAUSE) note->note += octave*12;
	note->duration = length;
	note->amp = amp;
	
	// Check for end of string
	if(*string == 0) return 0;
	// Return the byte after the next delimiter
	return string+1;
}

static void fetch_note(const music_songdef_t *song, music_notelist_t *notelist, const music_notedef_t *note)
{
	if(note->note != NOTE_PAUSE) {
		//printk("\nFetched note: note %i, dur %i amp %i, instr %i ", note->note, note->duration, note->amp, notelist->instrument);
		sg_play_note(note->note + notelist->note_offset, (float)note->amp / (float)song->max_amp, notelist->instrument);
	}
}

int music_play_song(music_songdef_t *song)
{
	int active_lists;
	music_notedef_t current_note[MAX_NOTE_LISTS], note;
	printk("Playing song...\n");
	for(int i = 0; i < song->num_note_lists; i++) {
		song->note_lists[i].active = true;
		song->note_lists[i].index = 0;
		song->note_lists[i].repeat_counter = 0;
		song->note_lists[i].str_decode_context.str_pointer = song->note_lists[i].note_string;
	}
	do {
		active_lists = 0;
		for(int nl = 0; nl < song->num_note_lists; nl++) {
			if(song->note_lists[nl].active) {
				if(song->note_lists[nl].index == 0 || song->note_lists[nl].current_note_lifetime >= current_note[nl].duration) {
					// Fetch a new note
					char *next_ptr = music_string_parser(&(song->note_lists[nl].str_decode_context), &note);
					song->note_lists[nl].index++;
					if(next_ptr == 0) {
						song->note_lists[nl].active = false;
					} else {
						current_note[nl] = note;
						song->note_lists[nl].str_decode_context.str_pointer = next_ptr;
						fetch_note(song, &(song->note_lists[nl]), &note);
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