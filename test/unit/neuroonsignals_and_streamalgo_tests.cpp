#include "../src/SignalSimulator.h"
#include "test_utils.h"
#include "../src/DataSink.h"

#include <gtest/gtest.h>
#include <chrono>
#include "../src/SignalSimulator.h"
#include "test_utils.h"
#include "../src/DataSink.h"
#include "../src/NeuroonSignals.h"

#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include <thread>
#include <sstream>
#include <stdio.h>
#include <random>
#include <algorithm>

struct NeuroonSignalsAndStreamAlgoTests: public ::testing::Test {

  // ------------ CSV FRAMES SOURCES -------------------

  // this file should contain values from 0 to 249
  const std::string sample_csv1 = "../test/test_data/sample1.csv";

  // this file should contain values from 0 to 499 as column under header signal
  const std::string sample_csv2 = "../test/test_data/sample2.csv";

  std::unique_ptr<EegFramesSource> eeg_source_sample1;
  std::unique_ptr<EegFramesSource> eeg_source_sample2;
  std::unique_ptr<PatFramesSource> irled_source_sample2;

  template<class T>
  LambdaSignalFrameDataSink<T> accumulate_to_vector_sink(std::vector<T> & out){
    return LambdaSignalFrameDataSink<T>([&out] (T data) { out.push_back(data); });
  }


  // ------------ GOOGLE TEST'S ------------------------

  virtual void SetUp(){
    eeg_source_sample1 = std::unique_ptr<EegFramesSource>(new EegFramesSource(sample_csv1,0));
    eeg_source_sample2 = std::unique_ptr<EegFramesSource>(new EegFramesSource(sample_csv2,"signal"));
	}


	void TearDown() {
	}

};


TEST_F(NeuroonSignalsAndStreamAlgoTests, NeuroonSignalsSimpleEegConsume) {

  std::vector<EegFrame> frames = {};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-15, 15);

  NeuroonSignals ns;

  const int N = 100;
  for(std::size_t i=0;i<N;i++){
    EegFrame ef;
    ef.timestamp = i * EegFrame::DefaultEmissionInterval_ms + dis(gen);
    for(std::size_t j=0; j< EegFrame::Length; j++){
      ef.signal[j] = i+j;
    }

    auto ts = ef.timestamp + std::max(static_cast<std::size_t>(0), ef.Length - 1) * ns.specs(SignalOrigin::EEG).ms_per_sample();
    ns.consume(std::make_shared<EegFrame>(ef));

    EXPECT_EQ(ts, ns.last_timestamp(SignalOrigin::EEG));
  }

  for(std::size_t i=0;i<N;i++){
    for(std::size_t j=0; j< EegFrame::Length; j++){
      EXPECT_EQ(i+j, ns.eeg_signal()[EegFrame::Length*i + j]);
    }
  }

}


TEST_F(NeuroonSignalsAndStreamAlgoTests, NeuroonSignalsPatFrameConsume) {

  std::vector<PatFrame> frames = {};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(-15, 15);

  NeuroonSignals ns;

  const int N = 100;
  for(std::int16_t i = 0; i < N; i++){
    PatFrame ef;

    ef.timestamp = i * PatFrame::DefaultEmissionInterval_ms + dis(gen);

    ef.ir_led = i;
    ef.red_led = i+1;
    ef.accel_axes.x = i+2;
    ef.accel_axes.y = i+3;
    ef.accel_axes.z = i+4;
    ef.temperature[0] = 11;
    ef.temperature[1] = 12;

    auto ts = ef.timestamp;
    ns.consume(std::make_shared<PatFrame>(ef));

    EXPECT_EQ(ts, ns.last_timestamp(SignalOrigin::IR_LED));
    EXPECT_EQ(ts, ns.last_timestamp(SignalOrigin::RED_LED));
    EXPECT_EQ(ts, ns.last_timestamp(SignalOrigin::ACCELEROMETER));
    EXPECT_EQ(ts, ns.last_timestamp(SignalOrigin::TEMPERATURE));
  }

  for(std::size_t i=0;i<N;i++){
      EXPECT_EQ(i, ns.ir_led_signal()[i]);
      EXPECT_EQ(i+1, ns.red_led_signal()[i]);
      EXPECT_EQ(i+2, ns.accel_axes_signal()[i].x);
      EXPECT_EQ(i+3, ns.accel_axes_signal()[i].y);
      EXPECT_EQ(i+4, ns.accel_axes_signal()[i].z);
      EXPECT_EQ(12, ns.temperature_signal()[i]);
  }

}
// TEST_F(NeuroonSignalsAndStreamAlgoTests, TrivialSinkTest) {

//   std::vector<int> v = {};
//   auto sink = accumulate_to_vector_sink(v);
//   for(int i=0; i< 5 ; i++){
//     sink.consume(i);
//   }
//   std::vector<int> expected = {0,1,2,3,4};

//   EXPECT_EQ_VECTORS(expected,v);
// }
