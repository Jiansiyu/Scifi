#!/bin/sh

#run=$1

root -l<<EOF
.x conv1.cc(4653,1)
.x conv1.cc(4653,0)

.x conv1.cc(4654,1)
.x conv1.cc(4654,0)

.q
EOF

