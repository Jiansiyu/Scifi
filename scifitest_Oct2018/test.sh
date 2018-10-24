#!/bin/sh


# run=$1



for run in {5006..5048}
do
#scp wm122:~/FADC_${run}.dat data/
#scp eel122@hapc4:~/data2/SciFi_${run}.dat data/

#analyzer "replay_hcal_test.C(${run},100000)" <<EOF
analyzer "replay_hcal_test.C(${run})" <<EOF

.x conv1.cc(${run},0)
.x conv1.cc(${run},1)
.q
EOF


done
#root -l<<EOF
#.x 
#EOF
