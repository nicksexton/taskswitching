3#!/bin/bash

# ./sim_4_clean.sh
# ./sim_6_clean.sh

rm sim_4_trials.conf
rm sim_6_trials.conf
rm sim_4_lookup.txt
rm sim_6_lookup.txt
rm sim_4_data.txt
rm sim_6_data.txt
rm sim_4_act.txt
rm sim_6_act.txt

rm sim_6_log.txt
rm sim_6_log_small.txt

./sim_4_asymmetric.py -n 500
mv sim_4_trials.conf sim_10_trials.conf
mv sim_4_lookup.txt sim_10_lookup.txt

# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_10_trials.conf -m sim_10_model_asymmetric.conf > sim_10_log.txt # test conf file

mv 3task_act.txt sim_10_act.txt
mv 3task_data.txt sim_10_data.txt
