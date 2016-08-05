#include "MultilayerPerceptron.h"
#include <gtest/gtest.h>
#include "x_cube_neural_network.h"
#include <dlib/matrix.h>
#include <iostream>

TEST(MultilayerPerceptronTest, basic_predict_test1) {
	dlib::matrix<double> input(10, 2);
	dlib::set_all_elements(input, 99);

	dlib::matrix<double> W1(2, 100);
	dlib::set_all_elements(W1, 0);
	dlib::matrix<double> W2(100, 1);
	dlib::set_all_elements(W2, 0);

	dlib::matrix<double> I1(1, 100);
	dlib::set_all_elements(I1, 0);
	dlib::matrix<double> I2(1, 1);
	dlib::set_all_elements(I2, 42);

	std::vector<dlib::matrix<double>> weights({W1, W2});
	std::vector<dlib::matrix<double>> intercepts({I1, I2});
	MultilayerPerceptron mlp(weights, intercepts);

	dlib::matrix<double> output = mlp.predict(input);

	for (int i = 0; i != output.nr(); ++i) {
		EXPECT_FLOAT_EQ(output(i, 0), 42);
	}
}

TEST(MultilayerPerceptronTest, basic_predict_test2) {
	dlib::matrix<double> input(3, 1);
	input(0, 0) = 1;
	input(1, 0) = -1;
	input(2, 0) = 0;

	dlib::matrix<double> W1(1, 2);
	dlib::set_all_elements(W1, 0);
	W1(0, 0) = 1;
	W1(0, 1) = -0.5;

	dlib::matrix<double> W2(2, 1);
	dlib::set_all_elements(W2, 0);
	W2(0, 0) = 1;
	W2(1, 0) = 1;

	dlib::matrix<double> I1(1, 2);
	dlib::set_all_elements(I1, 0);
	dlib::matrix<double> I2(1, 1);
	dlib::set_all_elements(I2, 0);

	std::vector<dlib::matrix<double>> weights({W1, W2});
	std::vector<dlib::matrix<double>> intercepts({I1, I2});
	MultilayerPerceptron mlp(weights, intercepts);

	dlib::matrix<double> output = mlp.predict(input);
	std::cout << output;

	EXPECT_FLOAT_EQ(output(0, 0), 1);
	EXPECT_FLOAT_EQ(output(1, 0), 0.5);
	EXPECT_FLOAT_EQ(output(2, 0), 0);
}

TEST(MultilayerPerceptronTest, x_cube_network_test) {
	MultilayerPerceptron* mlp = get_xcube_mlp();
	dlib::matrix<double> input = dlib::trans(dlib::linspace(-10, 10, 1000));
	dlib::matrix<double> input_cubed = dlib::pow(input, 3);
	dlib::matrix<double> output = mlp->predict(input);
	dlib::matrix<double> residuals = output - input_cubed;

	//std::cout << residuals;
	for (int i = 0; i != residuals.size(); ++i) {
		bool relative_less_than_001 = residuals(i,0) / input_cubed(i,0) < 0.01;
		bool absolute_less_than_1 = residuals(i,0) / 1;

		ASSERT_TRUE(relative_less_than_001|| absolute_less_than_1);
	}

	delete mlp;

}
