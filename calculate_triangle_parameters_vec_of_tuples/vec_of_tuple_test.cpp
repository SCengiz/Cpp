/*
  For Triangle :
    Area = 1/2 * b * c * sin(angle(a))
    Area = 1/2 * a * c * sin(angle(b))
    Area = 1/2 * a * b * sin(angle(c))
  Using this formulas we calculate something
    ( area, diameter, other angles, triangle type )
    ...in the script for a triangle.

  21.02.2021 20:48
    Soray Cengiz
*/

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cmath>
#include <cassert>

// Everything in this namespace Encapsulation !!
namespace sc_test{

// We need a class for triangle parameters  
class Triangle
{
// TODO: Update check_datas() this is bad 
private:
  double edge_a_, edge_b_, edge_c_;
  double angle_a_;
  std::string rect_name_;
  void check_datas()
  {
    using namespace std;

    if
    (
      edge_a_ <= 0 || edge_b_ <= 0 || edge_c_ <= 0 || 
        angle_a_ < 0 || angle_a_ > 180
    )
    { 
      cout << " Wrong input parameters \n";
      exit(1);
    }
  } // check_datas
  
public:
//   Constructor
  Triangle
  (
   double edge_a, 
   double edge_b,
   double edge_c, 
   double angle_a,
   std::string rect_name
  )
  :
  edge_a_(edge_a) ,edge_b_(edge_b), edge_c_(edge_c), 
    angle_a_(angle_a), rect_name_(rect_name)
  {
    check_datas();
  }
  
// Destructor
  ~Triangle() = default;

  double get_angle() { return angle_a_; }
  
//   We need a << operator for showing rectangle parameters.
  friend std::ostream& operator<<
    (std::ostream& os, const Triangle& obj)
  {
    using namespace std;
    os << endl
       << " Triangle name  : " << obj.rect_name_ << endl
       << " edge_a         : " << obj.edge_a_    << endl
       << " edge_b         : " << obj.edge_b_    << endl
       << " edge_c         : " << obj.edge_c_    << endl
       << " angle_a        : " << obj.angle_a_   ;
    return os;
  }
  
// Our compute functions.
  double compute_area();  
  double compute_perimeter() const{ return edge_a_ + edge_b_ + edge_c_ ;}
  double compute_angle_b();
  double compute_angle_c();
  
  void triangle_type(); 
  
}; // class Rectangle

double Triangle::compute_area()
{
// Conversion this angle of a 
  double convert_radian_to_degree 
    = (angle_a_ * M_PI) / 180. ;
// Calculate area with specified formula for triangle
  double calculated 
    = 0.5 * edge_b_ * edge_c_ * sin(convert_radian_to_degree);
      
  return calculated;
} // compute_area

double Triangle::compute_angle_b()
{
//   compute angle_b using area and other parameters
//   Here we use just a formula in first lines.
  using namespace std;
 
  auto sin_angle_b
    = compute_area() / (0.5 * edge_a_ * edge_c_);
    
  auto converted_angle_b
    = (asin(sin_angle_b) * 180) / M_PI;
    
  return converted_angle_b;
} // compute_angle_b

double Triangle::compute_angle_c()
{
//   same as compute_angle_b but for angle c
  using namespace std;
 
  auto sin_angle_c
    = compute_area() / (0.5 * edge_a_ * edge_b_);
    
  auto converted_angle_c
    = (asin(sin_angle_c) * 180) / M_PI;
    
  return converted_angle_c;
} // compute_angle_c

void Triangle::triangle_type()
{
//   Founding type of our rectangle we do a basic if else condition
  using namespace std;
    
  if(angle_a_ == 90 || compute_angle_b() == 90 || compute_angle_c() == 90)
    cout << " Triangle type  : " << "RIGHT-ANGLED TRIANGLE" << endl;
  else if
    (angle_a_ > 90 || compute_angle_b() > 90 || compute_angle_c() > 90)
    cout << " Triangle type  : " << "OBTUSE TRIANGLE" << endl;
  else
    cout << " Triangle type  : " << "ACUTE TRIANGLE" << endl;
} // triangle_type
  
namespace types{
//   Our types for this script.
  using vec_type 
      = std::vector<std::shared_ptr< Triangle >>;
  
  using lambda_type  
    = std::function<void(void)>;
// NOTE:  Everything will be stored in the this type!!!
  using tuple_type
    = std::tuple
      <
        Triangle,
        double, 
        double,
        double,
        double,
        lambda_type
      >;
  
  using vec_of_tuple_type
    = std::vector<tuple_type>;
        
} // types

void tuple_output(types::tuple_type& in_tuple)
{
//  Show everything in the tuple !!!
  using namespace std;
  using namespace sc_test::types;

//  Thanks to auto keyword we take all argument from tuple without
// ... knowing their types.  
  auto in_instance = std::get<0>(in_tuple);
  auto in_area     = std::get<1>(in_tuple);
  auto in_dia      = std::get<2>(in_tuple);
  auto in_angle_b  = std::get<3>(in_tuple);
  auto in_angle_c  = std::get<4>(in_tuple);
  auto in_lambda   = std::get<5>(in_tuple);
  
  cout << in_instance          << endl
       << " angle_b        : " << in_angle_b  << endl
       << " angle_c        : " << in_angle_c  << endl
       << " Area           : " << in_area     << endl
       << " Dia            : " << in_dia      << endl;
       
// NOTE: Last item in tuple is a function.
  in_lambda();
  
} // tuple_output

void tuple_test()
{
  using namespace std;
  using namespace  sc_test;
  using namespace  sc_test::types;

  cout << "\n *** TUPLE TEST ***\n " << endl;
  
//   Our data_type for input triangles  !!
  vec_type vec_triangles;
  
  // 90 45 45 triangle
  vec_triangles.push_back
  (
    std::make_shared< Triangle >
      (2, 2, 2*sqrt(2), 45, " << 45-45-90 >> ")
  );
  
  // 30 60 90 triangle
  vec_triangles.push_back
  (
    std::make_shared< Triangle >
      (2*sqrt(3), 2, 4, 60, " << 30-60-90 >> ")
  );
  
  // // 30 30 120 triangle
  vec_triangles.push_back
  (
    std::make_shared< Triangle >
      (2*sqrt(3), 2, 2, 120, " << 30-30-120 >>")
  );

//   We will work with multiple instances and everything will be in vector
//   ... of tuples
  vec_of_tuple_type 
    vec_of_tuples;

//  Automatic make_tuple for our instances !!
  auto make_tuple_instances
    = [](std::shared_ptr< Triangle >& in_instance)->tuple_type
    {
      return std::make_tuple
      (
        *in_instance,
        in_instance->compute_area(),
        in_instance->compute_perimeter(),
        in_instance->compute_angle_b(),
        in_instance->compute_angle_c(),
        [&in_instance]()
        {
          in_instance->triangle_type();
        }
      );
    };

//   For all triangle instances 
  vec_of_tuples.emplace_back(make_tuple_instances(vec_triangles[0]));
  vec_of_tuples.emplace_back(make_tuple_instances(vec_triangles[1]));
  vec_of_tuples.emplace_back(make_tuple_instances(vec_triangles[2]));
  
//   Show all of them
  for(auto &item : vec_of_tuples)
    tuple_output(item);
    
} // end tuple_test
  
}  // namespace

int main()
{
  using namespace  sc_test;
  
  tuple_test();
  
  return 0;
}
