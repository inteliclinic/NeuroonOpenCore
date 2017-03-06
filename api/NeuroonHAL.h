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

#ifdef __cplusplus
}
#endif

#endif /* !NEUROONHAL_H */
