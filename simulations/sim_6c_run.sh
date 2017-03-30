#!/bin/bash

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
rm sim_6_data_conflict_off.txt
rm sim_6_data_conflict_clip.txt
rm sim_6_data_conflict_allow.txt
rm sim_6_data_conflict_rescale.txt


rm sim_6_log.txt
rm sim_6_log_small.txt

./sim_4_asymmetric.py -n 200
mv sim_4_trials.conf sim_6_trials.conf
mv sim_4_lookup.txt sim_6_lookup.txt


# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_off.conf > sim_6_log.txt # test conf file
#mv 3task_act.txt sim_6_act.txt
mv 3task_data.txt sim_6_data_conflict_off.txt


# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_clip.conf > sim_6_log.txt # test conf file
#mv 3task_act.txt sim_6_act.txt
mv 3task_data.txt sim_6_data_conflict_clip.txt


# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_allow.conf > sim_6_log.txt # test conf file
#mv 3task_act.txt sim_6_act.txt
mv 3task_data.txt sim_6_data_conflict_allow.txt


# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_rescale.conf > sim_6_log.txt # test conf file
#mv 3task_act.txt sim_6_act.txt
mv 3task_data.txt sim_6_data_conflict_rescale.txt


