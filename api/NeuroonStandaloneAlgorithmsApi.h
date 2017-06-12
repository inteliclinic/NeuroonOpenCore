/**
 *  @file    NeuroonStandaloneAlgorithmsApi.h
 *  @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 *  @date    October, 2016
 *  @brief   Public API for standalone Neuroon algorithms.
 *
 *  This file provides the definitions of all the data structures
 *  and functions required to use the algorithms:
 *  - cross-over point computation
 *  - perfect napping schedule
 *  - jetlag therapy algorithms
 *  Specifically it defines the functions to initialize stream processing,
 * change
 *  its state, provide the callbacks for receiving the results of
 *  the computations etc.
 *
 *  The contents of this file are a part of API to the Neuroon-Core library.
 */

#ifndef NEUROON_STANDALONE_ALGORITHMS_API
#define NEUROON_STANDALONE_ALGORITHMS_API

#include "NeuroonApiCommons.h"

//  -------------------- Crossover time computation.
//  ----------------------------

/**
 *  Enumeration of circadian events types.
 *  These are events with a potential to change circadian rhythm of a user.
 */
enum ncCircadianEventType {
  CET_NOCTURNAL_SLEEP = 0,
  CET_CROSSOVER_SHIFTING_THERAPY,
  CET_NAP,
  CET_LIGHT_BOOST,
  CET_LIGHT_SEEKING,
  CET_LIGHT_AVOIDING,
};

/**
 *  @struct Struct representing a circadian event instance.
 *          These are events with a potential to change circadian rhythm
 *          of a user.
 */
struct ncCircadianRhythmEvent {
  ncCircadianEventType type;
  ncUnixTimestamp start;
  ncUnixTimestamp end;
};

/**
 *  @brief Estimates crossover point based on a usual wake up time of a user.
 *  @param[out]  usual_wake_up_time     Hour and minutes of a usual wake-up.
 *  @return Hour and minutes of estimated crossoverpoint.
 */
ncDayTimeInstant ncEstimateCrossoverTime(ncDayTimeInstant usual_wake_up_time);

/**
 *  @brief Computes crossover point using data about biorhythm affecting events.
 *  @param[in]  current_crossover  Current and up-to-date cross-over point.
 *  @param[in]  new_biorhythm_events Pointer to the array of events that might
 *              affect the body clock. These should be events that happened
 * since
 *              the last time the crossover point was calculated.
 *  @return Hour and minutes of computed crossoverpoint.
 *
 *  @remark new_biorhythm_events should containt events passed
 *          SINCE LAST CROSSOVER COMPUTATION.
 */
ncDayTimeInstant
ncCalculateCrossoverPoint(ncDayTimeInstant current_crossover,
                          ncCircadianRhythmEvent *new_biorhythm_events);

//  -------------------- Perfect napping schedule ------------------------------

/**
 *  @struct A structure aggregating results from napping schedule computation.
 */
struct ncNappingSchedule {
  ncDayTimeInstant power_nap_interval_start;
  ncDayTimeInstant power_nap_interval_end;

  ncDayTimeInstant body_nap_interval_start;
  ncDayTimeInstant body_nap_interval_end;

  ncDayTimeInstant rem_nap_interval_start;
  ncDayTimeInstant rem_nap_interval_end;

  ncDayTimeInstant ultimate_nap_interval_start;
  ncDayTimeInstant ultimate_nap_interval_end;
};

/**
 *  @brief Computes optimal napping schedule based on recent sleeps architecture
 *         user's crossover point.
 *  @param[in]  current_crossover  Current and up-to-date cross-over point.
 *  @param[in]  recent_biorhythm_events Pointer to the array of events that
 * might
 *              affect the body clock.
 *  @return Hour and minutes of computed crossoverpoint.
 */
ncNappingSchedule
compute_napping_schedule(ncDayTimeInstant current_crossover,
                         ncCircadianRhythmEvent *recent_biorhythm_events,
                         ncStagingElement **recent_nocturnal_sleeps);

//  -------------------- Jetlag progress computation
//  ----------------------------

/**
 *  @enum Directions of a flight.
 */
enum ncFlightDirection { FD_EASTWARDS, FD_WESTWARDS };

/**
 *  @struct This structure contains all the data the library needs about jetlag
 *          therapy to use its algorithms.
 *
 *  The contents of it are considered private, i.e. the clients of the library
 *  should not try to read them, modify them, delete them etc.
 *  The pointer to an instance of this structure provides a kind of an
 *  access token to the library that is required to be passed to all the library
 *  calls except the one used for initialization.
 *
 *  The data can also be serialised to and from byte-string.
 */

struct ncJetLagTherapyState;

/**  @struct This structure holds public data of a jet lag therapy.
 */
struct ncJetLagTherapyInfo {
  /**  When the first event will be scheduled by the therapy */
  ncUnixTimestamp therapy_start;

  /**  The therapy estimated end date. This may change should the usser
   *     does not make use of therapy events. */
  ncUnixTimestamp therapy_end;

  /** An immutable date passed when the therapy got created */
  ncUnixTimestamp flight_date;

  /**  An immutable date of a return flight. If its
   *  earlier than flight date the algorithm will
   *  assume that no return is planned. */
  ncUnixTimestamp return_date;

  /** Direction of the flight. */
  ncFlightDirection flight_direction;

  /** Difference between timezones in hours.
   *     Should be no less than 3 hour.. */
  unsigned char timezone_difference;

  /** How much of jetlag is beaten. This is a part o timezone
   *     differece */
  float progress;
};

/**
 *  This function creates a pointer to private state of jetlag therapy.
 *  It should be called once for a therapy instance.
 *  Returned pointer should not be modified or it may cause the library to
 * crash.
 *  To get rid of the pointer look for destroy_jet_lag_therapy function.
 *
 *  @param[in]  flight_date  Date of the departure of the flight.
 *  @param[in]  return_date  Date of arrival back to the original time zone.
 *                           If it's not greater than flight_date, the algorithm
 *                           will assume that no return is planned.
 *  @param[in]  flight_direction  Direction of the flight.
 *  @param[in]  timezone_difference  Positive number of hours of between
 *                                   timezones.
 *
 *  @return  Pointer containing entire state of jetlag therapy. Do not modify it
 *           and use it as a token to jet lag therapy api functions.
 */
ncJetLagTherapyState *ncCreateJetlagTherapy(ncUnixTimestamp flight_date,
                                            ncUnixTimestamp return_date,
                                            ncFlightDirection flight_direction,
                                            unsigned char timezone_difference);

/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
void ncDestroyJetLagTherapy(ncJetLagTherapyState *therapy);

/** This function provides main functionality of jetlag therapy
 *  by returning schedule of upcoming therapy events within nearest 24h.
 *
 *  @param[in]  therapy            Therapy state instance.
 *  @param[in]  ncUnixTimestamp     Time of calling this function.
 *  @param[in]  current_crossover  Current and up-to-date cross-over point.
 *
 *  @return Newly allocated array of sheduled events.
 *
 *  @remark Always pass up-to-date crossover point.
 */
ncCircadianRhythmEvent *ncGetJetlagEvents(ncJetLagTherapyState *state,
                                          ncDayTimeInstant current_crossover,
                                          ncUnixTimestamp now);

/**  @brief   Get public therapy information.
 *  @remark  Part of fields may change after updating the therapy with
 *  crossover.
 *
 *  @param[in]  therapy            Therapy state instance.
 *
 *  @return Information about passed therapy.
 */

ncJetLagTherapyInfo getJetlagTherapyInfo(ncJetLagTherapyState *therapy);

/**  Encodes jet lag therapy as a byte string.
 *
 *  @return  Pointer to the newly allocated array of charts containing the
 *           encoded therapy.
 */
char *ncSerializeJetlagTherapy(ncJetLagTherapyState *state);

/**  Deserializes byte string to the before-encoded jet lag therapy state.
 *
 *  @return  Pointer containing entire state of jetlag therapy. Do not modify it
 *           and use it as a token to jet lag therapy api functions.
 *           Or null if there was a problem with deserialization.
 */
ncJetLagTherapyState *ncDeserializeJetlagTherapy(char *serialized_therapy);

#endif
