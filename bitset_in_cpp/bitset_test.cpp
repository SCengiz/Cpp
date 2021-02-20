/*
  C++ bitset
  20.02.2021 18:45 
    Soray Cengiz
*/
#include<iostream>
#include<iomanip>
#include<string>
#include<bitset>
#include<vector>
#include<memory>
#include<functional>
#include<map>

#define BIT_LENGTH 16

namespace sc_test{

namespace types{
  using data_type
    = std::bitset< BIT_LENGTH > ;
    
  using vec_of_data_type 
    = std::vector< data_type > ;
      
} // types
  
class BitExtractor
{
  
private:
  types::data_type data_;
      
public:
  BitExtractor(types::data_type data)
  :
  data_(data) {}
  
  BitExtractor()
  :
  data_( {0x0000} ) {}
  
  BitExtractor(const BitExtractor& cp)
  {
    data_ = cp.data_;
  }
  
  ~BitExtractor() = default;
  
  types::data_type get_data() const{ return data_; }
  void set_data( types::data_type data) 
  {
    data_ = data;
  }
  
  types::data_type do_mask(int mask_num);
  types::data_type 
    logical_shift_right_left(bool shift_flag, int shift_vale);
  
  
}; // BitExtractor 

namespace all_masks{
  
types::vec_of_data_type masks
{
  types::data_type 
    { 
      std::string("0001000100010001")
    },
  types::data_type {123},
  types::data_type {0x1FA4},
  types::data_type {0xB004},
  types::data_type {0x1E32},
  types::data_type {0x02D4},
  
};

} // all_masks

types::data_type BitExtractor::do_mask(int mask_num)
{
//   using namespace std;
  using namespace sc_test::all_masks;

  auto& cp_data = data_;
  auto result = cp_data & masks[mask_num];
  
  return result;
} // do_mask

types::data_type 
  BitExtractor::logical_shift_right_left
    (bool shift_flag, int shift_vale)
{
  using namespace std;
  auto &cp_data = data_;
  
  enum choices
  {
    right = 1,
    left = 0,    
  };
  
/* 
  int choice ;

  if(shift_flag)
    choice = choices::left;
  else
    choice = choices::right;
*/  
  
// NOTE: Better use is std::map {}
  std::map<bool, int> choices_map;
  choices_map =
     {
       {true,  choices::right} ,
       {false, choices::left}
     };

  try{
    if(choices_map.find(shift_flag) == choices_map.end() )
      throw shift_flag;    
  }catch(bool flag){
    cout << flag << " is not founded " << endl;
    exit(1);    
  }
     
  int switch_statement = choices_map[shift_flag];
  
  switch(switch_statement)
  {
    case choices::right:  // that means true == 1
    {
      auto result = cp_data >> shift_vale;
      return result;
      
      break;
    }
    case choices::left:  // that means false == 0
    {
      auto result = cp_data << shift_vale;
      return result;
      
      break;
    }
      
    default:
      cout << " No choice " << endl; exit(1);
  }
  
} // logical_shift_right_left

template<typename B>
types::data_type
shifted_version(B in_result)
{
  unsigned int kk = 0;
  for(; kk < in_result.size(); ++kk)
  {
    if(in_result[kk])
      break;
    else
      ;
  }

//   Short way but i won't use this.
//   return types::data_type( in_result >> kk );

  BitExtractor in_instance;
  in_instance.set_data(in_result);
  auto shifted = 
    in_instance.logical_shift_right_left(true, kk);
  
  return shifted;
      
} // shifted_version

namespace all_tests{
  
  void obtain_mask_to_data_test()
  {
    using namespace std;
    using namespace types;
    using namespace all_masks;
    
    cout << "\n *** OBTAINED MASK TO DATA TEST *** \n" << endl;
    
    data_type test_data 
      = {0xF3A3};
    int mask_num = 3;

    BitExtractor test_instance(test_data);
    auto test_result 
      = test_instance.do_mask(mask_num);
    
    auto fh_print
      = [&mask_num](BitExtractor& in_instance, std::string SS)->void
      {
        cout << "   << " <<  SS  << " >>   \n"
             << " Data      : " << in_instance.get_data() << endl
             << " Mask      : " << all_masks::masks[mask_num] << endl
             << " Result    : " << in_instance.do_mask(mask_num)
             << endl;
      };

    fh_print(test_instance, " test_instance" );
    
    auto test_shifted_result
      = shifted_version(test_result);
      
    cout << " Shifted   : " << test_shifted_result << endl;
    
  } // obtain_mask_to_data_test
  
  void logical_shift_test()
  {
    using namespace std;
    using namespace types;

    cout << "\n *** LOGICAL SHIFT TEST *** \n" << endl;
  
    data_type test_data 
      = {0x70F0};
      
    BitExtractor test_instance(test_data);
    
    auto fh_print
      = []
      (
        BitExtractor& in_instance, 
        bool in_flag,
        int in_value,
        std::string SS
      )->void
      {
        cout << "   << " <<  SS  << " >>   \n";

        if(in_flag) cout << " Right Shift " << "'" << in_value << "'" 
                         << " times" << endl;
        else        cout << " Left Shift " << "'" << in_value << "'" 
                         << " times" << endl;
    
        auto in_test_result 
          = in_instance.logical_shift_right_left(in_flag, in_value); 
        
        cout << " Data      : " << in_instance.get_data() << endl
             << " Result    : " << in_test_result
             << endl;
             
        auto test_shifted_result 
          = shifted_version(in_test_result);
          
        cout << " Shifted   : " << test_shifted_result << endl;

      };
                  
    fh_print(test_instance, true, 3, "test_instance" );
    fh_print(test_instance, false, 4, "test_instance" );
    
    BitExtractor test_instance_new(test_instance);
    test_instance_new.set_data({0xFF01});
    fh_print(test_instance_new, true, 4, "test_instance_new" );
    
  } // logical_shift_test
  
} // all_tests

} // sc_test

int main()
{
  using namespace std;
  using namespace sc_test;
  using namespace sc_test::all_tests;

  obtain_mask_to_data_test();
  logical_shift_test();
  
  return 0;
}
