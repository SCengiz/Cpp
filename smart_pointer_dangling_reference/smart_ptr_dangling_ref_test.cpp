/*
  Are smart pointers capable of reference counting immune 
    to the hazards of dangling references in C++?
*/

#include <iostream>
#include <iomanip>
#include <memory>  
#include <string>

namespace sc_test{

  namespace types{
    using ptr_type 
      = std::shared_ptr< int > ;
    
    using pair_type 
      = std::pair< int, ptr_type > ;
    
  } // types
    
  void shared_pointer_test()
  {
    using namespace std;
    using namespace sc_test::types;
      
    cout << " *** SHARED_POINTER_TEST *** " << endl << endl; 
    
    auto pair
      = [](ptr_type& s_ptr)->pair_type
      {
        return std::make_pair(*s_ptr, s_ptr);
      };
      
    auto out
      = [](pair_type& in_pair, std::string in_string)->void
        {
          cout << " ** " << in_string << " ** " << endl
               << "Value   : " << in_pair.first  << endl
               << "Adress  : " << in_pair.second << endl;
        };
        
//     Perfect forwarding !
    auto s_ptr = std::make_shared<int>(1);
    auto s_pair = pair(s_ptr);
    
    out(s_pair, " s_ptr ");
    cout << "Counter : " << s_ptr.use_count() << endl << endl;

    auto ss_ptr = make_shared<int>(3);    
    pair_type ss_pair;
    ss_pair = pair(ss_ptr);
    
    out(ss_pair, " ss_ptr ");
    cout << "Counter : " << ss_ptr.use_count() << endl << endl;

//     s_ptr = NULL
    s_ptr = nullptr;
    cout << " << s_ptr = NULL >> " << endl;
    cout << "Adress  : " << s_ptr << endl;
    cout << "Value   : " << "SEG FAULT " << endl;
    cout << "Counter for s_ptr : " << s_ptr.use_count() << endl << endl;

//     s_ptr = ss_ptr
    cout << " << s_ptr = ss_ptr >> " << endl;
    s_ptr = ss_ptr;
    s_pair = pair(s_ptr);
    out(s_pair, "s_ptr");
    cout << "Counter : " << s_ptr.use_count() << endl << endl;
  
//     s_ptr is DEAD !! 
    s_ptr.reset();
    cout << " << s_ptr is DEAD >> " << endl;
    cout << " !!! HERE !!! \n CAPABLE FOR DANGLING REFERENCE " << endl;
    cout << "Counter for s_ptr : " << s_ptr.use_count() << endl << endl;
    
    out(ss_pair, " ss_ptr ");
    cout << "Counter : " << ss_ptr.use_count() << endl;
/*
    NOTE : Counter = 3 means 
      one for itself\n"
      one for pair lambda function line:69
      and one before pair function line:49
*/     
 
  } // shared_pointer_test 
  
  void raw_pointer_test()
  {
    using namespace std;
    cout << " *** RAW_POINTER_TEST *** " << endl << endl; 

    int *ptr;
    int a = 10;
    ptr = &a;
    
    cout << "Value  : " << *ptr << endl 
         << "Adress : " << ptr  << endl;

// SEG FAULT    
//     ptr = nullptr;
//     cout << *ptr << "  " << ptr << endl;

// CORE DUMPED
//     delete ptr;
//     cout << *ptr << "  " << ptr << endl;
    
  } // raw_pointer_test

} // sc_test
  
int main()
{
  using namespace std;
  using namespace sc_test;

  shared_pointer_test();
//   raw_pointer_test();
  
  return 0;
}


