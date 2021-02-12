/*
  How can you efficiently store a matrix of doubles?  
    Then desribe how you can exchange the locations of two rows in the matrix storing entity
*/

#include<iostream>
#include<vector>
#include<exception>
#include<stdexcept>
#include<algorithm>
#include<memory>

using namespace std;

namespace sc_test{

  namespace types{
    using vector_type 
      = std::vector< double >;
    using matrix_type 
      = std::vector< std::vector < double > > ;

  } // nested namespace
    
  using namespace sc_test::types;
  
  class Matrix
  {
  private:
    matrix_type mat_ ; 
    unsigned int n_rows_ , n_cols_ ;
    void check_input_data
      ( matrix_type & mat );
    
  public:  
    Matrix( matrix_type& mat ) 
    :
    mat_(mat), n_rows_( mat.size() ), n_cols_( mat[0].size() )
    {
      check_input_data(mat);
    }
    
//     std::vector< std::vector < double > > 
    matrix_type getMat() { return mat_; }
      
    int getRow() { return n_rows_ ;}
    int getCol() { return n_cols_ ;}

    void setMatRow(Matrix& mat, int row_1, int row_2);
    void setMatCol(Matrix& mat, int col_1, int col_2);
    
    friend ostream& operator <<
      (ostream &os ,Matrix obj);
      
    ~Matrix() = default;
    
  }; // end class
  
  void Matrix
  ::
  setMatRow(Matrix& mat, int row_1, int row_2)
  {
      auto cols = mat.getCol();
      auto rows = mat.getRow();
      
      if(row_1 < rows && row_2 < rows)
      {
        for(int i = 0 ; i < cols ; i++)
        {
          mat_[row_1][i] = mat.getMat()[row_2][i];
          mat_[row_2][i] = mat.getMat()[row_1][i];
        }
      }
      else 
        cout << "row_1 and row_2 is not suitable \n ";
  }  

  void Matrix
  ::
  setMatCol(Matrix& mat, int col_1, int col_2)
  {
      int rows = mat.getRow();
      int cols = mat.getCol();
      
      if(col_1 < cols && col_2 < cols)
      {
        for(int i = 0 ; i < rows ; i++)
        {
          mat_[i][col_1] = mat.getMat()[i][col_2];
          mat_[i][col_2] = mat.getMat()[i][col_1];
        }
      }
      else 
        cout << "col_1 and col_2 is not suitable \n ";
  }

  void Matrix
  ::
  check_input_data(matrix_type & mat)
  {
    using namespace std;
    if( n_rows_ <= 0 )
      throw std::invalid_argument
        ("Can not have zero");

// column size in first row
    unsigned int check_val = mat[0].size(); 

// Look up equality all column size each other.     
    bool result 
      = std::all_of
    (
      mat.begin() + 1 ,
      mat.end() ,
      [&check_val]
      (vector_type& item)->bool
      {
        return check_val == item.size();
      }
    );
    
    if( mat.size() > 1 )
    {
      if( !result )
      {
        throw std::invalid_argument
          ("Can not have different number.");
      }
    }
  } // func

//     Basic Ostream Operator Overload for Matrix Class.
  ostream& operator 
  <<
  (ostream &os , Matrix obj)
  {
    int i,j;
    auto rows = obj.getRow();
    auto cols = obj.getCol();
    
    for(i = 0 ; i < rows ; i++){
      for(j = 0 ; j < cols ; j++){
        os << obj.getMat()[i][j] << " " ;
      }
      cout << endl;
    }
    
    return os;
  } // func
  
//   This is global function. NOTE:Do it better change function.
  void change_row_or_col(Matrix& obj, int flag, int ch_1 = 0, int ch_2 = 1)
  {
    using namespace std;    
    Matrix copy_mx(obj);
      
    switch(flag)
    {
      case 0:      // Changing rows
        cout << " ** Row Changing case ** " << endl;
        cout << " Look up in these rows "
             << ch_1 << " and " << ch_2 << endl;
        copy_mx.setMatRow(obj, ch_1, ch_2);
        cout << copy_mx << endl;
        break;
      
      case 1:      // Changing cols
        cout << " ** Col Changing case ** " << endl;
        cout << " Look up in these cols "
             << ch_1 << " and " << ch_2 << endl;
        copy_mx.setMatCol(obj, ch_1, ch_2);
        cout << copy_mx << endl;
        break;
      
      default:
        cout << "No choice" << endl;
        break;
    } //switch
  } // func
     
  namespace tests{
    
    void basic_test()
    {
      using namespace std;
      using namespace sc_test;
      using namespace sc_test::types;
    
      cout << "NOTE : Don't forget indexes will start from 0 in C++ " << endl;
      cout << " *** BASIC_TEST *** " << endl;
            
      matrix_type mat =
      {
        vector_type {1, 2, 3, 0} ,
        vector_type {4, 5, 6, 0} ,
        vector_type {7, 8, 9, 0}
      };
  
//       I don't wanna change oa maybe make it constant.      
      Matrix oa(mat);
      cout << "Original Matrix :" << endl;
      cout << oa << endl;

      change_row_or_col(oa, 0);
      change_row_or_col(oa, 1); 

      change_row_or_col(oa, 0, 2, 1);
      change_row_or_col(oa, 1, 3, 1); 

    } // test
  
  } // nested namespace
  
} // sc_test namespace

int main()
{
  using namespace std;
  using namespace sc_test;
  using namespace sc_test::types;
  using namespace sc_test::tests;
  
  basic_test();
  
  return 0;
}
