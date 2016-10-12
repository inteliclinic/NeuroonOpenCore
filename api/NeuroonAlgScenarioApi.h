/**
 * @file    NeuroonAlgScenarioApi.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    October, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef NEUROONALGSCENARIOAPI_H
#define NEUROONALGSCENARIOAPI_H

#include <stddef.h>

/**
 * @brief 
 */
enum e_frameValidation{
  FRAMEVALIDATION_NEXT_STEP = 0x00,
  FRAMEVALIDATION_RESEND_FRAME,
  FRAMEVALIDATION_FRAME_IS_VALID,
  FRAMEVALIDATION_FRAME_NOT_VALID
};

/**
 * @brief 
 */
//typedef enum {
  //FRAMEVALIDATION_NEXT_STEP = 0x00,
  //FRAMEVALIDATION_RESEND_FRAME,
  //FRAMEVALIDATION_FRAME_IS_VALID,
  //FRAMEVALIDATION_FRAME_NOT_VALID
//}e_frameValidation;

/**
 * @brief
 *
 * @param[in] cmd_frame
 * @param[in] cmd_len
 * @param[in] response_frame
 * @param[in] response_len
 *
 * @return
 */
e_frameValidation response_sink(char *cmd_frame, size_t cmd_len, char *response_frame, size_t response_len);

#endif /* !NEUROONALGSCENARIOAPI_H */
