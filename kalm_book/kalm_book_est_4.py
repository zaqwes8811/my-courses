#!/usr/bin/env python
# coding: utf-8

import math

import numpy as np
from numpy.random import randn

from book_format import figsize, set_figsize
import code.book_plots as bp
import matplotlib.pyplot as plt
from matplotlib.pyplot import show, grid
from kf_internal import plot_dog_track

## model with process noise
# noisy_vel = vel + randn()*velocity_std
# x = x + noisy_vel*dt
#
## measure
# z = x + randn()*measurment_std

np.random.seed(1234)

class DogSimulation(object):
    def __init__(self, x0=0, velocity=1,
                 measurement_var=.0,
                 process_var=.0):
        self.x = x0
        self.velocity = velocity

        self.x_start = x0
        self.v_start = velocity

        self.meas_noise = math.sqrt(measurement_var)
        self.process_noise = math.sqrt(process_var)

    def move(self, dt=1.):
        dx = (self.velocity + randn() * self.process_noise) * dt
        self.x += dx
        return self.x

    def sense_position(self, dt=1.):
        # step
        dx = self.v_start * dt
        self.x_start += dx

        # measure
        measurement = self.x_start + randn() * self.meas_noise
        return measurement

    def move_and_sense(self):
        self.move()
        return self.sense_position()


# dog = DogSimulation(measurment_var=.0)
# N = 10
# xs = np.zeros( N )
# for i in range(N):
#    dog.move()
#    xs[i] = dog.sense_position()
#    print('%.1f' % xs[i]),
#    print " ",
#    
# bp.plot_track( xs, label='pos')
# grid()
# bp.show_legend()


def test_sensor(measurement_var, process_var=.0):
    dog = DogSimulation(0., 1., measurement_var, process_var)
    N = 50
    xs = np.zeros(N)
    pure_xs = np.zeros(N)
    for i in range(N):
        xs[i] = dog.move()
        pure_xs[i] = dog.sense_position()
    plot_dog_track(xs, pure_xs, measurement_var, process_var)

# fixme: сомнительный пример
# fixme: brocken example
# test_sensor(measurement_var=.0, process_var=.5)

# Math
from book_format import set_figsize, figsize
import  filterpy.stats as stats

# # depict our belief
# with figsize(y=3):
#     stats.plot_gaussian_pdf(mean=23, variance=5)
#
# show()
#
# dog = DogSimulation(23, 0, measurement_var=5, process_var=0.0)
# xs = range(100)
# ys = []
# for _ in xs:
#     dog.move()
#     ys.append(dog.sense_position())
# bp.plot_track(xs, ys, label='Dog position')
# plt.legend(loc='best');
#
# show()
