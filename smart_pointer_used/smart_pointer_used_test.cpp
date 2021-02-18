/*
  Why would we choose to utilize a shared pointer in C++ for raw pointer lifetime management
    if we would like to emulate the reference counting idiom for variables as in a scripting language,
      for example Python?
*/

#include <iostream>
#include <memory>   // for shared_ptr
#include <string>
#include <cassert>

namespace sc_test{

  class Rectangle
  {
  private:
    int length_;
    int breadth_;
    void check_datas()
    {
      using namespace std;
//       assert((length_ < 0) || (breadth_ < 0));

      if(length_ < 0 || breadth_ < 0)
      {
        std::cout << "length_ or breadth_ can not" 
                  << " equal 0 " 
                  << endl;
        exit(1);
      }
    }
    
  public:
    Rectangle() : length_(1), breadth_(1) { check_datas(); }
    Rectangle(int ll, int bb) : length_(ll), breadth_(bb) 
    {
      check_datas();
    }
    
    int getLength() const { return length_; }
    int getBreadth() const  { return breadth_; }
    int area() const { return length_ * breadth_; }  
    
  }; // class

  void output(std::unique_ptr<Rectangle>& R, std::string RR)
  {
    using namespace std;
    cout << " *** " << RR << " *** " << endl;
    cout << "length  : " << R->getLength()
         << " * "
         << "breadth : " << R->getBreadth()
         << endl << "Area = " 
         << R->area() << endl;
  } // output 

  void unique_test()
  {
    using namespace std;
    using namespace sc_test;
  // //   unique_ptr example
    cout << "\n***  UNIQUE_PTR TEST  ***" << endl << endl;
    
    unique_ptr<Rectangle> R1(new Rectangle(10,5));
    output(R1,"R1");
    cout << "Address of R1  : " << R1.get() << endl << endl;
      
    unique_ptr<Rectangle> R2;
  //   R2 = R1; Error
    R2 = std::move(R1);
    output(R2, "R2");
    cout << "Address of R2  : " << R2.get() << endl << endl;
    
    cout << " << DELETE R1 >> " << endl;
    R1.reset();  
  //   output(R1, "R1");  SEG FAULT
    output(R2, "R2");
    cout << "Address of R2  : " << R2.get() << endl;
    cout << "Address of R1  : " << R1.get() << endl << endl;
    
    cout << " << DELETE R2 >> " << endl;
    R2.reset();  
    cout << "Address of R2  : " << R2.get() << endl;
    cout << "Address of R1  : " << R1.get() << endl << endl;
  
 /*
    This  two lines gives us a SEG FAULT, because of unique_ptr...
        Rectange(10,5) object has been reset.

    output(R1);
    output(R2);
    
  */
    
  } // unique_test

  void shared_test()
  {
    using namespace std;
    using namespace sc_test;
    cout << "\n***  SHARED_PTR TETS  ***" << endl << endl;
    
  //   shared_ptr<Rectangle> R1(new Rectangle(5,5));
    auto R1 = make_shared<Rectangle>(5,5);
// out lambda function
    auto out = 
      [](shared_ptr<Rectangle>& R, std::string RR)->void
      {
        cout 
            << " ** " << RR << " ** " << endl
            << "length  : " << R->getLength()
            << " * "
            << "breadth : " << R->getBreadth()
            << endl << "Area = " 
            << R->area() << endl;
      };        
    out(R1, "R1");
    
//     İts OK for shared_ptr
    auto R2 = R1;
    out(R2, "R2");
    
    shared_ptr<Rectangle> R3(new Rectangle(2,2));
    out(R3, "R3");
    
    R3.swap(R2);
    out(R2, "R2");
    out(R3, "R3");

// Raw pointer R4    
    Rectangle *R4 = new Rectangle;

// Smart pointer R5
    shared_ptr<Rectangle> R5(R4);
    
    cout << endl;
    cout << "Test of raw pointer and shared_ptr\n" 
         << " can point same location?" << endl;
    if(R5.get() == R4)
      cout << "--YES " << endl;
    
    cout << "Addres of R5  : " << R5 << endl;
    cout << "Addres of R4  : " << R4 << endl << endl;
    
    cout << "Counter R1 : " << R1.use_count() << endl; 
    
    R3.reset(); 
    cout << "\n*** Deleting R3 ***" << endl;
    cout << "Counter R1 : " << R1.use_count() << endl; 
  
    R2.reset(); 
    cout << "\n*** Deleting R2 ***" << endl;
    cout << "Counter R1 : " << R1.use_count() << endl << endl; 
    out(R1, "R1");
    
    R1.reset(); 
    cout << "\n*** Deleting R1 ***" << endl;
    cout << "Counter R1 : " << R1.use_count() << endl << endl; 
    out(R1, "R1");

  } // shared_test

} // sc_test

int main()
{
  using namespace std;
  using namespace sc_test;
  
//   unique_test();
  shared_test();
  
  return 0;
} // main
