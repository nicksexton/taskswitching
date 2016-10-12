#!/bin/bash

# ./sim_3_clean.sh
rm sim_18_data.txt
rm sim_18_log.txt
rm sim_18_trials.conf
rm sim_18_lookup.txt

./sim_18_trials.py -n 50 -t 400 -p 0.33 # -p is probability of a repeat trial


../3task_basic_koch_conflict -t sim_18_trials.conf -m sim_18_model.conf > sim_18_log.txt


rm 3task_act.txt
mv 3task_data.txt sim_18_data_inhib.txt



../3task_basic_koch_conflict -t sim_18_trials.conf -m sim_18_model_noinhib.conf > sim_18_log.txt


rm 3task_act.txt
mv 3task_data.txt sim_18_data_noinhib.txt
