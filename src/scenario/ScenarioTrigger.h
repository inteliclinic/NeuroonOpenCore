/**
 * @file    ScenarioTrigger.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 *          Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    April, 2017
 * @brief   Brief description
 *
 * Description
 */

#ifndef TRIGGERSCENARIO_H
#define TRIGGERSCENARIO_H

#include "IScenarioTrigger.h"
#include "NeuroonMaskScenariosApi.h"
#include <functional>
#include <memory>
#include <queue>

#define NEW_UNIQUE(a) std::move(std::unique_ptr<a>(new a()))

template <class T> class ScenarioTrigger : public IScenarioTrigger {
private:
  template <class K> class Wrapped : public ScenarioTrigger<K> {
  private:
    std::unique_ptr<ScenarioTrigger<T>> _wrapped = nullptr;
    std::function<T(const K &)> _adapter = nullptr;

  public:
    Wrapped(std::unique_ptr<ScenarioTrigger<T>> wrapped,
            std::function<T(const K &)> adapter)
        : _wrapped(std::move(wrapped)), _adapter(adapter) {}

    virtual void update(const K &args) override {
      this->_wrapped->update(this->_adapter(args));
    }

    bool isActive() const override { return this->_wrapped->isActive(); }
  };

public:
  virtual void update(const T &) = 0;

  template <class K>
  static ScenarioTrigger<K> *WithAdapter(std::unique_ptr<ScenarioTrigger<T>> trigger,
                                     std::function<T(const K &)> adapter) {
    return new Wrapped<K>(std::move(trigger), adapter);
  }
};

template <class T> class StatelessTrigger : public ScenarioTrigger<T> {
private:
  bool t;
  std::function<bool(const T &)> _adapter = nullptr;

public:
  StatelessTrigger(std::function<bool(const T &)> input_function) {
    this->_adapter = input_function;
  }
  void update(const T &i) override { t = this->_adapter(i); }
  bool isActive() const override { return t; }
};

#endif /* !TRIGGERSCENARIO_H */
