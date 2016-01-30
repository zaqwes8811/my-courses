# coding: utf-8

import copy
import random


def get_random_edge(source_graph):
    """
    Keys is positive integer.

    Как выбрать если схлопываем узлы?
    """

    keys = source_graph.keys()

    source_v = random.randint(1, len(keys))

    connections = source_graph[source_v]
    end_v = connections[random.randint(0, len(connections) - 1)]
    print end_v, connections, source_graph

    return source_v, end_v


def compress_super_vertex(graph, super_vertex):
    # Удаляем ссылки друг на друга в пределах суперузла.
    # Здесь только для двух точек, вернее для суперузла.

    # Удаляем висячие вершины - нет ссылок на другие узлы.
    pass


def edge_contraction(graph, edge, super_vertices):
    begin = edge[0]
    end = edge[1]
    key_worker = begin.idx
    key_end = end.idx
    super_for_begin = begin.super_vertex
    super_for_end = end.super_vertex  # Нужно знать для объединения супервершин

    # Если вершины не подключены подключаем вершину к суперузлу
    # TODO: кажется это нужно делать не тут
    if not super_for_begin:
        begin.super_vertex = key_worker
    else:
        # За основу берем
        key_worker = begin.super_vertex

    if not super_for_end:
        end.super_vertex = key_worker

    if key_worker not in super_vertices \
            and key_end not in super_vertices:
        super_vertices[key_worker] = []

        # Добавляем обе
        super_vertices[key_worker].append(key_worker)
        super_vertices[key_worker].append(key_end)
    else:
        pass

    inner = super_vertices[key_worker]
    compress_super_vertex(graph, inner)


def main():
    """
    http://en.wikipedia.org/wiki/Karger%27s_algorithm

    Полагаем что доступ по ключу O(n)

    Как сохранить информацию о ребре, а не просто их схлопнуть?

    Использовать стопки, но это дополнительное место - O(n^2)!!

    Как хранить ссылки? Числа или сразу объекты?
    """

    class Vertex(object):
        def __init__(self, ends, source):
            self.end_points = ends
            self.super_vertex = None
            self.idx = source

    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}
    vertices = {}
    edges = []  # Копия со ссылками - по ней генерируем ребро - если вершина пустая
    # удаляем - это обеспечит попадание в ребро при случайном выборе.

    for k, v in source_graph.items():
        vertices[k] = Vertex(v, k)

    super_vertices = {}  # супервершины, когда будут появляться

    norm_v = len(source_graph)  # развязка появилась, но какие узлы остались?
    # Или может мы просто удаляем ребро, а ничего не схлопываем?
    # Не придется схлопнуть

    # One iteration
    # while norm_v > 2:
    edge = (vertices[1], vertices[2])  #get_random_edge(source_graph)
    edge_contraction(vertices, edge, super_vertices)
    #    norm_v -= 1

    print source_graph


main()

