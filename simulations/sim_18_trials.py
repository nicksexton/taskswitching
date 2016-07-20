#!/usr/bin/python

# Trials for strategic adaptation simulation
# Generates long blocks of trials, where trials are either allowed to repeat or to not repeat



import random
import sys, getopt # for parsing cmd line params
import csv # for writing to csv, for eyeballing the trials

filename_conf = "sim_18_trials.conf"
filename_lookup = "sim_18_lookup.txt"
num_blocks = 10 # number of times to run each sequence type
num_trials_default = 20

trialid = 0


# Store input and output file names
ifile=''
ofile=''
 
# Read command line args
myopts, args = getopt.getopt(sys.argv[1:],"n:t:p:")
 
###############################
# o == option
# a == argument passed to the o
###############################
for o, a in myopts:
    if o == '-n':
        num_blocks = int(a)
    elif o == '-t':
        num_trials_default = int(a)
    elif o == '-p':
        prob_repeat = float(a) # usage: probability of repeat is p/100
        print ("repeat probability %.2f" % prob_repeat)
    else:
        print("Usage: %s -n [number of blocks] -t [number of trials per block]" % sys.argv[0])
 




exp_cue_mapping = [0, 1, 2] #['colour', 'alphabet', 'style']
exp_stimuli_colour = [0, 1] #['red', 'green', 'blue']
exp_stimuli_alphabet = [0, 1] #[['d', 'e', 'f'], ['k', 'l', 'm'], ['u', 'v', 'w']]
exp_stimuli_style = [0, 1] #['bold', 'regular', 'italic']
# distractor_direction = [1, -1]
exp_response_mapping = [0, 1, 2] #['left', 'down', 'right']
exp_response_code = [0, 1, 1]


def write_trial (block_id, trial_id, cue, stim_A, stim_B, stim_C, param1, param2):

    f = open (filename_conf, "a") # opens file for appending
    f.write (block_id + "\t")
    f.write (str(trial_id) + "\t")
    f.write (str(cue) + "\t")
    f.write (str(stim_A) + "\t")
    f.write (str(stim_B) + "\t")
    f.write (str(stim_C) + "\t")
    f.write ((param1) + "\t")
    f.write ((param2)  + "\t")
    f.write ("\n")
    f.close()

# lookup trialid, sequence_condition, sequence, position, run_cong_seq, trial_cong
def write_lookup (trial_id, task_sequence_condition, task_sequence, position, run_cong_sequence, trial_cong):
    f = open (filename_lookup, "a")
    f.write (str(trial_id) + "\t")
    f.write (str(task_sequence_condition) + "\t")
    f.write (str(task_sequence) + "\t")
    f.write (str(position) + "\t")
    f.write (str(run_cong_sequence) + "\t")
    f.write (str(trial_cong))    
    f.write ("\n")
    f.close()

    

def generate_trials (num_trials, allow_task_repeat):
    # allow_task_repeat = False
    # num_trials = 20

    # first create task vector


# finally, write to the files

    block_num = 0
    block_type = ["NRP", "REP"]
    repeats = [0, 1]

    
    for block in range (0, num_blocks):
        blockid = str(block) + "_" + block_type[allow_task_repeat] 
        repeat_id = repeats[allow_task_repeat]

        tasks = [random.randint(0,2)] # randomly generate first trial

        if allow_task_repeat:
            # now randomly generate the rest of trials (no repeats)
            for trial in range (1, num_trials):

                # first, is trial a repeat?
                if random.uniform(0,1) < prob_repeat:
                    tasks.append (tasks[trial-1]) # randomly generate 2nd trial
#                print ("%d" % tasks[trial])
                else:
                    tasks.append ((tasks[trial-1] + random.randint(1,2)) % 3) # randomly generate 2nd trial
#                print("%d" % tasks[trial])
        else:
            for trial in range (1, num_trials):
                tasks.append ((tasks[trial-1] + random.randint(1,2)) % 3) # randomly generate 2nd trial         

            
        stimuli = [[-1, -1, -1]] # init the list
        target = [random.randint(0,1)] # randomly generate target stim
        stimuli[0][tasks[0]] = exp_response_code[target[0]] # randomly generate target stim
        for stim_dim in range (1, 3): # generate the distractor stimuli for this trial
            stimuli[0][(tasks[0]+stim_dim) % 3] = exp_response_code[(target[0]+stim_dim % 2)]

    
        for trialid in range (1, num_trials):
            stimuli.append ([-1, -1, -1])

            # randomly generate index of target stim by rotating the previous target
            target.append ((target[trialid-1] + random.randint(1,2)) % 3)
#           stim_direction = random.randint (0, 1) # which way round to apply distractor stimuli 

            # randomly generate target stim
            stimuli[trialid][tasks[trialid]] = exp_response_code[target[trialid]] 

            # generate distractor stimuli
            #stim_direction = distractor_direction[random.randint (0, 1)] # which way round to apply the stimuli in
            for stim_dim in range (1, 3): # generate the distractor stimuli for this trial
                stimuli[trialid][(tasks[trialid]+stim_dim) % 3] = exp_response_code[(target[trialid]+stim_dim) % 3]


        
        for n in range (0, num_trials): # where n is the nth trial
            if (n == 0) or (n == 1):
                switch = 'NA'

            elif tasks[n] == tasks[n-1]:
                if tasks[n] == tasks[n-2]:
                    switch = 'BLK' # ie AAA
                else:
                    switch = '0SW' # ie BAA

            else:
                if tasks[n-1] == tasks[n-2]:
                    switch = '1SW'
                    
                elif tasks[n] == tasks[n-2]:
                    switch = 'ALT'
                    
                else:
                    switch = '2SW'

                    
                    
            write_trial (blockid,
                         repeat_id*num_blocks*num_trials_default + block*num_trials + n,
                         exp_cue_mapping[tasks[n]],
                         exp_stimuli_colour[stimuli[n][0]],
                         exp_stimuli_alphabet[stimuli[n][1]],
                         exp_stimuli_style[stimuli[n][2]],
                         "",
                         "")

            write_lookup (repeat_id*num_blocks*num_trials_default + block*num_trials + n,
                          switch,
                          "NA",
                          n,
                          block_type[allow_task_repeat],
                          "NA")
                
# .conf blockid, trialid, cue, stimA, stimB, stimC, param1, param2
# lookup trialid, sequence_condition, sequence, position, run_cong_seq, trial_cong
        

    return


#trials = generate_singletask (100, 1)
#trials = generate_trials (100, False)
generate_trials (num_trials_default, True)
generate_trials (num_trials_default, False)













