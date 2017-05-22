#ifndef __DATA_SOURCE__
#define __DATA_SOURCE__

#include <functional>
#include <string>
#include <vector>

#include "DataSink.h"
#include "VectorView.h"

template <class T>
class IPullingDataSource : public virtual IDataSourceDelegate {
public:
  virtual T getNextValue() = 0;
  virtual bool isDepleted() const override = 0;
  virtual ~IPullingDataSource() {}
};

template <class T>
using IPullingDataSourceSp = IPullingDataSource<std::shared_ptr<T>>;

template <class T>
class IPushingDataSource : public virtual IDataSourceDelegate {
public:
  virtual ~IPushingDataSource() {}

  virtual void setSink(std::weak_ptr<IDataSink<T>>) = 0;

  virtual bool isDepleted() const override = 0;
  virtual void startStreaming() = 0;
  virtual void stopStreaming() = 0;
};

template <class T>
using IPushingDataSourceSp = IPushingDataSource<std::shared_ptr<T>>;


template <class K> class PullingSource : public IPullingDataSource<K> {
private:
  std::vector<K> _samples = {};
  std::size_t _count = 0;
  std::size_t _current = 0;

public:
  PullingSource(const std::vector<K> & samples)
    : _samples(samples), _count(samples.size()) {}

  PullingSource(const VectorView<K> & samples)
    : _samples(std::vector<K>(samples.begin(), samples.end())), _count(samples.size()) {}


  void reset() { _current = 0; }
  bool isDepleted() const override { return _current < _count; }

  K getNextValue() override {
    return !this->isDepleted() ? _samples[_current++] : nullptr;
  }

};
template <class T> class IPullBasedOfflineSource : public IPullingDataSource<T> {
private:

public:
  virtual VectorView<T> getValues(std::size_t count = 0) = 0;

  virtual void reset() = 0;

  virtual ~IPullBasedOfflineSource() {}

  PullingSource<T> getPullingDataSource() {
    return PullingSource<T>(this->getValues());
  }
};

template <class T>
using IPullBasedOfflineSourceSp = IPullBasedOfflineSource<std::shared_ptr<T>>;

#endif
