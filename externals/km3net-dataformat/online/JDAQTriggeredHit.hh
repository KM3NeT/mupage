#ifndef __JDAQTRIGGEREDHIT__
#define __JDAQTRIGGEREDHIT__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"
#include "km3net-dataformat/online/JDAQHit.hh"
#include "km3net-dataformat/online/JDAQKeyHit.hh"
#include "km3net-dataformat/online/JDAQTriggerMask.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * DAQ triggered hit
   */
  class JDAQTriggeredHit :
    public JDAQKeyHit,
    public JDAQTriggerMask
  {
  public:

    friend size_t getSizeof<JDAQTriggeredHit>();
    friend JReader& operator>>(JReader&, JDAQTriggeredHit&);
    friend JWriter& operator<<(JWriter&, const JDAQTriggeredHit&);

    /**
     * Default constructor.
     */
    JDAQTriggeredHit() :
      JDAQKeyHit     (),
      JDAQTriggerMask()
    {}


    /**
     * Constructor.
     *
     * \param  ID             module identifier
     * \param  hit            PMT hit
     * \param  mask           trigger mask
     */
    JDAQTriggeredHit(const JDAQModuleIdentifier& ID,
		     const JDAQHit&              hit,
		     const JDAQTriggerMask&      mask) :
      JDAQKeyHit(ID,hit),
      JDAQTriggerMask(mask)
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JDAQTriggeredHit()
    {}


    ClassDef(JDAQTriggeredHit,2);
  };


  /**
   * Equal operator for DAQ triggered hits.
   *
   * \param  first     triggered hit
   * \param  second    triggered hit
   * \result           true if first triggered hit equal to second; else false
   */
  inline bool operator==(const JDAQTriggeredHit& first,
			 const JDAQTriggeredHit& second)
  {
    return (static_cast<const JDAQKeyHit&>     (first) == static_cast<const JDAQKeyHit&>     (second) &&
	    static_cast<const JDAQTriggerMask&>(first) == static_cast<const JDAQTriggerMask&>(second));
  }


  /**
   * Not-equal operator for DAQ triggered hits.
   *
   * \param  first     triggered hit
   * \param  second    triggered hit
   * \result           true if first triggered hit not equal to second; else false
   */
  inline bool operator!=(const JDAQTriggeredHit& first,
			 const JDAQTriggeredHit& second)
  {
    return !(first == second);
  }
}

#endif
