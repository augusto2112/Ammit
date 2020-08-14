/*------------------------ Project Ammit Test ---------------------------------
------ Source repo: ReactOS (github.com/reactos/reactos)-----------------------
------ Description: Part of the ReactOS operating system's implementation of---
------ Windows' Unicode rendering system, this function manipulates the shape
------ of Unicode characters to be rendered.-----------------------------------
------ Location: reactos/dll/win32/usp10/shape.c-----------------------------*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long size_t;  // Customize by platform.
typedef long intptr_t; typedef unsigned long uintptr_t;
typedef long scalar_t__;  // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int boolean;
#define false 0
#define true 1

/* Forward declarations */
typedef  struct TYPE_5__   TYPE_2__ ;
typedef  struct TYPE_4__   TYPE_1__ ;

/* Type definitions */
typedef  int /*<<< orphan*/  WORD ;
typedef  int WCHAR ;
struct TYPE_4__ {int /*<<< orphan*/  wgBlank; int /*<<< orphan*/  wgDefault; } ;
struct TYPE_5__ {TYPE_1__ sfp; } ;
typedef  TYPE_2__ ScriptCache ;
typedef  int /*<<< orphan*/  SCRIPT_ANALYSIS ;
typedef  int INT ;
typedef  int /*<<< orphan*/  HDC ;

#define FIRST 0x000A
#define SECOND 0x000D
#define THIRD 0x1B
#define FOURTH 0x1C


/* Variables and functions */

__attribute__((used)) static void ContextualShape_Control(HDC hdc, ScriptCache *psc, SCRIPT_ANALYSIS *psa, WCHAR* pwcChars, INT cChars, WORD* pwOutGlyphs, INT* pcGlyphs, INT cMaxGlyphs, WORD *pwLogClust)
{
    int i;
    
    for (i=0; i < cChars; i++)
    {

        switch (pwcChars[i])
        {
            // INSERT HERE
			// START 1
            case FIRST:
            case SECOND:
                pwOutGlyphs[i] = psc->sfp.wgBlank;
                break;
            // END 1
            // START 2
            default:
                if (pwcChars[i] < FOURTH)
                    pwOutGlyphs[i] = psc->sfp.wgDefault;
                else
                    pwOutGlyphs[i] = psc->sfp.wgBlank;
                break;
            // END 2

        }
    }
}

ScriptCache *psc;
WCHAR* pwcChars;
INT cChars;
WORD* pwOutGlyphs;

void setup(unsigned long SIZE, int *elements, float *chances, int num_elements) {
  psc = (ScriptCache *)malloc(sizeof(ScriptCache));
  psc->sfp.wgBlank = rand();
  psc->sfp.wgDefault = rand();
  cChars = SIZE;

  pwcChars = (WCHAR *)malloc(SIZE * sizeof(WCHAR));
  pwOutGlyphs = (WORD *)malloc(SIZE * sizeof(WORD));

  for (unsigned long i = 0; i < SIZE; i++) {
      float prob = rand()/(float)RAND_MAX;
      int chance_index = -1;
      float total_prob = 0.0;
      do {
        total_prob += chances[++chance_index];
      } while (total_prob < prob && chance_index < num_elements);
      pwcChars[i] = elements[chance_index];      
  }
}


int parse(int argc, char** argv) {
  if (argc != 5) {
    fprintf(stderr, "Sintax: %s SIZE CHANCE_FIRST CHANCE_SECOND CHANCE_THIRD\n", argv[0]);
    return 0;
  } else {
    const unsigned SIZE = atoi(argv[1]);
    const float CHANCE_FIRST = atof(argv[2]);
    const float CHANCE_SECOND = atof(argv[3]);
    const float CHANCE_THIRD = atof(argv[4]);

    assert(CHANCE_FIRST + CHANCE_SECOND >= 0.0 && CHANCE_FIRST + CHANCE_SECOND <= 1.0 && 
    "Element distribution probabilities should be between 0 and 1");

    float chances[] = {CHANCE_FIRST, CHANCE_SECOND, CHANCE_THIRD};
    int elements[] = {FIRST, SECOND, THIRD, FOURTH};

    setup(SIZE, elements, chances, 3);
    return 1;
  }
}

int main(int argc, char** argv) {
  if (parse(argc, argv)) {
    clock_t start;
    clock_t end;
    start = clock();
    ContextualShape_Control(0, psc, NULL, pwcChars, cChars, pwOutGlyphs, NULL, 0, NULL);
    end = clock();
    double seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%.2lf", seconds);
  } else {
    return 1;
  } 
}
