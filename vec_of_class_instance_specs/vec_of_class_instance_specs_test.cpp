/*
  Vector of class instances with the same custom type. You would like
    to check which and/or how many items in the vector meet a certain criterion
*/

#include<iostream>
#include<iomanip>
#include<vector>
#include<algorithm>
#include<memory>

using namespace std;

namespace sc_test{

  class Custom
  {
  private:
    double *a_;
    int capacity_;
    
  public:
    Custom(double *a, int cap)     
    :
    a_(a), capacity_(cap){}
    
    int get_capacity() { return capacity_ ;}
    double *get_a()    { return a_; }

    void check_one_instance();
    
    friend ostream& operator 
    <<
    (ostream& os , Custom& other)
    {
      os << " ->  size : " << other.get_capacity() << " ~~";
      os << " array : ";
      for(int kk = 0; kk < other.get_capacity(); kk++)
        os << other.get_a()[kk] << " "; 
      return os;
    }

    ~Custom() = default;
    
  }; // class

  void Custom::check_one_instance()
  {
//     First condition is capacity_ > 2 
//     Second condition is is my array contains 4?
    auto in_a = get_a();
    int counter = 0;
    
    if(capacity_ > 2)
    {
      for(int kk = 0; kk < capacity_; kk++)
        if(in_a[kk] == 4)
          counter++;
        else
          ;
      
      if(counter > 0)
        cout << " Correct specs " << endl;        
      else
        cout << " Not correct specs " << endl;
      
    }   
    else
      cout << " Not correct specs " << endl;
    
  } //func

  namespace types{
    using vec_type
      = std::vector< std::shared_ptr< sc_test::Custom > >; 
    
    typedef double* doubleArr;
      
    using pair_type
      = std::pair<int, int>;
    
    using vec_pair_type
      = std::vector< pair_type >;
          
  } // nested namespace

  void check_whole_instaces(sc_test::types::vec_type in_vec)
  {
    cout << " ** check_whole_instaces ** " << endl;
//     First condition is capacity_ > 2 
//     Second condition is is my array contains 4?
    
    auto vec_size = in_vec.size();
    auto counter = 0;
    sc_test::types::vec_pair_type in_pair_vec;
    
    for(int kk = 0; kk < vec_size; kk++)
    {
      auto arr_part = in_vec[kk]->get_a();
      auto cap_part = in_vec[kk]->get_capacity();

      if(cap_part > 2)
        for(int jj = 0; jj < cap_part; jj++)
        {
          if(arr_part[jj] == 4)
          {
            in_pair_vec.push_back(std::make_pair(kk, jj));
            counter++;
          }
          else
            ;
        }        
      else
        ;
    }

    cout << "Number of correct instances in vector : " 
         << counter << endl << endl;

    int i = 0;
    for(auto &item : in_pair_vec)
    {
      cout << i+1 << "." 
           << "Correct instance in vec[" << item.first << "] = " ; 
           
      auto *ptr = in_vec[item.first]->get_a();
      auto size = in_vec[item.first]->get_capacity();
      
      for(int kk = 0; kk < size; kk++)
        cout << *(ptr + kk) << " ";
      cout << endl;        
      
      ++i;
    }
    
  } // func

  namespace tests{  
    
    void basic_test()
    {
      cout << " *** BASIC TEST *** " << endl;
      using namespace sc_test::types;
      vec_type my_vec;
      
      const int SZ = 5;
      int cap = 0;
      for(int kk = 0; kk < SZ; kk++)
      {
        cap = kk + 2;
        doubleArr arr = new double[cap];

        for(int jj = 0; jj < cap; jj++)
          arr[jj] = jj;

        my_vec.push_back(std::make_shared<Custom>(arr, cap));
      }
      
      auto fh_print
        = [](vec_type& i_vec)->void
      {
        cout << " ** fh_print ** " << endl;
        for(auto &item : i_vec)
          cout << *item << endl;
      };
      
      fh_print(my_vec);
      cout << endl;
      
      cout << "    *** CONDITIONS ***    " << endl;
      cout << " First condition is array capacity > 2 " << endl;
      cout << " Second condition is array contains 4  " << endl << endl;
      
      cout << "check for my_vec[1] : " ;
      my_vec[1]->check_one_instance();
      cout << "check for my_vec[4] : " ;
      my_vec[4]->check_one_instance();

      cout << endl;
      check_whole_instaces(my_vec);
      
    } // test
  } // nested namespace

  }  // namespace
  
int main()
{
  using namespace std;
  using namespace sc_test;
  using namespace sc_test::tests;

  basic_test();
  
  return 0;
}

