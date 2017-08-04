make
DIR="."
$DIR/my-solver --copy >copy.in
$DIR/my-solver --write <copy.in >input.tsp
$DIR/LKH lkh.par &>LKH-output
$DIR/my-solver --solve <copy.in
