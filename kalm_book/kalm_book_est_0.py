#!/usr/bin/env python
# coding: utf-8

import code.book_plots as book_plots
import book_format
import matplotlib.pyplot as plt
from matplotlib.pyplot import show, grid
import code.gh_internal as gh
from code.gh_internal import plot_gh_results
import numpy as np

weights = [158.0, 164.2, 160.3, 159.9, 162.1, 164.6, 169.6, 167.4, 166.4, 171.0, 171.2, 172.6]
time_step = 1. # day
scale_factor = 4./10

def predict_using_gain_guess( weight, gain_rate ):
	# store the filtered results
	estimates, predictions = [weight], []
	# most filter literature uses 'z' for measurements
	for z in weights:
		# predict
		prediction = weight + gain_rate * time_step

		# update
		residual = z - prediction
		weight = prediction + scale_factor * residual
		
		# save
		estimates.append( weight )
		predictions.append( prediction )
		gh.print_results( estimates, prediction, weight )

	# plot results
	gh.plot_gh_results(weights, estimates, predictions)

initial_guess = 160.

# если модель не соответствуе, то начинает перетягивать оценку на себя
#
# Фильтр, которые зависит от guess какого-то параметра - плохой фильтр 
#   Станет трудно отслеживать изменения
#predict_using_gain_guess(weight=initial_guess, gain_rate=0)#1)

#
#
#
#
weight = 160

# Чему мы больше верим, модели или измерениям
weight_scale = 4/10.  # g
gain_scale = 1/3.  # h

gain_rate = 1.
estimates = [weight]
predictions = []

weight_est = weight
for z in weights:
	# One Epoche

	# pred. - "system propagation
	# uses process model to from new state estimate"
	# == evolution
	w_prediction = weight_est + gain_rate*time_step
	g_prediction = gain_rate

	# upd. "measurement update"
	residual = z - w_prediction
	weight_est = w_prediction + weight_scale*residual
	gain_rate = g_prediction + gain_scale*(residual/time_step)

	# save
	predictions.append( w_prediction )
	estimates.append( weight_est )
# gh.plot_gh_results( weights, estimates, predictions )

# grid()
# show()


# #
#
#
#
from code.gh_internal import plot_g_h_results

# g - для величины
# h - для производной виличины
def g_h_filter( data, x0, dx, g, h, dt=1.):
	x = x0
	results = []
	for z in data:
		# pred. step
		x_est = x + dx*dt
		dx = dx

		# update step
		residual = z - x_est
		dx = dx + h * (residual) / dt
		x = x_est + g * residual		

		# save
		results.append( x )
	return np.array( results )

# book_plots.plot_track( [0, 11], [160, 172], label='a' )
# data = g_h_filter( data=weights, x0=160, dx=1, g=6./10, h=2./3)
# plot_g_h_results( weights, data )
# plt.show()


from numpy.random import randn

# overshoot and undershoot effects
# def gen_data( x0, dx, count, noise_factor):
# 	return [x0+dx*i+randn()*noise_factor for i in range(count)]

# "The ‘take home' point is
# that the filter is only as good as the mathematical 
# model used to express the system."
def gen_data(x0, dx, count, noise_factor, accel=0):
	zs = []
	for i in range(count):
		zs.append(x0 + dx*i + randn()*noise_factor)
		dx += accel
	return zs
zs = gen_data(x0=10, dx=0, count=20, noise_factor=0, accel=2)
data = g_h_filter( zs, x0=10, dx=0, g=.2, h=0.02)
# plot_g_h_results( zs, data )
# plt.xlim([0, 20])
# show()


# if g up -> преоритет за измерениями
np.random.seed(100)
zs = gen_data(x0=5, dx=5, count=50, noise_factor=50)
data1 = g_h_filter(data=zs, x0=0., dx=5., dt=1., g=0.1, h=0.01)
data2 = g_h_filter(data=zs, x0=0., dx=5., dt=1., g=0.4, h=0.01)
data3 = g_h_filter(data=zs, x0=0., dx=5., dt=1., g=0.8, h=0.01)
# with book_format.figsize(y=6):
# 	book_plots.plot_measurements(zs, lw=1, color='k')
# 	book_plots.plot_filter(data1, label='g=0.1', marker='s')
# 	book_plots.plot_filter(data2, label='g=0.4', marker='v')
# 	book_plots.plot_filter(data3, label='g=0.8', lw=2)
# 	book_plots.show_legend()
# 	book_plots.set_limits([20,40], [0, 250])

zs = [5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
for i in range(50):
	zs.append(14)
data1 = g_h_filter(data=zs, x0=4., dx=1., dt=1.,g=0.1, h=0.01)
data2 = g_h_filter(data=zs, x0=4., dx=1., dt=1.,g=0.5, h=0.01)
data3 = g_h_filter(data=zs, x0=4., dx=1., dt=1.,g=0.9, h=0.01)
# with book_format.figsize(y=5):
# 	book_plots.plot_measurements(zs)
# 	book_plots.plot_filter(data1, label='g=0.1', marker='s')
# 	book_plots.plot_filter(data2, label='g=0.5', marker="v")
# 	book_plots.plot_filter(data3, label='g=0.9', lw=2)
# 	book_plots.show_legend()
# 	plt.ylim([6, 20])

# show()

# from IPython.html.widgets import interact, FloatSlider
# zs = gen_data(x0=5, dx=5, count=100, noise_factor=50)
# def interactive_gh(x, dx, g, h):
# 	data = g_h_filter(data=zs, x0=x, dx=dx, g=g, h=h)
# 	plt.scatter(range(len(zs)), zs, edgecolor='k', facecolor='none',
# 	marker='+', lw=1)
# 	plt.plot(data, color='b')
# 	plt.show()
# interact(interactive_gh,
# 	x=FloatSlider(value=0., min=-50, max=50.),
# 	dx=FloatSlider(value=5., min=-50., max=50.),
# 	g=FloatSlider(value=0.1, min=0.01, max=2, step=.02),
# 	h=FloatSlider(value=0.02, min=0.0, max=0.5, step=0.01));

#"I know you would never set
#both g and h to zero as that takes a special kind of genius that only I
#possess, but I promise that if you are not careful you will set them lower
#than they should be."

#
#
# Tracking train
#
#
#from numpy.random import randn
def compute_new_position(pos, vel, dt=1):
	""" dt is the time delta in seconds."""
	return pos + (vel * dt)
def measure_position(pos):
	return pos + randn()*500
def gen_train_data(pos, vel, count):
	zs = []
	for t in range(count):
		pos = compute_new_position(pos, vel)
		zs.append(measure_position(pos))
	return np.asarray(zs)

pos, vel = 23*1000, 15
zs = gen_train_data(pos, vel, 100)
plt.plot(zs / 1000.) # convert to km
book_plots.set_labels('Train Position', 'time(sec)', 'km')

# ослабление h может помочь проверять отследить ускорение
show()