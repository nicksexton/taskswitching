#!/bin/bash

# ./sim_3_clean.sh
rm sim_16_data.txt
rm sim_16_log.txt
rm sim_16_trials.conf
rm sim_16_lookup.txt

./sim_16_trials.py -n 15 -t 8000 # use 100 for full simulation

# ../3task_basic -t sim_3_trials.conf -m sim_9_model_symmetric.conf > sim_9_log.txt
../3task_basic_koch_conflict -t sim_16_trials.conf -m sim_16_model.conf > sim_16_log.txt

# mv 3task_act.txt sim_9_act.txt
rm 3task_act.txt 
mv 3task_data.txt sim_16_data.txt
