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
 * @brief response function outcome
 *
 * Description
 */
typedef enum {
  FRAMEVALIDATION_NEXT_STEP = 0x00,        /**< go to next step */
  FRAMEVALIDATION_RESEND_FRAME = 0x01,     /**< resend */
  FRAMEVALIDATION_FRAME_IS_VALID = 0x02,   /**< valid */
  FRAMEVALIDATION_FRAME_NOT_VALID = 0x03   /**< not valid */
}e_frameValidation;

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
e_frameValidation response_sink(char *cmd_frame, size_t cmd_len, char *response_frame, size_t response_len);

#endif /* !NEUROONALGSCENARIOAPI_H */
