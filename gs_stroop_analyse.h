
// typedef enum {ALL_TRIALS, CORRECT_TRIALS, INCORRECT_TRIALS} analyse_trials_flag;

// outputs trial data to a logfile
// nb does not print newline so additional data can be written to the row
int gs_stroop_print_trial_data (FILE * fp, stroop_trial_data * trial_data);

// static int gs_stroop_record_means (struct subject_aggregate_data * my_data_means,
					 // struct subject_aggregate_data * my_data_sd,
					 //int totals_matrix[2][3], 
					 //int counter_matrix[2][3]);

int gs_stroop_analyse_subject_fixedblocks (subject * a_subject);

int gs_stroop_analyse_subject_mixedblocks (subject * a_subject);

int gs_stroop_print_allsubs_data (subject_popn * some_subjects);
int gs_stroop_data_filedump (subject_popn * some_subjects);
