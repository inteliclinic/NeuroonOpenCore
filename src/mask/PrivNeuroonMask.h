/**
 * @file    PrivNeuroonMask.h
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    November, 2016
 * @brief   Brief description
 *
 * Description
 */

#ifndef PRIVNEUROONMASK_H
#define PRIVNEUROONMASK_H

#include <map>

#define NUMBER_OF_DEVICES

class PrivNeuroonMaskDevice;

class PrivNeuroonMask{

  public:
    PrivNeuroonMask();
    void addDevice(std::string);
    std::vector<std::string> listDevices();

  private:
    std::map<std::string, std::shared_ptr<PrivNeuroonMaskDevice>> devices;
};

#endif /* !PRIVNEUROONMASK_H */
