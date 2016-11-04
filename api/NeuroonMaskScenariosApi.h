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
struct MaskInstructionList {
  unsigned int count;
  unsigned int *timestamps;
  char **instructions;
};

// -------------------- LUCID DREAMING SCENARIO  -------------------------------

/** @brief Structure holding internal state of lucid dreaming scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
struct LucidDreamScenario;

/** @enum Enumeration of starting level of light pulses intensity during the
 *          LD induction. */
enum LUCID_PULSES_INTENSITY { LP_DIMMED, LP_REGULAR, LP_BRIGHT };

/** @enum Enumeration of tresholds of stability at which LD induction begins.
 */
enum LUCID_REM_STABILITY_TRESHOLD {
  LRS_DEFAULT,
};

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
struct LucidDreamScenarioArgs {
  LUCID_PULSES_INTENSITY starting_intensity;
  LUCID_REM_STABILITY_TRESHOLD rem_stability_treshold;
};

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
struct LucidDreamScenarioInput {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  SLEEP_STAGE current_sleep_stage;

  /** Average signal quality since the last call */
  SIGNAL_QUALITY epoch_avg_signal_quality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float acc_vector_peak;
};

/** @brief Use this function to create an instance of lucid dreaming scenario.
 *
 *  @param[in] init_args Structure aggregating arguments to initialization call.
 *
 *  @return Pointer to a structure holding internal state of light boost
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
LucidDreamScenario *ld_init_scenario(LucidDreamScenarioArgs init_args);

/** @brief Destroys scenario releasing hold memory. */
void ld_destroy_scenario(LucidDreamScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  update_args Structure aggregating arguments for algorithm state
 *                          update
 */
MaskInstructionList
ld_get_mask_instructions(LucidDreamScenario *scenario,
                         const LucidDreamScenarioInput *update_args);

// -------------------- WAKE-UP SCENARIO --------------------------------------

/** @brief Structure holding internal state of wake up scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
struct WakeUpScenario;

/** @enum Enumeration of modes of sunrise lightning.*/
enum WU_SUNRISE_INTENSITY {
  SUN_OFF,
  SUN_DIMMED,
  SUN_NORMAL,
  SUN_BRIGHT,
};

/** @enum Enumeration of modes of work of Neuroon's Smart Wake-up */
enum WU_SMART_WAKEUP_MODE {
  SW_OFF,
  SW_SMART,
  SW_REM,
  SW_LIGHT,
};

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
struct WakeUpScenarioInitArgs {
  /** Sunrise light intensity. */
  WU_SUNRISE_INTENSITY sunrise_intensity;

  /** Is smart wake up active and at what level. */
  WU_SMART_WAKEUP_MODE smart_wake_up_mode;

  /** Time in minutes before planned wakeup at which smart wake up activates. */
  unsigned int smart_wakeup_window;

  /** Determines whether ringtone will be played at the end
      of scheduled wake-up time */
  bool is_ringtone_on;

  /** Determines whether vibrations will be used for waking the user */
  bool is_vibration_on;
};

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
struct WakeUpScenarioInput {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  SLEEP_STAGE current_sleep_stage;

  /** Average signal quality since the last call */
  SIGNAL_QUALITY epoch_avg_signal_quality;

  /** Peak value of accelerometer's vector length since last call of this
   *  function. */
  float acc_vector_peak;
};

/** @brief Use this function to create an instance of wake up scenario.
 *
 *  @param[in] init_args Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of the wake up
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt wake up
 *          scenario.
 */
WakeUpScenario *wu_init_scenario(WakeUpScenarioInitArgs init_args);

/** @brief Destroys scenario releasing hold memory. */
void wu_destroy_scenario(WakeUpScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  update_args Structure aggregating arguments for algorithm state
 *                          update
 */
MaskInstructionList
wu_get_mask_instructions(WakeUpScenario *scenario,
                         const WakeUpScenarioInput *update_args);

// -------------------- POWERNAP SCENARIO --------------------------------------

/** @brief Structure holding internal state of powernap scenario
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
struct PowernapScenario;

/** @enum Enumeration of types of naps.*/
enum NAP_TYPE {
  NT_POWERNAP,
  NT_BODY_NAP,
  NT_REM_NAP,
  NT_ULTIMATE_NAP,
};

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
struct PowernapScenarioInitArgs {
  /** Type of the nap. */
  NAP_TYPE nap_type;

  /** Sunrise light intensity. */
  WU_SUNRISE_INTENSITY sunrise_intensity;

  /** Type of a nap */
  /** Time in minutes before nap ends at which sunrise activates. */
  unsigned int sunrise_window;

  /** Determines whether ringtone will be played at the end
      of nap */
  bool is_ringtone_on;

  /** Determines whether vibrations will be used for waking the user */
  bool is_vibration_on;
};

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 */
struct PowernapScenarioInput {
  /** Time atm of the call for instructions */
  unsigned long timestamp;

  /** User's current sleep stage. */
  SLEEP_STAGE current_sleep_stage;
};

/** @brief Use this function to create an instance of powernap scenario.
 *
 *  @param[in] init_args Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of the powernap
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt powernap
 *          scenario.
 */
PowernapScenario *wu_init_scenario(PowernapScenarioInitArgs init_args);

/** @brief Destroys scenario releasing hold memory. */
void wu_destroy_scenario(PowernapScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  update_args Structure aggregating arguments for algorithm state
 *                          update
 */
MaskInstructionList
wu_get_mask_instructions(PowernapScenario *scenario,
                         const PowernapScenarioInput *update_args);

// -------------------- CIRCADIAN RHYTHM ADJUSTMENT SCENARIO -------------------

/** @brief Structure holding internal state of circadian adjustment scenario.
 *         Do not try directly modifying its content.
 *         Keep it and pass it to its interface functions.
 */
struct CircadianRhythmAdjustmentScenario;

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
struct CircadianRhythmAdjustmentInitScenarioArgs {};

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
*/
struct CircadianRhythmAdjustmentScenarioInput {
  unix_timestamp current_time;   /**< Time atm of the call for instructions */
  SIGNAL_QUALITY signal_quality; /**< Avg signal quality during last update
                                    period */
};

/** @brief Use this function to create an instance of circadian adjustment
 *         scenario.
 *
 *  @param[in] init_args Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of circadian
 *          adjustment scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
CircadianRhythmAdjustmentScenario *
    cra_init_scenario(CircadianRhythmAdjustmentInitScenarioArgs);

/** @brief Destroys scenario releasing hold memory. */
void cra_destroy_scenario(CircadianRhythmAdjustmentScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  update_args Structure aggregating arguments for algorithm state
 *                          update
 */
MaskInstructionList cra_get_mask_instructions(
    CircadianRhythmAdjustmentScenario *scenario,
    const CircadianRhythmAdjustmentScenarioInput *update_args);

// -------------------- Light boost scenario  -------------------------

/** @brief Structure holding internal state of light boost scenario.
 *         Do not try directly modifying its content.
 *         Keep it and pass it to interface functions wrt light boost
 *         scenario.
 */
struct LightBoostScenario;

/** @enum Enumeration of light intensity levels. */
enum LIGHT_INTENSITY_LEVEL { REGULAR, BRIGHT };

/** @struct Data structure aggregating all paremeters needed for
 *          scenario initialization.
 */
struct LightBoostInitScenarioArgs {
  unsigned int lenth_in_minutes;
  LIGHT_INTENSITY_LEVEL intensity;
};

/** @struct Data structure aggregating all paremeters needed for
 *          periodical update and progress of the scenario.
 *  @remark This is the simplest scenario, but the mask should be
 *          react to changes in electrode contact, so keep the update
            interval below 10s.
 */
struct LightBoostScenarioInput {
  unix_timestamp current_time;   /**< Time atm of the call for instructions */
  SIGNAL_QUALITY signal_quality; /**< Avg signal quality during last update
                                    period */
};

/** @brief Use this function to create an instance of light boost scenario.
 *
 *  @param[in] init_args Structure aggregating arguments to initializatoin call.
 *
 *  @return Pointer to a structure holding internal state of light boost
 *          scenario. Do not try directly modifying its content.
 *          Keep it and pass it to interface functions wrt light boost
 *          scenario.
 */
LightBoostScenario *lb_init_scenario(LightBoostInitScenarioArgs init_args);

/** @brief Destroys scenario releasing hold memory. */
void lb_destroy_scenario(LightBoostScenario *);

/** @brief Gets an array of direct instructions to the Neuroon Mask according
 *         to update parameters
 *
 *  @param[in]  scenario    Pointer token to the current scenario.
 *  @param[in]  update_args Structure aggregating arguments for algorithm state
 *                          update
*/
MaskInstructionList
lb_get_mask_instructions(LightBoostScenario *scenario,
                         const LightBoostScenarioInput *update_args);

// -------------------- Scenario frame validation ------------------------------

/**
 * @brief response function outcome
 *
 * Description
 */

typedef enum {
  FRAMEVALIDATION_NEXT_STEP = 0x00,          /**< go to next step */
  OFRAMEVALIDATION_NEXT_STEP = 0x00,         /**< go to next step */
  FRAMEVALIDATION_NEXT_STEP_HOLD_RES = 0x00, /**< go to next step */
  FRAMEVALIDATION_RESEND_FRAME = 0x01,       /**< resend */
  FRAMEVALIDATION_FRAME_IS_VALID = 0x02,     /**< valid */
  FRAMEVALIDATION_FRAME_NOT_VALID = 0x03     /**< not valid */
} e_frameValidation;

/**
 * @brief
 *
 * @param[in] cmd_frame       sent data
 * @param[in] cmd_len         sent data lenght
 * @param[in] response_frame  response data from mask
 * @param[in] response_len    response data length
 *
 * @return  returns next step
 */
e_frameValidation response_sink(char *cmd_frame, size_t cmd_len,
                                char *response_frame, size_t response_len);

#endif /* !NEUROONMASKSCENARIOSAPI_H */
