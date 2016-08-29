/*
 * ViterbiAlgorithm.h
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#ifndef SRC_SLEEP_STAGING_ONLINEVITERBISEARCH_H_
#define SRC_SLEEP_STAGING_ONLINEVITERBISEARCH_H_

#include <vector>
#include <dlib/matrix.h>
#include <limits>
#include <utility>

class OnLineViterbiSearch {

struct PathElement {
	double log_prob;
	int previous_state;
	int state;

	PathElement() {
		state = -1;
		previous_state = -1;
		log_prob = -std::numeric_limits<double>::infinity();
	}

	bool operator < (const PathElement& other) const {
		return log_prob < other.log_prob;
	}
};

public:
	OnLineViterbiSearch(const std::vector<int>& states, const dlib::matrix<double>& start_probabilities,
				  const dlib::matrix<double>& final_probabilities, const dlib::matrix<double> transition_matrix,
				  double viterbi_weight = 1
				  );

	void step(const dlib::matrix<double>& emission_probabilities);
	void stop();

	std::vector<int> best_sequence() const;
	double log_prob();

private:
	std::pair<int, double> find_path_leading_here(int next_state) const;
	int most_probable_final() const;
	void appendNewStep();
	void print_path_matrix() const; //for debug only

	std::vector<std::vector<PathElement>> m_paths;
	int m_current_step;
	double m_viterbi_weight;

	std::vector<int> m_states;
	dlib::matrix<double> m_start_p;
	dlib::matrix<double> m_final_p;
	dlib::matrix<double> m_transition_matrix;

	const int INVALID_STATE_INDEX = -1;
};

#endif /* SRC_SLEEP_STAGING_ONLINEVITERBISEARCH_H_ */
