#!/usr/bin/env python
# coding: utf-8

import numpy as np

from book_format import figsize, set_figsize
import code.book_plots as bp
import matplotlib.pyplot as plt
from matplotlib.pyplot import show, grid

# def predict( belief, move, p_correct, p_under, p_over):
# 	n = len(belief)
# 	result = np.zeros( n )
# 	w = [p_under, p_correct, p_over]
# 	V = move
# 	for i in range( n ):
# 		result[i] = (
# 			belief[(i-V)%n] * p_correct + belief[(i-V-1)%n] * p_over +
# 			belief[(i-V+1)%n] * p_under)
# 	belief[:] = result

# belief = np.array([0., 0., 0., 1., 0., 0., 0., 0., 0., 0.])
# predict( belief, 2, .8, .1, .1)
# bp.bar_plot( belief )
# grid()
# show()

#
#
#
#
belief = np.array([1/10.]*10)
np.set_printoptions(precision=3)
print belief

hallway = np.array( [1,1,0,0,0,0,0,0,1,0])
belief = hallway*1./3
set_figsize( y=2 )
bp.bar_plot( belief )