#include "../src/CsvUtils.h"

#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <functional>
#include <numeric>
#include <algorithm>
#include "test_utils.h"

struct CsvUtilsTest : public ::testing::Test {

  const char* sample_no_headers_fname = "sample_no_headers.csv";
  const char* sample_with_headers_fname = "sample_with_headers.csv";
  const char* sample_for_matrix_no_headers_fname = "sample_for_matrix_no_headers.csv";
  const char* sample_for_matrix_with_headers_fname = "sample_for_matrix_with_headers.csv";
  const string sample_for_matrix_no_headers = "1,3.0,2.5\n-30,-3e-3,3.43\n";
  const string sample_for_matrix_with_headers = "col1,col2,col3\n" + sample_for_matrix_no_headers;
  const string sample_no_headers = "1,3.0,NonREM1\n-30,-3e-3,Awake\n";
  const string sample_with_headers = "int,double,string\n" + sample_no_headers;

  void create_sample_files(){
    std::ofstream ofs(sample_with_headers_fname);
    ofs << sample_with_headers;
    ofs.close();
    std::ofstream ofs2(sample_no_headers_fname);
    ofs2 << sample_no_headers;
    ofs2.close();
    std::ofstream ofs3(sample_for_matrix_no_headers_fname);
    ofs3 << sample_for_matrix_no_headers;
    ofs3.close();
    std::ofstream ofs4(sample_for_matrix_with_headers_fname);
    ofs4 << sample_for_matrix_with_headers;
    ofs4.close();
  }

	virtual void SetUp() {
    create_sample_files();
	}

	void TearDown() {
    remove(sample_no_headers_fname);
    remove(sample_with_headers_fname);
    remove(sample_for_matrix_with_headers_fname);
    remove(sample_for_matrix_no_headers_fname);
	}
};


TEST_F(CsvUtilsTest, read_csv_no_headers_nothrow) {
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_no_headers(sample_no_headers);
    });
}

TEST_F(CsvUtilsTest, read_csv_no_headers) {
  auto a = CsvReader::read_csv_no_headers(sample_no_headers);
  // for(auto vo:a){
  //   for(auto vi:vo){
  //     cout << vi << " ";
  //   }
  //   cout << endl;
  // }
  EXPECT_EQ(InValue::Type::INT, a[0][0].type());
  EXPECT_EQ(InValue::Type::DOUBLE, a[1][0].type());
  EXPECT_EQ(InValue::Type::STRING, a[2][0].type());
  EXPECT_EQ(InValue::Type::INT, a[0][1].type());
  EXPECT_EQ(InValue::Type::DOUBLE, a[1][1].type());
  EXPECT_EQ(InValue::Type::STRING, a[2][1].type());
}

TEST_F(CsvUtilsTest, read_csv_with_headers_nothrow) {
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_with_headers(sample_with_headers);
    });
}

TEST_F(CsvUtilsTest, read_csv_with_headers) {
  auto a = CsvReader::read_csv_with_headers(sample_with_headers);
  // for(auto vo:a){
  //   for(auto vi:vo){
  //     cout << vi << " ";
  //   }
  //   cout << endl;
  // }
  EXPECT_EQ(InValue::Type::INT, a["int"][0].type());
  EXPECT_EQ(InValue::Type::DOUBLE, a["double"][0].type());
  EXPECT_EQ(InValue::Type::STRING, a["string"][0].type());
  EXPECT_EQ(InValue::Type::INT, a["int"][1].type());
  EXPECT_EQ(InValue::Type::DOUBLE, a["double"][1].type());
  EXPECT_EQ(InValue::Type::STRING, a["string"][1].type());
}

TEST_F(CsvUtilsTest, read_csv_no_headers_from_path_nothrow){
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_no_headers_from_path(sample_no_headers_fname);
    });
}

TEST_F(CsvUtilsTest, read_csv_with_headers_from_path_nothrow) {
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_with_headers_from_path(sample_with_headers_fname);
    });
}

TEST_F(CsvUtilsTest, read_csv_double_matrix_no_headers_nothrow){
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_double_matrix(sample_for_matrix_no_headers);
    });
}

TEST_F(CsvUtilsTest, read_csv_double_matrix_no_headers){
  auto a = CsvReader::read_csv_double_matrix(sample_for_matrix_no_headers);
  dlib::matrix<double> b(2,3);
  b = 1,3.0,2.5, -30,-3e-3,3.43;
  EXPECT_EQ(true, a==b);
}
TEST_F(CsvUtilsTest, read_csv_double_matrix_with_headers_nothrow){
  std::vector<string> asd;
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_double_matrix(sample_for_matrix_with_headers, &asd);
    });
}

TEST_F(CsvUtilsTest, read_csv_double_matrix_with_headers){
  std::vector<string> asd;
  auto a = CsvReader::read_csv_double_matrix(sample_for_matrix_with_headers, &asd);
  std::vector<string> bsd = {"col1","col2","col3"};
  EXPECT_EQ_VECTORS(bsd,asd);
  dlib::matrix<double> b(2,3);
  b = 1,3.0,2.5, -30,-3e-3,3.43;
  EXPECT_EQ(true, a==b);
}
TEST_F(CsvUtilsTest, read_csv_double_matrix_no_headers_from_file_nothrow){
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_double_matrix_from_path(sample_for_matrix_no_headers_fname);
    });
}

TEST_F(CsvUtilsTest, read_csv_double_matrix_with_headers_from_file_nothrow){
  std::vector<string> asd;
  ASSERT_NO_THROW({
      auto a = CsvReader::read_csv_double_matrix_from_path(sample_for_matrix_with_headers_fname,&asd);
    });
}
