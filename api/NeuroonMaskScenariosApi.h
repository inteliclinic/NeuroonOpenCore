/**
 * @file    NeuroonMaskScenariosApiNew.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * [TODO] Documentation
 */

#ifndef NEUROONMASKSCENARIOSAPINEW_H
#define NEUROONMASKSCENARIOSAPINEW_H
#ifdef __cplusplus
extern "C" {
#endif

#include "NeuroonApiCommons.h"

/** Length of a single instruction to the mask.
 *  Enforced by the size of Bluetooth frame.
 *
 *  I've made it a macro. Makes more sense since I'm using it in variables
 * declaration
 *  -Paweł Kaźmierzewski
*/
#define MASK_INSTRUCTION_LENGTH 20

typedef void *ncScenario;

// ==================== LUCID DREAMING ============================

/** @brief Structure holding internal state of lucid dreaming scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
typedef struct LucidDreamScenario ncLucidDreamScenario;

/** @enum Enumeration of starting level of light pulses intensity during the
 *          LD induction. */
typedef enum { LP_DIMMED, LP_REGULAR, LP_BRIGHT } ncLucidPulsesIntensity;

/** @enum Enumeration of tresholds of stability at which LD induction begins.
 */
typedef enum {
  LRS_DEFAULT,
} ncLucidRemStabilityTreshold;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  ncLucidPulsesIntensity startingIntensity;
  ncLucidRemStabilityTreshold remStabilityTreshold;
} ncLucidDreamScenarioInitArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
typedef struct {
  /** User's current sleep stage. */
  ncSleepStage currentSleep_stage;

  /** Average signal quality since the last call */
  ncSignalQuality epochAvgSignalQuality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float accVectorPeak;
} ncLucidDreamScenarioInput;

// ==================== WAKE-UP / COMMON SLEEP ============================

/** @brief Structure holding internal state of wake up scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
typedef struct WakeUpScenario ncWakeUpScenario;

/** @enum Enumeration of modes of sunrise lightning.*/
typedef enum {
  SUN_OFF,
  SUN_DIMMED,
  SUN_NORMAL,
  SUN_BRIGHT,
} ncSunriseIntensity;

/** @enum Enumeration of modes of work of Neuroon's Smart Wake-up */
typedef enum {
  SW_OFF,
  SW_SMART,
  SW_REM,
  SW_LIGHT,
} ncSmartWakeupMode;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  /** Time of absolute wake-up. */
  ncUnixTimestamp wakeupTimestamp;


  /** Time in minutes before planned wakeup at which sunrise starts */
  unsigned int artificialSunriseWindow;

  /** Sunrise light intensity. */
  ncSunriseIntensity sunriseIntensity;

  /** Is smart wake up active and at what level. */
  ncSmartWakeupMode smartWakeUpMode;

  /** Time in minutes before planned wakeup at which smart wake up activates. */
  unsigned int smartWakeupWindow;

  /** Determines whether ringtone will be played at the end
      of scheduled wake-up time */
  bool isRingtoneOn;

  /** Determines whether vibrations will be used for waking the user */
  bool isVibrationOn;
} ncCommonSleepScenarioInitArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
typedef struct {
  /** User's current sleep stage. */
  ncSleepStage currentSleepStage;

  /** Average signal quality since the last call */
  ncSignalQuality epochAvgSignalQuality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float accVectorPeak;
} ncCommonSleepScenarioInput;

/** @brief Destroys scenario releasing hold memory. */
void ncWuDestroyScenario(ncWakeUpScenario *);

typedef struct {
  /** User's current sleep stage. */
  ncSleepStage current_sleep_stage;
} ncPowernapScenarioInput;

typedef struct {
  ncSleepStage currentSleepStage; /**< User's current sleep stage. */
  ncSignalQuality signalQuality;  /**< Avg signal quality during last update
                                     period */
} ncCircadianRhythmAdjustmentScenarioInput;

// ==================== LIGHTBOOST ============================

typedef struct {
  ncSignalQuality signalQuality; /**< Avg signal quality during last update
                                    period */
} ncLightBoostScenarioInput;

/** @enum Enumeration of light intensity levels. */
typedef enum { REGULAR, BRIGHT } ncLightIntensityLevel;

/** @struct Data structure aggregating all paremeters needed for
  *          scenario initialization.
  */
typedef struct {
  unsigned int lengthInMinutes;    /**< Length of the scenario in minutes. */
  ncLightIntensityLevel intensity; /**< Level of intensity of led light */
} ncLightBoostScenarioInitArgs;

// TODO: they are just copied!
typedef struct {
  unsigned int lengthInMinutes;    /**< Length of the scenario in minutes. */
  ncLightIntensityLevel intensity; /**< Level of intensity of led light */
} ncPowernapScenarioInitArgs;

typedef struct {
  unsigned int lengthInMinutes;    /**< Length of the scenario in minutes. */
  ncLightIntensityLevel intensity; /**< Level of intensity of led light */
} ncCircadianRhythmAdjustmentScenarioInitArgs;

// =================== UNIONS =================================

typedef struct {
  union {
    ncLucidDreamScenarioInput lucidDream;
    ncCommonSleepScenarioInput commonSleep;
    ncPowernapScenarioInput powerNap;
    ncCircadianRhythmAdjustmentScenarioInput CircadianRhythm;
    ncLightBoostScenarioInput lightBoost;
  } scenarioSpecific;
  struct {
    ncUnixTimestamp currentTime;
  } commonInput;
} ncScenarioInput;

typedef union {
  ncLucidDreamScenarioInitArgs lucidDream;
  ncCommonSleepScenarioInitArgs commonSleep;
  ncLightBoostScenarioInitArgs lightBoost;
  ncPowernapScenarioInitArgs powerNap;
  ncCircadianRhythmAdjustmentScenarioInitArgs CircadianRhythm;
} ncScenarioInitArgs;

typedef enum {
  SCENARIO_LUCIDDREAM,
  SCENARIO_LIGHTBOOST,
  SCENARIO_SLEEP,
  SCENARIO_POWERNAP,
  SCENARIO_CIRCADIANRHYTHM
} ncScenarioType;

typedef enum {
  UPDATE_OK,
  UPDATE_NEW_DATA,
  UPDATE_SCENARIO_FINISHED,
  UPDATE_ERROR
} ncUpdateOutput;

/**
 * @struct binnary data for BLE interface with timestamp
 */
typedef struct {
  unsigned int time;                  /*!<  */
  char data[MASK_INSTRUCTION_LENGTH]; /*!<  */
} ncAtomicInstruction;

/**
 * @brief Create desired scenario
 *
 * @param[in] scenarioType  Desired scenario input(@ref ncScenarioType)
 * @param[in] args          Scenarioa initial arguments(@ref ncScenarioInitArgs)
 *
 * @return Scenario handle (@ref ncScenario)
 */
ncScenario ncCreateScenario(ncScenarioType scenarioType,
                            const ncScenarioInitArgs *args);

/**
 * @brief Function to unload queue of scenarios instruction sets
 *
 * @param[in] scenario  Scenario handle input(@ref ncScenario)
 *
 * @return Single instruction(@ref ncAtomicInstruction)
 */
ncAtomicInstruction ncGetNextInstruction(ncScenario scenario);

/**
 * @brief Periodic scenario update
 *
 * @param[in] scenario    Scenario handle input(@ref ncScenario)
 * @param[in] updateArgs  Update information input(@ref ncScenarioInput)
 *
 * @return Information regarding scenario state(@ref ncUpdateOutput)
 */
ncUpdateOutput ncScenarioUpdate(ncScenario scenario,
                                const ncScenarioInput *updateArgs);

// mutes scenario for given number of seconds
/**
 * @brief Mute function.
 *
 * @param scenario
 * @param time_s
 *
 * @return Information regarding scenario state(@ref ncUpdateOutput)
 */
ncUpdateOutput ncMuteScenario(ncScenario scenario, unsigned int time_s);

/**
 * @brief Scenario destroyer
 *
 * @param[in] scenario  Scenario handle input(@ref ncScenario)
 *
 * @return Single instruction(@ref ncAtomicInstruction)
 */
ncAtomicInstruction ncDestroyScenario(ncScenario scenario);

/**
 * @brief Function checks if there are available mask instructions
 *
 * @param[in] scenario  Scenario handle input(@ref ncScenario)
 *
 * @return true if queue has pending instruction, false otherwise
 */
bool ncAvailableMaskInstruction(ncScenario scenario);

#ifdef __cplusplus
}
#endif

#endif /* !NEUROONMASKSCENARIOSAPINEW_H */
