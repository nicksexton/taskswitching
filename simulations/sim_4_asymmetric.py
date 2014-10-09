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

filename_conf = "sim_4_trials.conf"
filename_lookup = "sim_4_lookup.txt"
num_blocks = 250 # number of times to run each sequence type
trialid = 0

# for now, just use incongruent stimuli for all non-relevant tasks
run_congruency_levels = [
    [0, 0, 0, "II/II/II"], # 0 = II, 1 = IC
  #  [0, 0, 1, "II/II/IC"],
  #  [0, 1, 0, "II/IC/II"],
  #  [1, 0, 0, "IC/II/II"],
  #  [0, 1, 1, "II/IC/IC"],
  #  [1, 0, 1, "IC/II/IC"],
  #  [1, 1, 0, "IC/IC/II"],
  #  [1, 1, 1, "IC/IC/IC"],
]

stimulus_congruency = [
    [0, 1, 1, "II"], #target, distractor 1, distractor 2
    [0, 0, 1, "IC"]
]

sequence_levels = [
    [1, 0, 0, "0SW"], # no switch
    [1, 1, 0, "1SW"], # one switch
    [2, 1, 0, "2SW"], # two switch
    [0, 1, 0, "ALT"], # alternating switch
]

# for rotating round stimuli/tasks
map_offset = [2, 1, 0]
map_direction = [1, -1]


def set_stimuli (congruency, cue, direction, offset):

    stimuli = [-1, -1, -1] # init the list

    stimuli[cue] = stimulus_congruency[congruency][0]
    stimuli[(cue + 1 * direction) % 3] = stimulus_congruency[congruency][1]
    stimuli[(cue + 2 * direction) % 3] = stimulus_congruency[congruency][2]

#    print "cue: " + str(cue) + "; direction: " + str(direction) + "; offset: " + str(offset) + "; - [" + ",".join(str(x) for x in stimuli) + "]"


# add flip
    stim_flip = random.randint (0, 1)
    stimuli = [((x + stim_flip) % 2) for x in stimuli]

#    print "flipped: [" + ",".join(str(x) for x in stimuli) + "]"


    return stimuli


def write_trial (block_id, trial_id, cue, stim_A, stim_B, stim_C, param1, param2):

    f = open (filename_conf, "a") # opens file for appending
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


def write_lookup (task_sequence_condition, task_sequence, run_cong_sequence, trial_id, position, trial_cong):
    f = open (filename_lookup, "a")
    f.write (str(trial_id) + "\t")
    f.write (str(task_sequence_condition) + "\t")
    f.write (str(task_sequence) + "\t")
    f.write (str(position) + "\t")
    f.write (str(run_cong_sequence) + "\t")
    f.write (str(trial_cong))    
    f.write ("\n")
    f.close()



for run in run_congruency_levels:
    for sequence in sequence_levels:
        for offset in map_offset:
            for direction in map_direction:
                for block in range(0, num_blocks):

                    block_name = str(sequence[3]) + "_" + '/'.join(str((x * direction + offset)%3) for x in sequence[0:3])

    # write block of trials

            # trial 0
                    for trial in range (0, 3):

                        stim_flip = random.randint (0, 1)

#                        cue = (sequence[trial] + offset) % 3
                        cue = (sequence[trial] * direction + offset) % 3
                        stim_inputs = set_stimuli (run[trial], cue, direction, offset)

                        write_trial (block_name + "_" + str(block), 
                                     trialid+0, 
                                     cue,
                                     stim_inputs[0],
                                     stim_inputs[1],
                                     stim_inputs[2],
                                     "HebP=0", "")

                        write_lookup (sequence[3], 
                                      '/'.join(str((x * direction + offset)%3) for x in sequence[0:3]),
                                      run[3], 
                                      trialid, 
                                      trial, 
                                      stimulus_congruency[run[0]][3])

                        trialid += 1

