/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: mruby (github.com/mruby/mruby)-----------------------------
------ Description: Part of the mruby-strip tool for the Ruby programming------
------ language reference implementation, this function simply parses the input
------ arguments fed to the tool through the command line.---------------------
------ Location:mruby/mrbgems/mruby-bin-strip/tools/mruby-strip/mruby-strip.c*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

/* By default, we understand bool (as a convenience). */
typedef int bool;
#define FALSE 0
#define TRUE 1

/* Forward declarations */

/* Type definitions */
struct strip_args {int argc_start; int argc; char** argv; int lvar; } ;

/* Variables and functions */

__attribute__((used)) static int
parse_args(int argc, char **argv, struct strip_args *args)
{
  int i;

  args->argc_start = 0;
  args->argc = argc;
  args->argv = argv;
  args->lvar = FALSE;

  for (i = 1; i < argc; ++i) {
    const size_t len = strlen(argv[i]);
    if (len >= 2 && argv[i][0] == '-') {
      switch (argv[i][1]) {
      // INSERT HERE
      // START 1
      case 'l':
        args->lvar = TRUE;
        break;
      // END 1
      // START 2
      case '-':
        if (strncmp((*argv) + 2, "lvar", len) == 0) {
          args->lvar = TRUE;
        }
        break;
      // END 2
      default:
		printf("got here, argv[%d][1] = %c\n", i, argv[i][1]);
        return -1;
      }
    }
    else {
      break;
    }
  }

  args->argc_start = i;
  return i;
}

int input_argc;
char **input_argv;
struct strip_args *input_args;

void setup(unsigned long SIZE1, char *elements, float *chances, int num_elements) {
	input_argc = SIZE1;
	input_argv = (char **) malloc(SIZE1*sizeof(char*));
	for (unsigned long i = 0; i < SIZE1; i++) {
		input_argv[i] = (char *) malloc(3 * sizeof(char));
		input_argv[i][0] = '-';
		float prob = rand()/(float)RAND_MAX;
		int chance_index = -1;
		float total_prob = 0.0;
		do {
			total_prob += chances[++chance_index];
		} while (total_prob < prob && chance_index < num_elements);
		input_argv[i][1] = elements[chance_index];
		input_argv[i][2] = '\0';
	}
	input_args = (struct strip_args*) malloc(sizeof(struct strip_args));
}

int parse(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Syntax: %s SIZE1 CHANCE_FIRST\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE1 = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    assert(CHANCE_FIRST >= 0.0 && CHANCE_FIRST <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST};
    char elements[] = {'l', '-'};

    setup(SIZE1, elements, chances, 1);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    parse_args(input_argc, input_argv, input_args);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 127;
  }
  return 0;
}
