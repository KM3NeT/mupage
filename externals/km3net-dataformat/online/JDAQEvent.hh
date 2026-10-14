#ifndef __JDAQEVENT__
#define __JDAQEVENT__

#include <ostream>
#include <iomanip>
#include <vector>

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQPreamble.hh"
#include "km3net-dataformat/online/JDAQEventHeader.hh"
#include "km3net-dataformat/online/JDAQHit.hh"
#include "km3net-dataformat/online/JDAQKeyHit.hh"
#include "km3net-dataformat/online/JDAQTriggeredHit.hh"
#include "km3net-dataformat/online/JDAQPrint.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  typedef  JDAQKeyHit             JDAQSnapshotHit;


  /**
   * DAQ Event.
   */
  class JDAQEvent :
    public JDAQPreamble,
    public JDAQEventHeader
  {
  public:

    using JDAQTriggerMask::getTriggerMask;

    friend size_t getSizeof(const JDAQEvent&);
    friend JReader& operator>>(JReader&, JDAQEvent&);
    friend JWriter& operator<<(JWriter&, const JDAQEvent&);

    /**
     * Default constructor.
     */
    JDAQEvent() :
      JDAQPreamble(JDAQType<JDAQEvent>()),
      JDAQEventHeader()
    {}


    /**
     * Constructor.
     *
     * \param  header         header
     */
    JDAQEvent(const JDAQEventHeader& header) :
      JDAQPreamble(JDAQType<JDAQEvent>()),
      JDAQEventHeader(header)
    {}


    /**
     * Template const_iterator
     */
    template<class T> 
    class const_iterator : 
      public std::vector<T>::const_iterator
    {
    public:
      /**
       * Default constructor.
       */
      const_iterator() :
	std::vector<T>::const_iterator()
      {}

      
      /**
       * Copy constructor.
       *
       * \param  i              iterator
       */
      const_iterator(const typename std::vector<T>::const_iterator& i) :
	std::vector<T>::const_iterator(i)
      {}
    };


    /**
     * Template const_reverse_iterator
     */
    template<class T> 
    class const_reverse_iterator : 
      public std::vector<T>::const_reverse_iterator
    {
    public:
      /**
       * Default constructor.
       */
      const_reverse_iterator() :
	std::vector<T>::const_reverse_iterator()
      {}
      

      /**
       * Copy constructor.
       *
       * \param  i              iterator
       */
      const_reverse_iterator(const typename std::vector<T>::const_reverse_iterator& i) :
	std::vector<T>::const_reverse_iterator(i)
      {}
    };


    /**
     * Get container with hits.
     *
     * \return                container with hits
     */
    template<class T>
    const std::vector<T>& getHits() const;


    /**
     * Get container with hits.
     *
     * \return                container with hits
     */
    template<class T>
    std::vector<T>& getHits();


    /**
     * Get begin of data.
     *
     * \return                begin of data
     */
    template<class T>
    const_iterator<T> begin() const;


    /**
     * Get end of data.
     *
     * \return                end of data
     */
    template<class T>
    const_iterator<T> end() const;


    /**
     * Get reverse begin of data.
     *
     * \return                begin of data
     */
    template<class T>
    const_reverse_iterator<T> rbegin() const;


    /**
     * Get reverse end of data.
     *
     * \return                end of data
     */
    template<class T>
    const_reverse_iterator<T> rend() const;


    /**
     * Get number of hits.
     *
     * \return                number of hits
     */
    template<class T>
    unsigned int size() const;


    /**
     * Check emptyness of hit container.
     *
     * \return                true if empty; else false
     */
    template<class T>
    bool empty() const;


    /**
     * Clear event.
     */
    void clear()
    {
      snapshotHits .clear();
      triggeredHits.clear();
    }


    /**
     * Add hit.
     *
     * \param  hit            hit
     */
    void push_back(const JDAQKeyHit& hit)
    {
      snapshotHits.push_back(hit);
    }


    /**
     * Add hit.
     *
     * \param  hit            hit
     */
    void push_back(const JDAQTriggeredHit& hit)
    {
      triggeredHits.push_back(hit);
    }


    /**
     * Get trigger mask of given hit.
     *
     * \param  hit            hit
     * \return                trigger mask
     */
    static JTriggerMask_t getTriggerMask(const JDAQTriggeredHit& hit) 
    {
      return hit.getTriggerMask();
    }


    /**
     * Get trigger mask of given hit.
     *
     * \param  hit            hit
     * \return                trigger mask
     */
    JTriggerMask_t getTriggerMask(const JDAQSnapshotHit& hit) const
    {
      for (std::vector<JDAQTriggeredHit>::const_iterator i = triggeredHits.begin(); i != triggeredHits.end(); ++i) {
	if (*i == hit) {
	  return i->getTriggerMask();
	}
      }

      return JTriggerMask_t(0);
    }


    /**
     * Print DAQ Event.
     *
     * \param  out         output stream
     * \param  lpr         long print
     * \return             output stream
     */
    std::ostream& print(std::ostream& out, const bool lpr = false) const;


    ClassDef(JDAQEvent,4);


  protected:
    std::vector<JDAQTriggeredHit> triggeredHits;
    std::vector<JDAQSnapshotHit>  snapshotHits;
  };


  /**
   * Equal operator for DAQ events.
   *
   * \param  first     event
   * \param  second    event
   * \result           true if first event equal to second; else false
   */
  bool operator==(const JDAQEvent& first,
		  const JDAQEvent& second);
  

  /**
   * Not-equal operator for DAQ events.
   *
   * \param  first     event
   * \param  second    event
   * \result           true if first event not equal to second; else false
   */
  inline bool operator!=(const JDAQEvent& first,
			 const JDAQEvent& second)
  {
    return !(first == second);
  }

  
  /**
   * Print DAQ Event.
   *
   * \param  out         output stream
   * \param  event       event
   * \return             output stream
   */
  inline std::ostream& operator<<(std::ostream& out, const JDAQEvent& event)
  {
    return event.print(out, getDAQLongprint());
  }


  /**
   * Get time difference between two events.
   *
   * \param  first       event
   * \param  second      event
   * \result             time difference [s]
   */
  inline double getTimeDifference(const JDAQEvent& first, const JDAQEvent& second);
}

#endif
