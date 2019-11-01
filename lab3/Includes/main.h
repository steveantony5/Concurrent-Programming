/***********************************
            Includes
***********************************/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include "sort_algorithm.h"
#include <omp.h>

/***********************************
            Globals
***********************************/
int32_t total_elts;