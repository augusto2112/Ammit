/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: FreeBSD (github.com/freebsd/freebsd)-----------------------
------ Description: Part of the FreeBSD kernel's file system, this function----
------ checks if the timestamps of a given file are within a given value range.
------ Location: freebsd/bin/pax/sel_subs.c----------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/* By default, we understand bool (as a convenience). */
typedef long scalar_t__;  // Either arithmetic or pointer type.
typedef int bool;
#define false 0
#define true 1

/* Type definitions */
struct AUX {int st_mtime; int st_ctime; } ;
struct ARCHD {struct AUX sb; } ;
struct TIME_RNG {int flgs; struct TIME_RNG* fow; int high_time; int low_time; } ;

/* Variables and functions */
#define  CMPCTME 0x1
#define  CMPMTME 0x2
#define  CMPBOTH 0x3
#define  HASHIGH 0x4
#define  HASLOW 0x8

struct TIME_RNG* trhead ; 

__attribute__((used)) static int
trng_match(struct ARCHD *arcn)
{
	struct TIME_RNG *pt;
    long long int s1 = 0, s2 = 0, s3 = 0;

	/*
	 * have to search down the list one at a time looking for a match.
	 * remember time range limits are inclusive.
	 */
	pt = trhead;
	while (pt != NULL) {
		switch(pt->flgs & CMPBOTH) {
        // INSERT HERE
        // START 1
		case CMPBOTH:
			/*
			 * user wants both mtime and ctime checked for this
			 * time range
			 */
            s1++;
			if (((pt->flgs & HASLOW) &&
			    (arcn->sb.st_mtime < pt->low_time) &&
			    (arcn->sb.st_ctime < pt->low_time)) ||
			    ((pt->flgs & HASHIGH) &&
			    (arcn->sb.st_mtime > pt->high_time) &&
			    (arcn->sb.st_ctime > pt->high_time))) {
				pt = pt->fow;
				continue;
			}
			break;
        // END 1
        // START 2
		case CMPCTME:
            s2++;
			/*
			 * user wants only ctime checked for this time range
			 */
			if (((pt->flgs & HASLOW) &&
			    (arcn->sb.st_ctime < pt->low_time)) ||
			    ((pt->flgs & HASHIGH) &&
			    (arcn->sb.st_ctime > pt->high_time))) {
				pt = pt->fow;
				continue;
			}
			break;
        // END 2
        // START 3
		case CMPMTME:
		default:
            s3++;
			/*
			 * user wants only mtime checked for this time range
			 */
			if (((pt->flgs & HASLOW) &&
			    (arcn->sb.st_mtime < pt->low_time)) ||
			    ((pt->flgs & HASHIGH) &&
			    (arcn->sb.st_mtime > pt->high_time))) {
				pt = pt->fow;
				continue;
			}
			break;
        // END 3
		}
		break;
	}

    volatile int force_opt = 1;
    fprintf(stdout, "both%lli,ctme%lli,mtme%lli\n", s1, s2, s3);

	if (pt == NULL)
		return(1);
	return(0);
}

struct ARCHD *input_archd;
void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  input_archd = (struct ARCHD*) malloc(sizeof(struct ARCHD));
  input_archd->sb.st_mtime = 1;
  input_archd->sb.st_ctime = 1;

  trhead = (struct TIME_RNG*) malloc(SIZE*sizeof(struct TIME_RNG));

  struct TIME_RNG* curr = trhead;
  for (unsigned long i = 0; i < SIZE; i++) {
    curr->high_time = 0;
    curr->low_time = 0;

    curr->flgs |= HASHIGH;
    curr->flgs |= HASLOW;

    curr->fow = (struct TIME_RNG*) malloc(sizeof(struct TIME_RNG));
    float prob = rand()/(float)RAND_MAX;
    int chance_index = -1;
    float total_prob = 0.0;
    do {
      total_prob += chances[++chance_index];
    } while (total_prob < prob && chance_index < num_elements);
    curr->flgs |= elements[chance_index];
    curr = curr->fow;
  }

  curr->fow = NULL;
}

int parse(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST CHANCE_SECOND\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);
    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND};
    int elements[] = {CMPBOTH, CMPCTME, CMPMTME};

    setup(SIZE, elements, chances, 2);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    trng_match(input_archd);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
