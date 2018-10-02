#!/usr/bin/env python2

import random
import math

math.pi = 3.14159265358979323846264338327950288419716939937510
precision = 100000000000.0

random.seed(2333)

def lonlat2xyz(lon, lat):
    x = math.cos(lon)
    y = math.sin(lon)
    z = math.sin(lat)
    return x, y, z

num_points = 100000
# random on lat-lon global grid
min_lon = 0
max_lon = 360
min_lat = -90
max_lat = 90

corpus = set()
#old_len = 0
#count = 0
while len(corpus) < num_points:
    lon = random.randint(min_lon * precision, max_lon * precision - 1) / precision
    lat = random.randint(min_lat * precision, max_lat * precision) / precision
    corpus.add((lon, lat))
    #if old_len == len(corpus):
    #    count += 1
    #else:
    #    old_len = len(corpus)
    #    count = 0
    #if count > 100:
    #    break

fp = open("lonlat_random_global_%d.dat" % num_points, "w")
for l in corpus:
    x, y, z = lonlat2xyz(math.radians(l[0]), math.radians(l[1]))
    fp.write("%.10lf %.10lf %.10lf\n" % (x, y, z))
fp.close()


# random on lat-lon regional grid
min_lon = 0
max_lon = 180
min_lat = -45
max_lat = 45

corpus.clear()
while len(corpus) < num_points:
    lon = random.randint(min_lon * precision, max_lon * precision - 1) / precision
    lat = random.randint(min_lat * precision, max_lat * precision) / precision
    corpus.add((lon, lat))

fp = open("lonlat_random_regional_%d.dat" % num_points, "w")
for l in corpus:
    x, y, z = lonlat2xyz(math.radians(l[0]), math.radians(l[1]))
    fp.write("%.10lf %.10lf %.10lf\n" % (x, y, z))
fp.close()


# random on lat-lon global grid
min_lon = 0
max_lon = 360
min_lat = -90
max_lat = 90

fp = open("lonlat_uniform_global_%d.dat" % num_points, "w")
edge_points = int(math.sqrt(num_points))
for i in xrange(edge_points):
    for j in xrange(edge_points):
        lon = min_lon + (max_lon - min_lon) * i / edge_points
        lat = min_lat + (max_lat - min_lat) * i / (edge_points-1)
        x, y, z = lonlat2xyz(lon, lat)
        fp.write("%.10lf %.10lf %.10lf\n" % (x, y, z))
fp.close()