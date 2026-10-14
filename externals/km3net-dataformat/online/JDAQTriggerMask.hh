#ifndef __JDAQTRIGGERMASK__
#define __JDAQTRIGGERMASK__

#include <string>
#include <istream>
#include <ostream>

#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  /**
   * Type definition of trigger mask.
   */
  typedef unsigned long long int                            JTriggerMask_t;


  /**
   * Number of trigger bits.
   */
  static const unsigned int NUMBER_OF_TRIGGER_BITS = sizeof(JTriggerMask_t) * 8;
 

  /**
   * Convert trigger bit to trigger mask.
   *
   * \param  bit        trigger bit
   * \return            trigger mask
   */
  inline JTriggerMask_t getTriggerMask(const unsigned int bit)
  {
    if (bit < NUMBER_OF_TRIGGER_BITS)
      return JTriggerMask_t(1) << bit;
    else
      return 0;
  }


  /**
   * Auxiliary class for trigger mask.
   */
  class JDAQTriggerMask {
  public:

    friend size_t getSizeof<JDAQTriggerMask>();
    friend JReader& operator>>(JReader&, JDAQTriggerMask&);
    friend JWriter& operator<<(JWriter&, const JDAQTriggerMask&);

    /**
     * Default constructor.
     */
    JDAQTriggerMask() :
      trigger_mask(0)
    {}


    /**
     * Constructor.
     *
     * \param  mask           trigger mask
     */
    JDAQTriggerMask(const JTriggerMask_t mask) :
      trigger_mask(mask)
    {}


    /**
     * Constructor.
     *
     * \param  buffer         trigger bit pattern (i.e. sequence of '0' and '1').
     */
    JDAQTriggerMask(const std::string& buffer) :
      trigger_mask(valueOf(buffer).getTriggerMask())
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JDAQTriggerMask()
    {}

    
    /**
     * Get DAQ trigger mask.
     *
     * \return                DAQ trigger mask
     */
    const JDAQTriggerMask& getDAQTriggerMask() const
    {
      return static_cast<const JDAQTriggerMask&>(*this);
    }
    

    /**
     * Get trigger mask.
     *
     * \return                trigger mask
     */
    JTriggerMask_t getTriggerMask() const
    {
      return trigger_mask;
    }


    /**
     * Set trigger mask.
     *
     * \param  mask           trigger mask
     */
    void setDAQTriggerMask(const JDAQTriggerMask& mask)
    {
      trigger_mask = mask.trigger_mask;
    }


    /**
     * Set trigger mask.
     *
     * \param  mask           trigger mask
     */
    void setTriggerMask(const JTriggerMask_t mask)
    {
      trigger_mask = mask;
    }


    /** 
     * Add trigger bit pattern.
     *
     * \param  mask           trigger mask
     * \return                trigger mask
     */
    JDAQTriggerMask& addTriggerMask(const JDAQTriggerMask& mask)
    {
      trigger_mask |= mask.trigger_mask;

      return *this;
    }


    /** 
     * Has trigger bit pattern.
     *
     * \param  mask           trigger mask
     * \return                true if one of the bits is set; else false
     */
    inline bool hasTriggerMask(const JDAQTriggerMask& mask) const
    {
      return trigger_mask & mask.trigger_mask;
    }
   

    /**
     * Add trigger bit.
     *
     * \param  bit            trigger bit
     * \return                trigger mask
     */
    inline JDAQTriggerMask& addTriggerBit(const unsigned int bit)
    {
      trigger_mask |= KM3NETDAQ::getTriggerMask(bit);

      return *this;
    }


    /**
     * Check trigger bit.
     *
     * \param  bit            trigger bit
     * \return                true if bit is set; else false
     */
    inline bool hasTriggerBit(const unsigned int bit) const
    {
      return trigger_mask & KM3NETDAQ::getTriggerMask(bit);
    }


    /**
     * Extract trigger mask.
     *
     * \param  buffer         trigger bit pattern (i.e. sequence of '0' and '1').
     * \return                trigger mask
     */
    static JDAQTriggerMask valueOf(const std::string& buffer)
    {
      JDAQTriggerMask trigger_mask;

      unsigned int bit = 0;

      for (std::string::const_reverse_iterator i = buffer.rbegin(); i != buffer.rend() && bit != NUMBER_OF_TRIGGER_BITS; ++i, ++bit) {
	if (*i == '1') {
	  trigger_mask.addTriggerBit(bit);
	}
      }

      return trigger_mask;
    }


    /**
     * Convert trigger mask.
     *
     * \return                trigger bit pattern (i.e. sequence of '0' and '1').
     */
    std::string toString() const
    {
      std::string buffer(NUMBER_OF_TRIGGER_BITS, '0');

      unsigned int bit = 0;

      for (std::string::reverse_iterator i = buffer.rbegin(); i != buffer.rend() && bit != NUMBER_OF_TRIGGER_BITS; ++i, ++bit) {
	if (hasTriggerBit(bit)) {
	  *i = '1';
	}
      }

      return buffer;
    }
    

    /**
     * Read JDAQTriggerMask from input stream.
     *
     * \param  in             input stream
     * \param  mask           JDAQTriggerMask
     * \return                input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQTriggerMask& mask)
    {
      std::string buffer;
      
      if (in >> buffer) {
	mask= JDAQTriggerMask::valueOf(buffer);
      }
 
      return in;
    }


    /**
     * Write JDAQTriggerMask to output stream.
     *
     * \param  out           output stream
     * \param  mask          JDAQTriggerMask
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQTriggerMask& mask)
    {
      out << mask.toString();

      return out;
    }


    ClassDef(JDAQTriggerMask,1);


  protected:
    JTriggerMask_t  trigger_mask;
  };
  

  static const JDAQTriggerMask TRIGGER_MASK_ON   =  JDAQTriggerMask(~JTriggerMask_t(0));  //!< Trigger mask on;
  static const JDAQTriggerMask TRIGGER_MASK_OFF  =  JDAQTriggerMask( JTriggerMask_t(0));  //!< Trigger mask off;


  /**
   * Equal operator for DAQ trigger masks.
   *
   * \param  first     trigger mask
   * \param  second    trigger mask
   * \result           true if first trigger mask equal to second; else false
   */
  inline bool operator==(const JDAQTriggerMask& first,
			 const JDAQTriggerMask& second)
  {
    return (first.getTriggerMask() == second.getTriggerMask());
  }


  /**
   * Not-equal operator for DAQ trigger masks.
   *
   * \param  first     trigger mask
   * \param  second    trigger mask
   * \result           true if first trigger mask not equal to second; else false
   */
  inline bool operator!=(const JDAQTriggerMask& first,
			 const JDAQTriggerMask& second)
  {
    return !(first == second);
  }
}

#endif
