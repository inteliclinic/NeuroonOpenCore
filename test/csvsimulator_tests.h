#include "../src/SignalSimulator.h"
#include "test_utils.h"
#include "../src/DataSink.h"

#include <gtest/gtest.h>
/* #include <vector> */
/* #include <memory> */
/* #include <functional> */
/* #include <numeric> */
/* #include <algorithm> */
#include <chrono>
#include "../src/SignalSimulator.h"
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

struct StreamingPipelineAndCsvSimulatorTests : public ::testing::Test {

  // ------------ CSV FRAMES SOURCES -------------------

  // this file should contain values from 0 to 249
  const std::string sample_csv1 = "../test/test_data/sample1.csv";

  // this file should contain values from 0 to 499 as column under header signal
  const std::string sample_csv2 = "../test/test_data/sample2.csv";

  std::unique_ptr<EegFramesSource> eeg_source_sample1;
  std::unique_ptr<EegFramesSource> eeg_source_sample2;
  std::unique_ptr<CsvAccelLedsTempFrameSource> irled_source_sample2;

  template<class T>
  LambdaSignalFrameDataSink<T> accumulate_to_vector_sink(std::vector<T> & out){
    return LambdaSignalFrameDataSink<T>([&out] (T & data) { out.push_back(data); });
  }


  // ------------ GOOGLE TEST'S ------------------------

  virtual void SetUp(){
    eeg_source_sample1 = std::unique_ptr<EegFramesSource>(new EegFramesSource(sample_csv1));
    eeg_source_sample2 = std::unique_ptr<EegFramesSource>(new EegFramesSource(sample_csv2));

	}


	void TearDown() {
	}

};


TEST_F(StreamingPipelineAndCsvSimulatorTests, SimpleCsvEegFrameSource1) {

  auto frames = eeg_source_sample1->get_values();
  auto frame_length = EegFrame::Length;

  EXPECT_TRUE(frames.size() > 0);

  for(uint16_t i=0; i<250; i+= frame_length){
    auto & f = frames[i/frame_length];
    if(250 - i >= frame_length)
      for(uint16_t j=0; j<frame_length;j++){
        EXPECT_EQ(i+j, f.signal[j]);
      }
  }

}

TEST_F(StreamingPipelineAndCsvSimulatorTests, TrivialSinkTest) {

  std::vector<int> v = {};
  auto sink = accumulate_to_vector_sink(v);
  for(int i=0; i< 5 ; i++){
    sink.consume(i);
  }
  std::vector<int> expected = {0,1,2,3,4};

  EXPECT_EQ_VECTORS(expected,v);
}
// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_1020ms_normal_time) {

//   auto sim = SignalSimulator();
//   sim.sources.push_back(eeg_source_sample1.get());
//   sim.sinks.push_back(&_frames_to_vector_sink);
//   auto start = std::chrono::high_resolution_clock::now();
//   int ms_to_pass = 1020;
//   sim.pass_time(ms_to_pass);
//   // std::cout << "Time to pass: " << ms_to_pass << "ms.";
//   auto end = std::chrono::high_resolution_clock::now();
//   std::chrono::duration<double, std::milli> elapsed = end-start;
//   // std::cout << " Passed: " << elapsed.count() << "ms.\n";

//   ASSERT_TRUE(elapsed.count()>=ms_to_pass);

//   std::vector<int> expected = {};
//   for(int i=0;i<127;i++){ expected.push_back(i);}
//   EXPECT_EQ_VECTORS(_sink_vector,expected);
// }

// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_entire_instant) {

//   auto sim = SignalSimulator();
//   sim.sources.push_back(eeg_source_sample1.get());
//   sim.sinks.push_back(&_frames_to_vector_sink);
//   sim.pass_time(0,0);

//   std::vector<int> expected = {};
//   for(int i=0;i<250;i++){ expected.push_back(i);}
//   EXPECT_EQ_VECTORS(_sink_vector,expected);
// }

// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_entire_double_source_instant) {

//   auto sim = SignalSimulator();
//   sim.sources.push_back(eeg_source_sample2.get());
//   sim.sources.push_back(irled_source_sample2.get());
//   sim.sinks.push_back(&_frames_to_vector_split_sink);
//   sim.pass_time(0,0);

//   std::vector<int> expected = {};
//   for(int i=0;i<500;i++){ expected.push_back(i);}
//   // printf("eegsize: %zu irled_size %zu", _eeg_sink_vector.size(), _irled_sink_vector.size());
//   EXPECT_EQ_VECTORS(expected,_eeg_sink_vector);
//   EXPECT_EQ_VECTORS(expected,_irled_sink_vector);
// }

// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_1001ms_and_then_500_ms_double_source_instant) {

//   auto sim = SignalSimulator();
//   sim.sources.push_back(eeg_source_sample2.get());
//   sim.sources.push_back(irled_source_sample2.get());
//   sim.sinks.push_back(&_frames_to_vector_split_sink);
//   sim.pass_time(1001,0);

//   std::vector<int> expected1 = {};
//   std::vector<int> expected2 = {};
//   for(int i=0;i<120;i++){ expected1.push_back(i);}
//   for(int i=0;i<22;i++){ expected2.push_back(i);}
//   EXPECT_EQ_VECTORS(expected1,_eeg_sink_vector);
//   EXPECT_EQ_VECTORS(expected2,_irled_sink_vector);

//   sim.pass_time(500,0);
//   for(int i=120;i<187;i++){ expected1.push_back(i);}
//   for(int i=22;i<37;i++){ expected2.push_back(i);}
//   EXPECT_EQ_VECTORS(expected1,_eeg_sink_vector);
//   EXPECT_EQ_VECTORS(expected2,_irled_sink_vector);
// }
