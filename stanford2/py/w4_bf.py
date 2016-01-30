# encoding: utf-8
"""
A[0, s] = 0; A[0, v] = +inf for all v != s

# Порядок обхода важен
# Correct if no neg cycles
n  # count vetrex
m  # #edges
# FIXME: а что с параллельными ребрами?
i  # budget
for i = 1, 2... n-1:
    for each v in V:
        A[i, v] = min (
                A[i-1, v]  # 1)
                # in/out edges - in-degree
                # more then O(1). can be O(n)
                # FIXME: (w, v) == (v, w)?
                min (by (w, v) in E) { A[i-1, w] + C_w_v}  # 2)
            )

# O(mn)
# sparse and dense graphs
O(n * sun(in_deg(v)) by V)

# Stopping early
# Calc multiply times
for some j < n-1
A[j, v] = A[j-1, v] for all vertices

# If Neg Cycles
# addition iteration
A[n-1, v] = A[n, v]  # Ok

# Opt space. Now O(n^2)
# O(n) can be if no need reconstruction
# DP algs may be space opt

# Reconstruction
#Add
B[i, v]  # Computing Predcessor Ptrs
# it's little birdy

# Base case
B[0, v] = 0 for add v in V

B[i, v] = B[i-1, v]   # 1)
# 2)

# Opts:
early termination B-F
"Complexity of Network Synchronization BARUCH AWERBUCH "

### Floyd-Warshall
"""

import py_details.io_helper as io_helper

# work
print io_helper.get_fake_graph()

