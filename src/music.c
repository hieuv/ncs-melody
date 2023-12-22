#include <music.h>
#include <sound_gen.h>
#include <string.h>
#include <ctype.h>

const int str_keywords_index[] = {0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,                              NOTE_PAUSE, NOTE_END};
const char *str_keywords[] = {"c", "cs", "db", "d", "eb", "e", "f", "gb", "g", "ab", "a", "bb", "b",    "p",         "end"};
#define STR_KEYWORD_LIST_LEN (sizeof(str_keywords) / sizeof(str_keywords[0]))

volatile bool stop_playing_flag = false;

static const unsigned char *parse_number(const unsigned char *string, int *number)
{
	*number = 0;
	while(isdigit(*string)) {
		*number *= 10;
		*number += (int)(*string - '0');
		string++;
	}
	return string;
}

static const unsigned char *music_string_parser(str_decode_context_t *context, music_notedef_t *note)
{
	const unsigned char *string = context->str_pointer;
	unsigned char keyword[16];
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
			if (stop_playing_flag) break;
			if(song->note_lists[nl].active) {
				if(song->note_lists[nl].index == 0 || song->note_lists[nl].current_note_lifetime >= current_note[nl].duration) {
					// Fetch a new note
					const unsigned char *next_ptr = music_string_parser(&(song->note_lists[nl].str_decode_context), &note);
					song->note_lists[nl].index++;
					if(next_ptr == 0 || song->note_lists[nl].mute) {
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
		if (stop_playing_flag) break;
		k_msleep(song->speed);
	} while(active_lists > 0);
	stop_playing_flag = false;
	printk("Song completed\n");
	return 0;
}