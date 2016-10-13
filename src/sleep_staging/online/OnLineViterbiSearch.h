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


/**
 * This class implements a 'step-by-step' version of the Viterbi algorithm
 * used for smoothing the hypnogram as a post-processing stage of the online
 * staging algorithm.
 *
 * Each call to the step method results in returning the most probable sequence
 * of sleep stages up to this point.
 *
 * Calling the stop method causes to take the probabilities of the final state
 * into account
 */ 
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
/**
 * The constructor.
 *
 * @param states : the vector of states; only integer states are supported at the moment
 * @param start_probabilities : a 1-D matrix containing the probabilities that
 the state machines start stage is the i-th state of the model.
 Can be used to force the first part of the hypnogram to point to 'Awake' stage.
 * @param final_probabilities : same as above but refers to the last element of
 the sequence. Can be used to force the last element of the hypnogram 
 to be the 'Awake' sequence
 * @param transition_matrix : the matrix giving the probabilities of transitions
 * between the states.
 * @param viterbi_weight : parameter controlling how much weight should this
 step have. For low values, the emission probabilities have much larger impact
 than the transition matrix, thus making the hypnogram more 'dynamic'. Values
 greater than 1 make the transition matrix more and more important thus making
 the hypnogram look more smooth.
 */ 
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
