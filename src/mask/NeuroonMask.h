/**
 * @file    NeuroonMask.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    November, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef NEUROONMASK_H
#define NEUROONMASK_H

#include <vector>
#include <string>

class PrivNeuroonMask;

class NeuroonMask{

  public:
    NeuroonMask();
    ~NeuroonMask();
    std::vector<std::string> listDevices();

  private:
    PrivNeuroonMask *priv;

};

#endif /* !NEUROONMASK_H */
