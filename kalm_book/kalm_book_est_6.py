#!/usr/bin/env python
# coding: utf-8

# mus and sigmas - не достаточно

# fixme: похоже чтобы воспользоваться корреляцией
#   нужно знать ковариационную матрицу

import sys
import math

import numpy as np
from numpy.random import randn
from filterpy.kalman import KalmanFilter
from filterpy.stats import plot_covariance_ellipse
from matlab_ext.plotter import show
from filterpy.common import Q_discrete_white_noise
from mkf_internal import plot_track
from book_format import set_figsize, figsize
from nonlinear_plots import plot_gaussians

def compute_dog_data( z_var, process_var, count=1, dt=1.):
    x, vel = 0., 1.
    z_std = math.sqrt( z_var )
    p_std = math.sqrt( process_var )
    xs, zs = [], []
    cntr = 0
    for _ in range( count ):
        v = vel + (randn() * p_std * dt)
        x += v*dt
        xs.append( x )
        zs.append( x + randn()*z_std)
        
        if cntr == 20000:
            vel = 0.2
        
        cntr += 1
    return np.array( [xs, zs]).T

def template():
    kf = KalmanFilter(dim_x=2, dim_z=1)
    
    # x0 
    kf.x = np.array([[.0], [.0]])
        
    # P - change over time
    kf.P = np.diag([500., 49.])
    
    # F - state transition matrix
    dt = .1
    kf.F = np.array([[1, dt], [0, 1]])
    
    ## now can predict
    ## дисперсия растет и становится видна корреляция
    #plot_covariance_ellipse(kf.x, kf.P, edgecolor='r')
    #kf.predict()
    #plot_covariance_ellipse(kf.x, kf.P, edgecolor='k', ls='dashed')
    #show()
    
    # Control information
    kf.B = 0
    
    # !!Attention!! Q! Process noise
    kf.Q = Q_discrete_white_noise( dim=2, dt=dt, var=2.35)
    
    # H - measurement function
    kf.H = np.array([[1., 0.]])
    
    # R - measure noise matrix
    kf.R = np.array([[5.]])

def pos_vel_filter(x, P, R, Q=0., dt=1.0):
    """ Returns a KalmanFilter which implements a
    constant velocity model for a state [x dx].T
    """
    kf = KalmanFilter(dim_x=2, dim_z=1)
    kf.x = np.array([x[0], x[1]]) # location and velocity6.4. TRACKING A DOG
    kf.F = np.array([[1, dt], [0, 1]])  # state transition matrix
    kf.H = np.array([[1, 0]])  # Measurement function
    kf.R *= R  # measurement uncertainty
    if np.isscalar(P):
        kf.P *= P # covariance matrix
    else:
        kf.P[:] = P
    if np.isscalar(Q):
        kf.Q = Q_discrete_white_noise(dim=2, dt=dt, var=Q)
    else:
        kf.Q = Q
    return kf
    
#def plot_track(ps, actual, zs, cov, std_scale=1,
#           plot_P=True, y_lim=None, dt=1.,
#           xlabel='time', ylabel='position',
#           title='Kalman Filter'):

def run(x0=(0.,0.), P=500, R=0, Q=0, dt=1.0, data=None,
        count=0, do_plot=True, **kwargs):
    """
    ‘data‘ is a 2D numpy array; the first column contains
    the actual position, the second contains the measurements
    """
    # Simulate dog if no data provided. This is handy because
    # it ensures that the noise in the dog simulation and the
    # kalman filter are the same.
    if data is None:
        data = compute_dog_data(R, Q, count)
    # create the Kalman filter
    kf = pos_vel_filter(x0, R=R, P=P, Q=Q, dt=dt)
    # run the kalman filter and store the results
    xs, cov = [], []
    c = 0
    print data.shape
    for z in data[:, 1]:
        kf.predict()
        if True: #c > 20 or c < 10:        
            kf.update(z)
            
        c += 1
            
        xs.append(kf.x)
        cov.append(kf.P)
    xs, cov = np.array(xs), np.array(cov)
    if do_plot:
        plot_track(xs[:, 0], data[:, 0], data[:, 1], cov, dt=dt, **kwargs)
    return xs, cov
    
P = np.diag([500., 49.])
#Ms, Ps = run(count=50, R=20, Q=0.01, P=P, do_plot=True)
#with figsize(y=8):
#    plot_gaussians(Ms[::10], Ps[::10], (-5,90), (-5, 5), 55)

from numpy.random import seed
seed(2)
data = compute_dog_data(z_var=225, process_var=.02, count=50)
run(data=data, R=225, Q=200, P=P, plot_P=False, title='R_var = 225 $m^2$, Q_var = 20 $m^2$')
run(data=data, R=225, Q=.02, P=P, plot_P=False, title='R_var = 225 $m^2$, Q_var = 0.02 $m^2$');
run(data=data, R=10000, Q=.2, P=P, plot_P=False,
title='R=$10,000\, m^2$, Q=$.2\, m^2$');











