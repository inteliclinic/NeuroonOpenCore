#import "NeuroonStandaloneAlgorithmsApi.h"
#import "serialization_utils.h"
#include <cstring>

ncDayTimeInstant ncEstimateCrossoverTime(ncDayTimeInstant usual_wake_up_time) {
  // TODO prepare actual logic here
  auto &m = usual_wake_up_time.minute;
  auto &h = usual_wake_up_time.hour;

  m -= 20;
  if (m < 0)
    h--;
  h += m / 60;
  m = m % 60;
  if (m < 0)
    m += 60;
  h -= 2;
  h = h % 24;
  if (h < 0)
    h += 24;

  return {h, m};
}

ncDayTimeInstant
ncCalculateCrossoverPoint(ncDayTimeInstant current_crossover,
                          ncCircadianRhythmEvent *new_biorhythm_events,
                          unsigned long events_count) {
  // TODO prepare actual logic here
  int m = current_crossover.minute;
  int h = current_crossover.hour;

  m += events_count;
  h += m / 60;
  m = m % 60;
  h = h % 24;

  current_crossover.minute = m % 60;
  current_crossover.hour = h % 24;
  return current_crossover;
}

ncNappingSchedule compute_napping_schedule(
    ncDayTimeInstant current_crossover, ncUnixTimestamp now,
    ncCircadianRhythmEvent *recent_biorhythm_events,
    unsigned int biorhythm_events_count, ncSleepStaging *recent_sleeps,
    unsigned int recent_sleeps_count) {

  return {{14, 00}, {19, 00}, {16, 00}, {19, 00},
          {12, 00}, {14, 00}, {15, 30}, {17, 30}};
}

struct JetLagTherapyState {
  ncJetLagTherapyInfo info;
};

ncJetLagTherapyState *ncCreateJetlagTherapy(ncUnixTimestamp flight_date,
                                            ncUnixTimestamp return_date,
                                            ncFlightDirection flight_direction,
                                            unsigned char timezone_difference) {

  ncUnixTimestamp therapy_start = flight_date - 24 * 3600;
  ncUnixTimestamp therapy_end = return_date + 24 * 3600;
  float progress = 0;
  return new ncJetLagTherapyState{{therapy_start, therapy_end, flight_date,
                                   return_date, flight_direction,
                                   timezone_difference, progress}};
}

void ncDestroyJetlagTherapy(ncJetLagTherapyState *therapy) { delete therapy; }

unsigned int ncGetJetlagEvents(ncJetLagTherapyState *state,
                               ncDayTimeInstant current_crossover,
                               ncUnixTimestamp now,
                               ncCircadianRhythmEvent *out_buffer,
                               unsigned int passed_buffer_size) {

  auto sz = 2 * sizeof(ncCircadianRhythmEvent);
  if (passed_buffer_size >= sz) {
    out_buffer[0] = {CET_LIGHT_SEEKING, now + 3600, now + 3 * 3600};
    out_buffer[1] = {CET_LIGHT_AVOIDING, now + 5 * 3600, now + 7 * 3600};
  }
  return sz;
}

ncJetLagTherapyInfo ncGetJetlagTherapyInfo(ncJetLagTherapyState *therapy) {
  return therapy->info;
}

std::size_t ncSerializeJetlagTherapy(ncJetLagTherapyState *state,
                                     char *output_buffer, std::size_t sz) {
  return serialize(*state, output_buffer, sz);
}

std::size_t ncDeserializeJetlagTherapy(ncJetLagTherapyState *state,
                                       char *input_buffer, std::size_t sz) {
  return deserialize(*state, input_buffer, sz);
}
