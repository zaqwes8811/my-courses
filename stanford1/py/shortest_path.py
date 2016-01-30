# coding: utf-8

# C++
# http://www.e-olimp.com/articles/21
# http://hardfire.ru/Dij_sparse

# Weak 5
# TODO: hear lession + lession about heap

"""
NAIVE:
Initialize:
- X = [S]  [vert. processed so far]
- A[S] = 0  [computed shortest path distances]
[- B[S] = []]  [path - help only]

- while X != V:
    - among all edges (v, w) in E, with v in X, w not in X
    - pick the one that minimize A[v] + l_vw  [call it (v*, w*)]
    - add w* to X
    - set A[w*] := A[v*] + l_v*w*
    [- set B[w*] := B[v*] or (v*, w*)]

WITH HEAP:

"""