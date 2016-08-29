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
#include "../src/FrameStreamPipe.h"

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

  std::shared_ptr<CsvEegFramesSource> eeg_source_sample1;
  std::shared_ptr<CsvEegFramesSource> eeg_source_sample2;
  std::shared_ptr<CsvAccelLedsTempFrameSource> irled_source_sample2;

  template<class T>
  LambdaSignalFrameDataSink<T> accumulate_to_vector_sink(std::vector<T> & out){
    return LambdaSignalFrameDataSink<T>([&out] (T & data) { out.push_back(data); });
  }


  // ------------ GOOGLE TEST'S ------------------------

  virtual void SetUp(){
    eeg_source_sample1 = std::unique_ptr<CsvEegFramesSource>(new CsvEegFramesSource(sample_csv1));
    eeg_source_sample2 = std::unique_ptr<CsvEegFramesSource>(new CsvEegFramesSource(sample_csv2));

	}


	void TearDown() {
	}

};


TEST_F(StreamingPipelineAndCsvSimulatorTests, SimpleCsvEegFrameSource1) {

  auto frames = eeg_source_sample1->get_frames();
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


TEST_F(StreamingPipelineAndCsvSimulatorTests, SimpleFrameStreamPipe) {

  auto frames = eeg_source_sample1->get_frames();
  auto frame_length = EegFrame::Length;

  std::vector<EegFrame> v = {};
  auto sink = accumulate_to_vector_sink(v);
  auto sink_sp = std::make_shared<LambdaSignalFrameDataSink<EegFrame>>(sink);

  auto pipe = FrameStreamPipe<EegFrame>(eeg_source_sample1, sink_sp);

  for(uint16_t i=0; i<250/frame_length; i++){
    // only full frames are created
    if(250 - i*frame_length >= frame_length){
      pipe.pass_next_frame_with_timestamp(i);
      EXPECT_EQ(v.size(),i+1);
      auto & passed = v[i];
      EXPECT_EQ(i, passed.timestamp);
      for(uint16_t j=0; j<frame_length;j++){
        EXPECT_EQ(frames[i].signal[j], passed.signal[j]);
      }
    }
  }

  EXPECT_TRUE(pipe.is_exhausted());

  sink_sp = nullptr;
  EXPECT_TRUE(pipe.is_broken());

}


TEST_F(StreamingPipelineAndCsvSimulatorTests, CsvSimSingleEegPipe_1020ms_normal_time) {

  auto frames = eeg_source_sample1->get_frames();
  auto frame_length = EegFrame::Length;

  std::vector<EegFrame> v = {};
  auto sink = accumulate_to_vector_sink(v);
  auto sink_sp = std::make_shared<LambdaSignalFrameDataSink<EegFrame>>(sink);

  auto pipe_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sample1, sink_sp));


  auto sim = CsvSignalSimulator();
  sim.add_streaming_pipe(pipe_up, EegFrame::DefaultEmissionInterval_ms);

  auto start = std::chrono::high_resolution_clock::now();
  int ms_to_pass = 1020;
  sim.pass_time(ms_to_pass);

  // std::cout << "Time to pass: " << ms_to_pass << "ms.";
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  // std::cout << " Passed: " << elapsed.count() << "ms.\n";

  ASSERT_TRUE(elapsed.count()>=ms_to_pass);

  std::vector<int> expected = {};

  for(uint16_t i=0; i<120/frame_length; i++){
    // only full frames are created
    if(250 - i*frame_length >= frame_length){
      auto & passed = eeg_source_sample1->get_frames()[i];
      auto & received = v[i];
      // EXPECT_EQ(i, passed.timestamp);
      for(uint16_t j=0; j<frame_length;j++){
        EXPECT_EQ(passed.signal[j], received.signal[j]);
      }
    }
  }

  // pass rest of frames instantly
  sim.pass_time(0,0);
  for(uint16_t i=120/frame_length; i<250/frame_length; i++){
    // only full frames are created
    if(250 - i*frame_length >= frame_length){
      auto & passed = eeg_source_sample1->get_frames()[i];
      auto & received = v[i];
      // EXPECT_EQ(i, passed.timestamp);
      for(uint16_t j=0; j<frame_length;j++){
        EXPECT_EQ(passed.signal[j], received.signal[j]);
      }
    }
  }

  // EXPECT_TRUE(pipe.is_exhausted());

  // sink_sp = nullptr;
  // EXPECT_TRUE(pipe.is_broken());

}

TEST_F(StreamingPipelineAndCsvSimulatorTests, CsvSimTwoEegPipeSingleSink_instant) {

  auto frames = eeg_source_sample1->get_frames();

  std::vector<EegFrame> v = {};
  auto sink = accumulate_to_vector_sink(v);
  auto sink_sp = std::make_shared<LambdaSignalFrameDataSink<EegFrame>>(sink);

  auto pipe_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sample1, sink_sp));
  auto pipe2_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sample2, sink_sp));

  auto pipe_cheat_rp = pipe_up.get();
  auto pipe2_cheat_rp = pipe2_up.get();

  auto sim = CsvSignalSimulator();
  sim.add_streaming_pipe(pipe_up, EegFrame::DefaultEmissionInterval_ms);
  sim.add_streaming_pipe(pipe2_up, EegFrame::DefaultEmissionInterval_ms);

  int ms_to_pass = 1985;

  sim.pass_time(ms_to_pass, 0);
  EXPECT_EQ(true,pipe_cheat_rp->is_exhausted());
  EXPECT_EQ(false,pipe2_cheat_rp->is_exhausted());

  EXPECT_EQ(62, v.size());
  sim.pass_time(0, 0);
  EXPECT_EQ(true,pipe2_cheat_rp->is_exhausted());
  EXPECT_EQ(93, v.size());
}

TEST_F(StreamingPipelineAndCsvSimulatorTests, CsvSimTwoEegPipeSingleSource_instant) {

  auto frames = eeg_source_sample1->get_frames();

  std::size_t frame_count = 0;
  llong frame_sum = 0;
  auto counting_sink_sp = std::make_shared<LambdaSignalFrameDataSink<EegFrame>>(
                                                                                LambdaSignalFrameDataSink<EegFrame>([&frame_count] (EegFrame&) {frame_count+=1;}));

  auto suming_sink_sp = std::make_shared<LambdaSignalFrameDataSink<EegFrame>>(
                                                                              LambdaSignalFrameDataSink<EegFrame>([&frame_sum] (EegFrame& f) { for(auto v: f.signal){frame_sum+=v;}}));
  auto pipe_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sample1, counting_sink_sp));
  auto pipe2_up = std::unique_ptr<IFrameStreamPipe>(new FrameStreamPipe<EegFrame>(eeg_source_sample1, suming_sink_sp));

  auto pipe_cheat_rp = pipe_up.get();
  auto pipe2_cheat_rp = pipe2_up.get();

  auto sim = CsvSignalSimulator();
  sim.add_streaming_pipe(pipe_up, EegFrame::DefaultEmissionInterval_ms);
  sim.add_streaming_pipe(pipe2_up, EegFrame::DefaultEmissionInterval_ms);

  sim.pass_time(0, 0);

  EXPECT_EQ(250/EegFrame::Length, frame_count);

  llong expected_sum = 0;
  for(ullong i=0; i<EegFrame::Length*(250/EegFrame::Length); i++){
    expected_sum += i;
  }
  EXPECT_EQ(expected_sum, frame_sum);

  EXPECT_EQ(true,pipe_cheat_rp->is_exhausted());
  EXPECT_EQ(true,pipe2_cheat_rp->is_exhausted());
}
// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_1020ms_normal_time) {

//   auto sim = CsvSignalSimulator();
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

//   auto sim = CsvSignalSimulator();
//   sim.sources.push_back(eeg_source_sample1.get());
//   sim.sinks.push_back(&_frames_to_vector_sink);
//   sim.pass_time(0,0);

//   std::vector<int> expected = {};
//   for(int i=0;i<250;i++){ expected.push_back(i);}
//   EXPECT_EQ_VECTORS(_sink_vector,expected);
// }

// TEST_F(CsvSourceAndSimulatorTests, csv_simulator_pass_entire_double_source_instant) {

//   auto sim = CsvSignalSimulator();
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

//   auto sim = CsvSignalSimulator();
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
