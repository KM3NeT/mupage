#ifndef __JDAQSUPERFRAME__
#define __JDAQSUPERFRAME__

#include "km3net-dataformat/online/JDAQ.hh"
#include "km3net-dataformat/online/JDAQPreamble.hh"
#include "km3net-dataformat/online/JDAQSuperFrameHeader.hh"
#include "km3net-dataformat/online/JDAQFrame.hh"
#include "km3net-dataformat/online/JDAQHit.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * Data frame of one optical module.
   */
  class JDAQSuperFrame :
    public JDAQPreamble,
    public JDAQSuperFrameHeader,
    public JDAQFrame
  {
  public:

    typedef  JDAQFrame::const_iterator           const_iterator;
    typedef  JDAQFrame::iterator                 iterator;
    typedef  JDAQFrame::const_reverse_iterator   const_reverse_iterator;
    typedef  JDAQFrame::reverse_iterator         reverse_iterator;

    friend size_t getSizeof(const JDAQSuperFrame&);
    friend JReader& operator>>(JReader&, JDAQSuperFrame&);
    friend JWriter& operator<<(JWriter&, const JDAQSuperFrame&);

    /**
     * Default constructor.
     */
    JDAQSuperFrame() :
      JDAQPreamble(JDAQType<JDAQSuperFrame>()),
      JDAQSuperFrameHeader(),
      JDAQFrame()
    {}


    /**
     * Constructor.
     *
     * \param  header         JDAQ super frame header
     */
    JDAQSuperFrame(const JDAQSuperFrameHeader& header) :
      JDAQPreamble(JDAQType<JDAQSuperFrame>()),
      JDAQSuperFrameHeader(header),
      JDAQFrame()
    {}


    /**
     * Copy constructor.
     *
     * \param  super_frame    JDAQ super frame
     */
    JDAQSuperFrame(const JDAQSuperFrame& super_frame) :
      JDAQPreamble(JDAQType<JDAQSuperFrame>()),
      JDAQSuperFrameHeader(super_frame.getDAQSuperFrameHeader()),
      JDAQFrame(super_frame)
    {}


    /**
     * Add data from same optical module.
     *
     * \param  super_frame    JDAQ super frame
     * \return                this JDAQ super frame
     */
    JDAQSuperFrame& add(const JDAQSuperFrame& super_frame) 
    {
      if (this->getModuleID() != super_frame.getModuleID())
	throw JDAQException("JDAQSuperFrame::add(): Different module identifiers.");

      return add(static_cast<const JDAQFrame&>(super_frame));
    }


    /**
     * Add hit.
     *
     * \param  hit              JDAQ hit
     * \return                  this JDAQ frame
     */
    JDAQSuperFrame& add(const JDAQHit& hit)
    {
      return add(1, &hit);
    }


    /**
     * Add data.
     *
     * \param  frame            JDAQ frame
     * \return                  this JDAQ frame
     */
    JDAQSuperFrame& add(const JDAQFrame& frame) 
    {
      return add(frame.numberOfHits, frame.buffer);
    }


    /**
     * Add data.
     *
     * \param  number_of_hits   number of hits
     * \param  data             pointer to data
     * \return                  this data frame
     */
    JDAQSuperFrame& add(const int number_of_hits, const JDAQHit* data)
    {
      JDAQFrame::add(number_of_hits, data);

      return *this;
    }


    ClassDef(JDAQSuperFrame,3);
  };


  /**
   * Equal operator for DAQ super frames.
   *
   * \param  first     super frame
   * \param  second    super frame
   * \result           true if first super frame equal to second; else false
   */
  inline bool operator==(const JDAQSuperFrame& first,
			 const JDAQSuperFrame& second)
  {
    if (first.getDAQSuperFrameHeader() == second.getDAQSuperFrameHeader() &&
	first.size()                   == second.size()) {

      for (JDAQSuperFrame::const_iterator p = first.begin(), q = second.begin(); p != first.end(); ++p, ++q) {
	if (*p != *q) {
	  return false;
	}
      }

      return true;

    } else {

      return false;
    }
  }


  /**
   * Not-equal operator for DAQ super frames.
   *
   * \param  first     super frame
   * \param  second    super frame
   * \result           true if first super frame not equal to second; else false
   */
  inline bool operator!=(const JDAQSuperFrame& first,
			 const JDAQSuperFrame& second)
  {
    return !(first == second);
  }
}
    
#endif
