#include <stdlib.h>
#include <glib.h>

#include "simulated_subjects.h"
#include "gs_stroop_subjects.h"



stroop_trial_data stroop_trial_data_create (int id, blocktype block_type, int stim_task, 
					      int stim_word, int stim_colour) {
  // stroop_trial_data * a_data_container = malloc (sizeof (stroop_trial_data));
  stroop_trial_data a_data_container;
  a_data_container.trial_id = id;
  a_data_container.block_type = NONE;
  a_data_container.stim_task = stim_task;
  a_data_container.stim_word = stim_word;
  a_data_container.stim_colour = stim_colour;

  a_data_container.response = 0;
  a_data_container.response_time = 0;

  // stim_task is word naming if =0, colour otherwise 
  (stim_task == 0) ? 
    (a_data_container.stim_correct_response = stim_word) : 
    (a_data_container.stim_correct_response = stim_colour);

  return a_data_container;
} 
		     


/* subject constructor */ // TODO - incl subject parameters as argument
subject * subject_create (int num_of_trials) {

  subject * new_subject = malloc (sizeof(subject));
  new_subject->params = (gs_stroop_params*) malloc (sizeof(gs_stroop_params));
  new_subject->num_of_trials = num_of_trials;
  new_subject->trials = g_array_sized_new (FALSE, FALSE, sizeof(stroop_trial_data), num_of_trials);
  // g_array_set_clear_func (new_subject->trials, (gpointer) stroop_trial_data_free); // Might not work

  // TODO - code to create params object here 
  ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_inhibitory = -2.5; // TEMP CODE!!!
  ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_excitatory = 2.5;

  return new_subject;

}
