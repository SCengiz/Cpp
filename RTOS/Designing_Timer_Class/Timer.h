/*
  CSE37 HW02 : Soray CENGİZ 161024077
    22.04.2021
*/

#include<iostream>
#include<functional>
#include<chrono>
#include<thread>
#include<mutex>
#include<atomic>
#include<queue>
#include<stdexcept>

using namespace std;
using namespace chrono;

namespace sc_test {

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;

class Abstract_Timer {

public:
  
  virtual ~Abstract_Timer() = default;

//   run the callback once at time point tp.
  virtual void registerTimer( const Timepoint& tp, const TTimerCallback& cb ) = 0;

//   run the callback periodically forever. The first call will be executed after the first period.
  virtual void registerTimer( const Millisecs& period, const TTimerCallback& cb ) = 0;
  
//   run the callback periodically until time point tp. The first call will be executed after the first period.
  virtual void registerTimer( const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb ) = 0;
  
//   run the callback periodically. After calling the callback every time, call the predicate to check if the
//   ... termination criterion is satisfied. If the predicate returns false, stop calling the callback.
  virtual void registerTimer( const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb ) = 0;
  
  
}; // ITimer

//  Abstract_Timer class'dan türeyecek olan timer class'ını kullanarak basit bir time_event_generator tasarlanmıştır.
class Timer : public Abstract_Timer { 
  
private:
//  Thread safe olabilmesi için std::mutex değişkeni ile ilgili member fonksiyonların kritik bölgeleri koruma altına alınacaktır.
  mutable std::mutex mtx_;
//  std::deque değişkeni own_Thread_'in farklı zamanlarda çalışan durumlarını barındırmaktadır. ( NOTE: Düzelt )
  std::thread own_Thread_;    
  std::deque<std::thread> deq_;
    
public:
  virtual ~Timer() = default;
  
//  Bu fonksiyon thread'ler arasındaki context-switch'i gerçekleştirecektir.  
  void timing_Func() {    
    try {
      std::unique_lock<std::mutex> lock( mtx_ );
      for( auto &item : deq_ ) 
        item.join();                
    } 
    catch( std::system_error& err ) {  
      std::unique_lock< std::mutex > lock( mtx_ );
      std::cout << err.code() << " meaning : " 
                << err.what() << std::endl;
      exit(1);
    }        
  }

//  İlk fonksiyon ile verilen fonksiyonun çağırılması belli spesifik bir zamanda yapılacaktır.
  virtual void registerTimer( const Timepoint& tp, const TTimerCallback& cb ) override {
    std::unique_lock< std::mutex > lock( mtx_ );

//  Burada bir lambda fonksiyonu aracılığı ile verilen fonksiyonun şu andan ne zaman sonra çağırılacağını sleep_for komutu aracılığı
//  ... ile o kadar süre bekleterek yapıyoruz. CPU burada thread'i bekleyenler kuyruğuna alınacak ve zamanı geldiğinde cb() fonksiyonu çalışacak.
    auto lm_func_1
      = [&, tp, cb]()->void
      {
        std::this_thread::sleep_for( tp - CLOCK::now() );
        cb();   
      };
//  Lambda fonksiyonunun thread'in constructor'una veriyoruz ve "deq" değişkenine push ediyoruz.
// ... push yapılırken own_Thread_ değişkenini move edilerek push ediliyor.
    own_Thread_ = std::thread( lm_func_1 );
    deq_.emplace_back( std::move(own_Thread_) );
  
  }

//  İkinci fonksiyonda verilen bir fonksiyonun istenilen period'da sürekli olarak çalışmasını sağlamaktadır.  
  virtual void registerTimer( const Millisecs& period, const TTimerCallback& cb ) override {
    std::unique_lock< std::mutex > lock( mtx_ );
    
    auto lm_func_2
      = [&, period, cb]( )
      {
        while( true ) {
          std::this_thread::sleep_for( std::chrono::milliseconds( period ) );
          cb();        
        }
      };    

    own_Thread_ = std::thread( lm_func_2 );    
    deq_.emplace_back( std::move(own_Thread_) );
      
  }

//  Üçüncü fonksiyonda verilen bir fonksiyonun istenilen 2 zaman aralığında periyodik olarak çalışması sağlanacaktır.  
  virtual void registerTimer( const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb ) override {
    std::unique_lock< std::mutex > lock( mtx_ );
    
    auto lm_func_3
      = [&, tp, period, cb]()
      {      
        while( CLOCK::now() < tp - Millisecs(500) ) {
          std::this_thread::sleep_for( std::chrono::milliseconds( period ) );
          cb();        
        }
      };

    own_Thread_ = std::thread( lm_func_3 );     
    deq_.emplace_back( std::move(own_Thread_) );
     
  }

//  Son fonksiyonda verilen bir fonksiyonun verilen başka bir predicate fonksiyonuna uyduğu sürece( istenilen durum sağlandığı sürece ) 
// ... çalışması sağlanacaktır.
  virtual void registerTimer( const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb ) override {
    std::unique_lock< std::mutex > lock( mtx_ );
    
    auto lm_func_4
      = [&, pred, period, cb]()
      {      
        while( pred() ) {
          std::this_thread::sleep_for( std::chrono::milliseconds( period ) );
          cb();        
        }
      };

    own_Thread_ = std::thread( lm_func_4 );    
    deq_.emplace_back( std::move(own_Thread_) );
    
  }

};

} // sc_test

