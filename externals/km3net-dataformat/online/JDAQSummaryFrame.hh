#ifndef __JDAQSUMMARYFRAME__
#define __JDAQSUMMARYFRAME__

#include <limits>
#include <cmath>
#include <vector>

#include "km3net-dataformat/online/JDAQException.hh"
#include "km3net-dataformat/online/JDAQ.hh"
#include "km3net-dataformat/online/JDAQClock.hh"
#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"
#include "km3net-dataformat/online/JDAQFrameStatus.hh"
#include "km3net-dataformat/online/JDAQSuperFrame.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  /**
   * Forward declaration for friend declaration of JDAQSummaryFrame inside JDAQRate.
   */
  class JDAQSummaryFrame;


  /**
   * Data storage class for rate measurement of one PMT.
   *  
   * Note that the rate value is compressed. 
   * The number of bins and corresponding abscissa values can be obtained with  
   * methods JDAQRare::getN and JDAQRate::getData, respectively. 
   */
  class JDAQRate {
  public:

    friend class JDAQSummaryFrame;


    typedef unsigned char  JRate_t;                 // type of value to store rate

    
    /**
     * Get minimal rate (below this, value is set to zero)
     *
     * \return                rate [Hz]
     */
    static double getMinimalRate()
    {
      return 2.0e3;
    }

    
    /**
     * Get maximal rate (above this, value is set to maximum)
     *
     * \return                rate [Hz]
     */
    static double getMaximalRate()
    {
      return 2.0e6;
    }
    

    /**
     * Get value.
     *
     * \param  numberOfHits   number of hits
     * \param  frameTime_ns   frame time [ns]
     * \return                value
     */
    static JRate_t getValue(const int numberOfHits, const double frameTime_ns)
    {
      return getValue(numberOfHits * 1.0e9 / frameTime_ns);
    }


    /**
     * Get value.
     *
     * \param  rate_Hz        rate [Hz]
     * \return                value
     */
    static JRate_t getValue(const double rate_Hz)
    {
      if      (rate_Hz <= getMinimalRate())
	return 0;
      else if (rate_Hz >= getMaximalRate())
	return std::numeric_limits<JRate_t>::max();
      else
	return (JRate_t) (log(rate_Hz/getMinimalRate()) / getFactor() + 0.5);
    }


    /**
     * Get count rate.
     *
     * \param  value          value
     * \return                rate [Hz]
     */
    static double getRate(const JRate_t value)
    {
      if (value == 0)
	return 0.0;
      else
	return get_rate(value);
    }


    /**
     * Get weight.
     *
     * \param  value          value
     * \return                weight [Hz^-1]
     */
    static double getWeight(const JRate_t value)
    {
      double W = 1.0;
      
      if      (value == 0)
	W = get_rate(0.5)   - getMinimalRate();
      else if (value == std::numeric_limits<JRate_t>::max())
	W = getMaximalRate() - get_rate(std::numeric_limits<JRate_t>::max() - 0.5);
      else
	W = get_rate((double) value + 0.5) - get_rate((double) value - 0.5);

      return 1.0 / W;
    }


    /**
     * Get number of bins.
     *
     * \return                number of bins
     */
    static int getN() 
    {
      return (int) std::numeric_limits<JRate_t>::max() + 1;
    }


    /**
     * Get abscissa values.
     *
     * \param  factor         scaling factor
     * \return                abscissa values
     */
    static const double* getData(const double factor = 1.0) 
    {
      static std::vector<double> buffer;

      buffer.clear();

      buffer.push_back(getMinimalRate() * factor);

      for (int i = 1; i != JDAQRate::getN(); ++i) {
	buffer.push_back(get_rate(i - 0.5) * factor);
      }
      
      buffer.push_back(getMaximalRate() * factor);

      return buffer.data();
    }
    
    
    /**
     * Default constructor.
     */
    JDAQRate() :
      value(0)
    {}

  
    /**
     * Get value.
     *
     * \return                value
     */
    JRate_t getValue() const
    {
      return value;
    }


    /**
     * Set value.
     *
     * \param  numberOfHits   number of hits
     * \param  frameTime_ns   frame time [ns]
     */
    void setValue(const int numberOfHits, const double frameTime_ns)
    {
      value = getValue(numberOfHits, frameTime_ns);
    }


    /**
     * Set value.
     *
     * \param  rate_Hz        rate [Hz]
     */
    void setValue(const double rate_Hz)
    {
      value = getValue(rate_Hz);
    }

  
    /**
     * Get count rate.
     *
     * \return                rate [Hz]
     */
    double getRate() const
    {
      return getRate(value);
    }


    /**
     * Get weight.
     *
     * \return                weight [Hz^-1]
     */
    double getWeight() const
    {
      return getWeight(value);
    }


    /** 
     * Scale rate.
     *
     * \param  factor         multiplication factor
     * \return                this rate
     */
    JDAQRate& mul(const double factor)
    {
      setValue(getRate() * factor);
      
      return *this;
    }


    /** 
     * Scale rate.
     *
     * \param  factor         multiplication factor
     * \return                this rate
     */
    JDAQRate& div(const double factor)
    {
      setValue(getRate() / factor);
      
      return *this;
    }


    /**
     * Check validity of rate.
     * The rate is considered valid if it is between the minimal and the maximal rate. 
     *
     * \return                true if valid; else false
     */
    bool is_valid() const
    {
      return (value != 0 && value != std::numeric_limits<JRate_t>::max());
    }


  protected:
    JRate_t value;
    

  private:
    /**
     * Get conversion factor.
     *
     * \return             factor
     */
    static const double getFactor()
    {
      return std::log(getMaximalRate() / getMinimalRate()) / std::numeric_limits<JRate_t>::max();
    }


    /**
     * Get count rate.
     *
     * \param  value          value
     * \return                rate [Hz]
     */
    static double get_rate(const double value)
    {
      return getMinimalRate() * std::exp(value * getFactor());
    }
  };


  /**
   * Equal operator for DAQ rates.
   *
   * \param  first     rate
   * \param  second    rate
   * \result           true if first rate equal to second; else false
   */
  inline bool operator==(const JDAQRate& first,
			 const JDAQRate& second)
  {
    return (first.getValue() == second.getValue());
  }


  /**
   * Not-equal operator for DAQ rates.
   *
   * \param  first     rate
   * \param  second    rate
   * \result           true if first rate not equal to second; else false
   */
  inline bool operator!=(const JDAQRate& first,
			 const JDAQRate& second)
  {
    return (first.getValue() != second.getValue());
  }
  

  /**
   * Data storage class for rate measurements of all PMTs in one module.
   */
  class JDAQSummaryFrame :
    public JDAQModuleIdentifier,
    public JDAQFrameStatus
  {
  public:

    typedef JDAQRate::JRate_t  JRate_t;

    friend size_t getSizeof<JDAQSummaryFrame>();
    friend JReader& operator>>(JReader&, JDAQSummaryFrame&);
    friend JWriter& operator<<(JWriter&, const JDAQSummaryFrame&);

    /**
     * Default constructor.
     */
    JDAQSummaryFrame() :
      JDAQModuleIdentifier(),
      JDAQFrameStatus()
    {}


    /**
     * Constructor.
     *
     * \param  id             module identifier
     */
    JDAQSummaryFrame(const JDAQModuleIdentifier& id) :
      JDAQModuleIdentifier(id),
      JDAQFrameStatus(JDAQFrameStatus::getInstance())
    {}


    /**
     * Constructor.
     *
     * Note that normally the rate is set to the number of hits per unit frame time
     * but if either the high-rate veto or FIFO (almost) full bit is on, 
     * the rate is set to the number of hits divided by the time of the last hit.
     *
     * \param  input          super frame 
     */
    JDAQSummaryFrame(const JDAQSuperFrame& input) :
      JDAQModuleIdentifier(input.getModuleID()),
      JDAQFrameStatus     (input.getDAQFrameStatus())
    {
      using namespace std;

      typedef JDAQHit::JPMT_t  JPMT_t;
      typedef JDAQHit::JTDC_t  JTDC_t;

      vector<int> counter(numeric_limits<JPMT_t>::max(), 0);

      int n = input.size();
      
      for (JDAQSuperFrame::const_iterator i = input.begin(); n != 0; --n, ++i) {
	++counter[i->getPMT()];
      }

      for (int i = 0; i != NUMBER_OF_PMTS; ++i) {
	data[i].setValue(counter[i], getFrameTime());
      }
      
      if (input.testHighRateVeto() || input.testFIFOStatus()) {

	// determine last hit for each PMT

	vector<JTDC_t> limit(numeric_limits<JPMT_t>::max(), 0);

	int n = input.size();

	for (JDAQSuperFrame::const_iterator i = input.begin(); n != 0; --n, ++i) {
	  if (i->getT() > limit[i->getPMT()]) {
	    limit[i->getPMT()] = i->getT();
	  }
	}
	
	for (int i = 0; i != NUMBER_OF_PMTS; ++i) {
	  if (input.testHighRateVeto(i) || input.testFIFOStatus(i)) {
	    if (limit[i] != 0) {
	      data[i].setValue((double) counter[i] * 1.0e9 / (double) limit[i]);
	    }
	  }
	}
      }
    }

  
    /**
     * Get DAQ rate of given PMT.
     *
     * \param  tdc            TDC
     * \return                JDAQRate
     */
    const JDAQRate& operator[](const int tdc) const
    {
      if (tdc >= 0 && tdc < NUMBER_OF_PMTS)
	return data[tdc];
      else
	throw JDAQException("TDC out of range.");
    }

  
    /**
     * Get DAQ rate of given PMT.
     *
     * \param  tdc            TDC
     * \return                JDAQRate
     */
    JDAQRate& operator[](const int tdc)
    {
      if (tdc >= 0 && tdc < NUMBER_OF_PMTS)
	return data[tdc];
      else
	throw JDAQException("TDC out of range.");
    }


    /**
     * Get value.
     *
     * \param  tdc            TDC
     * \return                value
     */
    JRate_t getValue(const int tdc) const
    {
      return data[tdc].getValue();
    }


    /**
     * Get count rate.
     *
     * \param  tdc            TDC
     * \param  factor         scaling factor
     * \return                rate x scaling factor [Hz]
     */
    double getRate(const int tdc, const double factor = 1.0) const
    {
      return data[tdc].getRate() * factor;
    }


    /**
     * Get weight.
     *
     * \param  tdc            TDC
     * \param  factor         scaling factor
     * \return                weight / scaling factor [Hz^-1]
     */
    double getWeight(const int tdc, const double factor = 1.0) const
    {
      return data[tdc].getWeight() / factor;
    }


    /**
     * Set count rate.
     *
     * \param  tdc            TDC
     * \param  rate_Hz        rate [Hz]
     */
    void setRate(const int tdc, const double rate_Hz)
    {
      return data[tdc].setValue(rate_Hz);
    }


    static int ROOT_IO_VERSION;     //!< Streamer version of JDAQSummaryslice as obtained from ROOT file.


    ClassDefNV(JDAQSummaryFrame,2);


  protected:

    JDAQRate data[NUMBER_OF_PMTS];
  };


  /**
   * Equal operator for DAQ summary frames.
   *
   * \param  first     summary frame
   * \param  second    summary frame
   * \result           true if first summary frame equal to second; else false
   */
  inline bool operator==(const JDAQSummaryFrame& first,
			 const JDAQSummaryFrame& second)
  {
    if (first.getModuleIdentifier() == second.getModuleIdentifier() &&
	first.getDAQFrameStatus()   == second.getDAQFrameStatus()) {

      for (int i = 0; i != NUMBER_OF_PMTS; ++i) {
	if (first[i] != second[i]) {
	  return false;
	}
      }

      return true;
      
    } else {

      return false;
    }	    
  }


  /**
   * Not-equal operator for DAQ summary frames.
   *
   * \param  first     summary frame
   * \param  second    summary frame
   * \result           true if first summary frame not equal to second; else false
   */
  inline bool operator!=(const JDAQSummaryFrame& first,
			 const JDAQSummaryFrame& second)
  {
    return !(first == second);
  }
}

#endif
