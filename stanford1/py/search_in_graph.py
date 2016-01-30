# coding: utf-8

# Info:
# http://eddmann.com/posts/depth-first-search-and-breadth-first-search-in-python/
#
# ToThink:
# http://opendatastructures.org/versions/edition-0.1e/ods-java/12_3_Graph_Traversal.html - зависит от вх. предст.

from pprint import pprint
import copy

from Queue import Queue  # Thread-safe - overhead

import random


def get_fake_graph():
    return {'s': ['a', 'b'],  # s
            'a': ['s', 'c'],  # a
            'b': ['s', 'c', 'd'],  # b
            'c': ['a', 'b', 'd', 'e'],  # c
            'd': ['b', 'c', 'e'],  # d
            'e': ['c', 'd']  # e
    }


class Vertex(object):
    """
    About:

    TODO: To expensive impl. """

    def __init__(self, own, ends):
        self.self = own
        self.ends = ends

        # Coupled with node
        self.explored = False
        self.distance = 0

    def __str__(self):
        return "self: %s, ex: %s, dist: %d" % (self.self, self.explored, self.distance)

    @staticmethod
    def recode_graph(g):
        # Prepare data
        # TODO: BAD: дополнительная структура данных и увеличиваю константы в сложности
        # TODO: но! в обхода нет поиска по ключу, но такой же поиск при преобразовании
        store = {}
        for self, raw_ends in g.items():
            store[self] = Vertex(self, raw_ends)

        for no_used, vertex in store.items():
            ref_ends = []
            for elem in vertex.ends:
                ref_ends.append(store[elem])
            vertex.ends = ref_ends
        return store


def dfs_expensive_impl(G, SV):
    """
     Constraint: граф не взвешенный?

     http://cs.stackexchange.com/questions/4973/does-a-weighted-breadth-first-search-have-memory-when-moving-to-the-next-verte

     Dijkstra's algorithm for weighted?

     SV - start vertex
    """
    assert G
    assert SV

    # Finding
    graph_store = Vertex.recode_graph(G)
    sv = graph_store[SV]

    def __dfs(vertex):
        vertex.explored = True
        ends = vertex.ends
        for w in ends:
            if not w.explored:
                __dfs(w)

    __dfs(sv)


def bfs_expensive_impl(g, start):
    """
     Constraint: граф не взвешенный?

     http://cs.stackexchange.com/questions/4973/does-a-weighted-breadth-first-search-have-memory-when-moving-to-the-next-verte

     Dijkstra's algorithm for weighted?
    """
    assert start
    assert g

    # Finding
    graph_store = Vertex.recode_graph(g)
    start = graph_store[start]

    # Mark
    start.explored = True
    Q = Queue()
    Q.put(start)

    assert Q.qsize() == 1

    while not Q.empty():
        size = Q.qsize()
        v = Q.get()
        print v.self  # data extracting
        assert Q.qsize() == size - 1
        for w in v.ends:
            if not w.explored:
                w.explored = True
                # mark patch
                w.distance = v.distance + 1
                Q.put(w)

    assert Q.empty()


if __name__ == '__main__':
    graph = get_fake_graph()
    dfs_expensive_impl(graph, 's')


