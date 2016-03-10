#!/usr/bin/env python
# coding: utf-8

# mus and sigmas - не достаточно

# fixme: похоже чтобы воспользоваться корреляцией
#   нужно знать ковариационную матрицу

import sys
import math

sys.path.append('./code')

import numpy as np
from numpy.random import randn
from filterpy.kalman import KalmanFilter

#from mkf_internal import show_residual_chart
#show_residual_chart()

def compute_dog_data( z_var, process_var, count=1, dt=1.):
    x, vel = 0., 1.
    z_std = math.sqrt( z_var )
    p_std = math.sqrt( process_var )
    xs, zs = [], []
    for _ in range( count ):
        v = vel + (randn() * p_std * dt)
        x += v*dt
        xs.append( x )
        zs.append( x + randn()*z_std)
    return np.array( [xs, zs]).T
    
