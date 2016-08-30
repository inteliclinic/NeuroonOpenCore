/*
 * ViterbiAlgorithm.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#include "OnLineViterbiSearch.h"
#include <algorithm>
#include <cassert>
#include "logger.h"
OnLineViterbiSearch::OnLineViterbiSearch(const std::vector<int>& states, const dlib::matrix<double>& start_probabilities,
				     	 	 	 	 	 const dlib::matrix<double>& final_probabilities, const dlib::matrix<double> transition_matrix,
										 double viterbi_weight)
: m_states(states),
  m_start_p(start_probabilities),
  m_final_p(final_probabilities),
  m_transition_matrix(transition_matrix),
  m_viterbi_weight(viterbi_weight)
{
	m_current_step = -1;

	m_transition_matrix = dlib::pow(m_transition_matrix, viterbi_weight);
}

void OnLineViterbiSearch::appendNewStep() {
	m_paths.push_back(std::vector<OnLineViterbiSearch::PathElement>(m_states.size()));
	++m_current_step;
}

// debug only
void OnLineViterbiSearch::print_path_matrix() const {
	for (int i = 0; i != m_paths.size(); ++i) {
		std::cout << i << ". ";
		for (int j = 0; j != m_states.size(); ++j) {
			OnLineViterbiSearch::PathElement e = m_paths[i][j];
			std::cout << e.previous_state << "->" << e.state <<": " << e.log_prob << '\t';
		}
		std::cout << std::endl;
	}
}

void OnLineViterbiSearch::step(const dlib::matrix<double>& emission_probabilities) {

	assert(dlib::is_finite(emission_probabilities));
	LOG(INFO) << emission_probabilities;
	appendNewStep();

	assert(dlib::sum(emission_probabilities) != 0);

	for (int state = 0; state != m_states.size(); ++state) {
		m_paths[m_current_step][state].state = state;

		double emit_p = emission_probabilities(state, 0);
		if (m_current_step == 0) {
			m_paths[m_current_step][state].log_prob = std::log(emit_p) + std::log(m_start_p(state, 0));
			m_paths[m_current_step][state].previous_state = INVALID_STATE_INDEX;
			continue;
		}

		std::pair<int, double> origin = find_path_leading_here(state);
		int previous_state = origin.first;
		double log_prob = origin.second;
		m_paths[m_current_step][state].log_prob = log_prob + std::log(emit_p);
		m_paths[m_current_step][state].previous_state = previous_state;

//		std::cout << "step: " << m_current_step << ", state: " << state << "," << m_paths[m_current_step][state].state
//				  << ", previous state: " << m_paths[m_current_step][state].previous_state << std::endl;
	}

	//print_path_matrix();
}

std::pair<int, double> OnLineViterbiSearch::find_path_leading_here(int next_state) const {
	int result_state = -1;
	double best_log_prob = -std::numeric_limits<double>::infinity();
	for (int state = 0; state != m_states.size(); ++state) {
		double log_prob =  m_paths[m_current_step - 1][state].log_prob + std::log(m_transition_matrix(state, next_state));
		if (log_prob > best_log_prob) {
			best_log_prob = log_prob;
			result_state = state;
		}
	}

	return std::make_pair(result_state, best_log_prob);
}

void OnLineViterbiSearch::stop() {
	if (m_current_step == INVALID_STATE_INDEX) {
		LOG(WARNING) << "Tried to stop a Viterbi search without executing any steps";
		return;
	}

	for (int state = 0; state != m_states.size(); ++state) {
		m_paths[m_current_step][state].log_prob += std::log(m_final_p(state, 0));
	}

	//print_path_matrix();
}

int OnLineViterbiSearch::most_probable_final() const {
	auto it_to_max = std::max_element(m_paths[m_current_step].begin(), m_paths[m_current_step].end());
	int index_of_max= std::distance(m_paths[m_current_step].begin(), it_to_max);
	return index_of_max;
}

std::vector<int> OnLineViterbiSearch::best_sequence() const {
	std::vector<int> best_sequence;

	int state = most_probable_final();
	int step = m_current_step;
	while (state != INVALID_STATE_INDEX) {
		best_sequence.push_back(m_states[state]);
		state = m_paths[step][state].previous_state;
		--step;
	}
	std::reverse(best_sequence.begin(), best_sequence.end());
	return best_sequence;
}
