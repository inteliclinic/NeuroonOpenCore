/*
 * ViterbiAlgorithm.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: tomek
 */

#include "OnLineViterbiSearch.h"
#include <algorithm>
#include <cassert>

OnLineViterbiSearch::OnLineViterbiSearch(const std::vector<int>& states, const dlib::matrix<double>& start_probabilities,
				     	 	 	 	 	 const dlib::matrix<double>& final_probabilities, const dlib::matrix<double> transition_matrix)
: m_states(states),
  m_start_p(start_probabilities),
  m_final_p(final_probabilities),
  m_transition_matrix(transition_matrix)
{
	m_current_step = -1;
}

void OnLineViterbiSearch::appendNewStep() {
	m_paths.push_back(std::vector<OnLineViterbiSearch::PathElement>(m_states.size()));
	++m_current_step;
}

void OnLineViterbiSearch::print_path_matrix() {
	for (int i = 0; i != m_paths.size(); ++i) {
		for (int j = 0; j != m_states.size(); ++j) {
			OnLineViterbiSearch::PathElement e = m_paths[i][j];
			std::cout << e.previous_state << "->" << e.state <<": " << e.log_prob << '\t';
		}
		std::cout << std::endl;
	}
}

void OnLineViterbiSearch::step(const dlib::matrix<double>& emission_probabilities) {
	appendNewStep();

	std::cout << "step: " << m_current_step << "emissions: " << emission_probabilities << std::endl;
	assert(dlib::sum(emission_probabilities) != 0);

	for (int state = 0; state != m_states.size(); ++state) {
		m_paths[m_current_step][state].state = state;

		double emit_p = emission_probabilities(state, 0);
		if (m_current_step == 0) {
			m_paths[m_current_step][state].log_prob = std::log(emit_p) + std::log(m_start_p(state, 0));
			m_paths[m_current_step][state].previous_state = INVALID_STATE_INDEX;
			continue;
		}

		OnLineViterbiSearch::PathElement previous = find_path_leading_here(state);
		m_paths[m_current_step][state].log_prob = previous.log_prob + std::log(emit_p);
		m_paths[m_current_step][state].previous_state = previous.state;

		std::cout << "step: " << m_current_step << ", state: " << state << "," << m_paths[m_current_step][state].state
				  << ", previous state: " << m_paths[m_current_step][state].previous_state << std::endl;
	}

	print_path_matrix();
}

OnLineViterbiSearch::PathElement OnLineViterbiSearch::find_path_leading_here(int next_state) const {
	OnLineViterbiSearch::PathElement result;
	for (int state = 0; state != m_states.size(); ++state) {
		double log_prob =  m_paths[m_current_step - 1][state].log_prob + m_transition_matrix(state, next_state);
		if (log_prob > result.log_prob) {
			result = m_paths[m_current_step - 1][state];
		}
	}

	return result;
}

void OnLineViterbiSearch::stop() {
	for (int state = 0; state != m_states.size(); ++state) {
		m_paths[m_current_step][state].log_prob += std::log(m_final_p(state, 0));
	}

	print_path_matrix();
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
		best_sequence.push_back(state);
		state = m_paths[step][state].previous_state;
		--step;
	}
	std::reverse(best_sequence.begin(), best_sequence.end());
	return best_sequence;
}
