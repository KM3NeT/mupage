#ifndef __JDAQSUMMARYSLICEHEADER__
#define __JDAQSUMMARYSLICEHEADER__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQHeader.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * JDAQ summaryslice header.
   */
  class JDAQSummarysliceHeader :
    public JDAQHeader
  {
  public:
    /**
     * Default constructor.
     */
    JDAQSummarysliceHeader() :
      JDAQHeader()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    DAQ chronometer
     */
    JDAQSummarysliceHeader(const JDAQChronometer& chronometer) :
      JDAQHeader(chronometer)
    {}

    
    /**
     * Get DAQ summary slice header.
     *
     * \return                DAQ summary slice header
     */
    const JDAQSummarysliceHeader& getDAQSummarysliceHeader() const
    {
      return static_cast<const JDAQSummarysliceHeader&>(*this);
    }
    

    ClassDef(JDAQSummarysliceHeader,2);
  };


  /**
   * Equal operator for DAQ summary slice headers.
   *
   * \param  first     summary slice header
   * \param  second    summary slice header
   * \result           true if first summaryslice header equal to second; else false
   */
  inline bool operator==(const JDAQSummarysliceHeader& first,
			 const JDAQSummarysliceHeader& second)
  {
    return (first.getDAQHeader() == second.getDAQHeader());
  }


  /**
   * Not-equal operator for DAQ summary slice headers.
   *
   * \param  first     summary slice header
   * \param  second    summary slice header
   * \result           true if first summary slice header not equal to second; else false
   */
  inline bool operator!=(const JDAQSummarysliceHeader& first,
			 const JDAQSummarysliceHeader& second)
  {
    return !(first == second);
  }
}

#endif
