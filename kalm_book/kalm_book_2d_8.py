# -*- coding: utf-8 -*-
"""
Created on Sun Mar 13 14:42:31 2016

@author: zaqwes
"""

from numpy.random import randn
import copy
import matplotlib.pyplot as plt
import numpy as np
import book_plots as bp
from filterpy.kalman import KalmanFilter
from scipy.linalg import block_diag
from filterpy.common import Q_discrete_white_noise
from filterpy.stats import plot_covariance_ellipse
from book_plots import plot_kf_output

class PosSensor1(object):
    def __init__(self, pos=(0, 0), vel=(0, 0), noise_std=1.):
        self.vel = vel
        self.noise_std = noise_std
        self.pos = [pos[0], pos[1]]
        
    def read(self):
        self.pos[0] += self.vel[0]
        self.pos[1] += self.vel[1]
        return [self.pos[0] + randn() * self.noise_std,
                self.pos[1] + randn() * self.noise_std]
     
if False:           
    pos, vel = (4, 3), (2, 1)
    sensor = PosSensor1(pos, vel, noise_std=1)
    ps = np.array([sensor.read() for _ in range(50)])
    #bp.plot_measurements(ps[:, 0], ps[:, 1]);

def tracker1():
    #
    #
    # Design 2D filter
    #
    #
    
    # 1. Choose state vars - x
    
    # 2. Design state trans. Function - F
    tracker = KalmanFilter(dim_x=4, dim_z=2)
    dt = 1.
    # time step 1 second
    tracker.F = np.array([[1, dt, 0, 0],
                            [0, 1, 0, 0],
                            [0, 0, 1, dt],
                            [0, 0, 0, 1]])
                            
    # 3. Design Process Noise Mat - Q
    v = 0.05
    q = Q_discrete_white_noise(dim=2, dt=dt, var=v)
    tracker.Q = block_diag(q, q)

    # 4. B
    # 5. Design measurement function
    tracker.H = np.array([[1/0.3048, 0, 0, 0],
                          [0, 0, 1/0.3048, 0]])
                          
    # 6. Design Meas. Noise Mat - R
    tracker.R = np.array([[5, 0],[0, 5]])
    
    # Init conditions
    tracker.x = np.array([[0, 0, 0, 0]]).T
    tracker.P = np.eye(4) * 500.
    return tracker
    
def a():
    class ConstantVelocityObject(object):
        def __init__(self, x0=0, vel=1., noise_scale=0.06):
            self.x = x0
            self.vel = vel
            self.noise_scale = noise_scale
        def update(self):
            self.vel += randn() * self.noise_scale
            self.x += self.vel
            return (self.x, self.vel)
            
    def sense(x, noise_scale=1.):
        return x[0] + randn()*noise_scale

    np.random.seed(124)
    obj = ConstantVelocityObject()
    xs, zs = [], []
    for i in range(50):
        x = obj.update()
        z = sense(x)
        xs.append(x)
        zs.append(z)
    xs = np.asarray(xs)
    bp.plot_track(xs[:, 0])
    bp.plot_measurements(range(len(zs)), zs)
    plt.legend(loc='best');
    
    def ZeroOrderKF(R, Q, P=20):
        """ Create zero order Kalman filter.
        Specify R and Q as floats."""
        kf = KalmanFilter(dim_x=1, dim_z=1)
        kf.x = np.array([0.])
        kf.R *= R
        kf.Q *= Q
        kf.P *= P
        kf.F = np.eye(1)
        kf.H = np.eye(1)
        return kf
        
    def FirstOrderKF(R, Q, dt):
        """ Create first order Kalman filter.
        Specify R and Q as floats."""
        kf = KalmanFilter(dim_x=2, dim_z=1)
        kf.x = np.zeros(2)
        kf.P *= np.array([[100, 0], [0, 1]])
        kf.R *= R
        kf.Q = Q_discrete_white_noise(2, dt, Q)
        kf.F = np.array([[1., dt],
        [0., 1]])
        kf.H = np.array([[1., 0]])
        return kf
        
    def SecondOrderKF(R_std, Q, dt, P=100):
        """ Create second order Kalman filter.
        Specify R and Q as floats."""
        kf = KalmanFilter(dim_x=3, dim_z=1)
        kf.x = np.zeros(3)
        kf.P[0, 0] = P
        kf.P[1, 1] = 1
        kf.P[2, 2] = 1
        kf.R *= R_std**2
        kf.Q = Q_discrete_white_noise(3, dt, Q)
        kf.F = np.array([[1., dt, .5*dt*dt],
        [0., 1.,
        dt],
        [0., 0.,
        1.]])
        kf.H = np.array([[1., 0., 0.]])
        return kf
    
    def simulate_system(Q, count):
        obj = ConstantVelocityObject(x0=.0, vel=0.5, noise_scale=Q)
        xs, zs = [], []
        for i in range(count):
            x = obj.update()
            z = sense(x)
            xs.append(x)
            zs.append(z)
        return np.asarray(xs), np.asarray(zs)
        
    def filter_data(kf, zs):
        xs, ps = [], []
        for z in zs:
            kf.predict()
            kf.update(z)
            xs.append(kf.x)
            ps.append(kf.P.diagonal()) # just save variances
        return np.asarray(xs), np.asarray(ps)
        
    def plot_residuals(xs, filter_xs, Ps, title, y_label, stds=1):
        res = xs - filter_xs
        plt.plot(res)
        bp.plot_residual_limits(Ps, stds)
        bp.set_labels(title, 'time (sec)', y_label)
        plt.show()
                
    # 1
    R, Q = 1, 0.03
    xs, zs = simulate_system(Q=Q, count=50)
    kf = FirstOrderKF(R, Q, dt=1)
    filter_xs1, ps1 = filter_data(kf, zs)
    plt.figure()
    bp.plot_kf_output(xs, filter_xs1, zs)
    plot_residuals(xs[:, 0], filter_xs1[:, 0], ps1[:, 0],
                   title='First Order Position Residuals(1$\sigma$)', y_label='meters')
    plot_residuals(xs[:, 1], filter_xs1[:, 1], ps1[:, 1],
        title='First Order Velocity Residuals(1$\sigma$)',
        y_label='meters/sec')
        
    kf0 = ZeroOrderKF(R, Q)
    filter_xs0, ps0 = filter_data(kf0, zs)
    plot_kf_output(xs, filter_xs0, zs)

if __name__=='__main__':  
    if False:
        # fixme: vary R and Q
        N = 30
        sensor = PosSensor1 ((0, 0), (2, 1), 1.)
        zs = np.array([np.array([sensor.read()]).T for _ in range(N)])
        # run filter
        robot_tracker = tracker1()
        mu, cov, _, _ = robot_tracker.batch_filter(zs)
        
        for x, P in zip(mu, cov):
            # covariance of x and y
            cov = np.array([[P[0, 0], P[2, 0]],
            [P[0, 2], P[2, 2]]])
            mean = (x[0, 0], x[2, 0])
            plot_covariance_ellipse(mean, cov=cov, fc='g', alpha=0.15)
        print robot_tracker.P
    
        # plot results
        zs *= .3048 # convert to meters
        bp.plot_filter(mu[:, 0], mu[:, 2])
        bp.plot_measurements(zs[:, 0], zs[:, 1])
        plt.legend(loc=2)
        plt.gca().set_aspect('equal')
        plt.xlim((0, 20));
        
    if True:
        a()

