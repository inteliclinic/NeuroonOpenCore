#ifndef NEUROON_ALG_CORE_API_H
#define NEUROON_ALG_CORE_API_H


struct NeuroonAlgCoreData;

enum SLEEP_STAGE {
	AWAKE = 0,
	REM = -1,
	LIGHT = -3,
	DEEP = -4
};

struct staging_element_t {
	SLEEP_STAGE stage;
	unsigned long long timestamp;
};

typedef void (*staging_callback_t)(const staging_element_t*, int);
typedef void (*logger_callback_t)(const char*);

NeuroonAlgCoreData* initialize_neuroon_alg_core(staging_callback_t staging_callback);

void destroy_neuroon_alg_core(NeuroonAlgCoreData* data);

void start_sleep(NeuroonAlgCoreData* data);

void feed_eeg_data(NeuroonAlgCoreData* data, char* bytes, int size);

void feed_ir_led_data(NeuroonAlgCoreData* data, char* bytes, int size);

void install_log_callback(NeuroonAlgCoreData* data, logger_callback_t);

#endif
