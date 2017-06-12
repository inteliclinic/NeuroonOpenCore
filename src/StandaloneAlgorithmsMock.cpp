#import "NeuroonStandaloneAlgorithmsApi.h"

ncDayTimeInstant ncEstimateCrossoverTime(ncDayTimeInstant usual_wake_up_time
                                         ) {
  // TODO prepare actual logic here
  auto &m = usual_wake_up_time.minute;
  auto &h = usual_wake_up_time.hour;

  m -= 20;
  if (m<0) h--;
  h += m/60;
  m = m % 60;
  if(m<0) m+=60;
  h -= 2;
  h = h % 24;
  if(h<0) h+= 24;

  return {h, m};
}

ncDayTimeInstant
ncCalculateCrossoverPoint(ncDayTimeInstant current_crossover,
                          ncCircadianRhythmEvent *new_biorhythm_events,
                          unsigned long events_count){
  // TODO prepare actual logic here
  int m  = current_crossover.minute;
  int h = current_crossover.hour;

  m += events_count;
  h+=m/60;
  m = m % 60;
  h = h % 24;

  current_crossover.minute = m % 60;
  current_crossover.hour = h % 24;
  return current_crossover;
}
