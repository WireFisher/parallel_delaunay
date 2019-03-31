#ifndef PDLN_CCPL_UTILS_H
#define PDLN_CCPL_UTILS_H

#include "common_utils.h"

#define DEGREE_TO_RADIAN(data)    (data*PI/180.0)
#define RADIAN_TO_DEGREE(data)    (data*180.0/PI)

extern void calculate_stereographic_projection(double, double, double, double, double&, double&);
extern void fast_stereographic_projection(double, double, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, PAT_REAL, double&, double&);
extern bool point_in_circle(double, double, double*);

#endif
