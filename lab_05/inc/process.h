#ifndef _PROCESS_H_

#include "arrqueue.h"
#include "listqueue.h"
#include "task.h"
#include "defines.h"
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>

typedef struct
{
    int tasks_in;
    int tasks_out;
    int tasks_failed;
    int calls;
    int overall_len;
} sim_log_t;

float random_float(float min, float max);
float max(float a, float b, float c);
float min(float a, float b, float c);
void simulate_arr();
void simulate_list();
void make_and_del_arr();
void make_and_del_list();
void efficiency();
void hand_mode();

#define _PROCESS_H_
#endif