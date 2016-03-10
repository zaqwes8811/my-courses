#!/usr/bin/env python
# coding: utf-8

# mus and sigmas - не достаточно

# fixme: похоже чтобы воспользоваться корреляцией
#   нужно знать ковариационную матрицу

import numpy as np

from code.gaussian_internal import  plot_correlated_data
import code.mkf_internal as mkf_internal

height = [60, 62, 63, 65, 65.1, 68, 69, 70, 72, 74]
weight = [95, 120, 127, 119, 151, 143, 173, 171, 180, 210]
plot_correlated_data(height, weight, 'Height (in)', 'Weight (lbs)', False)

X = np.linspace( 1, 10, 100 )
Y = np.linspace( 1, 10, 100 )
print np.cov( X, Y )

mean = (2, 17)
cov = [[10.,0],
       [0,4.]]
       
# join prob.    
#mk.plot_3d_covariance( mean, cov )    

from book_format import set_figsize, figsize
with figsize(y=4):
    mkf_internal.plot_3_covariances() 