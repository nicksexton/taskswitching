#!/usr/bin/python

# simulation 3 - modelling the n-2 repetition cost
# in the symbolic classification paradigm - eg., Schuch & Koch 2003, experiment 2

# design:
# - two types of 3-trial run: ABA, CBA
# each trial can be either incongruent/congruent or incongruent/incongruent 
# w.r.t. the correct response of the cued stim dimension.
# ie., if cued task is A, stim [0 1 1] would be II and [0 0 1] would be IC.
# these need to be counterbalanced across all 3 trials for both run types (ie., 8 conditions per run type)
# disregarding congruent/congruent trials for now.



num_blocks = 2 # number of times to run each sequence type
trialid = 0

run_congruency_levels = [
    [0, 0, 0, "II/II/II"], # 0 = II, 1 = IC
    [0, 0, 1, "II/II/IC"],
    [0, 1, 0, "II/IC/II"],
    [1, 0, 0, "IC/II/II"],
    [0, 1, 1, "II/IC/IC"],
    [1, 0, 1, "IC/II/IC"],
    [1, 1, 0, "IC/IC/II"],
    [1, 1, 1, "IC/IC/IC"],
]

trial_congruency_levels = [
    [0, 1, 1, "II"], #target, distractor 1, distractor 2
    [0, 0, 1, "IC"]
]

sequence_levels = [
    [0, 1, 0, "ABA"], 
    [2, 1, 0, "CBA"] 
]



def write_trial (block_id, trial_id, cue, stim_A, stim_B, stim_C, param1, param2):
    f = open ("sim_3_trials.conf", "a") # opens file for appending
    f.write (block_id)
    f.write ("\t")

    f.write (str(trial_id))
    f.write ("\t")

    f.write (str(cue))
    f.write ("\t")

    f.write (str(stim_A))
    f.write ("\t")

    f.write (str(stim_B))
    f.write ("\t")

    f.write (str(stim_C))
    f.write ("\t")

    f.write (param1)
    f.write ("\t")

    f.write (param2)
    f.write ("\t\n")

    f.close()


def write_lookup (task_sequence, run_cong_sequence, trial_id, position, trial_cong):
    f = open ("sim_3_lookup.txt", "a")
    f.write (str(trial_id) + "\t")
    f.write (str(task_sequence) + "\t")
    f.write (str(position) + "\t")
    f.write (str(run_cong_sequence) + "\t")
    f.write (str(trial_cong) + "\t")

    f.write ("\n")
    f.close()



for block in range(0, num_blocks):
    for run in run_congruency_levels:
        for sequence in sequence_levels:
            block_name = str(sequence[3]) + "_" + str(run[3]) 

            # write ABA trials

            write_trial (block_name + "_" + str(block), 
                         trialid+0, 
                         sequence[0], #cue
                         trial_congruency_levels[run[0]][0], #stim_A 
                         trial_congruency_levels[run[0]][1], #stim_B
                         trial_congruency_levels[run[0]][2], #stim_C
                         "HebP=0", "")

            write_trial (block_name + "_" + str(block), 
                         trialid+1, 
                         sequence[1], 
                         trial_congruency_levels[run[1]][0], 
                         trial_congruency_levels[run[1]][1], 
                         trial_congruency_levels[run[1]][2], 
                         "HebP=0", "")

            write_trial (block_name + "_" + str(block), 
                         trialid+2, 
                         sequence[2],  
                         trial_congruency_levels[run[2]][0], 
                         trial_congruency_levels[run[2]][1], 
                         trial_congruency_levels[run[2]][2], 
                         "HebP=0", "")
 




            for trial in range(0, 3):
                write_lookup (sequence[3], 
                              run[3], 
                              trialid+trial, 
                              trial, 
                              trial_congruency_levels[run[trial]][3])

        
            trialid += 3

