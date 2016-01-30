package stanford_algoritms_part1.sort;

import com.google.common.collect.Ordering;
import stanford_algoritms_part1.sort.partioners.PartionerBase;
import stanford_algoritms_part1.sort.pivots.PivotFirst;
import stanford_algoritms_part1.sort.pivots.PivotLast;
import stanford_algoritms_part1.sort.pivots.PivotMedianOfThree;
import stanford_algoritms_part1.sort.pivots.PivotRandom;
import stanford_algoritms_part1.sort.util_local.FileOperations;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assume.assumeTrue;
import static org.junit.Assert.assertEquals;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 29.09.13
 * Time: 16:42
 * To change this template use File | Settings | File Templates.
 */
public class QuickSortImplTest {
  public QuickSortImplTest() {
    PATH_TO_FILE = "src\\stanford_algoritms_part1\\data\\QuickSort.txt";
  }

  @org.junit.Test
  public void testAsInVideo() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSortImpl();
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testRandom() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6, 10};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSortImpl();
    for (int i = 0; i < 10; i++) {
      sorter.sort(array);
      assumeTrue(Ordering.natural().isOrdered(array));
    }
  }

  private final String PATH_TO_FILE;

  @org.junit.Test
  public void testBoundary() throws Exception {
    Integer [] rawArray = {3,8,2,5, 6,4,7,1};

    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSortImpl();
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testFromFile() throws Exception {
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);

    int end = array.size();
    QuickSort sorter = new QuickSortImpl(new PivotLast(), new PartionerBase());
    sorter.sort(array.subList(0, end));
    assumeTrue(Ordering.natural().isOrdered(array.subList(0, end)));
  }

  @org.junit.Test
  public void testFromFileRandom() throws Exception {
    QuickSort sorter = new QuickSortImpl(new PivotRandom(), new PartionerBase());
    for (int i = 0; i < 10; ++i) {
      String filename = PATH_TO_FILE;
      List<Integer> array = FileOperations.fileToList(filename);
      int end = array.size();
      sorter.sort(array.subList(0, end));
      assumeTrue(Ordering.natural().isOrdered(array.subList(0, end)));
    }
  }

  @org.junit.Test
  public void testProgramQuestion1() throws Exception {
    QuickSort sorter = new QuickSortImpl(new PivotFirst(), new PartionerBase());
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testMedianOfThree() throws Exception {
    QuickSort sorter = new QuickSortImpl(new PivotMedianOfThree(), new PartionerBase());
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testSummator() throws Exception {
    Integer [] rawArray = {3,8,2,5, 6,4,7,1};

    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSortImpl();

    assertEquals(sorter.getSum(), 0);
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
    assumeTrue(sorter.getSum() > 0);
  }

  // Tasks
  @org.junit.Test
  public void testTask0() throws Exception {
    QuickSort sorter = new QuickSortImpl(new PivotFirst(), new PartionerBase());
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);
    sorter.sort(array);
    assumeTrue(Ordering.natural().isOrdered(array));
    System.out.println("Task0 "+sorter.getSum());
  }

  @org.junit.Test
  public void testTask1() throws Exception {
    QuickSort sorter = new QuickSortInv(new PivotFirst(), new PartionerBase());
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);
    sorter.sort(array);
    assertEquals(array.size(), 10000);
    assumeTrue(Ordering.natural().isOrdered(array));
    System.out.println("Task1 "+sorter.getSum());
  }

  @org.junit.Test
  public void testTask2() throws Exception {
    QuickSort sorter = new QuickSortInv(new PivotMedianOfThree(), new PartionerBase());
    String filename = PATH_TO_FILE;
    List<Integer> array = FileOperations.fileToList(filename);
    sorter.sort(array);
    assertEquals(array.size(), 10000);
    assumeTrue(Ordering.natural().isOrdered(array));
    System.out.println("Task2 "+sorter.getSum());
  }
}

