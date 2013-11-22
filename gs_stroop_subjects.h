#ifndef gs_stroop_subjects_h
#define gs_stroop_subjects_h
#include <gsl/gsl_rng.h>
#include <glib.h>

typedef enum {NONE, FIXED, MIXED} blocktype;
typedef enum {NEUTRAL = 0, CONGRUENT = 1, INCONGRUENT = 2} trialtype;
typedef enum {WORDREADING = 0, COLOURNAMING = 1} tasktype;

typedef struct stroop_response {
  int this_node; 
    // 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  double activation;
} stroop_response;


typedef struct stroop_trial_data {

  int trial_id;
  blocktype block_type; // NONE, FIXED, MIXED
  trialtype trial_type; // NEUTRAL, CONGRUENT, INCONGRUENT
  int stim_task; // 0 = word naming, 1 = colour reading
  int stim_word;
  int stim_colour;
  int stim_correct_response;
  int response;
  int response_time;

} stroop_trial_data;


typedef struct subject_aggregate_data {

  double fixed_neutral_CN_RT;
  double fixed_congruent_CN_RT;
  double fixed_incongruent_CN_RT;

  double fixed_neutral_WR_RT;
  double fixed_congruent_WR_RT;
  double fixed_incongruent_WR_RT;

} subject_aggregate_data;

typedef struct gs_stroop_params {

  double taskdemand_weights_inhibitory;
  double taskdemand_weights_excitatory;

} gs_stroop_params;


typedef struct subject {

  stroop_trial_data * fixed_trials; // pointer to array
  stroop_trial_data ** mixed_trials; // 2d array

  int num_fixed_trials;
  int num_mixed_runs;
  int num_mixed_trials_in_run;
  subject_aggregate_data DVs;
  void * params; 

} subject; 


typedef struct subject_popn {

  GArray * subjects;
  int number_of_subjects;

} subject_popn;



void subject_free (subject * subject_to_free);
subject_popn * subject_popn_create (int number_of_subjects);
void subject_popn_free (subject_popn * some_subjects);

stroop_trial_data stroop_trial_data_create (int id, 
					    blocktype block_type, 
					    trialtype trial_type, 
					    int stim_task, int stim_word, int stim_colour);



subject * subject_create (int num_fixed_trials, 
			  int num_mixed_runs, int num_mixed_trials_in_run);


int subject_init_trialblock_fixed (const gsl_rng * random_generator, 
				   subject * a_subject, 
   				   int ppn_N, int ppn_C, int ppn_I,
				   int ppn_WR, int ppn_CN);

int subject_init_trialblock_mixed ( subject * a_subject);

#endif
