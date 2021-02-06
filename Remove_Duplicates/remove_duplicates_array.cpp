#include <iostream>

namespace sc_test{
  void remove_num(int arr[], int& dup_index, int& size)
  {
    for(int kk = dup_index; kk < size - 1; kk++)
      arr[kk] = arr[kk + 1];  
      // Shifting 1 index to left.
    size--;
  }

  void remove_duplicates(int arr[], int& size)
  {
    for(int kk = 0; kk < size; kk++)
      for(int jj = kk + 1; jj < size; jj++)
        if(arr[kk] == arr[jj])
        {
          remove_num(arr, jj, size); 
          jj--;
        }
  }
  
  template 
  <class A, class S, class SS>
  void print_array
  (A arr, S& size, SS& info_string)
  {
    using namespace std;
    cout << " ** PRINT ARRAY ** " << endl;
    cout << info_string << endl;
    for(int kk = 0 ; kk < size ; kk++)
      cout << arr[kk] << ' ';
    cout << endl << endl;
  }
  
} // namespace

int main()
{
  using namespace std;
  using namespace sc_test;
  
  int SIZE = 10;  
//   NOTE: Can not be const in this case
  
  int* my_arr;
  my_arr = new int[SIZE];

  // int my_arr[] = { 1, 3, 5, 1, 3, 5, 1, 3, 5, 1 };
  
  for(int kk = 0 ; kk < SIZE ; kk++)
    my_arr[kk] = 1 + 2*(kk%3);

  print_array(my_arr, SIZE, " Original Array ");
  
  remove_duplicates(my_arr, SIZE);
  
  print_array(my_arr, SIZE, " Duplicated Array ");
  
  return 0;
}

