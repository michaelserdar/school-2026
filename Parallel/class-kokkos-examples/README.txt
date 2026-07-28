# Running Kokkos #
To use Kokkos on the cluster, you will want to load the Kokkos packages

spack load kokkos@4.7~cuda gcc@14 openmpi cmake

This loads Kokkos 4.7 without cuda, but with gcc, openmpi, cmake. If you want to run with cuda to test things, use +cuda instead of ~cuda.

You will also want to go to the directory cd /work/classes/csc4760-001-2026s/<username> and use your username.
Make sure to use this for building and running your code to avoid running out of space.
Additionally, run either "hpcshell --account=csc4760-001-2026s --cpus-per-task=2" or "salloc --account=csc4760-001-2026s --cpus-per-task=2" to go off the login node onto a node with actual resources. 
You can change --cpu-per-task to be more or less nodes. For more information, use --help flag or look up slurm documentation.

From here, you should be able to build and run any Kokkos program given the necessary cmake files.

To build and run this directory, after doing above, run "sbatch ./kokkos.sh" after putting this directory in /work/classes/csc4760-001-2026s/<username>/
This will create and go to a build directory, build files with cmake and make, then run the program.
All of this will be output to a slurm output file named something like, slurm-523080.out. You should see "Goodbye World" as the last line if all went well.

If you want you can alter this by editing HelloKokkos.cpp using the nano or vim text editors.
