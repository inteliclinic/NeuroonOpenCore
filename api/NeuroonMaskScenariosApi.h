/**
 * @file    NeuroonMaskScenariosApi.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Public API for high-level interaction with Neuroon mask
 * scenarios.
 *
 * This file provides the definitions of data structures and functions
 * required for preparing set of instructions for Neuroon mask.
 *
 * Specifically it defines the functions to realize scenarios such us:
 * - wake up scenario
 * - lucid dreaming scenario
 * - circadian adjustment scenario
 * - light boost scenario
 *
 * The contents of this file are a part of API to the Neuroon-Core library.
 *
 */

#ifndef NEUROONMASKSCENARIOSAPI_H
#define NEUROONMASKSCENARIOSAPI_H

#include "NeuroonApiCommons.h"
#include <stddef.h>

// -------------------- COMMON SCENARIO TYPES ----------------------------------

/** Length of a single instruction to the mask.
 *  Enforced by the size of Bluetooth frame.
*/
static const int MASK_INSTRUCTION_LENGTH = 20;

/** @struct Struct containing byte-encoded instructions for the mask.
 *          Each instruction in the list is a byte-array with
 *          MASK_INSTRUCTION_LENGTH elements.
 *          Each instruction has accompanying timestamp.
 *          Timestamp indicates time at which the instruction should be executed
 *  @remark After providing new data to the scenario the course of mask events
 *          may change. So always discard older, not yet executed instructions.
 *  @remark For now it can be assumed that instructions are sorted by their
 *          timestamp.
 */
typedef struct {
  unsigned int count;
  unsigned int *timestamps;
  char **instructions;
}ncMaskInstructionList;

// -------------------- LUCID DREAMING SCENARIO  -------------------------------

/** @brief Structure holding internal state of lucid dreaming scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
typedef struct LucidDreamScenatio ncLucidDreamScenario;

/** @enum Enumeration of starting level of light pulses intensity during the
 *          LD induction. */
typedef enum { LP_DIMMED, LP_REGULAR, LP_BRIGHT } ncLucidPulsesIntensity;

/** @enum Enumeration of tresholds of stability at which LD induction begins.
 */
typedef enum {
  LRS_DEFAULT,
}ncLucidRemStabilityTreshold;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  ncLucidPulsesIntensity startingIntensity;
  ncLucidRemStabilityTreshold remStabilityTreshold;
}ncLucidDreamScenarioArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
typedef struct {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  ncSleepStage currentSleep_stage;

  /** Average signal quality since the last call */
  ncSignalQuality epochAvgSignalQuality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float accVectorPeak;
}ncLucidDreamScenarioInput;

/** @brief Use this function to create an instance of lucid dreaming scenario.
 *
 *  @param[in] initArgs Structure aggregating arguments to initialization call.
 *
 *  @return Pointer to a structure holding internal state of light boost
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
ncLucidDreamScenario *ld_init_scenario(ncLucidDreamScenarioArgs initArgs);

/** @brief Destroys scenario releasing hold memory. */
void ncLdDestroyScenario(ncLucidDreamScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  updateArgs Structure aggregating arguments for algorithm state
 *                          update
 */
ncMaskInstructionList
ncldGetMaskInstructions(ncLucidDreamScenario *scenario,
                         const ncLucidDreamScenarioInput *updateArgs);

// -------------------- WAKE-UP SCENARIO --------------------------------------

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
}ncWuSunriseIntensity;

/** @enum Enumeration of modes of work of Neuroon's Smart Wake-up */
typedef enum {
  SW_OFF,
  SW_SMART,
  SW_REM,
  SW_LIGHT,
}ncWuSmartWakeupMode;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  /** Sunrise light intensity. */
  ncWuSunriseIntensity sunriseIntensity;

  /** Is smart wake up active and at what level. */
  ncWuSmartWakeupMode smartWakeUpMode;

  /** Time in minutes before planned wakeup at which smart wake up activates. */
  unsigned int smartWakeupWindow;

  /** Determines whether ringtone will be played at the end
      of scheduled wake-up time */
  bool isRingtoneOn;

  /** Determines whether vibrations will be used for waking the user */
  bool isVibrationOn;
}ncWakeUpScenarioInitArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
typedef struct {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  ncSleepStage currentSleepStage;

  /** Average signal quality since the last call */
  ncSignalQuality epochAvgSignalQuality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float accVectorPeak;
}ncWakeUpScenarioInput;

/** @brief Use this function to create an instance of wake up scenario.
 *
 *  @param[in] initArgs Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of the wake up
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt wake up
 *          scenario.
 */
WakeUpScenario *ncWuInitScenario(ncWakeUpScenarioInitArgs initArgs);

/** @brief Destroys scenario releasing hold memory. */
void ncWuDestroyScenario(ncWakeUpScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  updateArgs Structure aggregating arguments for algorithm state
 *                          update
 */
ncMaskInstructionList
ncWuGetMaskInstructions(ncWakeUpScenario *scenario,
                         const ncWakeUpScenarioInput *updateArgs);

// -------------------- POWERNAP SCENARIO --------------------------------------

/** @brief Structure holding internal state of powernap scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
typedef struct PowernapScenario ncPowernapScenario;

/** @enum Enumeration of types of naps.*/
typedef enum {
  NT_POWERNAP,
  NT_BODY_NAP,
  NT_REM_NAP,
  NT_ULTIMATE_NAP,
}ncNapType;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  /** Type of the nap. */
  ncNapType napType;

  /** Sunrise light intensity. */
  ncWuSunriseIntensity sunriseIntensity;

  /** Type of a nap */
  /** Time in minutes before nap ends at which sunrise activates. */
  unsigned int sunriseWindow;

  /** Determines whether ringtone will be played at the end
      of nap */
  bool isRingtoneOn;

  /** Determines whether vibrations will be used for waking the user */
  bool isVibrationOn;
}ncPowernapScenarioInitArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
typedef struct {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  ncSleepStage current_sleep_stage;
}ncPowernapScenarioInput;

/** @brief Use this function to create an instance of powernap scenario.
 *
 *  @param[in] initArgs Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of the powernap
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt powernap
 *          scenario.
 */
ncPowernapScenario *ncWuInitScenario(ncPowernapScenarioInitArgs initArgs);

/** @brief Destroys scenario releasing hold memory. */
void ncWuDestroyScenario(ncPowernapScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  updateArgs Structure aggregating arguments for algorithm state
 *                          update
 */
ncMaskInstructionList
ncWuGetMaskInstructions(ncPowernapScenario *scenario,
                         const ncPowernapScenarioInput *updateArgs);

// -------------------- CIRCADIAN RHYTHM ADJUSTMENT SCENARIO -------------------

/** @brief Structure holding internal state of circadian adjustment scenario.
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
typedef struct CircadianRhythmAdjustmentScenario ncCircadianRhythmAdjustmentScenario;

typedef enum {
  CRA_DEFAULT_MODE
}ncCircadianRhythmAdjustmentMode;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  ncCircadianRhythmAdjustmentMode mode;
  ncDayTimeInstant currentCrossover;
  ncUnixTimestamp currentTime;     /**< Time atm of the call for instructions */
  ncUnixTimestamp scheduledWakeup; /**< Time of sheduled wakeup */
}ncCircadianRhythmAdjustmentInitScenarioArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
*/
typedef struct {
  ncUnixTimestamp currentTime;   /**< Time atm of the call for instructions */
  ncSignalQuality signalQuality; /**< Avg signal quality during last update
                                    period */
}CircadianRhythmAdjustmentScenarioInput;

/** @brief Use this function to create an instance of circadian adjustment
 *         scenario.
 *
 *  @param[in] initArgs Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of circadian
 *          adjustment scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
ncCircadianRhythmAdjustmentScenario *
    ncCraInitScenario(ncCircadianRhythmAdjustmentInitScenarioArgs initArgs);

/** @brief Destroys scenario releasing hold memory. */
void ncCraDestroyScenario(ncCircadianRhythmAdjustmentScenario *scenario);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  updateArgs Structure aggregating arguments for algorithm state
 *                          update
 */
ncMaskInstructionList ncCraGetMaskInstructions(
    ncCircadianRhythmAdjustmentScenario *scenario,
    const ncCircadianRhythmAdjustmentScenarioInput *updateArgs);

// -------------------- Light boost scenario  -------------------------

/** @brief Structure holding internal state of light boost scenario.
 *         Do not try directly modifying its content.
 *         Keep it and pass it to interface functions wrt light boost
 *         scenario.
 */
typedef struct LightBoostScenario ncLightBoostScenario;

/** @enum Enumeration of light intensity levels. */
typedef enum { REGULAR, BRIGHT }ncLightIntensityLevel;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
typedef struct {
  unsigned int lenthInMinutes;
  ncLightIntensityLevel intensity;
}ncLightBoostInitScenarioArgs;

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 *  @remark This is the simplest scenario, but the mask should be
 *          react to changes in electrode contact, so keep the update
            interval below 10s.
 */
typedef struct {
  ncUnixTimestamp currentTime;   /**< Time atm of the call for instructions */
  ncSignalQuality signalQuality; /**< Avg signal quality during last update
                                    period */
}ncLightBoostScenarioInput;

/** @brief Use this function to create an instance of light boost scenario.
 *
 *  @param[in] initArgs Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of light boost
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
ncLightBoostScenario *ncLbInitScenario(ncLightBoostInitScenarioArgs initArgs);

/** @brief Destroys scenario releasing hold memory. */
void ncLbDestroyScenario(ncLightBoostScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  updateArgs Structure aggregating arguments for algorithm state
 *                          update
*/
ncMaskInstructionList
ncLbGetMaskInstructions(ncLightBoostScenario *scenario,
                         const ncLightBoostScenarioInput *updateArgs);

// -------------------- Scenario frame validation ------------------------------

/**
 * @enum Enumeration of scenario execution decision.
 *
 * After sending an instruction frame from scenario to the Neuroon mask,
 * the mask responses with another frame.
 */
typedef enum {
  /** go to next scenario step, discard response frame */
  FV_NEXT_STEP = 0x00,
  /** go to next scenario step, but hold the response */
  FV_NEXT_STEP_HOLD_RESP = 0x01,
  /** repeat sending procedure */
  FV_RESEND_FRAME = 0x02,
  /** extend waiting for a response frame */
  FV_WAIT_MORE = 0x03,
  /** there was something with the structures of call arguments */
  FV_STRUCTURAL_ERROR = 0x04
}ncFrameValidation;

/**
 * @brief After sending an instruction frame from scenario to the Neuroon mask,
 *        the mask responses with another frame. You should pass the scenario.
 *        and both of the frames to this function. To inform scenario about possible
 *        difficulties and get back decision about further execution of scenario.
 *        Function output is documented in the description of the enum.
 *
 * @param[in] scenarioPtr    Scenario that generated sent frame.
 * @param[in] cmdFrame       The frame sent to the mask. (raw bytes)
 * @param[in] cmdLen         Length of the sent frame.
 * @param[in] responseFrame  The frame sent to the mask. (raw bytes)
 * @param[in] responseLen    Length of the response frame.
 *
 * @return Enum value informing about decision wrt execution of scenario.
 */
ncFrameValidation ncResponseSink(void *scenarioPtr, char *cmdFrame,
                                size_t cmdLen, char *responseFrame,
                                size_t responseLen);

#endif /* !NEUROONMASKSCENARIOSAPI_H */
