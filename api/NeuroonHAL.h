/**
 * @file    NeuroonHAL.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    February, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef NEUROONHAL_H
#define NEUROONHAL_H

#ifdef __cplusplus
extern "C"{
#endif

#define NC_CHARACTERISTICS_NO 9
#define NC_CHARACTERISTIC_LEN 36

typedef char ncCharacteristicsNames[NC_CHARACTERISTICS_NO][NC_CHARACTERISTIC_LEN +1];

void ncNeuroonHalInit(ncCharacteristicsNames chars);

// NEURON CHARACTERISTICS
/*
 * Notificable characteristics
 */
int ncGetEEGCharacteristicIndex();
int ncGetPATCharacteristicIndex();
int ncGetStatusCharacteristicIndex();
int ncGetResponseCharacteristicIndex();

/*
 * Non-notificable characteristics
 */
int ncGetCMDCharacteristicIndex();

//DFU CHARACTERISTICS

/*
 * Notificable characteristics
 */
int ncGetDFUSettingsTxCharacteristicIndex();
int ncGetDFUTxCharacteristicIndex();

/*
 * Non-notificable characteristics
 */
int ncGetDFUSettingsRxCharacteristicIndex();
int ncGetDFURxCharacteristicIndex();

#ifdef __cplusplus
}
#endif

#endif /* !NEUROONHAL_H */
