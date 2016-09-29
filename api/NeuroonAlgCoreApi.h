#ifndef NEUROON_ALG_CORE_API_H
#define NEUROON_ALG_CORE_API_H

struct NeuroonAlgCoreData;

/**
 * Enum with the sleep stages
 */
enum SLEEP_STAGE {
	AWAKE = 0,
	REM = -1,
	LIGHT = -3,
	DEEP = -4
};


enum SIGNAL_QUALITY {
	NO_SIGNAL = 0,
	VERY_BAD = 1,
	BAD = 2,
	GOOD = 3,
	VERY_GOOD = 4
};

struct brain_wave_levels_t {
	double alpha;
	double beta;
	double delta;
	double theta;
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
	unsigned long long timestamp;
};

/**
 * Type of the callback for collecting the data about sleep stages
 */
typedef void (*staging_callback_t)(const staging_element_t*, int);


/**
 * Type of callback for receiving online data about brain waves and heart rate.
 * Works only if presentation mode is activated. Will be called relatively frequently
 * (a few times per second)
 */
typedef void (*presentation_callback_t)(const brain_wave_levels_t*, int, double, const double*, int);


/**
 * Type of the callback for receiving logs from the library
 */
typedef void (*logger_callback_t)(const char*);

/**
 * Initializes the NeuronAlgoCore library. Call this function only once on initialization.
 *
 * @param staging_callback : pointer to a function called each time a new online sleep staging is generated
 *
 * @param presentation_callback : pointer to a function called for real-time presentation of brain waves and heart rate
 *
 * @return a pointer to NeuroonAlgCoreData structure necessary for calling other API functions
 */
NeuroonAlgCoreData* initialize_neuroon_alg_core(staging_callback_t staging_callback, presentation_callback_t presentation_callback);


/**
 * Destroys the NeuroonAlgCoreData object and deinitializes the entire library
 */
void destroy_neuroon_alg_core(NeuroonAlgCoreData* data);

/**
 * Initializes the processing steps before sleep; It's necessary to call this function before starting to
 * feed BLE frames using feed_* functions below
 */
void start_sleep(NeuroonAlgCoreData* data);

/**
 * Stops the sleep. Afterwards the staging_callback (passed to initialize_neuroon_alg_core function) will be called with the final sleep staging.
 */
void stop_sleep(NeuroonAlgCoreData* data);

/**
 * Start computing the data for real-time presentation of brain waves and heart rate.
 * Activating this mode can potentially consume quite a lot of CPU resources of the device.
 *
 * After calling this function the 'presentation_callback' will be called
 * each time the real-time presentation algorithm returns the data to be presented
 * (brain waves and heart rate)
 */
void start_presentation(NeuroonAlgCoreData* data);


/**
  * Stop computing the data for real-time presentation of brain waves and heart rate
  */
void stop_presentation(NeuroonAlgCoreData* data);

/**
 * Feeds BLE EEG frame to the library
 *
 * @param bytes : pointer to the first element of an array of chars with the EEG data received from Neuroon mask
 * @param size : size of the array passed; currently only 20 bytes frames are supported
 */
void feed_eeg_data(NeuroonAlgCoreData* data, char* bytes, int size);

/**
 * Feeds BLE IR,ACC,TEMP frame to the library
 *
 * @param bytes : pointer to the first element of an array of chars with the IR,ACC and TEMP data received from Neuroon mask
 * @param size : size of the array passed; currently only 20 bytes frames are supported
 */
void feed_ir_led_data(NeuroonAlgCoreData* data, char* bytes, int size);


/**
 * Installs a log callback to the library
 *
 * This is provided because on some devices writing to stdout or opening log files can be prohibited
 * This callback allows the client of the library to receive log messages from the library and act accordingly.
 * For example the caller may want to use iOS API to save the messages to a file on a device's SD card
 *
 * @param callback : pointer to a void function taking C-style string (const char*), this function will be called
 * every time a log message is generated
 *
 */
void install_log_callback(NeuroonAlgCoreData* data, logger_callback_t callback);

#endif
