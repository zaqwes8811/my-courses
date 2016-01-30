# coding: utf-8


def shift_idx(f):
    def wrapper(*args):
        return f(*args)-1
    return wrapper


class Heap(object):
    def __init__(self):
        pass

    @staticmethod
    @shift_idx
    def parent(i):
        return i/2

    @staticmethod
    @shift_idx
    def left(i):
        return 2*i

    @staticmethod
    @shift_idx
    def right(i):
        return 2*i+1


# Runner
heap = [16, 14, 10, 8, 7, 9, 3, 2, 4, 1]

assert heap[Heap.parent(4)] == 14