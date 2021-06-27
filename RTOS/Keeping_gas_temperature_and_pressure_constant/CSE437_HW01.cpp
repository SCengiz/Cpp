/*
  CSE437 HW01 : Soray CENGİZ 161024077
    18.03.2021 
  
    Kodu linux'da derleyebilmek için basit bir makefile yazılmıştır. 
    Kodun bir proje şeklinde farklı dosyalarda ( .h .cc ) yazılmış versiyonu da bulunmaktadır; fakat
    ... basit bir kod istendiğinden o versiyonu paylaşılmamıştır.  
    
    Kod içerisinde Analog voltaj -> Dijital voltaj değerine geçerken 10 bitlik bir ADC kullanılmıştır.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <ratio>
#include <random>
#include <functional>
#include <iomanip>

using namespace std;
using namespace chrono;

// sc_test namespace'i altında, kurulmuş tüm yapı bulunmaktadır.
namespace sc_test{

// Bir Inheritance yapısı kurularak her iki sensör için de gerekli olan sensör üzerindeki analog değere bakarak karar verme işlemi 
// ... gerçekleşeceğinden base class'da bir analog_ değişkeni tanımlanmıştır.
// name_ değişkeni hangi sensörün( portun ) triggerADC ve readADC fonksiyonları çalışmaktadır bilgisini ekranda gözlemlemek için
// ... kullanılmaktadır.  
  class Abstract{    
  
  protected:
    double analog_;
    std::string name_;
    
  public:
    Abstract( double analog, std::string name ) : analog_( analog ), name_( name ) {}  

    double get_analog() const { return analog_; }  
    
// adc_flag değişkeni readADC fonksiyonunun ADC tetiklenmediği durumlarda çalışmamasını, hata vermesini sağlamak için kullanılmıştır.
// readADC() kullanılmadan önce triggerADC() kullanıldı ise bir sorun ile karşılaşılmayacaktır.
    bool adc_flag = false;
    void triggerADC()
    {
      std::cout << " TRIGGER ADC CALL FOR" << name_
                << std::endl;      
      
      adc_flag = true;
    }
    
// 10 Bitlik bir ADC kullanarak analog voltaj değerinin microcontroller içerisinde dijital karşılığı bulunacaktır.
    double readADC()
    {
      std::cout << " READ ADC CALL FOR" << name_ 
                << std::endl;
 
      if( adc_flag )
        return ( analog_ * 1023. ) / 5.;
      else{
        std::cout << " Need to Trigger ADC !!! " << std::endl;
        exit(1);
      }          
    }
    
    virtual ~Abstract() = default;
// Bazı bilgileri dış dünyaya göstermek için türeyecek tüm class'lar için yazılacak bir purely virtual fonksiyon tanımlanmıştır.
    virtual void output( double& , double&  ) = 0;
        
  }; // Abstract
  
// Abstract class'dan türemiş olan TempClass içerisinde sıcaklık için gerekli olan işlemler gerçekleşecektir. 
  class TempClass : public Abstract{
    
  public:
    TempClass( double analog_temp, std::string name ) : Abstract( analog_temp, name ) {}
    
    virtual ~TempClass() = default;
    
// Giriş parametresine bağlı olarak ısıtıcıyı açan bir fonksiyon yazılmıştır, burada ısıtıcının açılması beraberinde
// ... Plant içerisindeki sıcaklık sensörünün üzerindeki voltaja etki edeceğinden. Sensör üzerindeki analog değer
// ... değişimi kullanılarak bir yapı kurulmuştur.
    void updateHeater( bool condition )
    {
      if( condition ){
        analog_ += 0.01;
        std::cout << " HEATER IS ON ! \n" << std::endl;
      }
      else{
        analog_ -= 0.01;    
        std::cout << " HEATER IS OFF ! \n" << std::endl;
      }
    }
  
// Basitçe bazı veriler ekrana bastırılmıştır.
    virtual void output( double& digital_val, double& temp_value ) override{
      using std::cout;
      using std::endl;
      cout << " ANALOG    :  " << analog_     << endl
           << " DIGITAL   :  " << digital_val << endl
           << " TEMP      :  " << temp_value   << endl;            
    }
    
  }; // TempClass

// Abstract Class'dan türemiş olan PressureClass basıncı sabitlemek için gerekli olan fonksiyonları içermektedir.
  class PressureClass : public Abstract{

  public:
    PressureClass( double analog_pressure, std::string name ) : Abstract( analog_pressure, name ) {}
        
    virtual ~PressureClass() = default;
    
// writeDAC içerisinde pompaya uygulanacak değişiklik doğrudan basınç sensörü üzerinde görüleceği için
// ... değiştirilmiş digital_value değerini analog değere çevirerek sensör üzerindenki analog değeri değiştiriyoruz.
    void writeDAC( double digital_value )
    {
      analog_ = ( digital_value * 5 ) / 1023.;
    }

    virtual void output( double& digital_val, double& press_value ) override{
      using std::cout;
      using std::endl;
      cout << " ANALOG    :  " << analog_     << endl
           << " DIGITAL   :  " << digital_val << endl
           << " PRESSURE  :  " << press_value << endl;            
    }
    
  }; // PressureClass
  
// Basınç ve Sıcaklığı sabitlemek için yazılan 2 adet task yazılmıştır. Bu iki task daha sonra main içerisinde paralel çalışacaklardır.  
  namespace tasks{
    
// temp_Task için gerekli olan değişkenler global olarak tanımlanmıştır.
    const long period_Temp = 100.;      // 10HZ -> 100ms
    double in_digital_value_temp, in_analog_value_temp, in_temp;

// İlk değer olarak sıcaklık sensörü üzerindeki analog voltaj değeri random olarak belirlenip bir TempClass instance oluşturulmuştur. 
    std::random_device seed;
    std::default_random_engine engine( seed() );
    
    std::uniform_real_distribution< double > rand_temp( 0.0, 5.0 );
    
// Burada perfect-forwarding kullanarak bir TempClass smart pointer instance'ın constructor'una random bir double değer atanmıştır.
    auto temp_instance
      = std::make_shared< TempClass >( rand_temp( engine ), " TEMP " );

    void temp_Task()
    {      
          
      for( ;; ){
        auto start
          = std::chrono::steady_clock::now();

        temp_instance->triggerADC();

        auto end
          = std::chrono::steady_clock::now();

// Burada adc_trigger'ın çalışması için gereken süre delay olarak koyulmuştur.
        std::this_thread::sleep_for
          ( std::chrono::milliseconds
              ( (long)( period_Temp - std::chrono::duration< double, std::milli >
                ( end - start ).count() ) ) ); 
          
// Sıcaklık sensörü üzerindeki analog değerin dijital karşılığı hesaplanıp in_digital_value_temp'e atanmaktadır.
        in_digital_value_temp
          = temp_instance->readADC();

// Bir getter fonksiyonu aracılığı ile sensör üzerindeki analog değeri in_analog_value_temp'e atanmaktadır.
        in_analog_value_temp 
          = temp_instance->get_analog();
          
// Sıcaklık sensörü üzerindeki analog voltaj değerini belirli bir formüle bağlı kalarak sıcaklık değeri hesaplanmaktadır.
// Hesaplama için bir oran orantı kurulmuştur. 0V : -30 derece & 5V : 70 derece ( Basitçe )
// ... her 1 derece artışın analog olarak karşılığı 0.05V olacaktır.
        in_temp 
          = ( in_analog_value_temp  / 0.05 ) - 30;

        temp_instance->output( in_digital_value_temp, in_temp );

// Bulunan dijital değer microcontroller içerisinde bir sorgulama yaparak ısıtıcının durumunu güncellemektedir.
// 613.7 => 3V => 30 derece şeklinde bir orantı kurarak Plant içerisindeki sıcaklığı 30 dereceye sabit tutmak istenmektedir.  
        if( in_digital_value_temp < 613.7 )
          temp_instance->updateHeater( true );
        else if( in_digital_value_temp > 615.8 )
          temp_instance->updateHeater( false );
        else{
          std::cout << "\n *** TEMP IS STABLE !!! *** \n" << std::endl;
//           break;          
        }
        
      }
      
    } // temp_Task
    
// pressure_Task için gerekli olan değişkenlerin declaration'u global olarak yapılmıştır.
    const long period_Pressure = 10;     // 100HZ -> 10ms
    double in_digital_value_pressure, in_analog_value_pressure, in_pressure;

// Basıç sensörü üzerindeki ilk değeri random olarak class instace'ın constructor'una verilmektedir.
    std::uniform_real_distribution< double > rand_pressure( 0.0, 5.0 );
      
    auto pressure_instance
      = std::make_shared< PressureClass >( rand_pressure( engine ), " PRESSURE " );
      
    void pressure_Task()
    {
        
      for( ;; ){
        auto start
          = std::chrono::steady_clock::now();

        pressure_instance->triggerADC();

        auto end
          = std::chrono::steady_clock::now();        
         
        std::this_thread::sleep_for
          ( std::chrono::milliseconds
              ( (long)( period_Pressure - std::chrono::duration< double, std::milli >
                ( end - start ).count() ) ) ) ; 

        in_digital_value_pressure
          = pressure_instance->readADC();
        
        in_analog_value_pressure
          = pressure_instance->get_analog();
        
// Basınç sensörü üzerindeki voltaja bakarak bir pressure hesabı yapılmaktadır bu ilişki şöyle kurulmuştur;
// 0V : 0 atm & 5V : 100 atm olarak düşünerek her bir 0.05V artış karşılığında 1 atm basıncı arttıracaktır.
        in_pressure 
          = ( in_analog_value_pressure / 0.05 );
          
        pressure_instance->output( in_digital_value_pressure, in_pressure );  

// Basıncın 50 atm'de sabit tutulması istenmektedir, 2.5V => 511.5 => 50 atm şeklinde bir ilişki kurarak 
// ... writeDAC fonksiyonu ile STABLE konuma gelene kadar sensörün analog değeri güncellenecek bu da beraberinde basıncı değiştirecektir.
        if( in_digital_value_pressure < 511.5 ) {
          pressure_instance->writeDAC( in_digital_value_pressure + 1 );
          std::cout << " PUMP IS ON ! \n" << std::endl;          
        }
        else if( in_digital_value_pressure > 512.6 ) {
          pressure_instance->writeDAC( in_digital_value_pressure - 1 );          
          std::cout << " PUMP IS OFF ! \n" << std::endl;
        }
        else {
          std::cout << "\n *** PRESSURE IS STABLE !!! *** \n" << std::endl;
//           break;
        }       
      
      } 
        
    } // pressure_Task
    
  } // tasks
    
} // sc_test

int main(){
  
  using namespace sc_test::tasks;

// funcs değişkeni içerisinde task'ler tutulmaktadır.  
  std::vector < std::function< void(void) > > funcs;
  std::vector < std::thread > threads;

  funcs.emplace_back( pressure_Task ); 
  funcs.emplace_back( temp_Task );

// 2 adet task'i paralel olarak çalıştırmak için std::thread class'ı kullanılmaktadır.   
  for( int kk = 0; kk < (int)funcs.size(); ++kk )
    threads.emplace_back( std::thread( funcs[kk] ) );    

// join kullanarak o anki çalışan thread diğer thread bitene kadar durdurulur.
  for( auto &item : threads )
    item.join();
  
  return 0;
}
