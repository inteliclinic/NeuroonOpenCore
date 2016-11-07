
/**
 * @file    NeuroonSignalStreamApi.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    October, 2016
 * @brief   Commmon data types for Neuroon Core API

 *
 * This header provides the definitions of all common data structures
 * used throughout Neuroon Core Api
 *
 */

#ifndef NEUROON_COMMONS
#define NEUROON_COMMONS

// -------------------- Time related types ------------------------------------

/**
 * @brief This type represents number seconds since 1st. Jan 1970.
 */
typedef int unix_timestamp;

// -------------------- Sleep related data structures. ------------------------

/**
 * Enum defining the constants corresponding to the sleep stages
 * returned by the library.
 */
enum SLEEP_STAGE { AWAKE = 0, REM = -1, LIGHT = -3, DEEP = -4 };

/**
 * Enum defining the constants corresponging to the quality of the signal.
 */
enum SIGNAL_QUALITY {
  NO_SIGNAL = 0,
  VERY_BAD = 1,
  BAD = 2,
  GOOD = 3,
  VERY_GOOD = 4
};

/**
 * The structure aggregating the different types of brain waves.
 *
 * For reference the frequency bands corresponding to every brain wave types
 * are defined as:
 *
 * alpha: [8, 13] Hz
 * beta: [16, 31] Hz
 * delta: [0.1, 3] Hz
 * theta: [4, 7] Hz
 *
 */
struct brain_wave_levels_t {
  double alpha;
  double beta;
  double delta;
  double theta;
};

struct movement_description_t {
  // TODO
};

/**
 * A structure with a single datum about the sleep stage.
 * SLEEP_STAGE a value representing the stage
 * at the time defined by timestamp
 */
struct staging_element_t {
  SLEEP_STAGE stage;
  SIGNAL_QUALITY signal_quality;
  brain_wave_levels_t brain_waves;
  double heart_rate;
  movement_description_t movement;
  unsigned long long timestamp;
};

/**
 * @struct Simple struct holding time of day.
 *         Hour field should be an integer 0-23, and minutes 0-59.
 */
struct day_time_instant_t {
  char hour;
  char minute;
};

#endif
