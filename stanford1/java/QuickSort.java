package stanford_algoritms_part1.sort;

import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 26.10.13
 * Time: 13:22
 * To change this template use File | Settings | File Templates.
 */
public interface QuickSort {
  void resetSum();

  int getSum();

  void sort(List<Integer> array);
}
