#ifndef __JDAQTIMESLICEHEADER__
#define __JDAQTIMESLICEHEADER__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQHeader.hh"


/**
 * \author mdejong
 */
namespace KM3NETDAQ {

  /**
   * JDAQ timeslice header.
   */
  class JDAQTimesliceHeader :
    public JDAQHeader
  {
  public:
    /**
     * Default constructor.
     */
    JDAQTimesliceHeader() :
      JDAQHeader()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    DAQ chronometer
     */
    JDAQTimesliceHeader(const JDAQChronometer& chronometer) :
      JDAQHeader(chronometer)
    {}


    /**
     * Get DAQ time slice header.
     *
     * \return                DAQ time slice header
     */
    const JDAQTimesliceHeader& getDAQTimesliceHeader() const
    {
      return static_cast<const JDAQTimesliceHeader&>(*this);
    }
    

    ClassDef(JDAQTimesliceHeader,2);
  };


  /**
   * Equal operator for DAQ time slice headers.
   *
   * \param  first     time slice header
   * \param  second    time slice header
   * \result           true if first timeslice header equal to second; else false
   */
  inline bool operator==(const JDAQTimesliceHeader& first,
			 const JDAQTimesliceHeader& second)
  {
    return (first.getDAQHeader() == second.getDAQHeader());
  }


  /**
   * Not-equal operator for DAQ time slice headers.
   *
   * \param  first     time slice header
   * \param  second    time slice header
   * \result           true if first time slice header not equal to second; else false
   */
  inline bool operator!=(const JDAQTimesliceHeader& first,
			 const JDAQTimesliceHeader& second)
  {
    return !(first == second);
  }
}

#endif
