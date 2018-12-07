#!/bin/sh
# run=$1



#for run in {5192..5197}
#do

run=111893

# npe_th0.3
# scp wm122:~/FADC_${run}.dat data/
# scp eel122@hapc4:~/data2/SciFi_${run}.dat data/
#scp eel122@hapc4:~/data2/README RUN_DATA_INF
#scp /adaqfs/home/a-onl/apex/HallA-APEX-Online/replay/t2root/Rootfiles/apex_online_${run}.root data/

# scp eel122@hapc4:~/data2/README RUN_DATA_INFO

 analyzer  <<EOF

.x conv1_JW.cc(${run},0,0)


.q
EOF

# .x all_channel_ped_new.cc(${run})
# .x conv1_JW.cc(${run},0,0)
# .x all_channel_ped.cc(${run}, 1)
# .x conv1_JW.cc(${run},0)

# .x showcharge.cc(${run},0,1)
# .x ./singlePE/getSPE_allch.cc(${run})
# .x conv2.cc(${run},0)




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
