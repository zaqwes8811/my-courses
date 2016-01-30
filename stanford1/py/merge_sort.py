#-*- coding: utf-8 -*-
#BAD!!!


def _one_recursive_step(
        list_pair,
        size_task,
        current_doubled_size_task):
    """ """
    i = 0
    j = 0
    a_list = list_pair[:size_task]
    b_list = list_pair[size_task:]
    c_list = []

    for k in range(current_doubled_size_task):
        #print 'i',i, 'j', j, 'size_task', size_task
        # Защита от выхода за границы
        if i > size_task - 1:
            c_list.append(b_list[j])
            continue

        if j > size_task - 1:
            c_list.append(a_list[i])
            continue

        # Штатное сравнение
        if a_list[i] < b_list[j]:
            c_list.append(a_list[i])
            i += 1
        else:
            c_list.append(b_list[j])
            j += 1

    return c_list


def _merge(in_list, size_task):
    print '\tNew call...', in_list

    # Нужно разбить на пары
    size_in_list = len(in_list)
    ab_list = []
    ptr = 0
    current_doubled_size_task = size_task * 2
    while ptr < size_in_list:
        list_pair = in_list[ptr:ptr + current_doubled_size_task]

        # Сливаем два массива
        list_pair = _one_recursive_step(
            list_pair,
            size_task,
            current_doubled_size_task)

        # Переливаем подсчитанные данные
        for i in range(current_doubled_size_task):
            in_list[ptr + i] = list_pair[i]

        # Следующая пара
        ptr += current_doubled_size_task

    # Снова вызываем рекурсивный оборот
    new_size_task = current_doubled_size_task
    if new_size_task < size_in_list:
        _merge(in_list, new_size_task)


if __name__ == '__main__':
    """ Одна часть рекурсия, а другая слияние? """
    # length = n = 2^3
    in_list_main = [5, 4, 1, 8, 7, 2, 6, 3]
    print 'Input', in_list_main
    size_task_main = 1  # размерность задачи
    _merge(in_list_main, size_task_main)
    print 'Result', in_list_main
