#!/usr/bin/python

# simulation 2 - rebound effects/asymmetric restart costs
# corresponds to experiments 3, 4 of Allport & Wylie 2000

# design:
# extends simulation 1 (item-specific priming)
# manipulates following factors
#  - Priming (primed vs unprimed)
#  - RSI (long vs short)
#  - Task transition (switch vs. repeat)
#  - Task (dominant vs. non-dominant)

# base      switch     restart
# ABCD   vs AB-CD   vs ABC-D   # baseline (unprimed)
# ABBC   vs AB-BC   vs ABB-C   # primed switch
# ABCB   vs AB-CB   vs ABC-B   # primed nonswitch


num_blocks = 10000 # number of times to run each sequence type
trialid = 0
rsi_levels = [
    [1.0, 1.0, "all_short"], 
    [1.5, 1.0, "long_switch"], 
    [1.0, 1.5, "long_restart"]
    ] 


task_levels = [
    [1, 0, "C-W"], # CN -> WR 
#    [0, 1, "W-C"]  # WR -> CN - for simulation 2.0
    [1, 1, "C-C"]  # WR -> CN - for simulation 2.1

    ] 




def write_trial (block_id, trial_id, wordin, colourin, task, param1, param2):
    f = open ("sim_2_3_trials.conf", "a") # opens file for appending
    f.write (block_id)
    f.write ("\t")

    f.write (str(trial_id))
    f.write ("\t")

    f.write (str(wordin))
    f.write ("\t")

    f.write (str(colourin))
    f.write ("\t")

    f.write (str(task))
    f.write ("\t")

    f.write (param1)
    f.write ("\t")

    f.write (param2)
    f.write ("\t\n")

    f.close()

def write_lookup (sequence, trial_id, task_level_name, rsi_level_name, position):
    f = open ("sim_2_3_lookup.txt", "a")
    f.write (str(trial_id) + "\t")
    f.write (sequence + "\t")
    f.write (task_level_name + "\t")
    f.write (rsi_level_name + "\t")
    f.write (str(position))
    f.write ("\n")



# first write ABCD trials - baseline

for block in range(0, num_blocks):
    for task in task_levels:
        for level in rsi_levels:
            block_name = 'ABCD_' + str(block) + "_rsi" + str(level[0]) + "-" + str(level[1]) + "task" + str(task[0]) + "_" + str(task[1])

            write_trial (block_name, trialid+0, 0, 1, task[0], 
                         "HebP=1", "RSIs=1.0")
 
            write_trial (block_name, trialid+1, 1, 2, task[0], 
                         "HebP=1", "RSIs=1.0") 

            write_trial (block_name, trialid+2, 2, 0, task[1], 
                         "HebP=1", "RSIs=" + str(level[0])) 

            write_trial (block_name, trialid+3, 0, 1, task[1], 
                         "HebP=1", "RSIs=" + str(level[1])) 

            for trial in range(0, 4):
                write_lookup ("ABCD", trialid+trial, task[2], level[2], trial)

            trialid += 4


# second, write ABBC trials 
for block in range(0, num_blocks):
    for task in task_levels:
        for level in rsi_levels:
            block_name = 'ABBC_' + str(block) + "_rsi" + str(level[0]) + "-" + str(level[1]) + "task" + str(task[0]) + "_" + str(task[1])
            
            write_trial (block_name, trialid+0, 0, 1, task[0], 
                         "HebP=2", "RSIs=1.0") 

            write_trial (block_name, trialid+1, 1, 2, task[0], 
                         "HebP=2", "RSIs=1.0") 

            write_trial (block_name, trialid+2, 1, 2, task[1], 
                         "HebP=2", "RSIs=" + str(level[0])) 

            write_trial (block_name, trialid+3, 2, 0, task[1], 
                         "HebP=2", "RSIs=" + str(level[1])) 

            for trial in range(0, 4):
                write_lookup ("ABBC", trialid+trial, task[2], level[2], trial)

            trialid += 4


# third, write ABCB trials
for block in range(0, num_blocks):
    for task in task_levels:
        for level in rsi_levels:
            block_name = 'ABCB_' + str(block) + "_rsi" + str(level[0]) + "-" + str(level[1]) + "task" + str(task[0]) + "_" + str(task[1])

            write_trial (block_name, trialid+0, 0, 1, task[0], 
                         "HebP=2", "RSIs=1.0") 

            write_trial (block_name, trialid+1, 1, 2, task[0], 
                         "HebP=2", "RSIs=1.0") 

            write_trial (block_name, trialid+2, 2, 0, task[1], 
                         "HebP=2", "RSIs=" + str(level[0])) 

            write_trial (block_name, trialid+3, 1, 2, task[1], 
                         "HebP=2", "RSIs=" + str(level[1])) 

            for trial in range(0, 4):
                write_lookup ("ABCB", trialid+trial, task[2], level[2], trial)

            trialid += 4
