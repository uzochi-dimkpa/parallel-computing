CXX=mpic++
# CXX=mpicxx
LD=mpic++
# LD=mpicxx
# --


bcast:

bcastrun4: bcast
	mpirun -np 4 --output-filename something ./bcast

reduce:

reducerun4: reduce
	mpirun -np 4 --output-filename something ./reduce

scatter:

scatterrun4: scatter
	mpirun -np 4 --output-filename something ./scatter

gather:

gatherrun4: gather
	mpirun -np 4 --output-filename something ./gather
