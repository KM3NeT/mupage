#ifndef __JDAQHEADER__
#define __JDAQHEADER__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQChronometer.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * DAQ header.
   */
  class JDAQHeader :
    public JDAQChronometer
  {
  public:

    friend size_t getSizeof<JDAQHeader>();
    friend JReader& operator>>(JReader&, JDAQHeader&);
    friend JWriter& operator<<(JWriter&, const JDAQHeader&);

    /**
     * Default constructor.
     */
    JDAQHeader() :
      JDAQChronometer()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    chronometer
     */
    JDAQHeader(const JDAQChronometer& chronometer) :
      JDAQChronometer(chronometer)
    {}

    
    /**
     * Get DAQ header.
     *
     * \return                DAQ header
     */
    const JDAQHeader& getDAQHeader() const 
    { 
      return static_cast<const JDAQHeader&>(*this); 
    }


    /**
     * Set DAQ header.
     *
     * \param header          DAQ header
     */
    void setDAQHeader(const JDAQHeader& header)
    { 
      static_cast<JDAQHeader&>(*this) = header;
    }


    ClassDef(JDAQHeader,2);
  };


  /**
   * Equal operator for DAQ headers.
   *
   * \param  first     header
   * \param  second    header
   * \result           true if first header equal to second; else false
   */
  inline bool operator==(const JDAQHeader& first,
			 const JDAQHeader& second)
  {
    return (first.getDAQChronometer() == second.getDAQChronometer());
  }


  /**
   * Not-equal operator for DAQ headers.
   *
   * \param  first     header
   * \param  second    header
   * \result           true if first header not equal to second; else false
   */
  inline bool operator!=(const JDAQHeader& first,
			 const JDAQHeader& second)
  {
    return !(first == second);
  }
}

#endif
