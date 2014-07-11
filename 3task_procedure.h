#ifndef 3task_procedure_h
#define 3task_procedure_h

#include <stdbool.h>
#include <gtk/gtk.h>
#include <gsl/gsl_rng.h>


typedef struct three_task_data {

  int trial_id;
  int cue; // task type: 0, 1, 2
  int stim_0;
  int stim_1;
  int stim_2;
  int stim_correct_response;
  int response;
  int response_time;

} ThreeTaskData;


#endif
