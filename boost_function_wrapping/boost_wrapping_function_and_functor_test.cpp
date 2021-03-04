#include<iostream>
#include<vector>
// Boost functions helps somewhere.
#include<boost/bind.hpp>
#include<boost/function.hpp>
#include<boost/shared_ptr.hpp>

// Namespace for Encapsulation.
namespace sc_test{
  
// Class for calculating cumulative sum of a vector using static member variable.
class Container{
  public:
    static int cumulative_sum;
    
    Container(int num = 0) : num_(num) 
    {
      cumulative_sum += num_;
    }     
    ~Container() = default;
  
  private:
    int num_;

}; // Container

// Initalizatio of static member.
int Container::cumulative_sum = 0;  

// A Functor definition have to be using a class.
class myFunctor{
  public:
    void operator() (int in_num)
    {
      printf(" -> %d \n", in_num);
    }
    
}; // myFunctor

// This global function will use for wrapping function object!!!
void output(int in_num)
{
  std::cout << " -> " << in_num << std::endl;  
} // output

namespace all_test{
  void basic_test()
  {
    using std::cout;
    using std::endl;
    
// integer of vector type for this application
    using vec_type
      = std::vector< int > ;
    
    cout << " ***  Container::cumulative_sum  *** "  
         << endl << " Initially : " 
         << Container::cumulative_sum 
         << endl << endl;

// Sıze of the vector that we used for founding cumulative summation.
    const int SZ = 10;
    vec_type in_vec;
// Initialization of input vector.
    for(int i = 0; i < SZ; i++)
      in_vec.emplace_back( i );

// 73.line is very important if you want to use std::transform
// ... you have to initialize your vector not enough just declaration. 
    vec_type cum_vec(in_vec.size(), 0);
    vec_type::iterator it = in_vec.begin();

// Iteratively we push back out vector with not using push_back function
    std::transform
    (
      it,
      (it + SZ),
      cum_vec.begin(),
      [](int item)
      {
        boost::shared_ptr< Container >
          test_instance( new Container(item) );
        return test_instance->cumulative_sum;
      }
    );
    
/* Alternative way for cum_vec initialization !!!
    while(kk < 10)
    {
      auto test_instance = new Container(kk);
      cum_vec.emplace_back
        (test_instance->cumulative_sum);
      ++kk;
      
      delete test_instance;
    }
*/
    
// This type use for wrapping functor and global function.
    using func_type  
      = boost::function< void(int) >;

// wrapping normal function !!
    func_type n_func( &output );

// wrapping functor !!
    auto functor_instance 
      = ( myFunctor* ) calloc( 1, sizeof(myFunctor) );
    func_type f_func( *functor_instance );

// Basic output lambda function. No data copy everything will be done with references.
// We see an example boost::bind using in here !!!
    auto fh_print
      = []
      ( 
        func_type& lm_func, 
        vec_type& lm_in_vec,
        vec_type& lm_cum_vec
      )->void
    {
      cout << "\n << OUTPUT >> \n" << endl;
      cout << " ** Inputs ** " << endl;
      int kk = 0;
      for(auto &in_item : lm_in_vec)
      { 
        boost::bind(lm_func, _1)( in_item );
        if(kk == 9)
        {
          cout << " ***  Container::cumulative_sum  *** "
               << endl << " -> ";
          for(auto &item : lm_cum_vec)
            cout << item << " ";
          cout << endl;
        }
        ++kk;
      }
    };

// With 2 types of wrapping functions we do same operations.
    fh_print(f_func, in_vec, cum_vec);
    fh_print(n_func, in_vec, cum_vec);
    
// Don't forget the delete raw ptr in heap.
    delete functor_instance;
  } // basic_test
  
} // all_test

} // sc_test

int main()
{
  using namespace sc_test::all_test;
  
  basic_test();
  
  return 0;
} // main
