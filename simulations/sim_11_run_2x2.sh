
	#!/bin/bash

# ./sim_4_clean.sh
# ./sim_6_clean.sh

rm 3task_act.txt
rm 3task_data.txt
rm sim_11_log_2x2.txt
rm sim_11_trials_2x2.conf
rm sim_11_lookup_2x2.txt

./sim_11_trials_2x2.py -n 4000
# ../3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_asymmetric.conf > sim_6_log_small.txt
../3task_basic_koch_conflict -t sim_11_trials_2x2.conf -m sim_11_model_symmetric.conf > sim_11_log_2x2.txt # test conf file

mv 3task_act.txt sim_11_act_2x2.txt
mv 3task_data.txt sim_11_data_2x2.txt
