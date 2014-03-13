
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <gsl/gsl_randist.h>
#include "pdp_objects.h"
#include "random_generator_functions.h" // for gaussian noise

#include "gs_stroop_subjects.h" // specialises simulated_subjects to
                                //stroop data/stimuli/params
#include "gs_stroop_analyse.h"

#include "gs_stroop.h"
#include "gs_stroop_global_params.h"



int main () {

  gsl_rng * random_generator = random_generator_create();
  int n;




  // <-------------------- SUBJECTS INIT -------------------->

  // create subject population
  
  subject_popn * my_subjects = subject_popn_create (NUM_SUBJECTS);

  for (n = 0; n < my_subjects->number_of_subjects; n++) {

    my_subjects->subj[n] = subject_create (NUM_TRIALS, NUM_TRIALS, MIXED_BLOCK_RUN_LENGTH);

    // parameterise subject
    subject_params_vary (my_subjects->subj[n], 
			 gsl_ran_flat (random_generator, 
				       TASKDEMAND_OUTPUT_INHIB_VARY_MIN, 
				       TASKDEMAND_OUTPUT_INHIB_VARY_MAX),
			 TASKDEMAND_OUTPUT_EXCITATORY_WT);

    // create subject data

    subject_init_trialblock_fixed (random_generator, my_subjects->subj[n], 
				 PPN_NEUTRAL, PPN_CONGRUENT, PPN_INCONGRUENT,
				 PPN_WORDREADING, PPN_COLOURNAMING);
				 
    subject_init_trialblock_mixed (my_subjects->subj[n]);



  }

  
  printf ("\nsubject: ");


  // <----------------------RUN SIMULATION ----------------------->

#ifdef _THREADED_
#pragma omp parallel for
#endif
  for (n = 0; n < my_subjects->number_of_subjects; n++) {

    printf ("%d ", n);
#ifdef _THREADED_
    if (n < my_subjects->number_of_subjects/4) {
#endif
    printf ("\n");
#ifdef _THREADED_
    }
#endif
    // <-------------------- GLOBAL MODEL INIT ---------------------->
    pdp_model * gs_stroop_model = pdp_model_create(n, "gs_stroop");


    act_func_params * activation_parameters = malloc (sizeof(act_func_params));
    activation_parameters->type = ACT_GS;
    activation_parameters->params.gs.step_size = STEP_SIZE;
    activation_parameters->params.gs.act_max = ACTIVATION_MAX;
    activation_parameters->params.gs.act_min = ACTIVATION_MIN;
    gs_stroop_model->activation_parameters = activation_parameters;
    
  // create the network & set weights
    gs_stroop_model_build (gs_stroop_model); // also inits the model for 1st sim



    model_init_params (gs_stroop_model, 
		       ((gs_stroop_params *)(my_subjects->subj[n]->params)));

    // <--------------------- a) RUN FIXED BLOCKS ---------------------->

    int trial, run;
    for (trial = 0; trial < my_subjects->subj[n]->num_fixed_trials; trial++) {
      
      // printf (" F%d", trial);
      // Note: need to run model_init immediately followed by update_associative_weights 
      // to zero associative weights for new subject, in mixed blocks trials 
      
      model_init_activation (gs_stroop_model, 0.0); // zero activations 
      
      /* run stroop trial(s) */
      run_stroop_trial (gs_stroop_model, 
			&(my_subjects->subj[n]->fixed_trials[trial]), 
		      random_generator);

      /* update weights */
      // update_associative_weights (gs_stroop_model); // ?!? remove this line for fixed blocks??
      
      
    }
    
    //<------------------------- b) RUN MIXED BLOCKS -------------------------->
    
    for (run = 0; run < my_subjects->subj[n]->num_mixed_runs; run ++) {
      // printf ("Mixed: %d", run);
      // Note: need to run model_init immediately followed by update_associative_weights 
      // to zero associative weights for new subject, in mixed blocks trials 
      model_init_activation (gs_stroop_model, 0.0); // zero activations (zero persisting taskd. act.)
      update_associative_weights (gs_stroop_model);
      
      for (trial = 0; trial < my_subjects->subj[n]->num_mixed_trials_in_run; trial++) {

	model_init_activation (gs_stroop_model, (1-SQUASHING_PARAM)); // zero activations 
	
	/* run stroop trial(s) */
	run_stroop_trial (gs_stroop_model, 
			  &(my_subjects->subj[n]->mixed_trials[run][trial]), 
			  random_generator);
	
	/* update weights */
	update_associative_weights (gs_stroop_model);
	
      }
    }

    free (gs_stroop_model->activation_parameters);
    pdp_model_free (gs_stroop_model);

  } //<---------------- CLOSE SIMULATION LOOP (per subject)--------------->


  /* prove it's worked */
    // TODO - save and analyse data

  for (n = 0; n < my_subjects->number_of_subjects; n++) {

    gs_stroop_analyse_subject_fixedblocks (my_subjects->subj[n]);
    gs_stroop_analyse_subject_mixedblocks (my_subjects->subj[n]);
  }

  gs_stroop_print_allsubs_data (my_subjects);
  gs_stroop_data_filedump (my_subjects);

  //   subject_free (subject_1); // temp
  subject_popn_free (my_subjects);
  random_generator_free (random_generator);  
  
  return 0;
}
