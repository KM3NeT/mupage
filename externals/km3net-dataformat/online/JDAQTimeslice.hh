#ifndef __JDAQTIMESLICE__
#define __JDAQTIMESLICE__

#include <ostream>
#include <iomanip>
#include <vector>
#include <map>

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQPreamble.hh"
#include "km3net-dataformat/online/JDAQTimesliceHeader.hh"
#include "km3net-dataformat/online/JDAQSuperFrame.hh"
#include "km3net-dataformat/online/JDAQException.hh"
#include "km3net-dataformat/online/JDAQPrint.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  class JDAQEvent;
  class JDAQSummaryslice;
  

  /**
   * Data time slice.
   */
  class JDAQTimeslice :
    public JDAQPreamble,
    public JDAQTimesliceHeader,
    public std::vector<JDAQSuperFrame>
  {
  public:

    friend size_t getSizeof(const JDAQTimeslice&);
    friend JReader& operator>>(JReader&, JDAQTimeslice&);
    friend JWriter& operator<<(JWriter&, const JDAQTimeslice&);

    /**
     * Default constructor.
     */
    JDAQTimeslice() :
      JDAQPreamble(JDAQType<JDAQTimeslice>()),
      JDAQTimesliceHeader(),
      std::vector<JDAQSuperFrame>()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    DAQ chronometer
     */
    JDAQTimeslice(const JDAQChronometer& chronometer) :
      JDAQPreamble(JDAQType<JDAQTimeslice>()),
      JDAQTimesliceHeader(chronometer),
      std::vector<JDAQSuperFrame>()
    {}
    

    /**
     * Constructor.
     *
     * \param  event          DAQ event
     * \param  snapshot       use shapshot hits (else use triggered hits)
     */
    JDAQTimeslice(const JDAQEvent& event,
		  const bool       snapshot = true);


    /**
     * Constructor.
     *
     * \param  event          DAQ event
     * \param  summary        summary
     * \param  snapshot       use shapshot hits (else use triggered hits)
     */
    JDAQTimeslice(const JDAQEvent&        event,
		  const JDAQSummaryslice& summary,
		  const bool              snapshot = true);

    
    /**
     * Virtual destructor.
     */
    virtual ~JDAQTimeslice()
    {
      clear();
    }


    /**
     * Clear data.
     */
    void clear()
    {
      for (iterator i = this->begin(); i != this->end(); ++i) {
	i->clear();
      }

      std::vector<JDAQSuperFrame>::clear();
    }


    /**
     * Assignment operator.
     *
     * \param  timeslice      timeslice
     * \return                this timeslice
     */
    JDAQTimeslice& operator=(const JDAQTimeslice& timeslice)
    {
      clear();

      setDAQChronometer(timeslice.getDAQChronometer());

      for (const_iterator i = timeslice.begin(); i != timeslice.end(); ++i) {
	push_back(*i);
      }

      return *this;
    }


    /**
     * Add another timeslice.
     *
     * \param  timeslice      timeslice
     * \return                this timeslice
     */
    JDAQTimeslice& add(const JDAQTimeslice& timeslice) 
    {
      using namespace std;

      map<JDAQModuleIdentifier, int> buffer;

      for (const_iterator i = this->begin(); i != this->end(); ++i) {
	buffer[i->getModuleIdentifier()] = distance(static_cast<const JDAQTimeslice&>(*this).begin(),i);
      }

      for (JDAQTimeslice::const_iterator i = timeslice.begin(); i != timeslice.end(); ++i) {

	map<JDAQModuleIdentifier, int>::const_iterator p = buffer.find(i->getModuleIdentifier());
	
	if (p != buffer.end()) {

	  JDAQSuperFrame& frame = this->at(p->second);

	  frame.add(*i);

	  sort(frame.begin(), frame.end());

	} else {

	  this->push_back(*i);
	}
      }
      
      return *this;
    }


    /**
     * Print DAQ Timeslice.
     *
     * \param  out           output stream
     * \param  lpr           long print
     * \return               output stream
     */
    std::ostream& print(std::ostream& out, const bool lpr = false) const
    {
      using namespace std;
      
      out << this->ClassName() << endl;
      out << dynamic_cast<const JDAQPreamble&>   (*this) << endl;
      out << dynamic_cast<const JDAQChronometer&>(*this) << endl;
      
      for (JDAQTimeslice::const_iterator frame = this->begin(); frame != this->end(); ++frame) {
	
	out << ' ' << setw(10) << frame->getModuleID();
	out << ' ' << setw(6)  << frame->getLength();
	out << ' ' << setw(6)  << frame->getDataType();
	out << ' ' << setw(6)  << frame->getTimesliceStart();
	out << ' ' << setw(8)  << setfill('0') << hex << frame->getStatus()     << dec << setfill(' ');
	out << '|' << setw(8)  << setfill('0') << hex << frame->getFIFOStatus() << dec << setfill(' ');
	out << ' ' << setw(2)  << frame->getUDPNumberOfReceivedPackets();
	out << '/' << setw(2)  << frame->getUDPMaximalSequenceNumber();
	out << ' ' << setw(6)  << frame->size();
	
	if (!lpr) {
	  
	  if (!frame->empty()) {
	    
	    out << ' ' << setw(10) << frame-> begin()->getT();
	    out << " ... ";
	    out << ' ' << setw(10) << frame->rbegin()->getT();
	  }
	  
	  out << endl;
	  
	} else {
	  
	  out << endl;
	  
	  int n = 1;
	  
	  for (JDAQFrame::const_iterator hit = frame->begin(); hit != frame->end(); ++hit, ++n) {
	    out << setw(2) << (int) hit->getPMT() << ' ' 
		<< setw(8) << (int) hit->getT()   << ' ' 
		<< setw(3) << (int) hit->getToT() << (n%10 == 0 ? '\n' : ' ');
	  }

	  out << endl;
	}
      }

      return out;
    }


    ClassDef(JDAQTimeslice,4);
  };


  /**
   * Equal operator for DAQ time slices.
   *
   * \param  first     time slice
   * \param  second    time slice
   * \result           true if first time slice equal to second; else false
   */
  inline bool operator==(const JDAQTimeslice& first,
			 const JDAQTimeslice& second)
  {
    return (first.getDAQTimesliceHeader()                          == second.getDAQTimesliceHeader()                          &&
	    static_cast<const std::vector<JDAQSuperFrame>&>(first) == static_cast<const std::vector<JDAQSuperFrame>&>(second));
  }


  /**
   * Not-equal operator for DAQ time slices.
   *
   * \param  first     time slice
   * \param  second    time slice
   * \result           true if first time slice not equal to second; else false
   */
  inline bool operator!=(const JDAQTimeslice& first,
			 const JDAQTimeslice& second)
  {
    return !(first == second);
  }
  

  /**
   * Timeslice data structure for L0 data.
   */
  struct JDAQTimesliceL0 : public JDAQTimeslice { ClassDef(JDAQTimesliceL0,1); };


  /**
   * Timeslice data structure for L1 data.
   */
  struct JDAQTimesliceL1 : public JDAQTimeslice { ClassDef(JDAQTimesliceL1,1); };


  /**
   * Timeslice data structure for L2 data.
   */
  struct JDAQTimesliceL2 : public JDAQTimeslice { ClassDef(JDAQTimesliceL2,1); };


  /**
   * Timeslice data structure for SN data.
   */
  struct JDAQTimesliceSN : public JDAQTimeslice { ClassDef(JDAQTimesliceSN,1); };
  

  /**
   * Print DAQ Timeslice.
   *
   * \param  out           output stream
   * \param  timeslice     timeslice
   * \return               output stream
   */
  inline std::ostream& operator<<(std::ostream& out, const JDAQTimeslice& timeslice)
  {
    return timeslice.print(out, getDAQLongprint());
  }
}

#endif
