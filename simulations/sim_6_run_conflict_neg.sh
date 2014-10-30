#!/bin/bash

# Script to run & compare 3 approaches to negative conflict: allow, clip and rescale.

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
rm sim_6_log_allow.txt
rm sim_6_log_basic.txt
rm sim_6_log_rescale.txt

#rm sim_6_data_conflict_neg_allow.txt
#rm sim_6_data_conflict_neg_clip.txt
#rm sim_6_data_conflict_neg_rescale.txt
#rm sim_6_act_conflict_neg_allow.txt
#rm sim_6_act_conflict_neg_clip.txt
#rm sim_6_act_conflict_neg_rescale.txt

./sim_4_asymmetric.py -n 500
mv sim_4_trials.conf sim_6_trials.conf
mv sim_4_lookup.txt sim_6_lookup.txt

# conflict off
../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_off.conf > sim_6_log_off.txt
mv 3task_act.txt sim_6_act_conflict_off.txt
mv 3task_data.txt sim_6_data_conflict_off.txt


# Allow negative conflict
#../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_allow.conf > sim_6_log_allow.txt
#mv 3task_act.txt sim_6_act_conflict_neg_allow.txt
#mv 3task_data.txt sim_6_data_conflict_neg_allow.txt

# clip negative conflict
#../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_clip.conf > sim_6_log_clip.txt
#mv 3task_act.txt sim_6_act_conflict_neg_clip.txt
#mv 3task_data.txt sim_6_data_conflict_neg_clip.txt

# rescale conflict
#../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_symmetric_conflict_rescale.conf > sim_6_log_rescale.txt
#mv 3task_act.txt sim_6_act_conflict_neg_rescale.txt
#mv 3task_data.txt sim_6_data_conflict_neg_rescale.txt