#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "OnLineViterbiSearch.h"

void print_sequence(const std::vector<int> seq) {
	for (auto e : seq) {
		std::cout << e << '\t';
	}
	std::cout << std::endl;
}

TEST(OnLineViterbiSearchTest, basic_test1) {
	std::vector<int> states({0, -1, -3, -4});
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

	for (int i = 0; i != emission_p.nr(); ++i) {
		viterbi.step(dlib::trans(dlib::rowm(emission_p, i)));
		auto seq = viterbi.best_sequence();
		std::cout << "size: " << seq.size() << std::endl;
		print_sequence(seq);
	}

	viterbi.stop();
    auto seq = viterbi.best_sequence();
	std::cout << "size: " << seq.size() << std::endl;
	print_sequence(seq);
}
