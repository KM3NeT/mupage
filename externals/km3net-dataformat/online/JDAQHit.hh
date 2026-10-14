#ifndef __JDAQHIT__
#define __JDAQHIT__

#include <istream>
#include <ostream>
#include <iomanip>

#ifndef __CINT__
#include <netinet/in.h>
#endif

#include "km3net-dataformat/online/JDAQ.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {
  /**
   * Hit data structure.
   *
   * N.B.
   * The size of this data structure (i.e. the value obtained with the <tt>sizeof()</tt> operator 
   * should exactly match the preset number of bytes from the DAQ system. 
   * The <tt>pragma</tt> statement is necessary to ensure the correct size of this object.
   * Furthermore, this data structure should have no virtual methods and no virtual destructor.
   * Consequently, the standard ROOT <tt>CLassDef()</tt> macro is replaced by
   * the designated <tt>ClassDefNV()</tt> macro.
   */
#pragma pack(push,1)
  class JDAQHit
  {
  public:

    typedef  unsigned char             JPMT_t;    //!< PMT channel in FPGA
    typedef  unsigned int              JTDC_t;    //!< leading edge [ns]
    typedef  unsigned char             JTOT_t;    //!< time over threshold [ns]

    friend size_t getSizeof<JDAQHit>();
    friend JReader& operator>>(JReader&, JDAQHit&);
    friend JWriter& operator<<(JWriter&, const JDAQHit&);

    /**
     * Default constructor.
     */
    JDAQHit()
    {}


    /**
     * Constructor.
     *
     * \param  pmt_id         PMT channel
     * \param  tdc_ns         time of hit         [ns]
     * \param  tot_ns         time over threshold [ns]
     */
    JDAQHit(const JPMT_t pmt_id,
	    const JTDC_t tdc_ns,
	    const JTOT_t tot_ns) :
      pmt(pmt_id),
      tdc(htonl(tdc_ns)),
      //tdc(tdc_ns),
      tot(tot_ns)
    {}


    /**
     * Get PMT.
     *
     * \return                PMT
     */
    inline JPMT_t getPMT() const
    { 
      return pmt;
    }


    /**
     * Get time.
     *
     * \return                time [ns]
     */
    inline JTDC_t getT() const
    { 
      return ntohl(tdc);
    }


    /**
     * Get time-over-threshold.
     *
     * \return                time-over-threshold [ns]
     */
    inline JTOT_t getToT() const
    { 
      return tot;
    }


    /**
     * Get maximal time-over-threshold.
     *
     * \return                time-over-threshold [ns]
     */
    static JTOT_t getMaximalToT()
    {
      return 0xFF;
    }


    /**
     * Read DAQ hit from input.
     *
     * \param  in             input stream
     * \param  hit            hit
     * \return                input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQHit& hit)
    {
      int pmt;
      int tdc;
      int tot;

      if (in >> pmt >> tdc >> tot) {
	hit = JDAQHit(pmt, tdc, tot);
      }

      return in;
    }


    /**
     * Write DAQ hit to output.
     *
     * \param  out           output stream
     * \param  hit           hit
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQHit& hit)
    {
      using namespace std;

      out << setw(2) << (int) hit.getPMT() << ' '
	  << setw(8) << (int) hit.getT()   << ' '
	  << setw(3) << (int) hit.getToT();

      return out;
    }


    ClassDefNV(JDAQHit,2);


  protected:
    JPMT_t pmt;       //!< PMT readout channel in FPGA
    JTDC_t tdc;       //!< leading edge        [ns]
    JTOT_t tot;       //!< time over threshold [ns]
  };
#pragma pack(pop)


  /**
   * Less than operator for DAQ hits.
   *
   * The less than operator is applied first to the time and then to the PMT channel of the hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result           true if first hit earlier than second; else false
   */
  inline bool operator<(const JDAQHit& first,
			const JDAQHit& second)
  {
    if (first.getT() != second.getT())
      return first.getT()   < second.getT();
    else
      return first.getPMT() < second.getPMT();
  }


  /**
   * Equal operator for DAQ hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result           true if first hit equal to second; else false
   */
  inline bool operator==(const JDAQHit& first,
			 const JDAQHit& second)
  {
    return (first.getPMT() == second.getPMT()  &&
	    first.getT()   == second.getT()    &&
	    first.getToT() == second.getToT());
  }


  /**
   * Not-equal operator for DAQ hits.
   *
   * \param  first     hit
   * \param  second    hit
   * \result           true if first hit not equal to second; else false
   */
  inline bool operator!=(const JDAQHit& first,
			 const JDAQHit& second)
  {
    return !(first == second);
  }
}

#endif
