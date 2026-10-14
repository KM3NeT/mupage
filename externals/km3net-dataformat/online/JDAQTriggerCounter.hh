#ifndef __JDAQTRIGGERCOUNTER__
#define __JDAQTRIGGERCOUNTER__

#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  /**
   * Type definition of trigger counter.
   */
  typedef unsigned long long int                            JTriggerCounter_t;


  /**
   * Auxiliary class to count triggers.
   */
  class JDAQTriggerCounter {
  private:
    /**
     * Constructor.
     *
     * \param  counter        counter value
     */
    JDAQTriggerCounter(const JTriggerCounter_t counter)
    {
      this->trigger_counter = counter;
    }


  public:

    friend size_t getSizeof<JDAQTriggerCounter>();
    friend JReader& operator>>(JReader&, JDAQTriggerCounter&);
    friend JWriter& operator<<(JWriter&, const JDAQTriggerCounter&);

    /**
     * Default constructor.
     *
     * This constructor increments the counter of the unique instance
     * of this class and stores the result in this object.
     */
    JDAQTriggerCounter()
    {
      trigger_counter = getInstance().next();
    }


    /**
     * Virtual destructor.
     */
    virtual ~JDAQTriggerCounter()
    {}


    /**
     * Get reference to unique instance of this class object.
     *
     * \return               reference to this class object
     */
    static JDAQTriggerCounter& getInstance()
    {
      static JDAQTriggerCounter counter(0);

      return counter;
    }


    /**
     * Get DAQ trigger counter.
     *
     * \return                DAQ trigger counter
     */
    const JDAQTriggerCounter& getDAQTriggerCounter() const
    {
      return static_cast<const JDAQTriggerCounter&>(*this);
    }


    /**
     * Reset counter of unique instance of this class object.
     */
    static void reset()
    {
      getInstance().trigger_counter = 0;
    }


    /**
     * Get trigger counter.
     *
     * \return                counter value
     */
    JTriggerCounter_t getCounter() const
    {
      return trigger_counter;
    }


    /**
     * Set trigger counter.
     *
     * \param  counter        counter value
     */
    void setCounter(const JTriggerCounter_t counter)
    {
      this->trigger_counter = counter;
    }


    /**
     * Increment trigger counter.
     *
     * \return                counter value
     */
    JTriggerCounter_t next()
    {
      return trigger_counter++;
    }


    ClassDef(JDAQTriggerCounter,1);


  protected:
    JTriggerCounter_t trigger_counter;
  };


  /**
   * Equal operator for DAQ trigger counters.
   *
   * \param  first     trigger counter
   * \param  second    trigger counter
   * \result           true if first trigger counter equal to second; else false
   */
  inline bool operator==(const JDAQTriggerCounter& first,
			 const JDAQTriggerCounter& second)
  {
    return (first.getCounter() == second.getCounter());
  }


  /**
   * Not-equal operator for DAQ trigger counters.
   *
   * \param  first     trigger counter
   * \param  second    trigger counter
   * \result           true if first trigger counter not equal to second; else false
   */
  inline bool operator!=(const JDAQTriggerCounter& first,
			 const JDAQTriggerCounter& second)
  {
    return !(first == second);
  }
}

#endif
