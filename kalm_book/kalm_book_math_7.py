# -*- coding: utf-8 -*-
"""
Created on Sun Mar 13 13:38:20 2016

@author: zaqwes
"""
import numpy as np

from matplotlib.pyplot import figure

# http://tutorial.math.lamar.edu/Classes/DE/SystemsDE.aspx
from filterpy.common import  van_loan_discretization

F = np.array( [[0., 1.], [-1., 0.]])
G = np.array( [[0.], [2.]] )
phi, Q = van_loan_discretization( F, G, dt=.1)
print Q

import sympy
from sympy import (init_printing, Matrix,MatMul, integrate, symbols)
from sympy import pprint

init_printing(use_latex=False)#use_latex='mathjax')
dt, phi = symbols('dt \Phi_s')
F_k = Matrix([[1, dt, dt**2/2],
[0, 1,
dt],
[0, 0,
1]])
Q_c = Matrix([[0, 0, 0],
[0, 0, 0],
[0, 0, 1]])*phi
Q=sympy.integrate(F_k * Q_c * F_k.T, (dt, 0, dt))
# factor phi out of the matrix to make it more readable
Q = Q / phi
pprint( sympy.MatMul(Q, phi) )