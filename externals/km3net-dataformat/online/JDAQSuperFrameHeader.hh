#ifndef __JDAQSUPERFRAMEHEADER__
#define __JDAQSUPERFRAMEHEADER__

#include "km3net-dataformat/online/JDAQ.hh"
#include "km3net-dataformat/online/JDAQHeader.hh"
#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"
#include "km3net-dataformat/online/JDAQFrameStatus.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */
namespace KM3NETDAQ {

  /**
   * DAQ super frame header.
   */
  class JDAQSuperFrameHeader :
    public JDAQHeader,
    public JDAQModuleIdentifier,
    public JDAQFrameStatus
 {
  public:

    friend size_t getSizeof<JDAQSuperFrameHeader>();
    friend JReader& operator>>(JReader&, JDAQSuperFrameHeader&);
    friend JWriter& operator<<(JWriter&, const JDAQSuperFrameHeader&);

    /**
     * Default constructor.
     */
    JDAQSuperFrameHeader() :
      JDAQHeader          (),
      JDAQModuleIdentifier(),
      JDAQFrameStatus     ()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    chronometer
     * \param  id             module identifier
     */
    JDAQSuperFrameHeader(const JDAQChronometer& chronometer,
			 const int              id) :
      JDAQHeader          (chronometer),
      JDAQModuleIdentifier(id),
      JDAQFrameStatus     (JDAQFrameStatus::getInstance())
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    chronometer
     * \param  id             module identifier
     * \param  status         frame status
     */
    JDAQSuperFrameHeader(const JDAQChronometer& chronometer,
			 const int              id,
			 const JDAQFrameStatus& status) :
      JDAQHeader          (chronometer),
      JDAQModuleIdentifier(id),
      JDAQFrameStatus     (status)
    {}


    /**
     * Get DAQ super frame header.
     *
     * \return                DAQ super frame header
     */
    const JDAQSuperFrameHeader& getDAQSuperFrameHeader() const
    {
      return static_cast<const JDAQSuperFrameHeader&>(*this);
    }


    /**
     * Set DAQ super frame header.
     *
     * \param  header         DAQ super frame header
     */
    void setDAQSuperFrameHeader(const JDAQSuperFrameHeader& header)
    {
      static_cast<JDAQSuperFrameHeader&>(*this) = header;
    }


    ClassDef(JDAQSuperFrameHeader,2);
  };


  /**
   * Equal operator for DAQ super frame headers.
   *
   * \param  first     super frame header
   * \param  second    super frame header
   * \result           true if first super frame header equal to second; else false
   */
  inline bool operator==(const JDAQSuperFrameHeader& first,
			 const JDAQSuperFrameHeader& second)
  {
    return (first.getDAQHeader()        == second.getDAQHeader()         &&
	    first.getModuleIdentifier() == second.getModuleIdentifier()  &&
	    first.getDAQFrameStatus()   == second.getDAQFrameStatus());
  }


  /**
   * Not-equal operator for DAQ super frame headers.
   *
   * \param  first     super frame header
   * \param  second    super frame header
   * \result           true if first super frame header not equal to second; else false
   */
  inline bool operator!=(const JDAQSuperFrameHeader& first,
			 const JDAQSuperFrameHeader& second)
  {
    return !(first == second);
  }
}

#endif
