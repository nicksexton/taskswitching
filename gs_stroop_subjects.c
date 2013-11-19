#include <stdlib.h>
#include <glib.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

#include "simulated_subjects.h"
#include "gs_stroop_subjects.h"



stroop_trial_data stroop_trial_data_create (int id, blocktype block_type, int stim_task, 
					      int stim_word, int stim_colour) {
  // stroop_trial_data * a_data_container = malloc (sizeof (stroop_trial_data));
  stroop_trial_data a_data_container;
  a_data_container.trial_id = id;
  a_data_container.block_type = block_type;
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
subject * subject_create (int num_fixed_trials) {

  subject * new_subject = malloc (sizeof(subject));
  new_subject->params = (gs_stroop_params*) malloc (sizeof(gs_stroop_params));
  new_subject->num_fixed_trials = num_fixed_trials;
  new_subject->fixed_trials = 
    g_array_sized_new (FALSE, FALSE, sizeof(stroop_trial_data), num_fixed_trials);
  // g_array_set_clear_func (new_subject->trials, (gpointer) stroop_trial_data_free); // Might not work

  // TODO - code to create params object here 
  ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_inhibitory = -2.5; // TEMP CODE!!!
  ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_excitatory = 2.5;

  return new_subject;

}


// Creates block of num_trials in proportion N:C:I 
int subject_init_trialblock_fixed (gsl_rng * random_generator, 
				   subject * a_subject, 
				   int num_trials, 
				   int ppn_N, int ppn_C, int ppn_I,
				   int ppn_WR, int ppn_CN) {

  // TODO - check that the subject is init'd and does not already have trials init'd

// first create an array to randomly permute:
  trialtype trial_order[num_trials];
  int task_order[num_trials]; // 0 = word reading, 1 = colour naming
  int num_N, num_C, num_WR, i;
  
  int ppn_total = ppn_N + ppn_C + ppn_I; // explicit cast for double division
  int task_ppn_total = ppn_WR + ppn_CN;
  
  stroop_trial_data some_data;

  num_N = num_trials * ppn_N / ppn_total; 
  num_C = num_trials * ppn_C / ppn_total; 
  
  num_WR = num_trials * ppn_WR / task_ppn_total;
  

  for (i = 0; i < num_trials; i++) { 
    

    if (i < num_N) 
      trial_order[i] = NEUTRAL; 
    else if (i < (num_N + num_C))
      trial_order[i] = CONGRUENT;
    else 
      trial_order[i] = INCONGRUENT;

    if (i < num_WR)
      task_order[i] = 0; // word reading
    else
      task_order[i] = 1; // colour naming

  }

  gsl_ran_shuffle (random_generator, trial_order, num_trials, sizeof(trialtype));
  gsl_ran_shuffle (random_generator, task_order, num_trials, sizeof(int));

  for (i = 0; i < num_trials; i++) {
    
    int stim_word, stim_colour;

    switch (trial_order[i]) {
    case NEUTRAL: 
      if (task_order[i] == 0) {
	stim_word = gsl_rng_uniform_int (random_generator, 3); 
	stim_colour = -1;
      }
      else {
	stim_word = -1;
	stim_colour = gsl_rng_uniform_int (random_generator, 3);
      } 
      break;
    

    case CONGRUENT: 
      stim_word = gsl_rng_uniform_int (random_generator, 3); 
      stim_colour = stim_word;
      break;
    
    case INCONGRUENT: 
      stim_word = gsl_rng_uniform_int (random_generator, 3); 
      int incr = gsl_rng_uniform_int (random_generator, 2);
      stim_colour = (stim_word + incr) % 3;
	break;
    
    }  

    some_data = stroop_trial_data_create (i, FIXED, task_order[i], stim_word, stim_colour); 

    // write trials data to the array
    g_array_append_val (a_subject->fixed_trials, some_data);
  }					  

  return 0;
}
