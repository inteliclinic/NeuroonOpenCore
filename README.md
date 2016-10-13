# neuroon-alg-core

Core library for Neuroon's algorithms and signal processing.


High level description
======================

The main purpose of the library is to provide algorithms necessary for the analysis
of sleep data provided by the Neuroon mask. This includes mainly computing the sleep staging
and providing data necessary for the visualizations. The API of the component is quite simple and at the moment consists of only one .h file in the api directory.

Apart from the library the project consists also of unit and functional tests developed in order to increas maintainability. Some command line tools for data parsing, computing sleep staging and simulating the behavior of the mask are also provided but are not necessary to make the library work on mobile devices.

The library is written using C++ programming language in the C++11 standard and to date has been successfully compiled and tested on desktop and android devices using the gcc and clang compilers.

API conventions
----------

At the moment the library can be thought of as a "passive" component, i.e. it doesn't start any threads of its own and is entirely dependable on the caller to call its functions. In other words all calls to the library are synchronous. Some functions such as passing the BLE frames into the library may once in a while trigger some computations, however in our experience these computations are not very heavy and should not block the calling thread for too long.

All the results from the library are returned by callback functions. The client code is required to provide pointers to functions that will receive result data such as for example the results of sleep staging algorithm. As mentioned before, the library does not use its own threads nor any synchronization mechanisms, so the callbacks are called in the caller's thread. More specifically they're called in the thread that called the feed_data_stream0 or feed_data_stream1 functions. Any heavy computations will also be called as a result of calling these two functions.

Dependencies
------------

To build the library no third party dependencies are necessary.

Compiling the unit and functional test will require the installation of the gtest testing framework.

Build
===========

To download and compile the latest version of the library issue the following commands:

~~~~~~~~~~~~~{.sh}
git  clone -b integration https://github.com/inteliclinic/neuroon-alg-core/
cd neuroon-alg-core
mkdir build
cd build
cmake .. -Ddesktop_build=OFF
make
~~~~~~~~~~~~~

The -Ddesktop_build parameter passed to cmake command controls whether the entirety of the project is to be built. For mobile builds it is advised to pass -Ddesktop_build=OFF in order to build just the library in its most basic form without any additions. Using the parameter ON (also the default value) will result in attempting to generate makefiles also for the tests and other tools which will require further dependencies to be available.


Usage example
=============

This section provides a basic usage example for the neuroon-alg-core. For the use in high-level programming languages such as C#, Objective-C, Python or Java using the library will obviously require some kind of a C-language interface such as JNI for java. This manual provides a usage example in C++.

First, we have to link to the library, include the proper header file and provide the required callback functions:

~~~~~~~~~~~~~{.cpp}
#include <iostream>
#include <NeuroonAlgCoreApi.h>

/** 
 * The callback for receiving the online staging data.
 * Will print the entire staging up to the current point in time
 * to the standard output.
 */
void staging_callback(const staging_element_t* stages, int size) {
	for (int i = 0; i != size; ++i) {
		cout << stages[i].timestamp << " " << static_cast<int>(stages[i].stage)
		     << " " << static_cast<int>(stages[i].signal_quality)
		     << " " << stages[i].brain_waves.delta
		     << " " << stages[i].brain_waves.theta
		     << " " << stages[i].brain_waves.alpha
		     << " " << stages[i].brain_waves.beta
		     << std::endl;
	}
}

/**
 * The callback for receiving the real-time presentation data
 */
void presentation_callback(const brain_wave_levels_t* brain_waves, int bw_size, double hr, const double* pulseoximetry, int po_size) {
     	std::cout << "Real-time presentation callback received" << std::endl;
     	// Here we could for example copy the values of
        // the arrays and trigger the code repainting the plots of
        // brain waves, pulse plot and heart rate indicator
}

/**
 *
 *
 */
void logger_callback(const char* message) {
	std::cout << message << std::endl;
}

~~~~~~~~~~~~~


The next step is the initialization of the library:

~~~~~~~~~~~~{.cpp}
NeuroonAlgCoreData* neuroon = initialize_neuroon_alg_core(staging_callback, presentation_callback);
install_log_callback(neuroon, logger_callback);
~~~~~~~~~~~~

The value returned by the initialization function is a pointer to the private data of the library and it is required to be passed to every other API call to it. The library's client should save that pointer.

When the user of the mask starts the sleep and the mask starts sending data through the BLE characteristics, the client of the library should call the start function sleep to prepare the library for receiving data:
~~~~~~~~~~~~{.cpp}
start_sleep(neuroon);
~~~~~~~~~~~~

When receiving data from the Bluetooth Low Energy interface we can feed it to the library so it'll handle parsing the frame and analyse the data:
~~~~~~~~~~~~{.cpp}
char bytes[20]; // normally this array would contain valid data received from BLE interface
feed_data_stream0 (neuroon, bytes, 20);
~~~~~~~~~~~~

the same applies to the data from the second BLE characteristic:

~~~~~~~~~~~~{.cpp}
char bytes[20]; // normally this array would containg valid data received from BLE interface
feed_data_stream1(neuroon, bytes, 20);
~~~~~~~~~~~~

These calls may (but don't have to!) trigger calls to the staging_callback provided by the library's client. The current implementation will trigger the callback once every 82 seconds of sleep data received. However, the frequency of receiving callbacks may change in the future.

When the user of the application stops sleeping and the mask stops sending the BLE data frames the library caller should call the stop_sleep function to receive the final data and reset the library to its default state:

~~~~~~~~~~~~{.cpp}
stop_sleep(neuroon);
~~~~~~~~~~~~

This call should trigger the staging_callback with the final sleep staging results.


Real-time presentation
----------------------

Another feature of the library is preprocessing the data provided by the mask so that it can be shown in real-time. This real-time presentation mode can be activated using the API call:
~~~~~~~~~~~~{.cpp}
start_presentation(neuroon);
~~~~~~~~~~~~

After that the library will start to preprocess the data to be shown to the user. The results of this preprocessing will be returned to the API caller by the presentation_callback. This mode may be CPU-intensive. To stop it one should call:

~~~~~~~~~~~~{.cpp}
stop_presentation(neuroon);
~~~~~~~~~~~~

The complete deinitialization of the library can be achieved issuing:

~~~~~~~~~~~~{.cpp}
destroy_neuroon_alg_core(neuroon);
~~~~~~~~~~~~

This will result in releasing all the resources handled by the library and invalidating the access token provided to the call.


    