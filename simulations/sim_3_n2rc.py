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

import random

num_blocks = 50 # number of times to run each sequence type
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


def get_stim_input (congruency, stim, cue):
    return trial_congruency_levels[congruency][(stim - cue) % 3]


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
    f.write (str(trial_cong))
    
    f.write ("\n")
    f.close()



for block in range(0, num_blocks):
    for run in run_congruency_levels:
        for sequence in sequence_levels:
            block_name = str(sequence[3]) + "_" + str(run[3]) 

            # write block of trials

            map_offset = random.randint (0, 2)
            map_direction = random.choice ([-1, 1])

            # trial 0
            stim_flip = random.randint (0, 1)
            stim_inputs = [ (get_stim_input (run[0], 0, sequence[0]) + stim_flip) % 2, 
                            (get_stim_input (run[0], 1, sequence[0]) + stim_flip) % 2,
                            (get_stim_input (run[0], 2, sequence[0]) + stim_flip) % 2,        
                            ]

            write_trial (block_name + "_" + str(block), 
                         trialid+0, 
                         (map_direction * sequence[0] + map_offset) % 3, #cue
                         stim_inputs[(0 * map_direction + map_offset) % 3],
                         stim_inputs[(1 * map_direction + map_offset) % 3],
                         stim_inputs[(2 * map_direction + map_offset) % 3],
                         "HebP=0", "")

            write_lookup (sequence[3], 
                          run[3], 
                          trialid + 0, 
                          0, # trial 
                          trial_congruency_levels[run[0]][3])

            # trial 1
            stim_flip = random.randint (0, 1)
            stim_inputs = [ (get_stim_input (run[1], 0, sequence[1]) + stim_flip) % 2, 
                            (get_stim_input (run[1], 1, sequence[1]) + stim_flip) % 2,
                            (get_stim_input (run[1], 2, sequence[1]) + stim_flip) % 2,        
                            ]


            write_trial (block_name + "_" + str(block), 
                         trialid+1, 
                         (map_direction * sequence[1] + map_offset) % 3, 
                         stim_inputs[(0 * map_direction + map_offset) % 3],
                         stim_inputs[(1 * map_direction + map_offset) % 3],
                         stim_inputs[(2 * map_direction + map_offset) % 3],
                         "HebP=0", "")

            write_lookup (sequence[3], 
                          run[3], 
                          trialid + 1, 
                          1, # trial 
                          trial_congruency_levels[run[1]][3])


            # trial 2
            stim_flip = random.randint (0, 1)
            stim_inputs = [ (get_stim_input (run[2], 0, sequence[2]) + stim_flip) % 2, 
                            (get_stim_input (run[2], 1, sequence[2]) + stim_flip) % 2,
                            (get_stim_input (run[2], 2, sequence[2]) + stim_flip) % 2,        
                            ]

            write_trial (block_name + "_" + str(block), 
                         trialid+2, 
                         (map_direction * sequence[2] + map_offset) % 3,  
                         stim_inputs[(0 * map_direction + map_offset) % 3],
                         stim_inputs[(1 * map_direction + map_offset) % 3],
                         stim_inputs[(2 * map_direction + map_offset) % 3],
                         "HebP=0", "")
 
            write_lookup (sequence[3], 
                          run[3], 
                          trialid + 2, 
                          2, # trial 
                          trial_congruency_levels[run[2]][3])


        
            trialid += 3

