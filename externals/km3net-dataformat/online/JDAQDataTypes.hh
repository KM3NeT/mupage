#ifndef __JDAQDATATYPES__
#define __JDAQDATATYPES__

#include "km3net-dataformat/definitions/daqdatatypes.hh"

/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * Auxiliary class for a DAQ type holder.\n
   * This class can be used to transfer a template class to a method argument.
   */
  template<class T>
  struct JDAQType {
    typedef T  data_type;
  };


  /**
   * Template definition for method returning data type.\n
   * The template argument refers to the data type for future I/O operations.
   */
  template<class T>
  inline int getDataType()
  {
    return getDataType(JDAQType<T>());
  }
 

  /**
   * Argument definition for method returning data type.\n
   * The method argument refers to the data type for future I/O operations.\n
   * This method should be overloaded for each corresponding class.
   *
   * \param  type          data type
   */
  template<class T>
  inline int getDataType(const JDAQType<T>& type);


  class  JDAQSuperFrame;
  class  JDAQSummaryFrame;
  class  JDAQTimeslice;
  struct JDAQTimesliceL0;
  struct JDAQTimesliceL1;
  struct JDAQTimesliceL2;
  struct JDAQTimesliceSN;
  class  JDAQSummaryslice;
  class  JDAQEvent;


  inline int getDataType(const JDAQType<JDAQSuperFrame>&)   { return DAQSUPERFRAME; }
  inline int getDataType(const JDAQType<JDAQSummaryFrame>&) { return DAQSUMMARYFRAME; }
  inline int getDataType(const JDAQType<JDAQTimeslice>&)    { return DAQTIMESLICE; }
  inline int getDataType(const JDAQType<JDAQTimesliceL0>&)  { return DAQTIMESLICEL0; }
  inline int getDataType(const JDAQType<JDAQTimesliceL1>&)  { return DAQTIMESLICEL1; }
  inline int getDataType(const JDAQType<JDAQTimesliceL2>&)  { return DAQTIMESLICEL2; }
  inline int getDataType(const JDAQType<JDAQTimesliceSN>&)  { return DAQTIMESLICESN; }
  inline int getDataType(const JDAQType<JDAQSummaryslice>&) { return DAQSUMMARYSLICE; }
  inline int getDataType(const JDAQType<JDAQEvent>&)        { return DAQEVENT; }
}

#endif
