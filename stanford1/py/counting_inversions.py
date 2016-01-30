#-*- coding: utf-8 -*-
#import uasio.os_io.io_wrapper as iow

# Что если массив нацело не делится на два
def count_and_sort(A, n):
    if n == 1:
        return A, 0
    else:
        b_raw = A[:n / 2]
        c_raw = A[n / 2:]  # может быть не равен длине первого
        print 'first'
        B, x = count_and_sort(b_raw, len(b_raw))
        print 'two'
        C, y = count_and_sort(c_raw, len(c_raw))
        print 'merge'
        D, z = count_and_merge(B, C, n)
        return D, x + y + z


def count_and_merge(B, C, n):
    """ 
    
    Args: 
        - Input arrays need be sorted
    """
    one_st_len = n / 2
    two_st_len = n - one_st_len  # Будет либо равна первой длине, либо будет больше

    # Returns
    D = []
    number_inversion = 0

    # tmp
    i = 0
    j = 0
    for k in range(n):
        # Один из массивов закончился
        if i > one_st_len - 1:
            D.append(C[j])
            j += 1
            continue

        if j > two_st_len - 1:
            D.append(B[i])
            i += 1
            continue

        # Штатное сравнение
        if B[i] < C[j]:
            D.append(B[i])
            i += 1
        else:
            D.append(C[j])
            number_inversion += one_st_len - i
            j += 1

    return D, number_inversion


def main():
    #sets = iow.get_utf8_template()
    #sets['name'] = 'D:/github-develop/tests-usage-algorithms/stanford_algoritms_part1/week_1/test_serial.txt'
    #sets['name'] = 'D:/github-develop/tests-usage-algorithms/stanford_algoritms_part1/week_1/IntegerArray.txt'
    # Модифицированное слияние
    A = [1, 3, 5, 2]#,4,6,8,7]
    A = [1, 2, 3, 4]#,5,6,7,8]
    #A = iow.file2list_int(sets)
    if not A:
        print 'Failure!'
        return
    n = len(A)

    a, num = count_and_sort(A, n)
    return num


if __name__ == '__main__':
    """ Одна часть рекурсия, а другая слияние? """
    print
    print 'Begin...'
    print main()

    print 'Done'
