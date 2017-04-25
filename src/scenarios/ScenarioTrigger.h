/**
 * @file    ScenarioTrigger.h
 * @author  Michał Adamczyk <m.adamczyk@inteliclinic.com>
 *          Paweł Kaźmierzewski <p.kazmierzewski@inteliclinic.com>
 * @date    Apri, 2017
 * @brief   [TODO]
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

template <class T> class ScenarioTrigger : public IScenarioTrigger {
private:
  template <class K> class Wrapped : public ScenarioTrigger<K> {
  private:
    std::unique_ptr<ScenarioTrigger<T>> _wrapped = nullptr;
    std::function<T(K)> _adapter = nullptr;

  public:
    Wrapped(std::unique_ptr<ScenarioTrigger<T>> wrapped,
            std::function<T(K)> adapter)
        : _wrapped(std::move(wrapped)), _adapter(adapter) {}

    virtual void update(K args) override {
      this->_wrapped->update(this->_adapter(args));
    }

    bool isActive() const override { return this->_wrapped->isActive(); }
  };

protected:
  ScenarioTrigger() {}

public:
  virtual ~ScenarioTrigger() {}
  virtual void update(T) = 0;

  template <class K>
  static ScenarioTrigger<K> *
  WithAdapter(std::unique_ptr<ScenarioTrigger<T>> trigger,
              std::function<T(K)> adapter) {
    return new Wrapped<K>(std::move(trigger), adapter);
  }
};

template <class T> class StatelessTrigger : public ScenarioTrigger<T> {
private:
  bool t;
  std::function<bool(T)> _adapter = nullptr;

public:
  StatelessTrigger(std::function<bool(T)> input_function) {
    this->_adapter = input_function;
  }
  virtual ~StatelessTrigger() {}
  void update(T i) override { t = this->_adapter(i); }
  bool isActive() const override { return t; }
};

template <class S, class T> class StatefulTrigger : public ScenarioTrigger<T> {
private:
  bool t;
  S _state;
  std::function<bool(S &, T)> _adapter = nullptr;

public:
  StatefulTrigger(S initial_state,
                  std::function<bool(S &, T)> input_function) {
    this->_state = initial_state;
    this->_adapter = input_function;
  }
  virtual ~StatefulTrigger() {}
  void update(T i) override { t = this->_adapter(this->_state, i); }
  bool isActive() const override { return t; }
};

#endif /* !TRIGGERSCENARIO_H */
