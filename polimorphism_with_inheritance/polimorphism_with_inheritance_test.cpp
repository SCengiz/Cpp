/*
  Each one in a number of classes in C++ possess a member function with the same
    signature, describing naturally a different technique for carrying out a similarly named task. 
      Is it possible to place instances of these classes in a vector, for example?
*/

#include <iostream>
#include <iomanip>
#include <memory>   // for shared_ptr
#include <vector> 
#include <string>
// #include <boost/shared_ptr.hpp>

namespace sc_test{
  
  namespace types{
    using vec_type
      = std::vector< int >;
          
  }; // types

  class Father
  {
  protected:
    sc_test::types::vec_type vec_;
    int number_;

  private:
    std::string class_name_;
    
  public:
//    Cons.    
    Father
    (
      sc_test::types::vec_type vec,
      int number,
      std::string class_name = " Father " 
    )
    :
    vec_(vec), number_(number), class_name_(class_name) {}
    
    sc_test::types::vec_type const getVec() { return vec_; }
    int getNumber() const { return number_; }
    std::string getName() const { return class_name_; }
    
    void setName(std::string name) { class_name_ = name; } 
    
    virtual ~Father() {} ;

//     Purely virtual function
    virtual
    sc_test::types::vec_type      
    calculate() = 0;

  }; // Father_class

//   Inherited from Father class
  class Addition : public Father
  {
  private:
    std::string class_name_;
    
  public:
    Addition
    (
      sc_test::types::vec_type vec, 
      int number, 
      std::string class_name = " Addition "
    )
    :
    Father(vec, number, class_name) {}
    
    virtual ~Addition() {} ;
    
    virtual
    sc_test::types::vec_type 
    calculate()
    {
      sc_test::types::vec_type vec_a;
      for(int i = 0 ; i < vec_.size() ; i++)
        vec_a.push_back(vec_[i] + number_);
      
      return vec_a;
    }

  }; // Addition_class
  
  class Multiple : public Father
  {

  private:
    std::string class_name_;  
    
  public:
    Multiple
    (
      std::vector<int> vec, 
      int number, 
      std::string class_name = " Multiple "
    )
    :
    Father(vec, number, class_name) {}
    
    virtual ~Multiple() {} ;
    
    virtual
    sc_test::types::vec_type  
    calculate()
    {
      sc_test::types::vec_type vec_m;
      for(int i = 0 ; i < vec_.size() ; i++)
        vec_m.push_back(vec_[i] * number_);
      
      return vec_m;
    }

  }; // Multiple class
  
  namespace types{
    using vec_type
      = std::vector< int >;
          
    using vec_of_father_inst
      = std::vector< std::shared_ptr< sc_test::Father >> ;
  }; // types

  template<class T, class S>
  void 
  output_for_better(T instance, S in_string)
  {
    using namespace std;
    cout << " *** OUTPUT_FOR_BETTER *** " << endl;
    cout << " << " << in_string << " >> " << endl;

    if(instance == NULL){
      cout << " !! ITS NULL PTR !! " << endl << endl;
//       exit(1);
    }
    
    else{
      cout << "Name       : " << instance->getName() << endl;
      cout << "Number     : " << instance->getNumber() << endl;
      cout << "Calculate  : ";
      for(auto &item : instance->calculate())
        cout << item << " ";
      cout << endl << endl;
    }
    
  } // output_for_better

  void basic_test()
  {
    using namespace std;
    using namespace sc_test;
    using namespace sc_test::types;
    
    cout << "\n *** BASIC_TEST *** \n" << endl;
    
    vec_type vec1 = {1,2,-1,-2,0};
    vec_type vec2 = {-4,2,1,-2,0};
    int n1 = 1, n2 = -1;
    
    vec_of_father_inst vv;
    
    vv.push_back( std::make_shared < Addition >(vec1, n1) );
    vv.push_back( std::make_shared < Addition >(vec1, n2) );

    vv.push_back( std::make_shared < Multiple >(vec2, n1) );
    vv.push_back( std::make_shared < Multiple >(vec2, n2) );
    
    auto output
      = []
      (
        std::vector< std::shared_ptr < Father > > &vec,
        int i
      )->void
      {
        cout << " *** OUTPUT FOR " << i << ".INDEX *** " << endl;
 
        auto ptr = vec[i];
 
        cout << "Name         :  "
             << ptr->getName() << endl;
 
        cout << "Vector       : ";
        for(auto &item : ptr->getVec())
          cout << setw(3) << item << " ";
        cout << endl;
  
        cout << "Number       :   "
             << ptr->getNumber() << endl;

        cout << "Calculate    : ";
        for(auto &item : ptr->calculate())
          cout << setw(3) << item << " ";
        cout << endl << endl;

      };
    
    output(vv, 0);
    
    output(vv, 3);
    
  } // basic_test
  
  void better_test()
  {
    using namespace std;
    using namespace sc_test::types;
  
    cout << "\n *** BETTER_TEST *** \n" << endl;
    
    vec_type vec = {-1,0,1};
    
    vec_of_father_inst vv;
    const int SZ = 5;
    for(int i = 1 ; i < SZ ; i++)
    {
      if(i%2 == 1)
        vv.emplace_back(std::make_shared< Addition >(vec, i));
      else
        vv.emplace_back(std::make_shared< Multiple >(vec, i));      
    }

    cout << "Original Vec : ";
    for(auto &item : vec)
      cout << item << " ";
    cout << endl << endl;
    
    output_for_better(vv[0], "0.index");
    output_for_better(vv[1], "1.index");
    
    cout << " *** DOWN AND UP CASTING TESTS *** \n" << endl;
    
    auto a_ptr = vv.at(0).get();
        
    auto addition_to_father 
      = dynamic_cast< Father* >(a_ptr); 
    output_for_better(addition_to_father, " addition_to_father ");
    
    auto father_to_addition 
      = dynamic_cast< Addition* >(addition_to_father);
    output_for_better(father_to_addition, " father_to_addition ");
    
    auto father_to_multiple
      = dynamic_cast< Multiple* >(a_ptr); 
    output_for_better(father_to_multiple, " father_to_multiple ");
    
  } // better_test
  
} // namespace

int main()
{
  using namespace std;
  using namespace sc_test;
  
//   basic_test();
  better_test();
  
  return 0;
}

