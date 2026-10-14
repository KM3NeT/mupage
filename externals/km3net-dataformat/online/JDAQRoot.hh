#ifndef __JDAQROOT__
#define __JDAQROOT__

#include <TObject.h>
#include <TBuffer.h>

#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \author mdejong
 */

namespace JIO {
  class JReader;       // forward declaration for friend
  class JWriter;       // forward declaration for friend
}

namespace KM3NETDAQ {

  /**
   * Definition of method to get size of data type.
   *
   * This method should be specialised for each desired data type with fixed length.
   *
   * \return               number of bytes
   */
  template<class T>
  inline size_t getSizeof();


  using JIO::JReader;
  using JIO::JWriter;
} 

namespace JPP {
}

using namespace KM3NETDAQ;

#endif
