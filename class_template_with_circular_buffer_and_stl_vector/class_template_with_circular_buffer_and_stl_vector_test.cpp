#include<iostream>
#include<boost/circular_buffer.hpp>
#include<boost/swap.hpp>
#include<cmath>
#include<vector>
#include<functional>
#include<algorithm>
#include<memory>
#include<numeric>

namespace sc_test{

// Create a template class named Container for store the datas.
template < typename C >
class Container{
  private:
    C stored_data_;
  
  public:
// Cons, copy cons, dest for class
    Container( C in_data ) : stored_data_( in_data ) {}
    Container( const Container& obj ) 
    {
      stored_data_ = obj.stored_data_;
    }
    ~Container() = default;

// Member Functions definitions for Container
    C getter() const;
    void setter(C in_data);

    void print_data();    
    double average_of_data();
    void make_all_positive_data();  
};

// Basic getter function for private value
template < typename C >
C Container< C >::getter() const 
{
  return stored_data_; 
} // getter

// Setter for class
template < typename C >
void Container< C >::setter( C in_data ) 
{
  stored_data_ = in_data;
} // setter

// Basic print function for showing datas.
template < typename C >
void Container< C >::print_data()
{
  auto o_data = stored_data_;

// Range based for loop HERE!
  for( auto &item : o_data )
    std::cout << item << " ";
  std::cout << std::endl;
}

// Calculating function for average of input datas
template < typename C >
double Container< C >::average_of_data()
{
  /*
  double sum = 0.0;
  std::for_each
  (
    stored_data_.begin(),
    stored_data_.end(),
    [&sum](int item)->void
    {
      sum += item;
    }
  );

  return sum / stored_data_.size();
  */

// Better way !  
  return static_cast< double > 
    ( std::accumulate
      (
        stored_data_.begin(), 
        stored_data_.end(),
        0.0
      ) / stored_data_.size()
    );    
} // print_data

// Convert whole the items positive in stored_data_ 
template < typename C >
void Container< C >::make_all_positive_data()
{
  auto capture_data
    = [this]()->void
    {
      for(auto &item : stored_data_)
        if( item < 0)
          item *= -1;
        else
          ;
    };
  
  capture_data();
}

// GLOBAL TEMPLATES

template< class out_Type >
void
print_data
( out_Type o_data )
{
  for( auto &item : o_data )
    std::cout << item << " ";
  std::cout << std::endl;
} //  output


// Creating random items for stored_data_ and return first pointer of random items
template < class data_Type >
int*
create_rand_arr
( data_Type& size )
{
  srand( time( NULL ));
  int* arr = new int[size];
  for(int kk = 0; kk < size; ++kk)
    arr[ kk ] = int( rand() % 10 + (-5) );

  return arr;
} //create_rand_arr


// TESTS

void stl_vector_test()
{ 
  printf( " \n *** STL_VECTOR_TEST *** \n \n" );

// Data_Size is 10 !  
  const int SIZE = 10; 
  std::vector<int> test_vec( SIZE );
 
// rand_int_ptr takes the first random item's address.
  auto rand_int_ptr 
    = create_rand_arr( SIZE );

// Update the test_vec items HERE!
  int kk = 0;
  for(auto &item : test_vec)
    item = *( rand_int_ptr + kk++ );
      
// Our vector type Container instance. Container < type > name
  Container< std::vector<int> > test_vec_instance
    = ( test_vec ) ;
 
// Using print_data function print the items inside the private member. 
  printf( " ** test_vec ** \n " );
  test_vec_instance.print_data();

// Calculate the average of items in stored_data_  
  std::cout << " ** average_of_data ** \n "  
            << test_vec_instance.average_of_data() << std::endl << std::endl;
  
// Here make all the items positive here. 
// If any of item in stored_data is positive its OK, if not then convert it to positive.
  test_vec_instance.make_all_positive_data();
  
  printf( " ** test_vec_all_positive ** \n " );
  test_vec_instance.print_data();
 
  std::cout << " ** average_of_all_positive_data ** \n "  
            << test_vec_instance.average_of_data() << std::endl;
  
} // stl_vector_test

void boost_circular_buffer_test()
{
  printf( " \n *** BOOST_CIRCULAR_BUFFER_TEST *** \n" );
  
// Same as stl_vector_test !!!
  const int SIZE = 10;
  boost::circular_buffer<int> test_buffer( 10 ); 
  
  print_data( test_buffer );
  
  auto rand_int_ptr
    = create_rand_arr( SIZE );
  
  for( int kk = 0; kk < SIZE; ++kk )
    test_buffer.push_back( *( rand_int_ptr + kk ) );
  
  Container < boost::circular_buffer<int> > test_buffer_instace
    = ( test_buffer );
  
  printf( " ** test_buffer ** \n " );
  test_buffer_instace.print_data();
  
  test_buffer_instace.make_all_positive_data();
  
  printf( " ** test_buffer_all_positive ** \n " );
  test_buffer_instace.print_data();
  
// Here create an 10 sized circular_buffer then initialize whole the items to 0
  Container < boost::circular_buffer<int> > test_buffer_instace_new
    = ( boost::circular_buffer<int> ( SIZE,0 ) );
  
  printf( "\n ** test_buffer_new ** \n " );
  test_buffer_instace_new.print_data();
  
  boost::swap( test_buffer_instace, test_buffer_instace_new );
//   std::swap( test_buffer_instace, test_buffer_instace_new );

  printf( " ** test_buffer_new ** \n " );
  test_buffer_instace_new.print_data();
  
// Take the stored_data for special instance
  auto get_data = test_buffer_instace_new.getter();

  boost::circular_buffer<int>::iterator it 
    = get_data.begin();

// Using iterators get rid of the 0's in the stored_data  
  int cnt = 0;
  for( ;it != get_data.end() ; ++it )
    if( *it == 0 )
    {
      get_data.erase( it );
      cnt++;
    }
   
// Update the size of buffer.
  get_data.resize( SIZE - cnt );

  std::cout << "\n ** updated_size_of_test_buffer_new **\n "
            << get_data.size() << std::endl;

// Update the instance's stored_data
  test_buffer_instace_new.setter( get_data );

  printf( " ** updated_test_buffer_new ** \n " );
  test_buffer_instace_new.print_data();
  
} // boost_circular_buffer_test

} // namespace

int main()
{
  using namespace sc_test;
    
  stl_vector_test();
  boost_circular_buffer_test();
  
  return 0;
}
