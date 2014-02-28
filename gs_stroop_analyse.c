#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gs_stroop_global_params.h"
#include "gs_stroop_subjects.h"
#include "gs_stroop_analyse.h"


// takes [2][3] matrices
static int gs_stroop_record_means (struct subject_aggregate_data * my_data_means,
				   // struct subject_aggregate_data * my_data_sd,
					 int totals_matrix[2][3], 
					 int counter_matrix[2][3]) {

  my_data_means->fixed_neutral_CN_RT = 
               (double) totals_matrix[1][0] / counter_matrix[1][0];
  my_data_means->fixed_congruent_CN_RT  = 
               (double) totals_matrix[1][1] / counter_matrix[1][1];
  my_data_means->fixed_incongruent_CN_RT  = 
               (double) totals_matrix[1][2] / counter_matrix[1][2];

  my_data_means->fixed_neutral_WR_RT  = 
               (double) totals_matrix[0][0] / counter_matrix[0][0];
  my_data_means->fixed_congruent_WR_RT  = 
               (double) totals_matrix[0][1] / counter_matrix[0][1];
  my_data_means->fixed_incongruent_WR_RT  = 
               (double) totals_matrix[0][2] / counter_matrix[0][2];

  my_data_means->fixed_inhibition_score = 
               my_data_means->fixed_incongruent_CN_RT - 
               my_data_means->fixed_congruent_CN_RT; 
  
  // TODO - strore number of trials (eg for correct/incorrect trials)
  return 0;

}

static int gs_stroop_record_sds (struct subject_aggregate_data * my_data_means,
				   // struct subject_aggregate_data * my_data_sd,
					 int totals_matrix[2][3], 
					 int counter_matrix[2][3]) {
  // subtract 1 from counter to get df


  my_data_means->fixed_neutral_CN_RT = 
    sqrt ((double) totals_matrix[1][0] / (counter_matrix[1][0] -1));
  my_data_means->fixed_congruent_CN_RT  = 
    sqrt ((double) totals_matrix[1][1] / (counter_matrix[1][1] -1));
  my_data_means->fixed_incongruent_CN_RT  = 
    sqrt((double) totals_matrix[1][2] / (counter_matrix[1][2] -1));

  my_data_means->fixed_neutral_WR_RT  = 
    sqrt((double) totals_matrix[0][0] / (counter_matrix[0][0] -1));
  my_data_means->fixed_congruent_WR_RT  = 
    sqrt((double) totals_matrix[0][1] / (counter_matrix[0][1] -1));
  my_data_means->fixed_incongruent_WR_RT  = 
    sqrt((double) totals_matrix[0][2] / (counter_matrix[0][2] -1));

  
  // TODO - strore number of trials (eg for correct/incorrect trials)
  return 0;

}



int gs_stroop_analyse_subject_fixedblocks (subject * a_subject) {

  // analyse fixed blocks

  int total_RT_alltrials[2][3] = {{0, 0, 0}, {0, 0, 0}}; // [stim_task][trial_type]
  int counter_alltrials[2][3] = {{0, 0, 0}, {0, 0, 0}};
  int ssq_error_alltrials[2][3] = {{0, 0, 0}, {0, 0, 0}};

  int total_RT_correct[2][3] = {{0, 0, 0}, {0, 0, 0}}; // [stim_task][trial_type]
  int counter_correct[2][3] = {{0, 0, 0}, {0, 0, 0}};
  int ssq_error_correct[2][3] = {{0, 0, 0}, {0, 0, 0}};

  int total_RT_errors[2][3] = {{0, 0, 0}, {0, 0, 0}}; // [stim_task][trial_type]
  int counter_errors[2][3] = {{0, 0, 0}, {0, 0, 0}};
  int ssq_error_errors[2][3] = {{0, 0, 0}, {0, 0, 0}};


  int n;
  int num_trials = a_subject->num_fixed_trials;
  
  

  for (n = 0; n < num_trials; n ++) {

    stroop_trial_data * this_trial = &(a_subject->fixed_trials[n]); 

    total_RT_alltrials[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
    counter_alltrials[this_trial->stim_task][this_trial->trial_type] ++;


    // analyse correct trials
    if ((this_trial->response %3) == (this_trial->stim_correct_response %3)) {
      total_RT_correct[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
      counter_correct[this_trial->stim_task][this_trial->trial_type] ++;
    }
    else {  // analyse incorrect trials
      total_RT_errors[this_trial->stim_task][this_trial->trial_type] += this_trial->response_time;
      counter_errors[this_trial->stim_task][this_trial->trial_type] ++;
    }    
  }
  

  gs_stroop_record_means (&(a_subject->DVs_alltrials),
					 total_RT_alltrials, 
					 counter_alltrials);

  gs_stroop_record_means (&(a_subject->DVs_correct),
					 total_RT_correct, 
					 counter_correct);

  gs_stroop_record_means (&(a_subject->DVs_errors),
					 total_RT_errors, 
					 counter_errors);


  // treat SD as mean of ssq_var with df = N-1 
  // Calc SD

  for (n = 0; n < num_trials; n++) {
    
    stroop_trial_data * this_trial = &(a_subject->fixed_trials[n]); 

    ssq_error_alltrials[this_trial->stim_task][this_trial->trial_type] += 
      pow((this_trial->response_time - 
       (total_RT_alltrials[this_trial->stim_task][this_trial->trial_type]/
	counter_alltrials[this_trial->stim_task][this_trial->trial_type])), 2);

    if ((this_trial->response %3) == (this_trial->stim_correct_response %3)) {
      ssq_error_correct[this_trial->stim_task][this_trial->trial_type] += 
	pow((this_trial->response_time - 
	     (total_RT_correct[this_trial->stim_task][this_trial->trial_type]/
	      counter_correct[this_trial->stim_task][this_trial->trial_type])), 2);
    }
    else {
      ssq_error_errors[this_trial->stim_task][this_trial->trial_type] += 
	pow((this_trial->response_time - 
	     (total_RT_errors[this_trial->stim_task][this_trial->trial_type]/
	      counter_errors[this_trial->stim_task][this_trial->trial_type])), 2);
    }

  }

  // subtract 1 from counter matrix (to get df)


  gs_stroop_record_sds (&(a_subject->DVs_alltrials_sd),
			  ssq_error_alltrials, 
			  counter_alltrials);

  gs_stroop_record_sds (&(a_subject->DVs_correct_sd),
			  ssq_error_correct, 
			  counter_correct);

  gs_stroop_record_sds (&(a_subject->DVs_errors_sd),
			  ssq_error_errors, 
			  counter_errors);




  /*

  printf ("Mean RTs: CN (all trials): neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs_alltrials.fixed_neutral_CN_RT, 
	  a_subject->DVs_alltrials.fixed_congruent_CN_RT, 
	  a_subject->DVs_alltrials.fixed_incongruent_CN_RT);

  printf ("Mean RTs: CN (%d correct trials only): neutral %6.3f, " 
	  "congruent %6.3f, incongruent %6.3f\n", 
	  counter_correct[1][0] + counter_correct[1][1] + counter_correct[1][2], 
	  a_subject->DVs_correct.fixed_neutral_CN_RT, 
	  a_subject->DVs_correct.fixed_congruent_CN_RT, 
	  a_subject->DVs_correct.fixed_incongruent_CN_RT);

  printf ("Mean RTs: CN (%d error trials only): neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  counter_errors[1][0] + counter_errors[1][1] + counter_errors[1][2], 
	  a_subject->DVs_errors.fixed_neutral_CN_RT, 
	  a_subject->DVs_errors.fixed_congruent_CN_RT, 
	  a_subject->DVs_errors.fixed_incongruent_CN_RT);

  printf ("\n");


  printf ("Mean RTs: WR (all trials): neutral %6.3f, congruent %6.3f, incongruent %6.3f\n", 
	  a_subject->DVs_alltrials.fixed_neutral_WR_RT, 
	  a_subject->DVs_alltrials.fixed_congruent_WR_RT, 
	  a_subject->DVs_alltrials.fixed_incongruent_WR_RT);

  printf ("Mean RTs: WR (%d correct trials only): neutral %6.3f, "
	  "congruent %6.3f, incongruent %6.3f\n", 
	  counter_correct[0][0] + counter_correct[0][1] + counter_correct[0][2], 
	  a_subject->DVs_correct.fixed_neutral_WR_RT, 
	  a_subject->DVs_correct.fixed_congruent_WR_RT, 
	  a_subject->DVs_correct.fixed_incongruent_WR_RT);

  printf ("Mean RTs: WR (%d error trials only): neutral %6.3f, congruent %6.3f, incongruent %6.3f\n",
	  counter_errors[0][0] + counter_errors[0][1] + counter_errors[0][2], 
	  a_subject->DVs_errors.fixed_neutral_WR_RT, 
	  a_subject->DVs_errors.fixed_congruent_WR_RT, 
	  a_subject->DVs_errors.fixed_incongruent_WR_RT);

  printf ("\n");

  printf ("Inhibition score (RTi - RTc) (all trials): %5.3f\n", 
	  a_subject->DVs_alltrials.fixed_inhibition_score);

  printf ("Inhibition score (RTi - RTc) (correct trials only): %5.3f\n", 
	  a_subject->DVs_correct.fixed_inhibition_score);

  printf ("Inhibition score (RTi - RTc) (error trials only): %5.3f\n", 
	  a_subject->DVs_errors.fixed_inhibition_score);

  */
  return 0;

}


int gs_stroop_analyse_subject_mixedblocks (subject * a_subject) {

  // generate RT vector for trial position

  int runlength = a_subject->num_mixed_trials_in_run;
 
  int total_RT_alltrials[runlength]; // sums RT for each trial position across all runs
  int count_trials_alltrials[runlength]; // number of valid eg., CORRECT/INCORRECT/ALL trials
  double mean_RT_alltrials[runlength];

  int total_RT_correct[runlength]; // sums RT for each trial position across all runs
  int count_trials_correct[runlength]; // number of valid eg., CORRECT/INCORRECT/ALL trials
  double mean_RT_correct[runlength];

  int total_RT_errors[runlength]; // sums RT for each trial position across all runs
  int count_trials_errors[runlength]; // number of valid eg., CORRECT/INCORRECT/ALL trials
  double mean_RT_errors[runlength];

  int run, trial;


  // zero total_RT
  for (trial = 0; trial < runlength; trial ++) {
    total_RT_alltrials[trial] = 0;
    total_RT_correct[trial] = 0;
    total_RT_errors[trial] = 0;

    count_trials_alltrials[trial] = 0;
    count_trials_correct[trial] = 0;
    count_trials_errors[trial] = 0;
  }

  for (run = 0; run < a_subject->num_mixed_runs; run ++) {
    for (trial = 0; trial < runlength; trial ++) {

      stroop_trial_data * this_trial = &(a_subject->mixed_trials[run][trial]); 

      
      // all trials
      total_RT_alltrials[trial] += this_trial->response_time;
	count_trials_alltrials[trial] ++;
      

	// correct trials
	if ((this_trial->response %3) == (this_trial->stim_correct_response %3)) {
	  total_RT_correct[trial] += this_trial->response_time;
	  count_trials_correct[trial] ++;
	}

	else { // incorrect trials 
	  total_RT_errors[trial] += this_trial->response_time;
	  count_trials_errors[trial] ++;
	}
	
    }
  }


  // calc mean RT for each trial    
  for (trial = 0; trial < runlength; trial ++) {
    mean_RT_alltrials[trial] = (double) total_RT_alltrials[trial] / count_trials_alltrials[trial];
    mean_RT_correct[trial] = (double) total_RT_correct[trial] / count_trials_correct[trial];
    mean_RT_errors[trial] = (double) total_RT_errors[trial] / count_trials_errors[trial];
  }


  // calc 'task switching' scores
  a_subject->DVs_alltrials.mixed_TSwc = mean_RT_alltrials[4] - 
        (mean_RT_alltrials[5] + mean_RT_alltrials[6] + mean_RT_alltrials[7])/3;
  a_subject->DVs_alltrials.mixed_TScw = mean_RT_alltrials[8] - 
        (mean_RT_alltrials[9] + mean_RT_alltrials[10] + mean_RT_alltrials[11])/3;

  a_subject->DVs_correct.mixed_TSwc = mean_RT_correct[4] - 
        (mean_RT_correct[5] + mean_RT_correct[6] + mean_RT_correct[7])/3;
  a_subject->DVs_correct.mixed_TScw = mean_RT_correct[8] - 
        (mean_RT_correct[9] + mean_RT_correct[10] + mean_RT_correct[11])/3;


  a_subject->DVs_errors.mixed_TSwc = mean_RT_errors[4] - 
        (mean_RT_errors[5] + mean_RT_errors[6] + mean_RT_errors[7])/3;
  a_subject->DVs_errors.mixed_TScw = mean_RT_errors[8] - 
        (mean_RT_errors[9] + mean_RT_errors[10] + mean_RT_errors[11])/3;
  /*
  printf ("Task switching scores (all trials): TSwc: %5.3f, TScw: %5.3f\n", 
	  a_subject->DVs_alltrials.mixed_TSwc, a_subject->DVs_alltrials.mixed_TScw);

  printf ("Task switching scores (correct trials only): TSwc: %5.3f, TScw: %5.3f\n", 
	  a_subject->DVs_correct.mixed_TSwc, a_subject->DVs_correct.mixed_TScw);

  printf ("Task switching scores (error trials only): TSwc: %5.3f, TScw: %5.3f\n", 
	  a_subject->DVs_errors.mixed_TSwc, a_subject->DVs_errors.mixed_TScw);
  */
  return 0;
}

int gs_stroop_print_allsubs_data (subject_popn * some_subjects) {

  int n;
  subject * this_subj;

  // print header
  printf ("id\t"
	  "TDwt_i\t" // task demand weight inhibitory
	  "RT-CNn\t(SD)\t\t" // mean RT - colour naming, neutral
	  "RT-CNc\t(SD)\t\t" // mean RT - colour naming, congruent
	  "RT-CNi\t(SD)\t\t" // mean RT - colour naming, incongruent
	  "RI\t" // response inhibition score (RTi - RTc)
	  "TSwc\t" // task switching (word -> colour)
	  "TScw\t\n"); // task switching (colour -> word)


  for (n = 0; n < some_subjects->number_of_subjects; n++) {
    this_subj = some_subjects->subj[n];

    printf ("%d\t", n);
    printf ("%4.2f\t", ((gs_stroop_params *)(this_subj->params))->taskdemand_weights_inhibitory);
    printf ("%4.2f\t%4.2f\t\t", this_subj->DVs_correct.fixed_neutral_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_neutral_CN_RT);
    printf ("%4.2f\t%4.2f\t\t", this_subj->DVs_correct.fixed_congruent_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_congruent_CN_RT);
    printf ("%4.2f\t%4.2f\t\t", this_subj->DVs_correct.fixed_incongruent_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_incongruent_CN_RT);
    printf ("%4.2f\t", this_subj->DVs_correct.fixed_inhibition_score);
    printf ("%4.2f\t", this_subj->DVs_correct.mixed_TSwc);
    printf ("%4.2f\t", this_subj->DVs_correct.mixed_TScw);

    printf ("\n");
  }

  return 0;
}


// machine readable data export format
int gs_stroop_data_filedump (subject_popn * some_subjects) {
  FILE * fp;
  int n;
  subject * this_subj;

  fp = fopen (DATAFILE, "w+");

  fprintf (fp, 
	   "result\t"
	   "TDwt_i\t" // task demand weight inhibitory
	   "RT-Cn\tCnSD\t" // mean RT - colour naming, neutral
	   "RT-Cc\tCcSD\t" // mean RT - colour naming, congruent
	   "RT-Ci\tCiSD\t" // mean RT - colour naming, incongruent
	   "RI\t" // response inhibition score (RTi - RTc)
	   "TSwc\t" // task switching (word -> colour)
	   "TScw\t\n"); // task switching (colour -> word)

  // OUTPUT CORRECT TRIALS

  for (n = 0; n < some_subjects->number_of_subjects; n++) {
    this_subj = some_subjects->subj[n];

    fprintf (fp, "Cor\t"); // subj id not needed, R creates this
    fprintf (fp, "%4.2f\t", ((gs_stroop_params *)(this_subj->params))->taskdemand_weights_inhibitory);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_correct.fixed_neutral_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_neutral_CN_RT);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_correct.fixed_congruent_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_congruent_CN_RT);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_correct.fixed_incongruent_CN_RT,
	                      this_subj->DVs_correct_sd.fixed_incongruent_CN_RT);
    fprintf (fp, "%4.2f\t", this_subj->DVs_correct.fixed_inhibition_score);
    fprintf (fp, "%4.2f\t", this_subj->DVs_correct.mixed_TSwc);
    fprintf (fp, "%4.2f\t", this_subj->DVs_correct.mixed_TScw);

    fprintf (fp, "\n");
  }

  for (n = 0; n < some_subjects->number_of_subjects; n++) {
    this_subj = some_subjects->subj[n];

    fprintf (fp, "Err\t"); // subj id not needed, R creates this
    fprintf (fp, "%4.2f\t", ((gs_stroop_params *)(this_subj->params))->taskdemand_weights_inhibitory);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_errors.fixed_neutral_CN_RT,
	                      this_subj->DVs_errors_sd.fixed_neutral_CN_RT);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_errors.fixed_congruent_CN_RT,
	                      this_subj->DVs_errors_sd.fixed_congruent_CN_RT);
    fprintf (fp, "%4.2f\t%4.2f\t", this_subj->DVs_errors.fixed_incongruent_CN_RT,
	                      this_subj->DVs_errors_sd.fixed_incongruent_CN_RT);
    fprintf (fp, "%4.2f\t", this_subj->DVs_errors.fixed_inhibition_score);
    fprintf (fp, "%4.2f\t", this_subj->DVs_errors.mixed_TSwc);
    fprintf (fp, "%4.2f\t", this_subj->DVs_errors.mixed_TScw);

    fprintf (fp, "\n");
  }



  // OUTPUT ERROR TRIALS

  /*
  fprintf (fp, "Key: \n"
	   "id: subject id"
	   "TDwt_i: task demand weight inhibitory\n" 
	   "RT-Cn: reaction time (colour naming, neutral)\n"
	   "CnSD: std dev, (colour naming, neutral)\n" 
	   "RT-Cc: reaction time (colour naming, congruent)\n"
	   "CcSD: std dev, (colour naming, congruent)\n" 
	   "RT-Ci: reaction time (colour naming, incongruent)\n"
	   "CiSD: std dev, (colour naming, incongruent)\n"  
	   "RI: response inhibition score (RTi - RTc)\n" 
	   "TSwc: task switching score (word -> colour)\n" // 
	   "TScw: task switching score (colour -> word)\n"); // 
  */

  fclose (fp);
  return 0;
}
