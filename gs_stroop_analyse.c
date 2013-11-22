#include <stdlib.h>
#include <stdio.h>

#include "gs_stroop_subjects.h"
#include "gs_stroop_analyse.h"




int gs_stroop_analyse_subject_fixedblocks (subject * a_subject, analyse_trials_flag trials) {

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

  a_subject->DVs.fixed_inhibition_score = 
    a_subject->DVs.fixed_incongruent_CN_RT - a_subject->DVs.fixed_congruent_CN_RT; 

  printf ("Mean RTs: CN: neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs.fixed_neutral_CN_RT, 
	  a_subject->DVs.fixed_congruent_CN_RT, 
	  a_subject->DVs.fixed_incongruent_CN_RT);

  printf ("Mean RTs: WR: neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs.fixed_neutral_WR_RT, 
	  a_subject->DVs.fixed_congruent_WR_RT, 
	  a_subject->DVs.fixed_incongruent_WR_RT);

  printf ("Inhibition score (RTi - RTc): %5.3f\n", a_subject->DVs.fixed_inhibition_score);

  return 0;

}


int gs_stroop_analyse_subject_mixedblocks (subject * a_subject, analyse_trials_flag trials) {

  // generate RT vector for trial position

  int runlength = a_subject->num_mixed_trials_in_run;
  int total_RT[runlength]; // sums RT for each trial position across all runs
  int count_trials[runlength]; // number of valid eg., CORRECT/INCORRECT/ALL trials
  double mean_RT[runlength];
  int run, trial;

  // zero total_RT
  for (trial = 0; trial < runlength; trial ++) {
    total_RT[trial] = 0;
  }

  for (run = 0; run < a_subject->num_mixed_runs; run ++) {
    for (trial = 0; trial < runlength; trial ++) {

      stroop_trial_data * this_trial = &(a_subject->mixed_trials[run][trial]); 

      switch (trials) {
      case ALL_TRIALS: 
	total_RT[trial] += this_trial->response_time;
	count_trials[trial] ++;
	break;

      case CORRECT_TRIALS:
	if ((this_trial->response %3) == (this_trial->stim_correct_response %3)) {
	  total_RT[trial] += this_trial->response_time;
	  count_trials[trial] ++;
	}
	break;
	
      case INCORRECT_TRIALS:
	if ((this_trial->response %3) != (this_trial->stim_correct_response %3)) {
	  total_RT[trial] += this_trial->response_time;
	  count_trials[trial] ++;
	}
	break;
      }
    }
  }

  // calc mean RT for each trial    
  for (trial = 0; trial < runlength; trial ++) {
    mean_RT[trial] = (double) total_RT[trial] / a_subject->num_mixed_runs;
  }


  // calc 'task switching' scores
  a_subject->DVs.mixed_TSwc = mean_RT[4] - (mean_RT[5] + mean_RT[6] + mean_RT[7])/3;
  a_subject->DVs.mixed_TScw = mean_RT[8] - (mean_RT[9] + mean_RT[10] + mean_RT[11])/3;

  printf ("Task switching scores: TSwc: %5.3f, TScw: %5.3f\n", 
	  a_subject->DVs.mixed_TSwc, a_subject->DVs.mixed_TScw);

  return 0;
}
