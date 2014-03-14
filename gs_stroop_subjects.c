#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>

// #include "simulated_subjects.h"
#include "gs_stroop_subjects.h"



subject_popn * subject_popn_create (int number_of_subjects) {

  subject_popn * some_subjects; // array of pointers to subjects
  some_subjects = malloc (sizeof(subject_popn));
  // some_subjects->subjects = g_array_sized_new (FALSE, FALSE, sizeof(subject), number_of_subjects);

  some_subjects->subj = malloc (number_of_subjects * sizeof(subject *));  
  some_subjects->number_of_subjects = number_of_subjects;

  return some_subjects;

}


void subject_popn_free (subject_popn * some_subjects) {

  // g_array_free (some_subjects->subjects, TRUE);

  int i;
  for (i = 0; i < some_subjects->number_of_subjects; i++) {
    subject_free (some_subjects->subj[i]);
  }
  free (some_subjects->subj);
  free (some_subjects);

  return;
}



// function currently super inefficent. reimplement?
stroop_trial_data stroop_trial_data_create (int id, 
					    blocktype block_type, 
					    trialtype trial_type, 
					    int stim_task, int stim_word, int stim_colour) {
  // stroop_trial_data * a_data_container = malloc (sizeof (stroop_trial_data));
  stroop_trial_data a_data_container;
  a_data_container.trial_id = id;
  a_data_container.block_type = block_type;
  a_data_container.trial_type = trial_type;
  a_data_container.stim_task = stim_task;
  a_data_container.stim_word = stim_word;
  a_data_container.stim_colour = stim_colour;

  a_data_container.response = -99;
  a_data_container.response_time = -99;

  // stim_task is word naming if =0, colour otherwise 
  (stim_task == 0) ? 
    (a_data_container.stim_correct_response = stim_word) : 
    (a_data_container.stim_correct_response = stim_colour);

  return a_data_container;
} 
		     

// returns a pointer to the specified trial in a fixed block
// need separate access function to get pointer to the trial in a mixed block
// pointer to const - do not use this function to write to trial data structure
const stroop_trial_data * fixed_block_trial_data_get (subject_popn * my_subjects, 
						      int subject_id, 
						      int trial_id) {
  stroop_trial_data * trial_data;
  trial_data = &(my_subjects->subj[subject_id]->fixed_trials[trial_id]);
  return trial_data;

}

bool stroop_trial_data_print_as_string (char * destination_string, int destination_length, const stroop_trial_data * trial_data) {

  char trialtype[12];
  char tasktype[13];

  int word_inputs[3] = {0, 0, 0};
  int colour_inputs[3] = {0, 0, 0};
  int correct[3] = {0, 0, 0};
  
  char textbuf[100];

  if (destination_length > 100) {
    return false;
  }

  switch (trial_data->trial_type) {
  case NEUTRAL: { strncpy (trialtype, "NEUTRAL", 12); break; }
  case CONGRUENT: { strncpy (trialtype, "CONGRUENT", 12); break; }
  case INCONGRUENT: { strncpy (trialtype, "INCONGRUENT", 12); break; }
  }
  switch (trial_data->stim_task) {
  case WORDREADING: { strncpy (tasktype, "WORDREADING", 13); break; }
  case COLOURNAMING: { strncpy (tasktype, "COLOURNAMING", 13); break; }
  }

  word_inputs[trial_data->stim_word] = 1;
  colour_inputs[trial_data->stim_colour] = 1;
  correct[trial_data->stim_correct_response % 3] = 1; 

  snprintf (textbuf, 100, "Input:[%d, %d, %d], [%d, %d, %d], Correct: [%d, %d, %d]\t%s\t%s", 
	    word_inputs[0], word_inputs[1], word_inputs[2], 
	    colour_inputs[0], colour_inputs[1], colour_inputs[2], 
	    correct[0], correct[1], correct[2],
	    trialtype, tasktype);

  strncpy (destination_string, textbuf, destination_length);

  return true;

}



/* subject constructor */ // TODO - incl subject parameters as argument
subject * subject_create (int num_fixed_trials, 
			  int num_mixed_runs, int num_mixed_trials_in_run) {

  int i;
  subject * new_subject = malloc (sizeof(subject)); 
  new_subject->params = (gs_stroop_params*) malloc (sizeof(gs_stroop_params));

  new_subject->num_fixed_trials = num_fixed_trials;
  new_subject->num_mixed_runs = num_mixed_runs;
  new_subject->num_mixed_trials_in_run = num_mixed_trials_in_run;

  new_subject->fixed_trials = malloc (num_fixed_trials * sizeof(stroop_trial_data));

  // mixed_trials is a (eg, 12x100) 2d array, implemented as an array of pointers to arrays
  new_subject->mixed_trials = malloc (num_mixed_runs * sizeof(stroop_trial_data*));

  for (i = 0; i < num_mixed_runs; i++) {
    new_subject->mixed_trials[i] = malloc(num_mixed_trials_in_run * sizeof(stroop_trial_data));
  }


  // TODO - code to create params object here 
  // ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_inhibitory = -2.5; // TEMP CODE!!!
  // ((gs_stroop_params*)(new_subject->params))->taskdemand_weights_excitatory = 2.5;

  return new_subject;

}


void subject_free (subject * subject_to_free) {

  int i;

  free (subject_to_free->params);
  free (subject_to_free->fixed_trials);  

  for (i = 0; i < subject_to_free->num_mixed_runs; i ++) {
    free (subject_to_free->mixed_trials[i]);
  }
  free (subject_to_free->mixed_trials);

  free (subject_to_free);

  
  return;
}


int subject_params_vary(subject * some_subject, double td_wt_inh, double td_wt_exc) {

  ((gs_stroop_params*)(some_subject->params))->
    taskdemand_weights_inhibitory = td_wt_inh; // TEMP CODE!!!
  ((gs_stroop_params*)(some_subject->params))->
    taskdemand_weights_excitatory = td_wt_exc;

  return 0;

}




// Creates block of num_trials in proportion N:C:I 
int subject_init_trialblock_fixed (const gsl_rng * random_generator, 
				   subject * a_subject, 
				   int ppn_N, int ppn_C, int ppn_I,
				   int ppn_WR, int ppn_CN) {

  // TODO - check that the subject is init'd and does not already have trials init'd

 
// first create an array to randomly permute:
  
  int num_trials = a_subject->num_fixed_trials;    
  trialtype trial_order[num_trials];
  int task_order[num_trials]; // 0 = word reading, 1 = colour naming
  int num_N, num_C, num_WR, i;
  

  int ppn_total = ppn_N + ppn_C + ppn_I; // explicit cast for double division
  int task_ppn_total = ppn_WR + ppn_CN;
  
  // stroop_trial_data * some_data;

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
      
      /*
      // TEST CODE
      if (task_order[i] == 0) {
	stim_word = 0;
	stim_colour = -1;
      }
      else {
	stim_word = -1;
	stim_colour = 0;
      }
      */
      break;

    case CONGRUENT:
       
      stim_word = gsl_rng_uniform_int (random_generator, 3); 
      stim_colour = stim_word;
      
      // TEST CODE
      /*
      stim_word = 0;
      stim_colour = 0;
      */

      break;
    
    case INCONGRUENT: 
      
      stim_word = gsl_rng_uniform_int (random_generator, 3); 
      int incr = gsl_rng_uniform_int (random_generator, 2);
      stim_colour = (stim_word + incr) % 3;
      
      /*
      // TEST CODE
      stim_word = 0;
      stim_colour = 1;
      */
      break;
    
    }  

    a_subject->fixed_trials[i] = 
      stroop_trial_data_create (i, FIXED, 
				trial_order[i], task_order[i], 
				stim_word, stim_colour); 
  }					  
  return 0;
}

// num_trials = number of trials to a run (default, 12)
// num_runs = number of runs to a blocks (default, 100)
int subject_init_trialblock_mixed ( subject * a_subject) {

  int trial, run;
  tasktype task;

  for (run = 0; run < a_subject->num_mixed_runs; run ++) {
    for (trial = 0; trial < a_subject->num_mixed_trials_in_run; trial ++) {
 
      // establish which task
      if (trial / 4 >= 1) {
	if (trial / 4 >= 2) {
	  task = WORDREADING;
	}
	else task = COLOURNAMING;
      }
      else task = WORDREADING;

      a_subject->mixed_trials[run][trial] = stroop_trial_data_create (trial, MIXED, 
								      INCONGRUENT, task, 
								      0, 1);  
      // NB in mixed blocks, stimuli are identical for all trials

    }
  }

  return 0;
}


int subject_print_fixed_trial_data (subject * a_subject) {
  
  int trial;
  for (trial = 0; trial < a_subject->num_fixed_trials; trial ++) {

  // printf ("trialid\ttrial\ttask\tWin\tCin\tcorrect\trespns\trt\n");

    printf ("%d:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
    	    a_subject->fixed_trials[trial].trial_id,
    	    a_subject->fixed_trials[trial].trial_type, 
    	    a_subject->fixed_trials[trial].stim_task,
   	    a_subject->fixed_trials[trial].stim_word,
    	    a_subject->fixed_trials[trial].stim_colour,
    	    a_subject->fixed_trials[trial].stim_correct_response,
    	   (a_subject->fixed_trials[trial].response % 3), // disambiguate the response 
   	    a_subject->fixed_trials[trial].response_time);
  }
  return 0;

 }
