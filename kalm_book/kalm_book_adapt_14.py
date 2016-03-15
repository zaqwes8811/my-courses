# -*- coding: utf-8 -*-

from math import sin, cos, radians
from numpy.random import randn
from numpy import array
import matplotlib.pyplot as plt
import book_plots as bp
import numpy as np
from matlab_ext.plotter import *

def angle_between(x, y):
    return min(y-x, y-x+360, y-x-360, key=abs)
    
class ManeuveringTarget(object):
    def __init__(self, x0, y0, v0, heading):
        self.x = x0
        self.y = y0
        self.vel = v0
        self.hdg = heading
        self.cmd_vel = v0
        self.cmd_hdg = heading
        self.vel_step = 0
        self.hdg_step = 0
        self.vel_delta = 0
        self.hdg_delta = 0
        
    def update(self):
        vx = self.vel * cos(radians(90-self.hdg))
        vy = self.vel * sin(radians(90-self.hdg))
        self.x += vx
        self.y += vy
        if self.hdg_step > 0:
            self.hdg_step -= 1
            self.hdg += self.hdg_delta
        if self.vel_step > 0:
            self.vel_step -= 1
            self.vel += self.vel_delta
        return (self.x, self.y)
        
    def set_commanded_heading(self, hdg_degrees, steps):
        self.cmd_hdg = hdg_degrees
        self.hdg_delta = angle_between(self.cmd_hdg, self.hdg) / steps
        if abs(self.hdg_delta) > 0:
            self.hdg_step = steps
        else:
            self.hdg_step = 0
        
    def set_commanded_speed(self, speed, steps):
        self.cmd_vel = speed
        self.vel_delta = (self.cmd_vel - self.vel) / steps
        if abs(self.vel_delta) > 0:
            self.vel_step = steps
        else:
            self.vel_step = 0
            
            
class NoisySensor(object):
    def __init__(self, noise_factor=1):
        self.noise_factor = noise_factor
        
    def sense(self, pos):
        return (pos[0] + randn()*self.noise_factor, pos[1] + randn()*self.noise_factor)
        



noise_factor = 2.
def generate_data(steady_count, noise_factor):
    t = ManeuveringTarget(x0=0, y0=0, v0=0.3, heading=0)
    xs = []
    ys = []
    for i in range(30):
        x, y = t.update()
        xs.append(x)
        ys.append(y)
    t.set_commanded_heading(310, 25)
    t.set_commanded_speed(1, 15)
    for i in range(steady_count):
        x, y = t.update()
        xs.append(x)
        ys.append(y)
    ns = NoisySensor(noise_factor=noise_factor)
    pos = array(list(zip(xs, ys)))
    zs = array([ns.sense(p) for p in pos])
    return pos, zs
    
#pos, zs = generate_data(50, noise_factor)
#bp.plot_track(*zip(*pos))
#plt.axis('equal')
#bp.set_labels(title='2D Track', x='X', y='Y')
#
#bp.plot_track(*zip(*pos))
#bp.plot_measurements(*zip(*zs))
#plt.axis('equal')
#plt.legend(loc=4)
#bp.set_labels(title='Track vs Measurements', x='X', y='Y')

from filterpy.kalman import KalmanFilter
from filterpy.common import Q_discrete_white_noise

def make_cv_filter(dt, noise_factor):
    cvfilter = KalmanFilter(dim_x = 2, dim_z=1)
    cvfilter.x = array([0., 0.])
    cvfilter.P *= 3
    cvfilter.R *= noise_factor
    cvfilter.F = array([[1, dt],
    [0, 1]], dtype=float)
    cvfilter.H = array([[1, 0]], dtype=float)
    cvfilter.Q = Q_discrete_white_noise(dim=2, dt=dt, var=2.)#0.02)
    return cvfilter
    
#    cvfilter = KalmanFilter(dim_x = 4, dim_z=2)
#    cvfilter.x = array([0., 0., 0., 0.])
#    cvfilter.P *= 300
#    cvfilter.R *= 1
#    cvfilter.F = array([[1, dt, 0, 0],
#                        [0, 1, 0, 0], [0, 0, 1, dt], [0, 0, 0, 1]], dtype=float)
#    cvfilter.H = array([[1, 0, 0, 0], [0, 0, 1, 0]], dtype=float)
#    q = Q_discrete_white_noise(dim=2, dt=dt, var=0.020)
#    cvfilter.Q[0:2, 0:2] = q
#    cvfilter.Q[2:4, 2:4] = q
#    return cvfilter
    
def initialize_filter(kf, noise_factor=None):
    """ helper function - we will be reinitialing the filter
    many times.
    """
    kf.x.fill(0)
    kf.P = np.eye(kf.dim_x) * .1
    if noise_factor is not None:
        kf.R = np.eye(kf.dim_z) * noise_factor

noise_factor = 2
dt = 0.1
figure()
# initialize filter
cvfilter= make_cv_filter(dt, noise_factor)
initialize_filter(cvfilter)
xs = pos[:, 0]
z_xs = zs[:, 0]
# plot the results
kxs, _, _, _ = cvfilter.batch_filter(z_xs)
t = np.arange(0, len(z_xs) * dt, dt)
bp.plot_track(t, xs)
bp.plot_filter(t, kxs[:, 0], label='KF')
bp.set_labels(title='Track vs KF', x='time (sec)', y='X');
plt.legend(loc=4);

figure()
# reinitialize filter
initialize_filter(cvfilter)
pos2, zs2 = generate_data(120, noise_factor)
xs2 = pos2[:, 0]
z_xs2 = zs2[:, 0]
t = np.arange(0, len(xs2) * dt, dt)
# plot the results
kxs2, _, _, _ = cvfilter.batch_filter(z_xs2)
bp.plot_track(t, xs2)
bp.plot_filter(t, kxs2[:, 0], label='KF')
plt.legend(loc=4)
bp.set_labels(title='Effects of Acceleration',
x='time (sec)', y='X')








        