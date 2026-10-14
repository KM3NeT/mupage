#ifndef __JDAQEVENTHEADER__
#define __JDAQEVENTHEADER__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQHeader.hh"
#include "km3net-dataformat/online/JDAQTriggerCounter.hh"
#include "km3net-dataformat/online/JDAQTriggerMask.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * JDAQ event header.
   */
  class JDAQEventHeader :
    public JDAQHeader,
    public JDAQTriggerCounter,
    public JDAQTriggerMask
  {
  public:

    friend size_t getSizeof<JDAQEventHeader>();
    friend JReader& operator>>(JReader&, JDAQEventHeader&);
    friend JWriter& operator<<(JWriter&, const JDAQEventHeader&);

    /**
     * Default constructor.
     */
    JDAQEventHeader() :
      JDAQHeader(),
      JDAQTriggerCounter(),
      JDAQTriggerMask(),
      overlays(0)
    {}

    
    /**
     * Get DAQ event header.
     *
     * \return                DAQ event header
     */
    const JDAQEventHeader& getDAQEventHeader() const
    {
      return static_cast<const JDAQEventHeader&>(*this);
    }


    /**
     * Check if header is same.
     *
     * \param  header         DAQ event header
     * \return                true if event headers are same; else false
     */
    inline bool is_same(const JDAQEventHeader& header) const
    {
      return (this->getDetectorID() == header.getDetectorID() &&
	      this->getRunNumber()  == header.getRunNumber()  &&
	      this->getFrameIndex() == header.getFrameIndex() &&
	      this->getCounter()    == header.getCounter());
    }
	

    /**
     * Get number of overlays.
     *
     * \return                number of overlays
     */
    unsigned int getOverlays() const
    {
      return overlays;
    }


    ClassDef(JDAQEventHeader,3);


  protected:
    unsigned int overlays;
  };


  /**
   * Equal operator for DAQ event headers.
   *
   * \param  first     event header
   * \param  second    event header
   * \result           true if first event header equal to second; else false
   */
  inline bool operator==(const JDAQEventHeader& first,
			 const JDAQEventHeader& second)
  {
    return (first.getDAQHeader()         == second.getDAQHeader()         &&
	    first.getDAQTriggerCounter() == second.getDAQTriggerCounter() &&
	    first.getDAQTriggerMask()    == second.getDAQTriggerMask());
  }


  /**
   * Not-equal operator for DAQ event headers.
   *
   * \param  first     event header
   * \param  second    event header
   * \result           true if first event header not equal to second; else false
   */
  inline bool operator!=(const JDAQEventHeader& first,
			 const JDAQEventHeader& second)
  {
    return !(first == second);
  }
}

#endif
