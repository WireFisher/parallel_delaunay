# PatCC

An Efficient **Pa**rallel **T**riangulation Algorithm for Spherical and Planar Grids with **C**ommonality and Parallel **C**onsistency.

## Feature

- MPI and OpenMP parallelism.
- Support for spherical grids and planar grids.
- Dynamic expansion.
- Low computing redundancy.
- High efficiency.
- Compatible with earth system model grids.

## Build

First of all, edit `Makefile` and set the `MPI_PATH` variable.

**For a quick start:**

Just execute `make` in this directory.

**For advanced usage:**

PATCC has some useful build-in functions, such as plotting, reading NetCDF files and debugging. For this purpose, some environment variables can be set, including `PDLN_USE_OPENCV`, `PDLN_USE_NETCDF`, `PDLN_TIMING` and `PDLN_DEBUG`.

## Execute

The command of executing is likely `OMP_NUM_THREADS=nt mpiexec -n np ./patcc gridFile`.  

**nt**: number of openMP threads.  
**np**: number of MPI processes.  
**gridFile**: a file containing formatted grid info.  

If you build the program without any corresponding environment variables set, the program will print very few logs on the screen.  
At end of the execution, the program will write results to `log/global_triangles_*` file.

### Grid file format

Grid file uses ASCII encoding method, and the format is as follows:

**1st line**: N, number of points to read.  
**2nd line**: Boundary of the points (minLon maxLon minLat maxLat).  
**3rd~N+2th lines**: Coordinate values in degree of each point (lon lat).  

An example of the grid file named `test.grid` can be found in this directory.

> This project runs as part of [C-Coupler](https://github.com/C-Coupler-Group/c-coupler-lib).
