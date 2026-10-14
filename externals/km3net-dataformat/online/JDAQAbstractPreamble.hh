#ifndef __JDAQABSTRACTPREAMBLE__
#define __JDAQABSTRACTPREAMBLE__

#include "km3net-dataformat/online/JDAQDataTypes.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author rbruijn
 */

namespace KM3NETDAQ {


  /**
   * Simple data structure for the DAQ preamble required for a correct calculation 
   * of the object size for binary I/O.
   *
   * Note that JDAQPreamble derives from this and adds I/O and ROOT functionality.
   */
  class JDAQAbstractPreamble {
  protected:
    /**
     * Constructor.
     *
     * \param  type           data type of derived class
     */
    template<class T>
    JDAQAbstractPreamble(JDAQType<T> type) :
      length(0),
      type  (KM3NETDAQ::getDataType(type))
    {}
    
  public:
    /**
     * Default constuctor
     */
    JDAQAbstractPreamble() :
      length(0),
      type  (0)
    {}
    

    /**
     * Get length.
     *
     * \return                number of bytes
     */
    int getLength() const 
    { 
      return length;
    }
    

    /**
     * Get data type.
     *
     * \return                data type
     */
    int getDataType() const 
    { 
      return type; 
    }


    ClassDefNV(JDAQAbstractPreamble,1);

  protected:
    int length;
    int type;
  };
}

#endif
