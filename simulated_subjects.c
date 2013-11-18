

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "simulated_subjects.h"

// #define NUMBER_OF_SUBJECTS 100



stroop_trial_data * stroop_trial_data_create (int id, blocktype block_type, int stim_task, 
					      int stim_word, int stim_colour) {
  stroop_trial_data * a_data_container = malloc (sizeof (stroop_trial_data));
  a_data_container->trial_id = id;
  a_data_container->block_type = NONE;
  a_data_container->stim_task = stim_task;
  a_data_container->stim_word = stim_word;
  a_data_container->stim_colour = stim_colour;

  a_data_container->response = 0;
  a_data_container->response_time = 0;

  // stim_task is word naming if =0, colour otherwise 
  (stim_task == 0) ? 
    (a_data_container->stim_correct_response = stim_word) : 
    (a_data_container->stim_correct_response = stim_colour);

  return a_data_container;
} 
		     



/* subject constructor */
subject * subject_create (int num_of_trials) {

  subject * new_subject = malloc (sizeof(subject));
  new_subject->num_of_trials = num_of_trials;
  new_subject->trials = g_array_sized_new (FALSE, FALSE, sizeof(stroop_trial_data), num_of_trials);

  return new_subject;

}


void subject_free (subject * subject_to_free) {

  free (subject_to_free->params);
  g_array_free (subject_to_free->trials, TRUE); // 2nd arg frees the data as well

  return;
}


subject_popn * subject_popn_create (int number_of_subjects) {

  subject_popn * some_subjects;
  some_subjects = malloc (sizeof(subject_popn));
  some_subjects->subjects = g_array_sized_new (FALSE, FALSE, sizeof(subject), number_of_subjects);
  some_subjects->number_of_subjects = number_of_subjects;

  return some_subjects;

}


void subject_popn_free (subject_popn * some_subjects) {
  g_array_free (some_subjects->subjects, TRUE);
  free (some_subjects);

  return;
}


/* 
int main () {

  subject_popn * my_subjects = subject_popn_create (NUMBER_OF_SUBJECTS);
  
  subject_popn_free (my_subjects); 

  return 0;

}
*/
