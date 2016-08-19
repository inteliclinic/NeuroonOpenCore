#include "DataSink.h"

void SignalFrameDataSink::consume(void* dfp){
  _frame_consume_fun((SignalFrame*)dfp);
}
