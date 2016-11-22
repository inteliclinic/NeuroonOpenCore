
/**
 * @file    NeuroonSignalStreamApi.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @date    October, 2016
 * @brief   Commmon data types for Neuroon Core API
 *
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
typedef int ncUnixTimestamp;

// -------------------- Sleep related data structures. ------------------------

/**
 * Enum defining the constants corresponding to the sleep stages
 * returned by the library.
 */
typedef enum { AWAKE = 0, REM = -1, LIGHT = -3, DEEP = -4 } ncSleepStage;

/**
 * Enum defining the constants corresponging to the quality of the signal.
 */
typedef enum {
  NO_SIGNAL = 0,
  VERY_BAD = 1,
  BAD = 2,
  GOOD = 3,
  VERY_GOOD = 4
}ncSignalQuality;

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
typedef struct {
  double alpha;
  double beta;
  double delta;
  double theta;
}ncBrainWaveLevels;

typedef struct {
  // TODO
}ncMovementDescription;

/**
 * A structure with a single datum about the sleep stage.
 * SLEEP_STAGE a value representing the stage
 * at the time defined by timestamp
 */
typedef struct {
  ncSleepStage stage;
  ncSignalQuality signal_quality;
  ncBrainWaveLevels brain_waves;
  double heart_rate;
  ncMovementDescription movement;
  unsigned long long timestamp;
}ncStagingElement;

/**
 * @struct Simple struct holding time of day.
 *         Hour field should be an integer 0-23, and minutes 0-59.
 */
typedef struct {
  char hour;
  char minute;
}ncDayTimeInstant;

#endif
