/* 
  In C++, why would you want a lambda expression in a member function to capture the
    “this” pointer? 
*/
#include<iostream>
#include<memory>
#include<vector>
#include<string>
#include<functional>
#include<algorithm>
#include<iterator>
#include<array>

// Everything is in namespace class, functions, other namespaces etc.
namespace sc_test{

// Nested namespace called types, all types are under this namespace  
  namespace types{
    using vec_type 
      = std::vector< int > ;
  
// No need this type we can do same operations with tuple_type
    using pair_type
      = std::pair<std::vector<int> , int>;

    using tuple_type
      = std::tuple<std::vector<int> , int>;

// Function type that return tuple_type and take void.
    using lambda_type
      = std::function<tuple_type(void)>;
      
  } // types
  
class Container
{
// Our input vector  
// int variables to take number of positive and negative numbers in the vec.
// NOTE: initially = 0;
private:
  types::vec_type vec_;
  int counter_p_ = 0 , counter_n_ = 0;
  
public:
// Constructor, default, copy cons.
  Container(types::vec_type vec = {0,0,0,0,0,0}) : vec_(vec) {}
  Container(const Container& cp) { vec_ = cp.vec_;}
  
// Destructor  
  ~Container() = default;

// Getters for class.  
  types::vec_type getVec() const { return vec_; }
  int getCount_n() const { return counter_n_; }
  int getCount_p() const { return counter_p_; }

// check fonction for input vector.
  void check_numbers();
// Other member functions.
  types::lambda_type positive_or_negegative_nums(bool flag);
  types::pair_type look_up_special_number(int special);
  
}; //  Container class  

void Container::check_numbers()
{
// Capture instance in lambda. Then calculate how many pos or neg items in vec.
// Update counters for neg and pos.
// for_each is iterator based loop
  std::for_each(vec_.begin(), vec_.end(), 
    [this](int item)
    {
      if(item > 0) counter_p_++;
      else if(item < 0) counter_n_++;
      else 
        ;
    }
  );
} // check_numbers

// positive_or_negegative_nums function returns a lambda function.
// depending on the input flag case will change
// founding positive or negative item's indexes.
types::lambda_type Container::positive_or_negegative_nums(bool flag)
{
  using namespace sc_test::types;
  
// Capture instance and input flag.
  auto fh_pos_or_neg_nums 
    = [this,flag]()->tuple_type
    {
// variables for pos or neg indexes for our input vec.
      vec_type positive_nums_indexes;
      vec_type negative_nums_indexes;

// using iterator for founding indexes.      
      vec_type::iterator it = vec_.begin();
        
// cnt is size of positive or negative indexes size.
// indexes is vec_ indexes.
      int indexes = 0, cnt = 0;
        
      for(; it < vec_.end() ; it++)
      {
        if(flag)
        {
          if(*it > 0)
          {
            positive_nums_indexes.emplace_back(indexes);
            cnt++;
          }
        }
        else
        {
          if(*it < 0)
          {
            negative_nums_indexes.emplace_back(indexes);
            cnt++;
          }
        } 
        indexes++;
      } 
// depending on flag using perfect forwarding return a tuple.
      if(flag)
        return std::make_tuple(positive_nums_indexes, cnt);
      else
        return std::make_tuple(negative_nums_indexes, cnt);
          
    };

// return a lambda function.
  return fh_pos_or_neg_nums;

} // positive_or_negegative_nums

// Look up special number in vector. 
types::pair_type Container::look_up_special_number(int special)
{
  using namespace sc_test::types;
  int cnt = 0;
  vec_type v_cnt;
    
  for(int kk = 0 ; kk < vec_.size() ; ++kk)
  {
    if(vec_.at(kk) == special)
    {
      cnt++;
      v_cnt.emplace_back(kk);
    }
    else 
      ;
  }
  return std::make_pair(v_cnt, cnt);
} // look_up_special_number

namespace types{
// Extra typedefs for basic_test.
  using s_ptr
    = std::shared_ptr< sc_test::Container > ;
  
  using vec_of_s_ptr
    = std::vector< s_ptr >;
  
} // types

namespace all_tests{

void basic_test()
{
// namespaces for basic test.
  using namespace std;
  using namespace sc_test;
  using namespace sc_test::types;
  
// a vector holds class pointer instances.
  vec_of_s_ptr  
    vec_of_instances;

// test_vec for class instance.
  vec_type test_vec 
    = {-2, 5, 2, -1, 2, 5, 6, 10}; 
    
  cout << " << Original vec >> " << endl;
  for(auto &item : test_vec)
    cout << item  << " " ;
  cout << endl;

// using make_shared we have a instance of Container. Then store it vec.  
  vec_of_instances.emplace_back
    (std::make_shared< Container >(test_vec));
  
// a lambda function for pos or neg counters.
  auto fh_check_numbers_print
    = [](s_ptr& in_ptr, std::string SS)->void
    {
      cout << "\n*** " << SS  << " ***" << endl
           << "Positive numbers are in my vec : "
           << in_ptr->getCount_p() << endl 
           << "Negative numbers are in my vec : " 
           << in_ptr->getCount_n() << endl ;           
    };

// find number of pos or neg for our instance.
  for(auto &item : vec_of_instances)
    item->check_numbers();

// print coutners.  
  fh_check_numbers_print(vec_of_instances[0], " << 0.index >>");  
  
// captured_instance for first item of vec_of_instances.
  auto captured_instance = vec_of_instances[0];
  
// print lambda fh_pos_or_neg_nums.
  auto fh_pos_or_neg_nums_print
    = [&captured_instance]
        (lambda_type& in_lambda , std::string SS, bool flag)
    {
      cout << "\n*** " << SS << " ***" << endl;
      if(flag)
        cout << "--This is Positive Case-- \n" ;
      else
        cout << "--This is Negative Case-- \n" ;
      
      auto in_tuple = in_lambda();
      
      auto in_tuple_vec = std::get<0>(in_tuple);
      auto in_tuple_int = std::get<1>(in_tuple);
      
      cout << "Result Vector indexes : \n ";
      
      for(auto &item : in_tuple_vec)
        cout << item << " ";
      cout << endl;
      
      cout << "Result Vector elements : \n ";
      for(auto &item : in_tuple_vec)
        cout << captured_instance->getVec()[item] << " " ;
      cout << endl;
      
      cout << "Size of vector : \n "
           << in_tuple_int << endl;
    };

  auto do_lambda 
    = vec_of_instances.at(0)->positive_or_negegative_nums(1);
    
  fh_pos_or_neg_nums_print(do_lambda, "<< 0.index >>", 1);   
  
  auto fh_look_up_special_number_print
    = [&captured_instance]
        (pair_type in_pair , std::string SS, int spec)
    {
      cout << "\n*** " << SS << " ***" << endl;
      cout << " --Found " << spec << "s--" << endl;
      
      auto in_pair_vec = in_pair.first;
      auto in_pair_int = in_pair.second;
      
      cout << "Result Vector indexes : \n ";
      
      for(auto &item : in_pair_vec)
        cout << item << " ";
      cout << endl;
      
      cout << "Result Vector elements : \n ";
      for(auto &item : in_pair_vec)
        cout << captured_instance->getVec()[item] << " " ;
      cout << endl;
      
      cout << "Size of vector : \n "
           << in_pair_int << endl;      
    };
  
  auto do_pair
    = vec_of_instances.at(0)->look_up_special_number(5);
      
  fh_look_up_special_number_print(do_pair, "<< 0.index >>", 5);
    
  } // end test fcn

} // all_tests

} // sc_test

int main()
{
  using namespace sc_test::all_tests;
  
  basic_test();  
  
  return 0;
}
