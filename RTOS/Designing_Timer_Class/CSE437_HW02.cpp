/*
  Test kodu için çağırılacak registerTimer fonksiyonlarının ardından
  ... timing_Func() member fonksiyonu çağırılmalıdır. 
*/

#include<memory>

#include"Timer.h"

using namespace std;
using namespace chrono;

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;

static CLOCK::time_point T0;

void logCallback(int id, const std::string& logstr) {
  auto dt = CLOCK::now() - T0;
  std::cout << "[" 
            << std::chrono::duration_cast< std::chrono::milliseconds >(dt).count()
            << "]"
            << " -> (cb " << id << "): " 
            << logstr << std::endl;
            
} //logCallback

void test_fcn() {
  using namespace sc_test;
  
//  timer adında bir instance üretiliyor ve bu instance üzerinden işlemler yapılıyor.  
  auto timer
    = std::make_shared< Timer >();
    
  std::this_thread::sleep_for( std::chrono::seconds(1) );  
  T0 = CLOCK::now();
  
  logCallback( -1, " main starting. " );
  
  auto t1 = CLOCK::now() + std::chrono::seconds(1);
  auto t2 = t1 + std::chrono::seconds(1);
    
  timer->registerTimer
  (
    t2, 
    [&]()
    { 
      logCallback( 1, "callback str" );       
    } 
  );

  timer->registerTimer
  (
    t1, 
    [&]()
    { 
      logCallback( 2, "callback str" );       
    } 
  );

  timer->registerTimer
  (
    Millisecs(700), 
    [&]()
    { 
      logCallback( 3, "callback str" );       
    } 
  );

  timer->registerTimer
  (
    t1 + Millisecs(300),
    Millisecs(500),
    [&]()
    { 
      logCallback( 4, "callback str" );       
    } 
  );

  timer->registerTimer
  (
    [&]()
    { 
      static int count = 0;
      return count++ < 3;
    },    
    Millisecs(500),
    [&]()
    { 
      logCallback( 5, "callback str" );       
    } 
  );

//  timing_Func aracılığı ile ilgili fonksiyonların belirlenen zamanlarda çalışması sağlanacaktır. 
//  ... 3. callback sonsuz bir loop olduğundan program sonlanmayacaktır. Bu fonksiyona bir zaman parametresi verilerek 
//  ... verilen zamana kadar çalışması sağlanabilirdi fakat test aşamasında hangi zamanların kullanılacağı bilinmediğinden 
//  ... programın sonlanmaması tercih edilmiştir.
  
  timer->timing_Func();

} // test_fcn

int main() 
{
  test_fcn();
  
  return 0;
}
