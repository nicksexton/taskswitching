#ifndef gs_stroop_subjects_h
#define gs_stroop_subjects_h

typedef enum {NONE, FIXED, MIXED} blocktype;
typedef enum {NEUTRAL, CONGRUENT, INCONGRUENT} trialtype;


typedef struct stroop_response {
  int this_node; 
    // 1 = wordR, 2 = wordG, 3 = wordB, 4 = colourR, 5 = colourG, 6 = colourB
  double activation;
} stroop_response;


typedef struct stroop_trial_data {

  int trial_id;
  blocktype block_type; 
  trialtype trial_type; 
  int stim_task; // 0 = word naming, 1 = colour reading
  int stim_word;
  int stim_colour;
  int stim_correct_response;
  int response;
  int response_time;

} stroop_trial_data;



typedef struct gs_stroop_params {

  double taskdemand_weights_inhibitory;
  double taskdemand_weights_excitatory;

} gs_stroop_params;




stroop_trial_data stroop_trial_data_create (int id, blocktype block_type, int stim_task, 
					      int stim_word, int stim_colour);


// unnecessary, handled by GList??
//void stroop_trial_data_free (stroop_trial_data * data_to_free);


subject * subject_create (int num_of_trials);

int subject_init_trialblock_fixed (gsl_rng * random_generator, 
				   subject * a_subject, 
				   int num_trials, 
				   int ppn_N, int ppn_C, int ppn_I,
				   int ppn_WR, int ppn_CN);


#endif
