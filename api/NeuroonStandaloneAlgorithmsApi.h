/**
 * @file    NeuroonStandaloneAlgorithmsApi.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    October, 2016
 * @brief   Public API for standalone Neuroon algorithms.
 *
 * This file provides the definitions of all the data structures
 * and functions required to use the algorithms:
 * - cross-over point computation
 * - perfect napping schedule
 * - jetlag therapy algorithms
 * Specifically it defines the functions to initialize stream processing, change
 * its state, provide the callbacks for receiving the results of
 * the computations etc.
 *
 * The contents of this file are a part of API to the Neuroon-Core library.
 *
 */

#ifndef NEUROON_STANDALONE_ALGORITHMS_API
#define NEUROON_STANDALONE_ALGORITHMS_API

#include "NeuroonApiCommons.h"

// -------------------- Common interface types ---------------------------------

/**
 * @struct Simple struct holding time of day.
 *         Hour field should be an integer 0-23, and minutes 0-59.
 */
struct day_time_instant_t {
  char hour;
  char minute;
};

/**
 * @brief This type represents number seconds since 1st. Jan 1970.
 */
typedef int unix_timestamp;

// -------------------- Crossover time computation. ----------------------------

/**
 * Enumeration of circadian events types.
 * These are events with a potential to change circadian rhythm of a user.
 */
enum CIRCADIAN_EVENT_TYPE {
  NOCTURNAL_SLEEP = 0,
  CROSSOVER_SHIFTING_THERAPY,
  NAP,
  LIGHT_BOOST,
  LIGHT_SEEKING,
  LIGHT_AVOIDING,
};

/**
 * @struct Struct representing a circadian event instance.
 *         These are events with a potential to change circadian rhythm
 *         of a user.
 */
struct circadian_rhythm_event_t {
  CIRCADIAN_EVENT_TYPE type;
  unix_timestamp start;
  unix_timestamp end;
};

/**
 * @brief Estimates crossover point based on a usual wake up time of a user.
 * @param[out]  usual_wake_up_time     Hour and minutes of a usual wake-up.
 * @return Hour and minutes of estimated crossoverpoint.
 */
day_time_instant_t
estimate_crossover_time(day_time_instant_t usual_wake_up_time);

/**
 * @brief Computes crossover point using data about biorhythm affecting events.
 * @param[in]  current_crossover  Current and up-to-date cross-over point.
 * @param[in]  new_biorhythm_events Pointer to the array of events that might
 *             affect the body clock. These should be events that happened since
 *             the last time the crossover point was calculated.
 * @return Hour and minutes of computed crossoverpoint.
 *
 * @remark new_biorhythm_events should containt events passed
           SINCE LAST CROSSOVER COMPUTATION.
 */
day_time_instant_t
calculate_crossover_point(day_time_instant_t current_crossover,
                          circadian_rhythm_event_t *new_biorhythm_events);

// -------------------- Perfect napping schedule  ------------------------------

/**
 * @struct A structure aggregating results from napping schedule computation.
 */
struct napping_schedule_t {
  day_time_instant_t power_nap_interval_start;
  day_time_instant_t power_nap_interval_end;

  day_time_instant_t body_nap_interval_start;
  day_time_instant_t body_nap_interval_end;

  day_time_instant_t rem_nap_interval_start;
  day_time_instant_t rem_nap_interval_end;

  day_time_instant_t ultimate_nap_interval_start;
  day_time_instant_t ultimate_nap_interval_end;
};

/**
 * @brief Computes optimal napping schedule based on recent sleeps architecture
          user's crossover point.
 * @param[in]  current_crossover  Current and up-to-date cross-over point.
 * @param[in]  recent_biorhythm_events Pointer to the array of events that might
 *             affect the body clock.
 * @return Hour and minutes of computed crossoverpoint.
 */
napping_schedule_t
compute_napping_schedule(day_time_instant_t current_crossover,
                         circadian_rhythm_event_t *recent_biorhythm_events,
                         staging_element_t **recent_nocturnal_sleeps);

// -------------------- Jetlag progress computation ----------------------------

/**
 * @enum Directions of a flight.
 */
enum DIRECTION { EASTWARDS, WESTWARDS };

/**
 * @struct This structure contains all the data the library needs about jetlag
 *         therapy to use its algorithms.
 *
 * The contents of it are considered private, i.e. the clients of the library
 * should not try to read them, modify them, delete them etc.
 * The pointer to an instance of this structure provides a kind of an
 * access token to the library that is required to be passed to all the library
 * calls except the one used for initialization.
 *
 * The data can also be serialised to and from byte-string.
 */
struct JetLagTherapyState;

/**
* @struct This structure holds public data of a jet lag therapy.
*/
struct jet_lag_therapy_info_t {
  /** When the first event will be scheduled
      by the therapy */
  unix_timestamp therapy_start;

  /** The therapy estimated end date. This may change should the usser
      does not make use of therapy events. */
  unix_timestamp therapy_end;

  /** An immutable date passed when the therapy got created */
  unix_timestamp flight_date;

  /** An immutable date of a return flight. If its
      earlier than flight date the algorithm will
      assume that no return is planned. */
  unix_timestamp return_date;

  /** Direction of the flight. */
  DIRECTION flight_direction;

  /** Difference between timezones in hours.
      Should be no less than 3 hour.. */
  unsigned char timezone_difference;

  /** How much of jetlag is beaten. This is a part o timezone
      differece */
  float progress;
};

/**
 * This function creates a pointer to private state of jetlag therapy.
 * It should be called once for a therapy instance.
 * Returned pointer should not be modified or it may cause the library to crash.
 * To get rid of the pointer look for destroy_jet_lag_therapy function.
 *
 * @param[in]  flight_date  Date of the departure of the flight.
 * @param[in]  return_date  Date of arrival back to the original time zone.
 *                          If it's not greater than flight_date, the algorithm
 *                          will assume that no return is planned.
 * @param[in]  flight_direction  Direction of the flight.
 * @param[in]  timezone_difference  Positive number of hours of between
 *                                  timezones.
 *
 * @return  Pointer containing entire state of jetlag therapy. Do not modify it
 *          and use it as a token to jet lag therapy api functions.
 */
JetLagTherapyState *create_jetlag_therapy(unix_timestamp flight_date,
                                          unix_timestamp return_date,
                                          DIRECTION flight_direction,
                                          unsigned char timezone_difference);

/**
 * Generate "go to dfu" command
 *
 * @param[out]  frame     pointer to 20 bytes array where frame will be
 *                        stored
 * @param[out]  len       pointer to size_t value where function will put length
 *                        of array
 * @param[in]   firmware  choose firmware type
 *
 * @return true if function was able to generate frame, false otherwise(ex.
 * wrong frame len or unsupported firmware)
 */
void destroy_jet_lag_therapy(JetLagTherapyState *therapy);

/**
  * This function provides main functionality of jetlag therapy
  * by returning schedule of upcoming therapy events within nearest 24h.
  *
  * @param[in]  therapy            Therapy state instance.
  * @param[in]  unix_timestamp     Time of calling this function.
  * @param[in]  current_crossover  Current and up-to-date cross-over point.
  *
  * @return Newly allocated array of sheduled events.
  *
  * @remark Always pass up-to-date crossover point.
  */
circadian_rhythm_event_t *
get_jetlag_events(JetLagTherapyState *state,
                  day_time_instant_t current_crossover, unix_timestamp now);

/**
 * @brief   Get public therapy information.
 * @remark  Part of fields may change after updating the therapy with
 * crossover.
 *
 * @param[in]  therapy            Therapy state instance.
 *
 * @return Information about passed therapy.
 */
jet_lag_therapy_info_t get_jetlag_therapy_info(JetLagTherapyState *therapy);

/** Encodes jet lag therapy as a byte string.
 *
 * @return  Pointer to the newly allocated array of charts containing the
            encoded therapy. */
char *serialize_jetlag_therapy(JetLagTherapyState *state);

/** Deserializes byte string to the before-encoded jet lag therapy state.

* @return  Pointer containing entire state of jetlag therapy. Do not modify it
*          and use it as a token to jet lag therapy api functions.
*          Or null if there was a problem with deserialization.
*/
JetLagTherapyState *deserialize_jetlag_therapy(char *serialized_therapy);

#endif
