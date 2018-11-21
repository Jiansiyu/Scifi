#!/bin/sh
# run=$1



#for run in {5192..5197}
#do

run=5195

# npe_th0.3
# scp wm122:~/FADC_${run}.dat data/
scp eel122@hapc4:~/data2/SciFi_${run}.dat data/
scp eel122@hapc4:~/data2/README RUN_DATA_INFO

 analyzer "replay_hcal_test.C(${run})" <<EOF


.x conv1_JW.cc(${run},0)
.x conv1_JW.cc(${run},1)
.x all_channel_ped.cc(${run}, 1)
.x conv1_JW.cc(${run},0)
.x conv1_JW.cc(${run},1)
.x showcharge.cc(${run},0,1)
.x ./singlePE/getSPE_allch.cc(${run})
.x conv2.cc(${run},0)
.x conv2.cc(${run},1)
.q
EOF

#done
#root -l<<EOF
#.x 
#EOF


# root -l <<EOF

#root -l  <<EOF
#.x all_channel_ped.cc(${run}, 1)
# .x conv1_JW.cc(${run},0)
# .x conv1_JW.cc(${run},1)
# .x showcharge.cc(${run},0,1)
# .x ./singlePE/getSPE.cc(${run})
