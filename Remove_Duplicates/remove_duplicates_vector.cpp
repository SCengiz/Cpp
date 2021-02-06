#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

namespace sc_test{
  namespace typedefs{
    using vec_type 
      = std::vector<int>;
  } // nested namespace
  
  typedefs::vec_type sort_after_unique(const typedefs::vec_type& i_vec)
  {
    using namespace std;
    using namespace sc_test::typedefs;
    cout << " ** sort_after_unique test ** " << endl;
    
    auto copy_vec = i_vec;
//     Don't wanna change original vec.
    std::sort(copy_vec.begin(), copy_vec.end());
    vec_type::iterator it;
    
    it = std::unique(copy_vec.begin(), copy_vec.end());
    copy_vec.resize(std::distance(copy_vec.begin(), it));
    
    return copy_vec;
  }

} // namespace

int main(){
  using namespace std;
  using namespace sc_test;
  using namespace sc_test::typedefs;
  
  const int SIZE = 10 ;
  vec_type my_vec;

  for(int kk = 0 ; kk < SIZE ; kk++)
    my_vec.push_back(1 + 2*(kk%3));
	
  auto vec_print
    = [](vec_type& i_vec, std::string i_str)->void
    {
      cout << i_str << endl;
      for(auto &item : i_vec)
        cout << item << ' ' ;
      cout << endl << endl;
    };
 
  vec_print(my_vec, " Original vector ");
    
  auto result_vec = sort_after_unique(my_vec);
  
  vec_print(result_vec, " Duplicated vector ");
  
  return 0;
}

