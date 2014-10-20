#!/bin/bash
# script which runs the asymmetric 3-task switching protocol with symmetric tasks

# ./sim_4_clean.sh
rm sim_4_trials.conf
rm sim_4_lookup.txt
rm sim_4_data.txt sim_4_01_data.txt
rm sim_4_act.txt sim_4_01_act.txt
rm sim_4_01_log.txt


./sim_4_asymmetric.py -n 200

../3task_basic -t sim_4_trials.conf -m sim_4_01_model_symmetric.conf > sim_4_01_log.txt

mv 3task_act.txt sim_4_01_act.txt
mv 3task_data.txt sim_4_01_data.txt
