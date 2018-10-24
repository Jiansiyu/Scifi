#!/bin/sh

#run=$1

root -l<<EOF
.x conv2.cc(4622,1)
.x conv2.cc(4623,1)
.x conv2.cc(4624,1)
.x conv2.cc(4625,1)

.x conv2.cc(4622,0)
.x conv2.cc(4623,0)
.x conv2.cc(4624,0)
.x conv2.cc(4625,0)
.q
EOF

