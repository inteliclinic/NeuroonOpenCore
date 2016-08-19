#include "../src/CsvSignalSimulator.h"
#include "test_utils.h"
#include "../src/DataSink.h"

#include <gtest/gtest.h>
/* #include <vector> */
/* #include <memory> */
/* #include <functional> */
/* #include <numeric> */
/* #include <algorithm> */
#include <chrono>
#include "../src/CsvSignalSimulator.h"
#include "test_utils.h"
#include "../src/DataSink.h"
#include "../src/NeuroonSignals.h"

#include <gtest/gtest.h>
/* #include <vector> */
#include <memory>
/* #include <functional> */
/* #include <numeric> */
/* #include <algorithm> */
#include <chrono>
#include <thread>
#include <sstream>
#include <stdio.h>

struct CsvSourceAndSimulatorTests : public ::testing::Test {

  // this file should contain values from 0 to 249
  const std::string sample_csv1 = "../test/test_data/sample1.csv";

  // this file should contain values from 0 to 499 as column under header signal
  const std::string sample_csv2 = "../test/test_data/sample2.csv";

  std::unique_ptr<CsvSignalSource> eeg_source_sample1;
  std::unique_ptr<CsvSignalSource> eeg_source_sample2;
  std::unique_ptr<CsvSignalSource> irled_source_sample2;

  LambdaSignalFrameDataSink _frames_to_vector_sink = LambdaSignalFrameDataSink([=](SignalFrame* sfp){
      // printf("Consuming frame: ");
      for(auto s : sfp->signal){
        // printf("%d ",s);
        this->_sink_vector.push_back(s);
      }
      // printf("\n");
    });

  LambdaSignalFrameDataSink _frames_to_vector_split_sink = LambdaSignalFrameDataSink([=](SignalFrame* sfp){
      // printf("Consuming frame: ");
      // std::string fname = sfp->origin == SignalOrigin::EEG ? "eeg" : "irled";
      // printf("%s",fname.c_str());
      for(auto s : sfp->signal){
        // printf("%d ",s);
        auto & v = (sfp->origin == SignalOrigin::EEG ? _eeg_sink_vector : _irled_sink_vector);
        v.push_back(s);
      }
      // printf("\n");
    });
  std::vector<int> _sink_vector= {};
  std::vector<int> _eeg_sink_vector= {};
  std::vector<int> _irled_sink_vector= {};

  virtual void SetUp(){
    eeg_source_sample1 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv1,NeuroonSignals::specs(SignalOrigin::EEG), 60));
    eeg_source_sample2 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv2,NeuroonSignals::specs(SignalOrigin::EEG), 60));
    irled_source_sample2 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv2,NeuroonSignals::specs(SignalOrigin::IR_LED), 150));
	}


	void TearDown() {
    _sink_vector.clear();
    _eeg_sink_vector.clear();
    _irled_sink_vector.clear();
	}

};

TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_1020ms_normal_time) {

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample1.get());
  sim.sinks.push_back(&_frames_to_vector_sink);
  auto start = std::chrono::high_resolution_clock::now();
  int ms_to_pass = 1020;
  sim.pass_time(ms_to_pass);
  // std::cout << "Time to pass: " << ms_to_pass << "ms.";
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  // std::cout << " Passed: " << elapsed.count() << "ms.\n";

  ASSERT_TRUE(elapsed.count()>=ms_to_pass);

  std::vector<int> expected = {};
  for(int i=0;i<127;i++){ expected.push_back(i);}
  EXPECT_EQ_VECTORS(_sink_vector,expected);
}

TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_entire_instant) {

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample1.get());
  sim.sinks.push_back(&_frames_to_vector_sink);
  sim.pass_time(0,0);

  std::vector<int> expected = {};
  for(int i=0;i<250;i++){ expected.push_back(i);}
  EXPECT_EQ_VECTORS(_sink_vector,expected);
}

TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_entire_double_source_instant) {

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample2.get());
  sim.sources.push_back(irled_source_sample2.get());
  sim.sinks.push_back(&_frames_to_vector_split_sink);
  sim.pass_time(0,0);

  std::vector<int> expected = {};
  for(int i=0;i<500;i++){ expected.push_back(i);}
  // printf("eegsize: %zu irled_size %zu", _eeg_sink_vector.size(), _irled_sink_vector.size());
  EXPECT_EQ_VECTORS(expected,_eeg_sink_vector);
  EXPECT_EQ_VECTORS(expected,_irled_sink_vector);
}

TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_1001ms_and_then_500_ms_double_source_instant) {

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample2.get());
  sim.sources.push_back(irled_source_sample2.get());
  sim.sinks.push_back(&_frames_to_vector_split_sink);
  sim.pass_time(1001,0);

  std::vector<int> expected1 = {};
  std::vector<int> expected2 = {};
  for(int i=0;i<120;i++){ expected1.push_back(i);}
  for(int i=0;i<22;i++){ expected2.push_back(i);}
  EXPECT_EQ_VECTORS(expected1,_eeg_sink_vector);
  EXPECT_EQ_VECTORS(expected2,_irled_sink_vector);

  sim.pass_time(500,0);
  for(int i=120;i<187;i++){ expected1.push_back(i);}
  for(int i=22;i<37;i++){ expected2.push_back(i);}
  EXPECT_EQ_VECTORS(expected1,_eeg_sink_vector);
  EXPECT_EQ_VECTORS(expected2,_irled_sink_vector);
}
