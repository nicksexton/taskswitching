#include <stdlib.h>
#include <stdio.h>

#include "gs_stroop_subjects.h"
#include "gs_stroop_analyse.h"


int gs_stroop_analyse_subject (subject * a_subject, analyse_trials_flag trials) {

  // analyse fixed blocks

  int total_RT[2][3] = {{0, 0, 0}, {0, 0, 0}}; // [stim_task][trial_type]
  int counter[2][3] = {{0, 0, 0}, {0, 0, 0}};

  int i;
  int num_trials = a_subject->num_fixed_trials;
  
  

  for (i = 0; i < num_trials; i ++) {

    stroop_trial_data * this_trial = &(a_subject->fixed_trials[i]); 

    switch (trials) {
    case ALL_TRIALS: 

      total_RT[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
      counter[this_trial->stim_task][this_trial->trial_type] ++;
      break;

    case CORRECT_TRIALS:
      if ((this_trial->response %3) == (this_trial->stim_correct_response %3)) {
	total_RT[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
	counter[this_trial->stim_task][this_trial->trial_type] ++;
      }
      break;

    case INCORRECT_TRIALS:
      if ((this_trial->response %3) != (this_trial->stim_correct_response %3)) {
	total_RT[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
	counter[this_trial->stim_task][this_trial->trial_type] ++;
      }
      break;

    }
  }

  a_subject->DVs.fixed_neutral_CN_RT = (double) total_RT[1][0] / counter[1][0];
  a_subject->DVs.fixed_congruent_CN_RT  = (double) total_RT[1][1] / counter[1][1];
  a_subject->DVs.fixed_incongruent_CN_RT  = (double) total_RT[1][2] / counter[1][2];

  a_subject->DVs.fixed_neutral_WR_RT  = (double) total_RT[0][0] / counter[0][0];
  a_subject->DVs.fixed_congruent_WR_RT  = (double) total_RT[0][1] / counter[0][1];
  a_subject->DVs.fixed_incongruent_WR_RT  = (double) total_RT[0][2] / counter[0][2];

  printf ("\n Mean RTs: CN: neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs.fixed_neutral_CN_RT, 
	  a_subject->DVs.fixed_congruent_CN_RT, 
	  a_subject->DVs.fixed_incongruent_CN_RT);

  printf ("\n Mean RTs: WR: neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs.fixed_neutral_WR_RT, 
	  a_subject->DVs.fixed_congruent_WR_RT, 
	  a_subject->DVs.fixed_incongruent_WR_RT);

  return 0;

}
