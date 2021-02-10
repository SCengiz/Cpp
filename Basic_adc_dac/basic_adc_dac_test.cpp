/*  IN THIS CODE BASIC DIGITAL TO ANALOG OR \
 *    ANALOG TO DIGITAL CONVERTER IS DESGINED
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<iomanip>
#include<functional>
#include<string>
#include<sstream>

using namespace std;

namespace sc_test{
  
class Abstract{
      
protected:
    float analog_;
    int digital_;
    void check_datas()
    {
//       NOTE: Do it better function.
      if(analog_ < 0 && analog_ > 5 && digital_ < 0 && digital_ > 255)
      {
        cout << " Invalid Input Arguments " << endl; 
        exit(1);
      }
    } 
public:
    Abstract(float analog, int digital = 0) 
    :
    analog_(analog), digital_(digital) { check_datas(); }
    
    Abstract(int digital, float analog = 0) 
    :
    analog_(analog), digital_(digital) { check_datas(); }
    
    float getAnalog() const { return analog_; }
    int getDigital() const { return digital_; }
    
    virtual ~Abstract() {};  
    
    // Purely virtual functions
    virtual float adc() = 0;
    virtual string decide() = 0;

    // Operator overloading     
    friend ostream& operator <<(ostream& os, Abstract& other)
    {
        if(other.getAnalog() == 0)
            os << other.getDigital();
        else 
            os << other.getAnalog();
        
        return os;
    }
    
}; // class

// Inheritance
class Analog_to_Digital : public Abstract{
    
public:
    Analog_to_Digital(float analog) : Abstract(analog){}
    
    virtual ~Analog_to_Digital() {};

    virtual float adc()
    {
        int digital_value = (analog_ * 255) / 5 ;
        return digital_value;
    }
    
    virtual string decide(){
        stringstream ss;
   
        if(analog_ >= 4)
            ss << "LOJIK_HIGH" << endl;
        else if(analog_ < 4 && analog_ > 1)
            ss << "UNDEFINED_REGION" << endl;
        else
            ss << "LOJIK_LOW" << endl;
        
        return ss.str();
    }
    
};

class Digital_to_Analog : public Abstract{
    
public:
    Digital_to_Analog(int digital) : Abstract(digital){}
    
    virtual ~Digital_to_Analog() {};
    
    virtual float adc()
    {
//         float analog_value = (digital_ * 5 ) / 255 ;
        return (digital_ * 5 ) / 255.;
    }
    
    virtual string decide(){
        stringstream ss;
        
        if(digital_ >= 204)
            ss << "LOJIK_HIGH" << endl;
        else if(digital_ < 204 && digital_ > 51)
            ss << "UNDEFINED_REGION" << endl;
        else
            ss << "LOJIK_LOW" << endl;
        
        return ss.str();
    }
};

namespace tests{
  
  void info()
  {
    cout << " *** ADC and DAC Application *** " << endl;
    cout << "ANALOG PART \n"
         << "ANALOG >= 4  LOJIK HIGH \n" 
         << "ANALOG < 4 && ANALOG > 1 NOT DEFINED \n" 
         << "ANALOG <= 1 LOJIK LOW \n\n" ; 
         
    cout << "DIGITAL PART \n"
         << "DIGITAL >= 204  LOJIK HIGH \n" 
         << "DIGITAL < 204 && DIGITAL > 51 NOT DEFINED \n" 
         << "DIGITALG <= 51 LOJIK LOW \n\n" ; 
  }
  
  void basic_test()
  {
    using namespace sc_test;
    info();
    cout << " *** BASIC_TEST *** " << endl;
    
//     vec of smart_ptr instances
    std::vector< std::shared_ptr <Abstract> > vec;

// Using perfect forwarding
    
//     vec.push_back(std::make_shared<Analog_to_Digital>(0.5));
//     vec.push_back(std::make_shared<Digital_to_Analog>(204));

    const int SZ = 5;
    float a_value = 0.1;
    int   d_value = 35;
    
    for(int kk = 0; kk < SZ; kk++)
    {
      vec.emplace_back(std::make_shared<Analog_to_Digital>( a_value + (kk)) );
      vec.emplace_back(std::make_shared<Digital_to_Analog>( d_value + (kk*50)) );
    }

//     vec of functions types.
    std::vector< std::function<float(void)> > vec_adc;    
    std::vector< std::function<string(void)> > vec_decide;    
    
//     Push functions to vector.
    for( unsigned int kk = 0 ; kk < vec.size() ; ++kk ){
        auto & temp_value = vec[kk];
        vec_adc.push_back
            (
                [temp_value](void)->float 
                {
                    return temp_value->adc();
                }
            );
//         auto & t_temp_value = vec[kk];
        vec_decide.push_back
            (
                [temp_value](void)->string 
                {   
                    return temp_value->decide();
                }
            );
    }
    
//     Range and Index based for loop together.
    int i = 0;
    for(auto item : vec_adc)
    { 
//       Assume we have a Analog_to_Digital instance ptr.
//       Try to cast A_to_D if it is not possible its a really Analog_to_Digital instance ptr.
      Analog_to_Digital *d_c;
      d_c = dynamic_cast<Analog_to_Digital* >(vec[i].get());
      
//       Alternative if(vec[i]->getAnalog == 0)   same result.
      if(d_c == NULL)
      {
        cout << i+1 << "."
             <<" This is a Digital to Analog Transmisson" << endl ;
        cout <<" Digital_Value = " << vec[i]->getDigital()
             <<" ---> " << "Analog Equivalent = " << item() << endl ;
        cout <<" Logic Equivalent = " << vec_decide[i]() << endl;   
      }
      else
      {
        cout << i+1 << "."  
             <<" This is a Analog to Digital Transmisson" << endl ;
        cout <<" Analog_Value = " << vec[i]->getAnalog()
             <<" ---> " << "Digital Equivalent = " << item() << endl;
        cout <<" Logic Equivalent = " << vec_decide[i]() << endl;   
      }
        i++;
    }
    
  } // func

  } // namespace
} // namespace

int main(void){
    using namespace sc_test;
    using namespace sc_test::tests;
    
    basic_test();
    
    return 0;   
}
