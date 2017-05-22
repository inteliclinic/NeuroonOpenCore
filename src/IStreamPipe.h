#ifndef _STREAM_PIPE__
#define _STREAM_PIPE__

class IStreamPipe {
public:
  virtual ~IStreamPipe() {}
  virtual bool passNextFrame() = 0;
  virtual bool isBroken() const = 0;
  virtual bool isDepleted() const = 0;
};

#endif
