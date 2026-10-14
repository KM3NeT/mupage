#ifndef __JDAQUTCEXTENDED__
#define __JDAQUTCEXTENDED__

/**
 * \author rbruijn
 */

#include <cstdint>
#include <istream>
#include <ostream>
#include <iomanip>
#include <limits>

#include "km3net-dataformat/online/JDAQRoot.hh"


namespace KM3NETDAQ {

  /**
   * Data structure for UTC time.
   */
  class JDAQUTCExtended 
  {
  public:

    friend size_t getSizeof<JDAQUTCExtended>();
    friend JReader& operator>>(JReader&, JDAQUTCExtended&);
    friend JWriter& operator<<(JWriter&, const JDAQUTCExtended&);


    /** 
     * Default constructor.
     */
    JDAQUTCExtended() :
      UTC_seconds(0),
      UTC_16nanosecondcycles(0)
    {}


    /**
     * Constructor.
     *
     * \param  seconds     seconds [s]
     * \param  cycles      cycles [16 ns] 
     */    
    JDAQUTCExtended(const uint32_t seconds,
		    const uint32_t cycles):
      UTC_seconds(seconds),
      UTC_16nanosecondcycles(cycles)
    {}
   

    /**
     * Constructor.
     *
     * \param  nanoseconds time [ns]
     */    
    JDAQUTCExtended(const double nanoseconds)
    {
      setTimeNanoSecond(nanoseconds);
    }


    /** 
     * Virtual destructor.
     */
    virtual ~JDAQUTCExtended()
    {}
        

    /** 
     * Get White Rabbit status.
     *
     * \return             true if okay; else false
     */
    bool getWRStatus() const
    {
      return (UTC_seconds & ~getMask()) != 0;
    }
        

    /** 
     * Get major time.
     *
     * \return             time [s]
     */
    uint32_t getUTCseconds() const
    {
      return (UTC_seconds & getMask());
    }
        

    /** 
     * Get minor time.
     *
     * \return             time [16 ns]
     */
    uint32_t getUTC16nanosecondcycles() const
    {
      return UTC_16nanosecondcycles;
    }
        

    /** 
     * Get time (limited to 16 ns cycles).
     *
     * \return             time [ns]
     */
    double getTimeNanoSecond() const
    {
      return getUTCseconds() * 1.0e9  +  getUTC16nanosecondcycles() * getTick();
    }
    

    /** 
     * Set time.
     *
     * \param  utc_ns      time [ns]
     */
    void setTimeNanoSecond(const double utc_ns)
    {
      UTC_seconds            = (uint32_t) ( utc_ns * 1.0e-9);
      UTC_16nanosecondcycles = (uint32_t) ((utc_ns - UTC_seconds*1.0e9) / getTick());
    }
    

    /** 
     * Add time.
     *
     * \param  t_ns        time [ns]
     */
    void addTimeNanoSecond(const double t_ns)
    {
      const double   x_ns  =  (double) getUTC16nanosecondcycles() * (double) getTick()  +  t_ns;
      const uint32_t t_s   =  (uint32_t) (x_ns * 1.0e-9);
      
      UTC_seconds            += t_s; 
      UTC_16nanosecondcycles  = (uint32_t) ((x_ns - t_s*1.0e9) / getTick());
    }


    /**
     * Get minimum possible value.
     *
     * \return                  minimum possible value
     */
    static JDAQUTCExtended min()
    {
      return JDAQUTCExtended(0,0);
    }


    /**
     * Get maximum possible value.
     *
     * \return                  maximum possible value
     */
    static JDAQUTCExtended max()
    {
      return JDAQUTCExtended(std::numeric_limits<uint32_t>::max(),
			     std::numeric_limits<uint32_t>::max());
    }


    /**
     * Get mask for seconds data.
     *
     * \return                  mask
     */
    static uint32_t getMask() 
    {
      return 0x7FFFFFFF;
    }


    /**
     * Get number of nano-seconds per tick.
     *
     * \return                  time [ns]
     */
    static double getTick()
    {
      return 16.0;
    }


    /**
     * Get arbitrary offset (e.g. for accuracy of time differences).
     *
     * \return                 UTC time
     */
    static const JDAQUTCExtended& getInstance() 
    {
      static JDAQUTCExtended utc(1600000000, 0);

      return utc;
    }


    /**
     * Read UTC time.
     *
     * \param  in            intput stream
     * \param  utc           UTC extended time
     * \return               intput stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQUTCExtended& utc)
    {
      in >> utc.UTC_seconds;
      in.get();
      in >> utc.UTC_16nanosecondcycles;
      
      return in;
    }


    /**
     * Write UTC time.
     *
     * \param  out           output stream
     * \param  utc           UTC extended time
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQUTCExtended& utc)
    {
      using namespace std;
      
      const char c = out.fill();

      out << setw(10) << utc.getUTCseconds();
      out << ':';
      out << setw(10) << setfill('0') << utc.getUTC16nanosecondcycles() << setfill(c);
      
      return out;
    }
    

    ClassDef(JDAQUTCExtended,1);


  protected:
    uint32_t UTC_seconds;
    uint32_t UTC_16nanosecondcycles;
  };


  /**
   * Less than operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time earlier than second UTC time; else false
   */
  inline bool operator<(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    if (first.getUTCseconds() == second.getUTCseconds())
      return first.getUTC16nanosecondcycles() < second.getUTC16nanosecondcycles();
    else
      return first.getUTCseconds() < second.getUTCseconds();
  }


  /**
   * Greater than operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time later than second UTC time; else false
   */
  inline bool operator>(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    return (second < first);
  }


  /**
   * Less than or equal operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time earlier than second UTC time; else false
   */
  inline bool operator<=(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    return !(second < first);
  }


  /**
   * Greater than or equal operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time earlier than second UTC time; else false
   */
  inline bool operator>=(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    return !(first < second);
  }

 
  /**
   * Equal operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time equal second UTC time; else false
   */
  inline bool operator==(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    return (first.getUTCseconds()            == second.getUTCseconds()            &&
	    first.getUTC16nanosecondcycles() == second.getUTC16nanosecondcycles());
  }

 
  /**
   * Not equal operator for UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             true if first UTC time not equal second UTC time; else false
   */
  inline bool operator!=(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    return !(first == second);
  }


  /**
   * Get time difference between two UTC times.
   *
   * \param  first       UTC time
   * \param  second      UTC time
   * \result             time difference [s]
   */
  inline double getTimeDifference(const JDAQUTCExtended& first, const JDAQUTCExtended& second)
  {
    const double utc_s   =  ((double) second.getUTCseconds()            - (double) first.getUTCseconds());
    const double utc_ns  =  ((double) second.getUTC16nanosecondcycles() - (double) first.getUTC16nanosecondcycles()) * JDAQUTCExtended::getTick();

    return utc_s  +  utc_ns*1.0e-9;
  }
}

#endif
