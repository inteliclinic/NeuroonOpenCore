/*
 * OnlineStagingAlgorithm.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlineStagingAlgorithm.h"

OnlineStagingAlgorithm::OnlineStagingAlgorithm() {
	// TODO Auto-generated constructor stub

}

OnlineStagingAlgorithm::~OnlineStagingAlgorithm() {
	// TODO Auto-generated destructor stub
}

void OnlineStagingAlgorithm::reset_state() {

}

void OnlineStagingAlgorithm::process_input(const NeuroonSignals & input) {
	//TODO:
	// 1. Check number of EEG samples from last algorithm run
	// 2. Check number of IR samples from last algorithm run
	// 3. If enhough of both EEG and IR, update the index of current computation
	//		and run the internet
}

void OnlineStagingAlgorithm::stop() {

}
