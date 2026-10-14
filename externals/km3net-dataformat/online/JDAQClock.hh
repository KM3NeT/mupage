#ifndef __JDAQCLOCK__
#define __JDAQCLOCK__

#include <istream>
#include <ostream>
#include <exception>
#include <cmath>

#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  namespace {
    static double FRAME_TIME_NS =  100000000.0;     //!< Frame time [ns]
    static double RESET_TIME_NS =  FRAME_TIME_NS;   //!< TDC dynamic range [ns]
  }


  /**
   * Auxiliary class to set DAQ system clock parameters.
   */
  class JDAQClock {
  public:
    /**
     * Clock types.
     */
    enum JDAQClock_t { KM3NET = 1, PPM_DU = 3, ANTARES = 101 };

  
    /**
     * Constructor.
     *
     * \param  clock        clock type
     */
    JDAQClock(const JDAQClock_t clock = KM3NET)
    {
      this->clock = clock;

      set();
    }


    /**
     * Get clock type.
     *
     * \return              clock type
     */
    JDAQClock_t get() const
    {
      return clock;
    }


    /**
     * Set clock type.
     *
     * \param  clock        clock type
     */
    void set(const JDAQClock_t clock)
    {
      this->clock = clock;

      set();
    }


    /**
     * Set DAQ clock parameters.
     */
    void set()
    {
      switch (this->clock) {

      case KM3NET:
	FRAME_TIME_NS =  100000000.0;
	RESET_TIME_NS =  FRAME_TIME_NS;
	break;
	
      case PPM_DU:
	FRAME_TIME_NS =  (1<<27);
	RESET_TIME_NS =  FRAME_TIME_NS;
	break;
	
      case ANTARES:
	FRAME_TIME_NS =  13107200.0 * 8;
	RESET_TIME_NS = 419430400.0;
	break;
	
      default:
	throw JDAQException("Undefined clock.");
      }
    }


    /**
     * Read clock from input.
     *
     * \param  in         input stream
     * \param  clock      JClock
     * \return            input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQClock& clock)
    {
      using namespace std;

      int clk;
	
      in >> clk;

      try {
	clock.set((JDAQClock_t) clk);
      }
      catch(const std::exception& error) {
	in.setstate(ios_base::badbit);
      }

      return in;
    }


    /**
     * Write clock to output.
     *
     * \param  out        output stream
     * \param  clock      JClock
     * \return            output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQClock& clock)
    {
      return out << clock.clock;
    }


  protected:
    JDAQClock_t clock;
  };


  /**
   * Equal operator for JDAQClock.
   *
   * \param  first        JDAQClock
   * \param  second       JDAQClock
   * \return              true if both clock types are equal; else false
   */
  inline bool operator==(const JDAQClock& first, const JDAQClock& second)
  {
    return first.get() == second.get();
  }


  /**
   * Get frame time duration.
   *
   * \return              frame time [ns]
   */
  inline double getFrameTime() 
  { 
    return FRAME_TIME_NS; 
  }


  /**
   * Get TDC dynamic range.
   *
   * \return              TDC dynamic range [ns]
   */
  inline double getRTS() 
  { 
    return RESET_TIME_NS; 
  }


  /**
   * Get start time of frame in ns since start of run for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeOfFrame(const int frame_index)
  {
    if (frame_index != 0)
      return (double) (frame_index - 1) * FRAME_TIME_NS;
    else
      return 0;
  }


  /**
   * Get start time of frame in ns since start of run for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeOfFrame(const unsigned int frame_index)
  {
    if (frame_index != 0)
      return (double) (frame_index - 1) * FRAME_TIME_NS;
    else
      return 0;
  }


  /**
   * Get time of last RTS in ns since start of run for a given time.
   *
   * \param  t_ns         time [ns]
   * \return              time [ns]
   */
  inline double getTimeOfRTS(const double t_ns)
  {
    return std::floor(t_ns/RESET_TIME_NS) * RESET_TIME_NS;
  }


  /**
   * Get time of last RTS in ns since start of run for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeOfRTS(const int frame_index)
  {
    return std::floor(getTimeOfFrame(frame_index)/RESET_TIME_NS) * RESET_TIME_NS;
  }


  /**
   * Get time of last RTS in ns since start of run for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeOfRTS(const unsigned int frame_index)
  {
    return std::floor(getTimeOfFrame(frame_index)/RESET_TIME_NS) * RESET_TIME_NS;
  }


  /**
   * Get frame index for a given time in ns.
   *
   * \param  t_ns         time [ns]
   * \return              frame index
   */
  inline int getFrameIndex(const double t_ns)
  {
    return (int) (t_ns / FRAME_TIME_NS) + 1;
  }


  /**
   * Get time in ns since last RTS for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeSinceRTS(const int frame_index)
  {
    return std::fmod(getTimeOfFrame(frame_index), RESET_TIME_NS);
  }


  /**
   * Get time in ns since last RTS for a given frame index.
   *
   * \param  frame_index  frame index
   * \return              time [ns]
   */
  inline double getTimeSinceRTS(const unsigned int frame_index)
  {
    return std::fmod(getTimeOfFrame(frame_index), RESET_TIME_NS);
  }


  /**
   * Get time in ns since last RTS for a given time in ns.
   *
   * \param  t_ns         time [ns]
   * \return              time [ns]
   */
  inline double getTimeSinceRTS(const double& t_ns)
  {
    return std::fmod(t_ns, RESET_TIME_NS);
  }
}

namespace ANTARES {
  /**
   * Set clock.
   */
  inline void setClock()
  {
    KM3NETDAQ::JDAQClock clock(KM3NETDAQ::JDAQClock::ANTARES);
  }

  using KM3NETDAQ::getFrameTime;
  using KM3NETDAQ::getRTS;
  using KM3NETDAQ::getTimeOfFrame;
  using KM3NETDAQ::getTimeOfRTS;
  using KM3NETDAQ::getFrameIndex;
  using KM3NETDAQ::getTimeSinceRTS;
}

namespace KM3NET {
  /**
   * Set clock.
   */
  inline void setClock()
  {
    KM3NETDAQ::JDAQClock clock(KM3NETDAQ::JDAQClock::KM3NET);
  }

  using KM3NETDAQ::getFrameTime;
  using KM3NETDAQ::getRTS;
  using KM3NETDAQ::getTimeOfFrame;
  using KM3NETDAQ::getTimeOfRTS;
  using KM3NETDAQ::getFrameIndex;
  using KM3NETDAQ::getTimeSinceRTS;
}

#endif
