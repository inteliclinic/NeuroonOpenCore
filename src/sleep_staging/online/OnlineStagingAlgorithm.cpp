/*
 * OnlineStagingAlgorithm.cpp
 *
 *  Created on: Aug 23, 2016
 *      Author: tomek
 */

#include "OnlineStagingAlgorithm.h"
#include "MlpClassifier.h"
#include <vector>



OnlineStagingAlgorithm::OnlineStagingAlgorithm() {

}

OnlineStagingAlgorithm::~OnlineStagingAlgorithm() {

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
