 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <argp.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

const char *argp_program_version = "wttr.in-c v0.1b";
const char *argp_program_bug_address = "<kyildirim14@ku.edu.tr>";

static char doc[] = "A simple wrapper for wttr.in written in C using curl.";

enum optioncodes{ //Option codes
	WTTR_LOCATION = 'l',
	WTTR_VERBOSE = 'v',
	WTTR_PNG = 'p',
	WTTR_METRIC = 'm',
	WTTR_USCS = 'u',
	WTTR_WIND_MS = 'M',
	WTTR_CURRENT = '0',
	WTTR_CURRENT_P1 = '1',
	WTTR_CURRENT_P2 = '2',
	WTTR_NARROW = 'n',
	WTTR_QUIET = 'q',
	WTTR_SUPER_QUIET = 'Q',
	WTTR_TERM_SEQ_OFF = 'T',
	WTTR_FRAME = 'f',
	WTTR_TRANS = 't',
	WTTR_OUT = 'o'
};

static char args_doc[] = "TO-DO Implement";

static struct argp_option options[] =
{
	{"verbose", WTTR_VERBOSE, 0, OPTION_HIDDEN, "Verbose mode for testing."},
	{"location", WTTR_LOCATION, "LOCATION", 0, "Location."},
	{"png", WTTR_PNG, 0, 0, "PNG mode."},
	{"metric", WTTR_METRIC, 0, 0, "Use SI units."},
	{"uscs", WTTR_USCS, 0, 0, "USCS."},
	{"wind-metric", WTTR_WIND_MS, 0, 0, "Show wind speed in m/s."},
	{"current", WTTR_CURRENT, 0, 0, "Show current weather only."},
	{"one-day", WTTR_CURRENT_P1, 0, 0, "Show current day and next."},
	{"two-day", WTTR_CURRENT_P2, 0, 0, "Show current and next two days."},
	{"narrow", WTTR_NARROW, 0, 0, "Narrow version."},
	{"quiet", WTTR_QUIET, 0, 0, "Quiet version."},
	{"superquiet", WTTR_SUPER_QUIET, 0, 0, "Superquiet version."},
	{"term-seq-off", WTTR_TERM_SEQ_OFF, 0, 0, "Switch terminal sequences off."},
	{"frame", WTTR_FRAME, 0, 0, "Add frame around output."},
	{"transparency", WTTR_TRANS, "TRANS", OPTION_ARG_OPTIONAL, "Transparency (default 150)."},
	{"output", WTTR_OUT, "FILE", 0, "Output file name."}
};

struct arguments {
	int valid;
	int verbose;
	int png_mode;
	int transparency;
	int flag_counter;
	char flags[16];
	char *location;
	char *output_file;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state){

	struct arguments *args = state->input;

	switch(key){
		case 'v':
			args->verbose = 1;
			break;
		case 'l':
			args->location = arg;
			break;
		case 'p':
			args->png_mode = 1;
			break;
		case 'm':
			args->flags[args->flag_counter] = 'm';
			args->flag_counter++;
			break;
		case 'u':
			args->flags[args->flag_counter] = 'u';
			args->flag_counter++;
			break;
		case 'M':
			args->flags[args->flag_counter] = 'M';
			args->flag_counter++;
			break;
		case '0':
			args->flags[args->flag_counter] = '0';
			args->flag_counter++;
			break;
		case '1':
			args->flags[args->flag_counter] = '1';
			args->flag_counter++;
			break;
		case '2':
			args->flags[args->flag_counter] = '2';
			args->flag_counter++;
			break;
		case 'n':
			args->flags[args->flag_counter] = 'n';
			args->flag_counter++;
			break;
		case 'q':
			args->flags[args->flag_counter] = 'q';
			args->flag_counter++;
			break;
		case 'Q':
			args->flags[args->flag_counter] = 'Q';
			args->flag_counter++;
			break;
		case 'T':
			args->flags[args->flag_counter] = 'T';
			args->flag_counter++;
			break;
		case 'f':
			args->flags[args->flag_counter] = 'q';
			args->flag_counter++;
			break;
		case 't':
			args->transparency = arg ? atoi(arg) : 150;
			break;
		case 'o':
			args->output_file = arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc };

int main(int argc, char **argv){

	struct arguments args;

	args.valid = 0;
	args.verbose = 0;
	args.png_mode = 0;
	args.transparency = -1;
	args.flag_counter = 0;
	memset(args.flags, '\0', sizeof(args.flags));
	args.output_file = NULL;

	argp_parse(&argp, argc, argv, 0, 0, &args);

	const char *base_url = "http://wttr.in/";
	char *target;
	target = malloc(strlen(base_url)+strlen(args.location)+(args.png_mode*4)+sizeof(args.flags)+1);
	strcpy(target, base_url);
	strcat(target, args.location);
	const char *seperator = args.png_mode ? "_" : "?";
	args.flag_counter==0 ? 0 : strcat(target, seperator);
	strcat(target, args.flags);
	args.png_mode ? strcat(target, ".png") : 0; 

	int arr_len;
	if(args.output_file!=NULL){
		arr_len =  MAX(strlen(target), strlen(args.output_file));
	}else{
		arr_len = strlen(target);
	}
	char *curl_args[arr_len];
	curl_args[0] = "curl";
	curl_args[1] = target;

	if(args.output_file != NULL){
		curl_args[2] = "-o";
		char *file = malloc((strlen(args.output_file)+4)*sizeof(char));
		strcpy(file, args.output_file);
		args.png_mode ? strcat(file, ".png") : strcat(file, ".txt");
		curl_args[3] = file;
		curl_args[4] = NULL;
	}else{
		curl_args[2] = NULL;
	}

	execvp(curl_args[0], curl_args);

	return 0;

}