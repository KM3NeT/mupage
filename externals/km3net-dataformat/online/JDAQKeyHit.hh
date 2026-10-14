#ifndef __JDAQKEYHIT__
#define __JDAQKEYHIT__

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"
#include "km3net-dataformat/online/JDAQHit.hh"
#include "km3net-dataformat/online/JDAQPMTIdentifier.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * DAQ key hit
   */
  class JDAQKeyHit :
    public JDAQModuleIdentifier,
    public JDAQHit
  {
  public:

    friend size_t getSizeof<JDAQKeyHit>();
    friend JReader& operator>>(JReader&, JDAQKeyHit&);
    friend JWriter& operator<<(JWriter&, const JDAQKeyHit&);

    /**
     * Default constructor.
     */
    JDAQKeyHit() :
      JDAQModuleIdentifier(),
      JDAQHit()
    {}


    /**
     * Constructor.
     *
     * \param  id             module identifier
     * \param  hit            PMT    hit
     */
    JDAQKeyHit(const JDAQModuleIdentifier& id,
	       const JDAQHit&              hit) :
      JDAQModuleIdentifier(id),
      JDAQHit(hit)
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JDAQKeyHit()
    {}


    /**
     * Type conversion operator.
     *
     * \return          axis
     */
    operator JDAQPMTIdentifier () const
    {
      return JDAQPMTIdentifier(this->getModuleID(), this->getPMT());
    }


    ClassDef(JDAQKeyHit,1);
  };



  /**
   * Less than operator for DAQ hits.
   *
   * The less than operator is applied first to the module idientifier then to the PMT channel and then to the time of the hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result           true if first hit before than second; else false
   */
  inline bool operator<(const JDAQKeyHit& first,
                        const JDAQKeyHit& second)
  {
    if (first.getModuleID() == second.getModuleID()) {
      if (first.getPMT() == second.getPMT())
	return first.getT()   < second.getT();
      else
	return first.getPMT() < second.getPMT();
    } else
      return first.getModuleID() < second.getModuleID();
  }



  /**
   * Equal operator for DAQ hits.
   *
   * The equal operator is applied to the module idientifier, to the PMT channel and to the time of the hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result         t  rue if first hit equal to second; else false
   */
  inline bool operator==(const JDAQKeyHit& first,
                         const JDAQKeyHit& second)
  {
    return (first.getModuleID() == second.getModuleID() &&
	    first.getPMT()      == second.getPMT()      &&
	    first.getT()        == second.getT());
  }


  /**
   * Not-equal operator for DAQ hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result           true if first hit not equal to second; else false
   */
  inline bool operator!=(const JDAQKeyHit& first,
			 const JDAQKeyHit& second)
  {
    return !(first == second);
  }
}

#endif
