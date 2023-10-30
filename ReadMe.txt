Compile with:
`make all`

Start standard FloodMax algorithm locally with:
`mpiexec -n <number_of_processes> ./FloodMax`
or on the cluster
`srun -n <number_of_processes> --mpi=pmi2 ./FloodMax`

Start the optimized FloodMax algorithm locally with:
`mpiexec -n <number_of_processes> ./OptFloodMax`
or on the cluster
`srun -n <number_of_processes> --mpi=pmi2 ./OptFloodMax`

For both algorithms, each process will output whether it is the leader or not.
Finally, the elected leader will output the total number of messages sent.
