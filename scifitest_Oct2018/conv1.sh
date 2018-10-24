#!/bin/sh

#run=$1

root -l<<EOF
.x conv1.cc(5011,1)
.x conv1.cc(5011,0)

.x conv1.cc(5011,1)
.x conv1.cc(5011,0)

.q
EOF

