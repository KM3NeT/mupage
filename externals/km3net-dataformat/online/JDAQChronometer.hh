#ifndef __JDAQCHRONOMETER__
#define __JDAQCHRONOMETER__

#include <ostream>
#include <iomanip>

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQUTCExtended.hh"
#include "km3net-dataformat/online/JDAQClock.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * DAQ chronometer.
   */
  class JDAQChronometer
  {
  public:

    friend size_t getSizeof<JDAQChronometer>();
    friend JReader& operator>>(JReader&, JDAQChronometer&);
    friend JWriter& operator<<(JWriter&, const JDAQChronometer&);

    /**
     * Default constructor.
     */
    JDAQChronometer() :
      detector_id(0),
      run        (0),
      frame_index(0),
      timeslice_start()
    {}


    /**
     * Constructor.
     *
     * \param  __detector_id        detector identifier
     * \param  __run                run number
     * \param  __frame_index        frame index
     */
    JDAQChronometer(const int __detector_id,
		    const int __run,
		    const int __frame_index) :
      detector_id(__detector_id),
      run        (__run),
      frame_index(__frame_index),
      timeslice_start(getTimeOfFrame(__frame_index))
    {}


    /**
     * Constructor.
     *
     * \param  __detector_id        detector identifier
     * \param  __run                run number
     * \param  __frame_index        frame index
     * \param  __timeslice_start    start of time slice
     */
    JDAQChronometer(const int __detector_id,
		    const int __run,
		    const int __frame_index,
		    const JDAQUTCExtended& __timeslice_start) :
      detector_id(__detector_id),
      run        (__run),
      frame_index(__frame_index),
      timeslice_start(__timeslice_start)
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JDAQChronometer()
    {}


    /**
     * Get DAQ chronometer.
     *
     * \return                DAQ chronometer
     */
    const JDAQChronometer& getDAQChronometer() const 
    { 
      return static_cast<const JDAQChronometer&>(*this); 
    }


    /**
     * Set DAQ chronometer.
     *
     * \param chronometer     DAQ chronometer
     */
    void setDAQChronometer(const JDAQChronometer& chronometer)
    { 
      static_cast<JDAQChronometer&>(*this) = chronometer;
    }


    /**
     * Get detector identifier.
     *
     * \return                detector identifier
     */
    int getDetectorID() const 
    { 
      return detector_id; 
    }


    /**
     * Get run number.
     *
     * \return                run number
     */
    int getRunNumber() const 
    { 
      return run; 
    }


    /**
     * Get frame index.
     *
     * \return                frame index
     */
    int getFrameIndex() const 
    { 
      return frame_index; 
    }
    
    
    /**
     * Get start of timeslice
     *
     * \return                timeslice start
     */
    
    JDAQUTCExtended getTimesliceStart() const
    {
      return timeslice_start;
    }
    

    /**
     * Set run number.
     *
     * \param run             run number
     */
    void setRunNumber(const int run)
    { 
      this->run = run;
    }


    /**
     * Set frame index.
     *
     * \param frame_index     frame index
     */
    void setFrameIndex(const int frame_index)
    { 
      this->frame_index = frame_index;
    }

    
    /**
     * Set timeslice start time
     *
     * \param timeslice_start timeslice start time
     */    
    void setTimesliceStart(const JDAQUTCExtended& timeslice_start)
    {
      this->timeslice_start = timeslice_start;
    }


    ClassDef(JDAQChronometer,3);


  protected:
    int detector_id;
    int run;
    int frame_index;
    JDAQUTCExtended timeslice_start;
  };


  /**
   * Print DAQ chronometer.
   *
   * \param  out           output stream
   * \param  chronometer   JDAQChronometer
   * \return               output stream
   */
  inline std::ostream& operator<<(std::ostream& out, const JDAQChronometer& chronometer)
  {
    using namespace std;

    out << setw(8) << chronometer.getDetectorID();
    out << ' ';
    out << setw(8) << chronometer.getRunNumber();
    out << ' ';
    out << setw(8) << chronometer.getFrameIndex();
    out << ' ';
    out <<            chronometer.getTimesliceStart();
    
    return out;
  }


  /**
   * Get time of last RTS in ns since start of run for a given chronometer.
   *
   * \param  chronometer  chronometer
   * \return              time [ns]
   */
  inline double getTimeOfRTS(const JDAQChronometer& chronometer)
  {
    return getTimeOfRTS(chronometer.getFrameIndex());
  }


  /**
   * Equal operator for DAQ chronometers.
   *
   * \param  first     chronometer
   * \param  second    chronometer
   * \result           true if first chronometer equal to second; else false
   */
  inline bool operator==(const JDAQChronometer& first,
			 const JDAQChronometer& second)
  {
    return (first.getDetectorID()     == second.getDetectorID()    &&
	    first.getRunNumber()      == second.getRunNumber()     &&
	    first.getFrameIndex()     == second.getFrameIndex()    &&
	    first.getTimesliceStart() == second.getTimesliceStart());
  }


  /**
   * Not-equal operator for DAQ chronometers.
   *
   * \param  first     chronometer
   * \param  second    chronometer
   * \result           true if first chronometer not equal to second; else false
   */
  inline bool operator!=(const JDAQChronometer& first,
			 const JDAQChronometer& second)
  {
    return !(first == second);
  }


  /**
   * Get time difference between two chronometers.
   *
   * \param  first       chronometer
   * \param  second      chronometer
   * \result             time difference [s]
   */
  inline double getTimeDifference(const JDAQChronometer& first, const JDAQChronometer& second)
  {
    return getTimeDifference(first.getTimesliceStart(), second.getTimesliceStart());
  }
}

#endif
