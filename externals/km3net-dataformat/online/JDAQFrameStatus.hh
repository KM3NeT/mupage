#ifndef __JDAQFRAMESTATUS__
#define __JDAQFRAMESTATUS__

#include "km3net-dataformat/online/JDAQ.hh"
#include "km3net-dataformat/online/JDAQException.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */
namespace KM3NETDAQ {

  /**
   * DAQ frame status.
   */
  class JDAQFrameStatus {
  public:

    friend size_t getSizeof<JDAQFrameStatus>();
    friend JReader& operator>>(JReader&, JDAQFrameStatus&);
    friend JWriter& operator<<(JWriter&, const JDAQFrameStatus&);

    /**
     * Default constructor.
     */
    JDAQFrameStatus() :
      daq     (0),
      status  (0),
      fifo    (0),
      status_3(0),
      status_4(0)
    {}
    

    /**
     * Constructor.
     *
     * \param  __daq          DAQ  status
     * \param  __status       TDC  status
     * \param  __fifo         FIFO status
     * \param  __status_3     spare
     * \param  __status_4     spare
     */
    JDAQFrameStatus(const int __daq,
		    const int __status,
		    const int __fifo,
		    const int __status_3 = 0,
		    const int __status_4 = 0) :
      daq     (__daq),
      status  (__status),
      fifo    (__fifo),
      status_3(__status_3),
      status_4(__status_4)
    {}
 

    /**
     * Get reference to unique instance of this class object.
     *
     * This instance has default values which correspond to a valid DAQ frame status.
     *
     * \return                reference to this class object
     */
    static const JDAQFrameStatus& getInstance()
    {
      static const JDAQFrameStatus status(DAQ_UDP_RECEIVED_PACKETS.write(2) | DAQ_UDP_SEQUENCE_NUMBER.write(1),
					  DAQ_WHITE_RABBIT.write(1),
					  DAQ_FIFO.write(0) | DAQ_UDP_TRAILER.write(1));

      return status;
    }
  

    /**
     * Get DAQ frame status.
     *
     * \return                DAQ frame status
     */
    const JDAQFrameStatus& getDAQFrameStatus() const
    {
      return static_cast<const JDAQFrameStatus&>(*this);
    }


    /**
     * Set DAQ frame status.
     *
     * \param  status         DAQ frame status
     */
    void setDAQFrameStatus(const JDAQFrameStatus& status)
    {
      static_cast<JDAQFrameStatus&>(*this) = status;
    }


    /**
     * Get DAQ status.
     *
     * \return                DAQ status
     */
    int getDAQStatus() const 
    { 
      return this->daq;
    }


    /**
     * Get TDC and White Rabbit status.
     *
     * \return                status
     */
    int getStatus() const 
    { 
      return this->status;
    }


    /**
     * Get FIFO status.
     *
     * \return                FIFO status
     */
    int getFIFOStatus() const 
    { 
      return this->fifo;
    }


    /**
     * Test DAQ status of packets.
     *
     * \return                true if okay; else false
     */
    bool testDAQStatus() const 
    { 
      return (getUDPNumberOfReceivedPackets() == (getUDPMaximalSequenceNumber() + 1)) && hasUDPTrailer();
    }


    /**
     * Test TDC and White Rabbit status.
     *
     * \return                true if okay; else false
     */
    bool testStatus() const 
    { 
      return testWhiteRabbitStatus() && testTDCStatus();
    }


    /**
     * Get number of received UDP packets.
     *
     * \return                UDP received packets
     */
    int getUDPNumberOfReceivedPackets() const 
    { 
      return DAQ_UDP_RECEIVED_PACKETS.read(this->daq);
    }


    /**
     * Get maximal sequence number of UDP packet.
     *
     * \return                UDP sequence number
     */
    int getUDPMaximalSequenceNumber() const 
    { 
      return DAQ_UDP_SEQUENCE_NUMBER.read(this->daq);
    }


    /**
     * Test White Rabbit status.
     *
     * \return                true if okay; else false
     */
    bool testWhiteRabbitStatus() const
    {
      return DAQ_WHITE_RABBIT.has(this->status);
    }


    /**
     * Test TDC status.
     *
     * \return                true if okay; else false
     */
    bool testTDCStatus() const 
    { 
      return !testHighRateVeto();
    }


    /**
     * Test high-rate veto status.
     *
     * \return                true if one of the TDCs is high-rate vetoed; else false
     */
    bool testHighRateVeto() const
    {
      return DAQ_TDC.has(this->status);
    }


    /**
     * Test high-rate veto status.
     *
     * \param  tdc            TDC
     * \return                true if TDC is high-rate vetoed; else false
     */
    bool testHighRateVeto(const int tdc) const
    {
      return JBit(tdc).has(this->status);
    }


    /**
     * Count high-rate veto status.
     *
     * \return                number of the TDCs with high-rate veto
     */
    int countHighRateVeto() const
    {
      int n = 0;

      if (testHighRateVeto()) {
	for (int pmt = 0; pmt != NUMBER_OF_PMTS; ++pmt) {
	  if (JBit(pmt).has(this->status)) {
	    ++n;
	  }
	}
      }
      
      return n;
    }


    /**
     * Test FIFO status.
     *
     * \return                true if one of the TDCs has FIFO almost full; else false
     */
    bool testFIFOStatus() const 
    { 
      return DAQ_FIFO.has(this->fifo);
    }


    /**
     * Test FIFO status.
     *
     * \param  tdc            TDC
     * \return                true if FIFO is almost full; else false
     */
    bool testFIFOStatus(const int tdc) const
    {
      return JBit(tdc).has(this->fifo);
    }


    /**
     * Count FIFO status.
     *
     * \return                number of the TDCs with FIFO almost full
     */
    int countFIFOStatus() const
    {
      int n = 0;

      if (testFIFOStatus()) {
	for (int pmt = 0; pmt != NUMBER_OF_PMTS; ++pmt) {
	  if (JBit(pmt).has(this->fifo)) {
	    ++n;
	  }
	}
      }

      return n;
    }

    /**
     * Count active channels.
     * \return                number of TDCs without high rate veto or FIFO almost full
     */
    int countActiveChannels() const
    {
      int n = NUMBER_OF_PMTS;

      if (testHighRateVeto() || testFIFOStatus()) {
	for (int pmt = 0; pmt != NUMBER_OF_PMTS; ++pmt) {
	  if ( JBit(pmt).has(this->status) || JBit(pmt).has(this->fifo) ) {
	    --n;
	  }
	}
      }

      return n;

    }


    /**
     * Get UDP trailer status.
     *
     * \return                true if UDP trailer present; else false
     */
    bool hasUDPTrailer() const
    {
      return DAQ_UDP_TRAILER.has(this->fifo);
    }


    /**
     * Set high-rate veto.
     *
     * \param  tdc            TDC
     * \param  value          value
     */
    void setHighRateVeto(const int tdc, const bool value)
    {
      JBit(tdc).set(this->status, value);
    }


    ClassDefNV(JDAQFrameStatus,1);

  protected:
    int  daq;         // DAQ  status
    int  status;      // TDC  status
    int  fifo;        // FIFO status
    int  status_3;    // spare
    int  status_4;    // spare
  };


  /**
   * Equal operator for DAQ frame status.
   *
   * \param  first     frame status
   * \param  second    frame status
   * \result           true if first frame status equal to second; else false
   */
  inline bool operator==(const JDAQFrameStatus& first,
			 const JDAQFrameStatus& second)
  {
    return (first.getDAQStatus()  == second.getDAQStatus()  &&
	    first.getStatus()     == second.getStatus()     &&
	    first.getFIFOStatus() == second.getFIFOStatus());
  }


  /**
   * Not-equal operator for DAQ frame status.
   *
   * \param  first     frame status
   * \param  second    frame status
   * \result           true if first frame status not equal to second; else false
   */
  inline bool operator!=(const JDAQFrameStatus& first,
			 const JDAQFrameStatus& second)
  {
    return !(first == second);
  }
}

#endif
