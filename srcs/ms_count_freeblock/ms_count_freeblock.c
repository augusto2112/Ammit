/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: Linux Kernel (github.com/torvalds/linux)-------------------
------ Description: Part of the Linux kernel's drivers for ENE Technology------
------ USB Mass Storage drives, this function counts the number of free memory-
------ blocks in the device's physical blocks.---------------------------------
------ Location: linux/drivers/usb/storage/ene_ub6250.c----------------------*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* By default, we understand bool (as a convenience). */
typedef int boolean;
#define false 0
#define true 1

/* Forward declarations */
typedef  struct TYPE_2__   TYPE_1__ ;

/* Type definitions */
typedef  size_t u32 ;
typedef  size_t u16 ;
struct us_data {scalar_t__ extra; } ;
struct TYPE_2__ {int* Phy2LogMap; } ;
struct ene_ub6250_info {TYPE_1__ MS_Lib; } ;

/* Variables and functions */
#define FIRST 600 
#define SECOND 601
#define THIRD 602 
 size_t MS_PHYSICAL_BLOCKS_PER_SEGMENT ; 

__attribute__((used)) static int ms_count_freeblock(struct us_data *us, u16 PhyBlock)
{
	u32 Ende, Count;
	struct ene_ub6250_info *info = (struct ene_ub6250_info *) us->extra;

	Ende = PhyBlock + MS_PHYSICAL_BLOCKS_PER_SEGMENT;
	for (Count = 0; PhyBlock < Ende; PhyBlock++) {
		switch (info->MS_Lib.Phy2LogMap[PhyBlock]) {
		// INSERT HERE
		// START 1
		case FIRST:
		// END 1
		// START 2
		case SECOND:
		// END 2
			Count++;
		default:
			break;
		}
	}

	return Count;
}

struct us_data *us;
u16 PhyBlock;
void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  us = (struct us_data *)malloc(sizeof(struct us_data));
  struct ene_ub6250_info *info= (struct ene_ub6250_info *)malloc(sizeof(struct ene_ub6250_info));
  info->MS_Lib.Phy2LogMap = (int *)malloc(SIZE * sizeof(int));
  us->extra = (long)info;
  PhyBlock = 0;
  MS_PHYSICAL_BLOCKS_PER_SEGMENT = SIZE;

  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      info->MS_Lib.Phy2LogMap[i] = elements[chance_index];      
  }
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
    int elements[] = {FIRST, SECOND, THIRD};

    setup(SIZE, elements, chances, 2);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
	  volatile int i = ms_count_freeblock(us, PhyBlock);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
