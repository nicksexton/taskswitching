#!/bin/bash

# ./sim_4_clean.sh
# ./sim_6_clean.sh

mkdir /media/ramdisk/simulation_6d_diagnostic
cp sim_4_asymmetric.py /media/ramdisk/simulation_6d_diagnostic
cp sim_6d_model_diagnostic.conf /media/ramdisk/simulation_6d_diagnostic
cd /media/ramdisk/simulation_6d_diagnostic

./sim_4_asymmetric.py -n 500
mv sim_4_trials.conf sim_6_trials.conf
mv sim_4_lookup.txt sim_6_lookup.txt

~/Programming/c_pdp_models/3task_basic_koch_conflict -t sim_6_trials.conf -m sim_6_model_diagnostic.conf > sim_6_log.txt # test conf file

mv 3task_act.txt sim_6_act.txt
mv 3task_data.txt sim_6_data.txt
