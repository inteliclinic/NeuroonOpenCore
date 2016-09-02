#ifndef __STREAMING_PIPELINE_TESTS__
#define __STREAMING_PIPELINE_TESTS__

#include "test_utils.h"
#include "DataSink.h"
#include "AlgCoreDaemon.h"
#include "CsvSignalSimulator.h"
#include "StreamingAlgorithm.h"

#include <memory>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

struct StreamingPipelineTests : public ::testing::Test {

  // this file should contain values from 0 to 249
  const std::string sample_csv1 = "../test/test_data/sample1.csv";

  // this file should contain values from 0 to 499 as column under header signal
  const std::string sample_csv2 = "../test/test_data/sample2.csv";

  std::unique_ptr<CsvSignalSource> eeg_source_sample1;
  std::unique_ptr<CsvSignalSource> eeg_source_sample2;
  std::unique_ptr<CsvSignalSource> irled_source_sample2;

  //-- dummy sink ------------------------
  class SumSink : public IDataSink<double>{
    double _sum;
  public:
    SumSink() : _sum(0.0) {}
    void consume(double d) override { _sum+=d; }
    double get_accumulator() const { return _sum; }
  };

  //-- dummy streaming algorithm ------------------------
  class IdentityStream : public SinkStreamingAlgorithm<double>{
    std::size_t _last_len = 0;
  public:
    IdentityStream(const std::vector<IDataSink<double>*> & sinks={}) : SinkStreamingAlgorithm(sinks) {
      printf("IdentityStream created.\n");
    }
    void reset_state() override { _last_len = 0; }
    void process_input(const NeuroonSignals & input) override{
      if (input.last_frame_origin() != SignalOrigin::EEG) return;
      auto e = input[SignalOrigin::EEG].end();
      /* printf("Zawartosc eeg\n"); */
      /* for(const auto & d : input[SignalOrigin::EEG]){ */
      /*   printf("%f ",d); */
      /* } */
      auto b = input[SignalOrigin::EEG].begin() + _last_len;
      std::size_t l=0;
      double suma = 0.0;
      printf("IdentityStream at %zu:\n",_last_len);
      for(; b != e; b++){
        suma += *b;
        printf("sample %f   sum %f\n", *b, suma);
        l++;
      }
      printf("\n");
      _last_len += l;
      feed_all_sinks(suma);
    }
    void end_streaming (const NeuroonSignals&) override {}
  };
  //------------------------

  virtual void SetUp(){
    eeg_source_sample1 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv1,NeuroonSignals::specs(SignalOrigin::EEG), 64));
    eeg_source_sample2 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv2,NeuroonSignals::specs(SignalOrigin::EEG), 64));
    irled_source_sample2 = std::unique_ptr<CsvSignalSource>(new CsvSignalSource(sample_csv2,NeuroonSignals::specs(SignalOrigin::IR_LED), 150));
	}

	void TearDown() {
	}

};

TEST_F(StreamingPipelineTests, double_sum_pipeline_test_instant) {

  AlgCoreDaemon d;
  SumSink sum_sink;
  auto algo = std::unique_ptr<IStreamingAlgorithm>(new IdentityStream({&sum_sink}));
  d.add_streaming_algorithms(algo);

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample1.get());
  sim.sources.push_back(irled_source_sample2.get());

  sim.sinks.push_back(&d);

  auto start = std::chrono::high_resolution_clock::now();
  int ms_to_pass = 0;
  d.start_processing();
  sim.pass_time(ms_to_pass,0);
  d.end_processing();
  // std::cout << "Time to pass: " << ms_to_pass << "ms.";
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  // std::cout << " Passed: " << elapsed.count() << "ms.\n";

  ASSERT_TRUE(elapsed.count()>=ms_to_pass);

  auto res = sum_sink.get_accumulator();

  double expected = 0.0;
  for(int i=0;i<250;i++){ expected+=(double)i; }
  EXPECT_EQ(expected, res);
}

TEST_F(StreamingPipelineTests, double_sum_pipeline_test_normal_time) {

  AlgCoreDaemon d;
  SumSink sum_sink;
  auto algo = std::unique_ptr<IStreamingAlgorithm>(new IdentityStream({&sum_sink}));
  d.add_streaming_algorithms(algo);

  auto sim = CsvSignalSimulator();
  sim.sources.push_back(eeg_source_sample1.get());
  /* sim.sources.push_back(irled_source_sample2.get()); */

  sim.sinks.push_back(&d);

  auto start = std::chrono::high_resolution_clock::now();
  int ms_to_pass = 1020;
  d.start_processing();
  sim.pass_time(ms_to_pass);
  d.end_processing();
  // std::cout << "Time to pass: " << ms_to_pass << "ms.";
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end-start;
  // std::cout << " Passed: " << elapsed.count() << "ms.\n";

  ASSERT_TRUE(elapsed.count()>=ms_to_pass);

  auto res = sum_sink.get_accumulator();

  double expected = 0.0;
  for(int i=0;i<120;i++){ expected+=(double)i; }
  EXPECT_EQ(expected, res);
}

#endif
