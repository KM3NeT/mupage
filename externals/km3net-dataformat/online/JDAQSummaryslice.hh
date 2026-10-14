#ifndef __JDAQSUMMARYSLICE__
#define __JDAQSUMMARYSLICE__

#include <ostream>
#include <iomanip>
#include <vector>

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQPreamble.hh"
#include "km3net-dataformat/online/JDAQSummaryFrame.hh"
#include "km3net-dataformat/online/JDAQSummarysliceHeader.hh"
#include "km3net-dataformat/online/JDAQTimeslice.hh"
#include "km3net-dataformat/online/JDAQClock.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * Data summary slice.
   */
  class JDAQSummaryslice :
    public JDAQPreamble,
    public JDAQSummarysliceHeader,
    public std::vector<JDAQSummaryFrame>
  {
  public:

    friend size_t getSizeof(const JDAQSummaryslice&);
    friend JReader& operator>>(JReader&, JDAQSummaryslice&);
    friend JWriter& operator<<(JWriter&, const JDAQSummaryslice&);

    /**
     * Default constructor.
     */
    JDAQSummaryslice() :
      JDAQPreamble(JDAQType<JDAQSummaryslice>()),
      JDAQSummarysliceHeader(),
      std::vector<JDAQSummaryFrame>()
    {}


    /**
     * Constructor.
     *
     * \param  chronometer    DAQ chronometer
     */
    JDAQSummaryslice(const JDAQChronometer& chronometer) :
      JDAQPreamble(JDAQType<JDAQSummaryslice>()),
      JDAQSummarysliceHeader(chronometer),
      std::vector<JDAQSummaryFrame>()
    {}


    /**
     * Constructor.
     *
     * \param  timeslice      time slice
     */
    JDAQSummaryslice(const JDAQTimeslice& timeslice) :
      JDAQPreamble(JDAQType<JDAQSummaryslice>()),
      JDAQSummarysliceHeader(timeslice),
      std::vector<JDAQSummaryFrame>()
    {
      for (JDAQTimeslice::const_iterator i = timeslice.begin(); i != timeslice.end(); ++i) {
	this->push_back(JDAQSummaryFrame(*i));
      }
    }


    /**
     * Virtual destructor.
     */
    virtual ~JDAQSummaryslice()
    {}


    /**
     * Print DAQ summary.
     *
     * \param  out         output stream
     * \param  lpr         long print
     * \return             output stream
     */
    std::ostream& print(std::ostream& out, const bool lpr = false) const
    {
      using namespace std;

      out << this->ClassName() << endl;
      out << dynamic_cast<const JDAQPreamble&>   (*this) << endl;
      out << dynamic_cast<const JDAQChronometer&>(*this) << endl;
    
      for (JDAQSummaryslice::const_iterator module = this->begin(); module != this->end(); ++module) {

	out << ' ' << setw(10) << module->getModuleID();
        out << ' ' << setw(8)  << setfill('0') << hex << module->getStatus()     << dec << setfill(' ');
        out << '|' << setw(8)  << setfill('0') << hex << module->getFIFOStatus() << dec << setfill(' ');
	out << ' ' << setw(2)  << module->getUDPNumberOfReceivedPackets();
	out << ' ' << setw(2)  << module->getUDPMaximalSequenceNumber();

	if (lpr) {
	  for (int pmt = 0; pmt != NUMBER_OF_PMTS; ++pmt) {
	    out << ' ' << fixed << showpoint << setw(4) << setprecision(1) << module->getRate(pmt) * 1.0e-3;
	  }
	}

	out << endl;
      }

      return out;
    }

    
    /**
     * Action method at file open.
     *
     * \param  version   version
     */
    static void actionAtFileOpen(int version)
    {
      JDAQSummaryFrame::ROOT_IO_VERSION = version;
    }


    ClassDef(JDAQSummaryslice,6);
  };


  /**
   * Print DAQ summary.
   *
   * \param  out         output stream
   * \param  summary     summary
   * \return             output stream
   */
  inline std::ostream& operator<<(std::ostream& out, const KM3NETDAQ::JDAQSummaryslice& summary)
  {
    return summary.print(out, getDAQLongprint());
  }


  /**
   * Equal operator for DAQ summary slices.
   *
   * \param  first     summary slice
   * \param  second    summary slice
   * \result           true if first summary slice equal to second; else false
   */
  inline bool operator==(const JDAQSummaryslice& first,
			 const JDAQSummaryslice& second)
  {
    return (first.getDAQSummarysliceHeader()                         == second.getDAQSummarysliceHeader()                         &&
	    static_cast<const std::vector<JDAQSummaryFrame>&>(first) == static_cast<const std::vector<JDAQSummaryFrame>&>(second));
  }


  /**
   * Not-equal operator for DAQ summary slices.
   *
   * \param  first     summary slice
   * \param  second    summary slice
   * \result           true if first summary slice not equal to second; else false
   */
  inline bool operator!=(const JDAQSummaryslice& first,
			 const JDAQSummaryslice& second)
  {
    return !(first == second);
  }
}

#endif
