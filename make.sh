#!/bin/bash

clear
mpigxx -g -DDEBUG -Wall -fopenmp -isystem /opt/intel/impi/3.2.0.011/include64 -isystem /home/yanghy/opt/opencv/include -isystem dependency/googletest/include -isystem dependency/googlemock/include -isystem /opt/netCDF-gcc4.4.7/include/ -L/home/yanghy/opt/opencv/lib64 -L/opt/netCDF-gcc4.4.7/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lnetcdf -pthread processing_unit_mgt.cxx delaunay_grid_decomposition_mgt.cxx delaunay_voronoi_2D.cxx component.cxx ccpl_utils.cxx dependency/googlemock/libgmock.a dependency/googletest/libgtest.a -o pDelaunay
