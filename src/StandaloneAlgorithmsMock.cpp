#import "NeuroonStandaloneAlgorithmsApi.h"

ncDayTimeInstant ncEstimateCrossoverTime(ncDayTimeInstant usual_wake_up_time
                                         ) {
  // TODO prepare actual logic here
  auto &m = usual_wake_up_time.minute;
  auto &h = usual_wake_up_time.hour;

  m -= 20;
  h -= (int)(m < 0);
  m = m % 60;
  h -= 2;
  h = h % 24;

  return {h, m};
}

ncDayTimeInstant
ncCalculateCrossoverPoint(ncDayTimeInstant current_crossover,
                          ncCircadianRhythmEvent *new_biorhythm_events,
                          unsigned long events_count){
  // TODO prepare actual logic here
  return current_crossover;
}
