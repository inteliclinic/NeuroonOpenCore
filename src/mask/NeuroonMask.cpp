/**
 * @file    NeuroonMask.cpp
 * @Author  Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    November, 2016
 * @brief   Brief description
 *
 * Description
 */


#include <memory>
#include <string>
#include "NeuroonMask.h"
#include "PrivNeuroonMask.h"
#include "ic_frame_handle.h"
#include "ic_low_level_control.h"

/** Device section */
class PrivNeuroonMaskDevice{

  public:
    PrivNeuroonMaskDevice():
      func(FUN_TYPE_OFF),
      intensity(0),
      duration(0),
      period(0)
    {/* */}

    PrivNeuroonMaskDevice(std::string &name){
      this->name = name;
      PrivNeuroonMaskDevice();
    }

    void setFunc(e_funcType func);
    void setIntensity(uint8_t);
    void setDuration(uint32_t);
    void setPeriod(uint32_t);
    void setName(std::string &name);

    e_funcType getFunc();
    uint8_t getIntensity();
    uint32_t getDuration();
    uint32_t getPeriod();
    std::string getName();

  private:
    e_funcType func;
    uint8_t intensity;
    uint32_t duration;
    uint32_t period;
    std::string name;

};

void PrivNeuroonMaskDevice::setFunc(e_funcType func){
  this->func = func;
}

void PrivNeuroonMaskDevice::setIntensity(uint8_t intensity){
  this->intensity = intensity;
}

void PrivNeuroonMaskDevice::setDuration(uint32_t duration){
  this->duration = duration;
}

void PrivNeuroonMaskDevice::setPeriod(uint32_t period){
  this->period = period;
}

void PrivNeuroonMaskDevice::setName(std::string &name){
  this->name = name;
}

e_funcType PrivNeuroonMaskDevice::getFunc(){
  return this->func;
}

uint8_t PrivNeuroonMaskDevice::getIntensity(){
  return this->intensity;
}

uint32_t PrivNeuroonMaskDevice::getDuration(){
  return this->duration;
}

uint32_t PrivNeuroonMaskDevice::getPeriod(){
  return this->period;
}

std::string PrivNeuroonMaskDevice::getName(){
  return name;
}
/** !Device section */

NeuroonMask::NeuroonMask(){
  priv = new PrivNeuroonMask();
}

NeuroonMask::~NeuroonMask(){
  delete priv;
}

std::vector<std::string> NeuroonMask::listDevices(){
  return priv->listDevices();
}

PrivNeuroonMask::PrivNeuroonMask(){
  addDevice("rightRedLED");
  addDevice("rightGreenLED");
  addDevice("rightBlueLED");
  addDevice("leftRedLED");
  addDevice("leftGreenLED");
  addDevice("leftBlueLED");
  addDevice("powerLED");
  addDevice("vibrator");
}

void PrivNeuroonMask::addDevice(std::string deviceName){
  devices[deviceName] = std::make_shared<PrivNeuroonMaskDevice>(deviceName);
}

std::vector<std::string> PrivNeuroonMask::listDevices(){
  std::vector<std::string> tmp;
  for(auto it : devices){
    tmp.push_back(it.second->getName());
  }
  return tmp;
}
