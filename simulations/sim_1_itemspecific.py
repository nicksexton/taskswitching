#!/usr/bin/python

# simulation 1 - item-specific priming effects
# replicating effects in experiment 5 of Allport & Wylie 2000
# straight replication of G&S simulation, see Gilbert & Shallice 2002


num_blocks = 100 # number of times to run each sequence type


def write_trial (block_id, trial_id, wordin, colourin, task, param1, param2):
    f = open ("sim_1_trials.conf", "a") # opens file for appending
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



# first write ABCD trials (tasks CCWW) - baseline
for block in range(0, num_blocks):
    block_name = 'ABCD_' + str(block) 

    write_trial (block_name, 0, 0, 1, 1, "HebP=1", "") 
    write_trial (block_name, 1, 1, 2, 1, "HebP=1", "") 
    write_trial (block_name, 2, 2, 0, 0, "HebP=1", "") 
    write_trial (block_name, 3, 0, 1, 0, "HebP=1", "") 


# next, write ABBC trials (tasks CCWW)
for block in range(0, num_blocks):
    block_name = 'ABBC_' + str(block)

    write_trial (block_name, 0, 0, 1, 1, "HebP=2", "") 
    write_trial (block_name, 1, 1, 2, 1, "HebP=2", "") 
    write_trial (block_name, 2, 1, 2, 0, "HebP=2", "") 
    write_trial (block_name, 3, 2, 0, 0, "HebP=2", "") 


# next, write ABCB trials
for block in range(0, num_blocks):
    block_name = 'ABBC_' + str(block)

    write_trial (block_name, 0, 0, 1, 1, "HebP=2", "") 
    write_trial (block_name, 1, 1, 2, 1, "HebP=2", "") 
    write_trial (block_name, 2, 2, 0, 0, "HebP=2", "") 
    write_trial (block_name, 3, 1, 2, 0, "HebP=2", "") 
