#include "MlpClassifier.h"
#include <gtest/gtest.h>
#include <dlib/matrix.h>
#include <iostream>


/**
 * Creates an mlp classifier that was trained to classify as:
 * 	  3 for x < -1
 * 	  0 for -1 > x > 0
 * 	  1 for  0 > x > 2
 * 	  2 for  x > 2
 */

MlpClassifier* create_example_classifier() {
	std::vector<double> w1({2.03190858e+00,   2.94533598e+00,  -3.41956538e-07, -3.67823768e+00,   1.13395459e+00});
	dlib::matrix<double> W1 = vector_to_dlib_matrix(w1);
	W1 = dlib::reshape(W1, 1, 5);

	std::vector<double> i1({2.86251402, -2.7785396 , -0.63567161,  2.05278654,  3.02311063});
	dlib::matrix<double> I1 = vector_to_dlib_matrix(i1);
	I1 = dlib::reshape(I1, 5, 1);

	std::vector<double> w2({1.37242151e-001,   1.18938028e+000,   5.66438814e-001, -2.38229357e+000,
							-5.28775220e-001,  -2.93381516e+000,   2.07970147e+000, -6.43388925e-013,
							9.90769207e-317,  -9.89565851e-316,  -2.95743743e-318, -1.18297611e-219,
							7.23121051e-002,  -3.12703544e+000,  -6.82325935e-001, 1.46535489e+000,
							2.78865399e-001,   1.02105960e+000,  -3.24681672e-001, -1.83596616e+000});
	dlib::matrix<double> W2 = vector_to_dlib_matrix(w2);
	W2 = dlib::reshape(W2, 5, 4);

	std::vector<double> i2({0.73985231,  2.06724105, -2.12685892, -1.13602575});
	dlib::matrix<double> I2 = vector_to_dlib_matrix(i2);
	I2 = dlib::reshape(I2, 4, 1);

	std::vector<dlib::matrix<double>> weights({W1, W2});
	std::vector<dlib::matrix<double>> intercepts({I1, I2});
	MlpClassifier* classifier = new MlpClassifier(weights, intercepts);
	return classifier;
}


TEST(MlpClassifierTest, basic_predict_test) {
	MlpClassifier* classifier = create_example_classifier();
	dlib::matrix<double> input = dlib::trans(dlib::linspace(-5, 5, 100));

	dlib::matrix<int> output = classifier->predict(input);

	EXPECT_EQ(output.nr(), input.nr());
	EXPECT_EQ(output.nc(), 1);

	for (int i = 0; i != output.nr(); ++i) {
		double x = input(i, 0);
		double y = output(i, 0);
		if (x < -1) {
			EXPECT_FLOAT_EQ(y, 3);
		} else if (x < 0 && x > -1) {
			EXPECT_FLOAT_EQ(y, 0);
		} else if (x < 2 && x > 0) {
			EXPECT_FLOAT_EQ(y, 1);
		} else if (x > 2) {
			EXPECT_FLOAT_EQ(y, 2);
		}
	}

	//std::cout << output;

	delete classifier;
}
