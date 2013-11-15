#include <glib.h>

typedef enum {FIXED, MIXED} blocktype;


typedef struct stroop_trial_data {

  int trial_id;
  blocktype block_type; 
  int stim_word;
  int stim_colour;
  int stim_task; // 0 = word naming, 1 = colour reading
  int stim_correct_response;
  int response;
  int response_time;

} stroop_trial_data;


typedef struct gs_stroop_params {

  double taskdemand_weights_inhibitory;
  double taskdemand_weights_excitatory;

} gs_stroop_params;


typedef struct subject {

  GArray * trials;
  int num_of_trials;
  gs_stroop_params * params;

} subject; 



typedef struct subject_popn {

  GArray * subjects;
  int number_of_subjects;

} subject_popn;


subject * subject_create (int num_of_trials);
void subject_free (subject * subject_to_free);
subject_popn * subject_popn_create (int number_of_subjects);
void subject_popn_free (subject_popn * some_subjects);
