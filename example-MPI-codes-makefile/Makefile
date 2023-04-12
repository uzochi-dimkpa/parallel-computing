CXX=mpic++
# CXX=mpicxx
LD=mpic++
# LD=mpicxx
# --


bcast:

bcastrun4: bcast
	mpirun -np 4 --output-filename bcast-out ./bcast

reduce:

reducerun4: reduce
	mpirun -np 4 --output-filename reduce-out ./reduce

scatter:

scatterrun4: scatter
	mpirun -np 4 --output-filename scatter-out ./scatter

gather:

gatherrun4: gather
	mpirun -np 4 --output-filename gather-out ./gather
