/**
 * @file    NeuroonSignalStreamApi.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 * @author  Tomasz Grel <t.grel@inteliclinic.com>
 * @date    October, 2016
 * @brief   Public API for interacting with Neuroon Core signal stream
 *          processing algorithms.
 *
 * This file provides the definitions of all the data structures
 * and functions required to use signal processing algorithms of Neuroon,
 * i.e. sleep staging, brain-wave levels calculation.
 *
 * Specifically it defines the functions to initialize stream processing, change
 * its state, provide the callbacks for receiving the results of
 * the computations etc.
 *
 * The contents of this file are a part of API to the Neuroon-Core library.
 *
 */

#ifndef NEUROON_SIGNAL_STREAM_API
#define NEUROON_SIGNAL_STREAM_API

#include "NeuroonApiCommons.h"

// -------------------- The state. ---------------------------------------------

/**
 * @struct This structure contains all the data held by the library.
 * The contents of it are considered private, i.e. the clients of the library
 * should not try to read them, modify them, delete them etc.
 * The pointer to an instance of this structure provides a kind of an
 * access token to the library that is required to be passed to all the library
 * calls except the one used for initialization.
 */
struct NeuroonSignalProcessingState;

// -------------------- Callback types definitions. // ----------------------------- 
/**
 * The type of the callback for collecting the data about sleep stages.
 *
 * The first parameter is an array of staging_element_t that contains
 * the information about the current sleep stages. The second parameter is
 * the size of this array.
 *
 * Please be aware that this callback may be called quite rarely,
 * i.e. it is perfecly normal situation to receive this callback once
 * every 82 seconds.
 *
 */
typedef void (*staging_callback_t)(const staging_element_t *staging_data,
                                   int staging_data_size);

/**
 * Type of callback for receiving online data about brain waves and heart rate.
 * It's called only if presentation mode is activated, but when active it is
 * going
 * to  be called relatively frequently (a few times per second).
 *
 * The callback is supposed to have the following parameters:
 *
 * @param bw_array : the array of brain_wave_levels_t elements, these elements
 * will
 * contain the data about the levels of brain wave types to be presented on the
 * screen of the device.
 * @param bw_array_size : the size of the bw_array
 * @param heart_rate : the heart rate of the sleeping person
 * @param pulsoximetry data : an array of double values to be presented as
 * the plot of the pulse of the sleeping person.
 * @param pulsoximetry_data_size : the size of the pulsoximetry_data array
 */
typedef void (*presentation_callback_t)(const brain_wave_levels_t *bw_array,
                                        int bw_array_size, double heart_rate,
                                        const double *pulsoximetry_data,
                                        int pulsoximetry_data_size);

/**
 * Type of the callback for receiving logs from the library
 *
 * This callback is intended for logging and debugging purposes only.
 * The only parameter is a C-style string containing the log message.
 * The caller may of course ignore the log messages if he so wishes
 */
typedef void (*logger_callback_t)(const char *log_message);

// -------------------- The interface. -----------------------------------------

/**
 * Initializes signal processing library module.
 *
 * Call this function only once on initialization of the library.
 * The value returned is the access token containing all the private data of the
 * library.
 * The client of the library is supposed to save the state token and pass it to
 * all other calls
 * to the library.
 *
 * Modifying or deleting the token will most probably result in crashing the
 * library so please
 * don't attempt it. Should you wish to deinitialize the library and delete its
 * data
 * you can use the destroy_neuroon_alg_core function described below.
 *
 * @param staging_callback : non-NULL pointer to a function called
 * each time a new online sleep staging is generated
 *
 * @param presentation_callback : non-NULL pointer to a function called
 * for real-time presentation of brain waves and heart rate
 *
 * @return a pointer to NeuroonSignalProcessingState structure necessary
 * for calling other API functions (access token)
 */
NeuroonSignalProcessingState *
initialize_neuroon_alg_core(staging_callback_t staging_callback,
                            presentation_callback_t presentation_callback);

/**
 * Destroys the NeuroonSignalProcessingState object and deinitializes the entire
 * library.
 *
 * @param data : the private data (access token) to the library,
 * the token will be invalid after calling the function,
 * so please do not use it afterwards as it may result in undefined behavior.
 */
bool destroy_neuroon_alg_core(NeuroonSignalProcessingState *data);

/**
 * Initializes the processing steps before sleep.
 *
 * It's necessary to call this function before starting to
 * feed BLE frames using feed_eeg_data and feed_ir_led_data functions below.
 *
 * @param data : the private data of the library
 */
bool start_sleep(NeuroonSignalProcessingState *data);

/**
 * Stops the sleep. Afterwards one last staging_callback
 * (passed to initialize_neuroon_alg_core function) will be called with
 * the final result of the sleep staging algorithm.
 *
 * @param data : the private data of the library
 */
bool stop_sleep(NeuroonSignalProcessingState *data);

/**
 * Start computing the data for real-time presentation of brain waves and heart
 * rate.
 * Activating this mode can potentially consume quite a lot of CPU resources of
 * the device.
 *
 * After calling this function the 'presentation_callback' will be called
 * each time the real-time presentation algorithm returns the data to be
 * presented
 * (brain waves and heart rate)
 *
 * @param data : pointer to the private data of the library
 */
bool start_presentation(NeuroonSignalProcessingState *data);

/**
  * Stop computing the data for real-time presentation of brain waves and heart
 * rate
  *
  * @param data : pointer to the private data of the library
  *
  */
bool stop_presentation(NeuroonSignalProcessingState *data);

/**
 * Feeds BLE EEG frame to the library
 *
 * @param bytes : pointer to the first element of an array of chars
 * with the EEG data received from Neuroon mask. Please do not modify these
 * bytes.
 * They're supposed to be exactly as received from the BLE connection.
 *
 * @param size : size of the array passed; currently only 20 bytes frames are
 * supported,
 * but this may change in the future.
 *
 * @param data : pointer to the private data of the library
 */
bool feed_data_stream0(NeuroonSignalProcessingState *data, char *bytes,
                       int size);

/**
 * Feeds BLE IR,ACC,TEMP frame to the library.
 *
 * @param bytes : pointer to the first element of an array
 * of chars with the IR,ACC and TEMP data received from Neuroon mask
 *
 * @param size : size of the array passed; currently only 20 byte frames are
 * supported.
 */
bool feed_data_stream1(NeuroonSignalProcessingState *data, char *bytes,
                       int size);

/**
 * Currently not used, but may be used in the future
 *
 * @param bytes : pointer to the first element of an array of chars with the
 * IR,ACC and TEMP data received from Neuroon mask
 * @param size : size of the array passed; currently only 20 bytes frames are
 * supported
 */
bool feed_data_stream2(NeuroonSignalProcessingState *data, char *bytes,
                       int size);

/**
 * Installs a log callback to the library
 *
 * This is provided because on some devices writing to stdout or opening log
 * files can be prohibited.
 * This callback allows the client of the library to receive log messages
 * from the library and act accordingly. For example the caller may want to
 * use iOS API to save the messages to a file on a device's SD card.
 *
 * @param callback : pointer to a void function taking C-style string (const
 * char*),
 * this function will be called every time a log message is generated.
 *
 */
bool install_log_callback(NeuroonSignalProcessingState *data,
                          logger_callback_t callback);

#endif
