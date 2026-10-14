#ifndef __JDAQ__
#define __JDAQ__

#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \file
 *
 * KM3NeT DAQ constants, bit handling, etc.
 * \author mdejong
 */


/**
 * %KM3NeT DAQ data structures and auxiliaries.
 */
namespace KM3NETDAQ {

#define KM3NET  1
#define ANTARES 2

#if NAMESPACE == ANTARES  
  static const int NUMBER_OF_PMTS          =  3;    //!< Total number of PMTs in module
#else
  static const int NUMBER_OF_PMTS          = 31;    //!< Total number of PMTs in module
#endif

#undef KM3NET
#undef ANTARES

  
  /**
   * Auxiliary data structure for single bit.
   */
  struct JBit {
    /**
     * Default constructor.
     */
    JBit() :
      bit(0)
    {}


    /** 
     * Constructor.
     *
     * \param  __bit        bit [0, 31]
     */
    JBit(int __bit) :
      bit(__bit)
    {
      if (bit < 0 || bit > 31) {
	throw JDAQException("JBit: illegal bit range.");
      }
    }


    /**
     * Get bit mask.
     *
     * In the returned mask, the single bit at <tt>bit</tt> set to 1.
     *
     * \return              bit mask
     */
    int get() const
    {
      return 1 << bit;
    }


    /**
     * Set bit in given bit mask.
     *
     * \param  mask         bit mask (I/O)
     */
    void set(int& mask) const
    {
      mask |=  get();
    }


    /**
     * Unset bit in given bit mask.
     *
     * \param  mask         bit mask (I/O)
     */
    void unset(int& mask) const
    {
      mask &= ~get();
    } 


    /**
     * Set bit in given bit mask.
     *
     * \param  mask         bit mask (I/0)
     * \param  value        bit status
     */
    void set(int& mask, const bool value) const
    {
      if (value)
	set  (mask);
      else
	unset(mask);
    }


    /**
     * Write given value as bit mask.
     *
     * \param  value        value
     * \return              bit mask
     */
    int write(const int value) const
    {
      return (value << bit) & get();
    }


    /**
     * Read given bit mask as value.
     *
     * \param  mask         bit mask
     * \return              value
     */
    int read(const int mask) const
    {
      return (mask & get()) >> bit;
    }


    /**
     * Test bit.
     *
     * \param  mask         bit mask
     * \return              true if bit set; else false
     */
    bool has(const int mask) const
    {
      return (mask & get()) != 0;
    } 


    int bit;        //!< bit
  };


  /**
   * Auxiliary data structure for range of bits.
   */
  struct JBits {
    /**
     * Default constructor.
     */
    JBits() :
      lsb(0),
      msb(0)
    {}


    /** 
     * Constructor.
     *
     * \param  __lsb        least significant bit [0,   31]
     * \param  __msb        most  significant bit [lsb, 31]
     */
    JBits(int __lsb, int __msb) :
      lsb(__lsb),
      msb(__msb)
    {
      if (lsb < 0   || lsb > 31 ||
	  msb < lsb || msb > 31) {
	throw JDAQException("JBits: illegal bit range.");
      }
    }


    /**
     * Get bit mask.
     *
     * In the returned mask, the bits from <tt>lsb</tt> (included) to <tt>msb</tt> (included) are set to 1.
     *
     * \return              bit mask
     */
    int get() const
    {
      static const unsigned int mask[] = { 0x00000001,
					   0x00000003,
					   0x00000007,
					   0x0000000F,
					   
					   0x0000001F,
					   0x0000003F,
					   0x0000007F,
					   0x000000FF,
				  
					   0x000001FF,
					   0x000003FF,
					   0x000007FF,
					   0x00000FFF,
					   
					   0x00001FFF,
					   0x00003FFF,
					   0x00007FFF,
					   0x0000FFFF,
					   
					   0x0001FFFF,
					   0x0003FFFF,
					   0x0007FFFF,
					   0x000FFFFF,
					   
					   0x001FFFFF,
					   0x003FFFFF,
					   0x007FFFFF,
					   0x00FFFFFF,
					   
					   0x01FFFFFF,
					   0x03FFFFFF,
					   0x07FFFFFF,
					   0x0FFFFFFF,
					   
					   0x1FFFFFFF,
					   0x3FFFFFFF,
					   0x7FFFFFFF,
					   0xFFFFFFFF };

      return (int) (mask[msb-lsb] << lsb);
    }


    /**
     * Write given value as bit mask.
     *
     * \param  value        value
     * \return              bit mask
     */
    int write(const int value) const
    {
      return (value << lsb) & get();
    }


    /**
     * Read given bit mask as value.
     *
     * \param  mask         bit mask
     * \return              value
     */
    int read(const int mask) const
    {
      return (mask & get()) >> lsb;
    }


    /**
     * Test bit mask.
     *
     * \param  mask         bit mask
     * \return              true if at least one of the bits is set; else false
     */
    bool has(const int mask) const
    {
      return (get() & mask) != 0;
    } 


    int lsb;        //!< least significant bit
    int msb;        //!< most  significant bit
  };


  static const JBit   DAQ_WHITE_RABBIT        (31);       //!< White Rabbit status
  static const JBits  DAQ_TDC                 ( 0, 30);   //!< TDC  high-rate veto status
  static const JBits  DAQ_FIFO                ( 0, 30);   //!< FIFO almost full bits

  static const JBit   DAQ_UDP_TRAILER         (31);       //!< UDP trailer
  static const JBits  DAQ_UDP_RECEIVED_PACKETS( 0, 15);   //!< Mask of UDP received packets
  static const JBits  DAQ_UDP_SEQUENCE_NUMBER (16, 31);   //!< Mask of UDP sequence number
}

#endif
