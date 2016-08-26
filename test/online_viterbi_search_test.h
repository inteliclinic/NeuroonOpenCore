#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "OnLineViterbiSearch.h"
#include <cstdlib>

void print_sequence(const std::vector<int> seq) {
	for (auto e : seq) {
		std::cout << e << ' ';
	}
	std::cout << std::endl;
}

TEST(OnLineViterbiSearchTest, basic_test1) {
	std::vector<int> states({0, 1, 2, 3});
	dlib::matrix<double> start_p(states.size(), 1);
	dlib::set_all_elements(start_p, 0);
	start_p(1, 0) = 1;

	dlib::matrix<double> final_p(states.size(), 1);
	dlib::set_all_elements(final_p, 0);
	final_p(1, 0) = 1;

	dlib::matrix<double> transition_matrix(states.size(), states.size());
	dlib::set_all_elements(transition_matrix, 0.25);

	OnLineViterbiSearch viterbi(states, start_p, final_p, transition_matrix);

	dlib::matrix<double> emission_p(10, states.size());
	dlib::set_all_elements(emission_p, 0.25);

	dlib::set_subm(emission_p, 2, 0, 4, 4) = 0;
	dlib::set_subm(emission_p, 2, 2, 4, 1) = 1;

	int i = 0;
	for (; i != emission_p.nr(); ++i) {
		viterbi.step(dlib::trans(dlib::rowm(emission_p, i)));
		auto seq = viterbi.best_sequence();
		EXPECT_EQ(seq.size(), i+1);
		std::cout << "size: " << seq.size() << std::endl;
		print_sequence(seq);
	}

	viterbi.stop();
    auto seq = viterbi.best_sequence();
    EXPECT_EQ(seq.size(), i);

    EXPECT_EQ(seq.front(), 1); // start probability 1 for the state '1'
    EXPECT_EQ(seq.back(), 1); // final probability 1 for the state '1'

	std::cout << "size: " << seq.size() << std::endl;
	print_sequence(seq);
}

/*
 * This test simulates an experiment with a fair and loaded dice. The fair dice has equal probabilities
 * of each outcome. The loaded one always shows the '0' outcome. The algorithm should be able
 * to distinguish the moment when the fair dice is changed to the loaded one, so it should
 * start returning 1 (loaded state) in the second part of the sequence.
 *
 */
TEST(OnLineViterbiSearch, big_viterbi_test) {
	std::vector<int> states({0, 1});

	dlib::matrix<double> emissions(6, 2);
	dlib::set_colm(emissions, 0) = 1. / 6.;
	dlib::set_colm(emissions, 1) = 0;
	emissions(0, 1) = 1;

	dlib::matrix<double> transition_m(2,2);
	dlib::set_all_elements(transition_m, 0.15);
	transition_m(0, 0) = 0.85;
	transition_m(1, 1) = 0.85;

	srand(42);
	const int n_samples = 200;

	dlib::matrix<double> start_p(6, 1);
	dlib::matrix<double> final_p(6, 1);
	dlib::set_all_elements(start_p, 1./6);
	dlib::set_all_elements(final_p, 1./6);

	OnLineViterbiSearch viterbi(states, start_p, final_p, transition_m);

	for (int i = 0; i != n_samples; ++i) {
		double random = std::rand() / double(RAND_MAX);
		int outcome = -1;
		if (i < n_samples / 2) {
			outcome = (int(random * 6)) % 6;
		} else {
			outcome = 0;
		}

		dlib::matrix<double> emit_for_observation = dlib::trans(dlib::rowm(emissions, outcome));
		viterbi.step(emit_for_observation);
	}
	viterbi.stop();
//	print_sequence(viterbi.best_sequence());
}
