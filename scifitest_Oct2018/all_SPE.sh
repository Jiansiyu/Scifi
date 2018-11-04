#!/bin/sh


# run=$1

run = 5056
#
# for run in {5006..5048}
# do
#
# npe_th0.3
#scp wm122:~/FADC_${run}.dat data/
#scp eel122@hapc4:~/data2/SciFi_${run}.dat data/

#analyzer "replay_hcal_test.C(${run},100000)" <<EOF
# analyzer "replay_hcal_test.C(${run})" <<EOF

root -l <<EOF
 

.x ./singlePE/getSPE_allch.cc(${run})



.x conv2.cc(${run},0)
.x conv2.cc(${run},1)


.q
EOF


#done
#root -l<<EOF
#.x 
#EOF
