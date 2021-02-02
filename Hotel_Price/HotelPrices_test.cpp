#include<iostream>
#include<vector>
#include<memory>
#include<functional>
#include<algorithm>
#include<iomanip>
#include<cstring>
#include<exception>
#include<stdexcept>
#include<tuple>

namespace sc_test{
  
  class HotelRoom{
    protected:
      int bedrooms_, bathrooms_;
      void check_bb();
    public:
      HotelRoom(int bedrooms, int bathrooms) 
      :
      bedrooms_(bedrooms), bathrooms_(bathrooms) 
      { check_bb(); } 
      ~HotelRoom() = default;

      int get_price() { return 50*bedrooms_ + 100*bathrooms_; }
    
}; // H_R class
  
  class HotelApartment : public HotelRoom{
    public:
      HotelApartment(int bedrooms, int bathrooms) 
      :
      HotelRoom(bedrooms, bathrooms) 
      { check_bb(); }
      
      ~HotelApartment() = default;
      
      int get_price() 
      {
        if(bedrooms_ == bathrooms_)
          return HotelRoom::get_price() + 100;
        else
          return HotelRoom::get_price(); 
      }
      
}; // H_A class
  
  void HotelRoom::check_bb()
  {
    if(bathrooms_ < 1 || bathrooms_ > 5 || bedrooms_ < 1 || bedrooms_ > 5)
      throw std::invalid_argument
        ("Warning!!! -> Inputs can not be less than 1 or much than 5.");
  }

  int calculation(char i_name[100], int& i_bed, int& i_bat)
  {
    using namespace std;
    const char h_r_name[100]  = "standard" ;
    const char h_a_name[100]  = "apartment" ;

    if(strcmp(i_name, h_r_name) == 0)
    {
      HotelRoom h_r(i_bed, i_bat);
      return h_r.get_price();
    }
    else if(strcmp(i_name, h_a_name) == 0)
    {
      HotelApartment h_a(i_bed, i_bat);
      return h_a.get_price();
    }
    else
      cout << "Warning!!! -> input_names must be 'standard' or 'apartment' " 
           << endl; return 0;    
  };
  
  namespace typedefs{
    using tuple_type
      = std::tuple<std::string, int, int>;
    using vector_type
      = std::vector<tuple_type> ;
    
  } // namespace

} // namesapce

namespace tests{
  using namespace sc_test;
  using namespace std;
  using namespace sc_test::typedefs;
  
  void basic_test()
  {
    cout << "*** BASIC TEST *** " << endl;
    
// NOTE: 1 <= n <= 100

    char i_name[100];
    int i_bedrooms, i_bathrooms;
      
    cout << " Inputs respectively " << endl;
    int n;
    cin >> n;
    
    int result = 0;
    for(int kk = 0 ; kk < n ; ++kk)
    {
      cin >> i_name;
      cin >> i_bedrooms >> i_bathrooms;
      
      auto tot = calculation(i_name, i_bedrooms, i_bathrooms);
      result += tot;      
    }
    cout << "Total : " << result << endl;
    
  } // basic_test
  
  void better_test()
  {
    cout << "*** BETTER TEST *** " << endl;

    std::string i_name;
    int i_bedrooms, i_bathrooms;
     
    cout << " Inputs respectively " << endl;
    int n;
    cin >> n;

// TODO: Get rid of code reuse in "vector_result" lambda function.
    
    auto vector_result
      = [](tuple_type& ifh_tuple)->int
      {
        std::string h_r_name  = "standard" ;
        std::string h_a_name  = "apartment" ;
                
        if(h_r_name.compare(std::get<0>(ifh_tuple)) == 0)
        {
          auto ptr_instance 
            = std::make_shared<HotelRoom>
              (
                std::get<1>(ifh_tuple),
                std::get<2>(ifh_tuple)
              );
      
          return ptr_instance->get_price();
        }
        else if(h_a_name.compare(std::get<0>(ifh_tuple)) == 0)
        {
          auto ptr_instance 
            = std::make_shared<HotelApartment>
              (
                std::get<1>(ifh_tuple),
                std::get<2>(ifh_tuple)
              );
      
          return ptr_instance->get_price();
        }
        else
          cout << "Warning!!! -> input_names must be 'standard' or 'apartment' " 
               << endl; return 0;    
      };
  
    auto all_cost
      = [](std::vector<int> ifh_vec)->int
      {
        int total = 0;
        for(auto &item : ifh_vec)
          total += item;
        return total;
      };
    
    std::vector<int> final_vec;
      
    while(n > 0)
    {
      cin >> i_name;
      cin >> i_bedrooms >> i_bathrooms;

      sc_test::typedefs::tuple_type i_tuple
        = std::make_tuple(i_name, i_bedrooms, i_bathrooms);
      
      auto next_item = vector_result(i_tuple);
      final_vec.emplace_back(next_item);
      
      --n;
    }
    
    auto result = all_cost(final_vec);
    cout << "Result : " << result << endl; 
    
  } // better_test
  
} // namespace

int main()
{
  using namespace std;
  using namespace sc_test;
  using namespace tests;
  
  basic_test();
  better_test();
  
  return 0;
}
