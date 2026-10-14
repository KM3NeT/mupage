#ifndef __JDAQPMTIDENTIFIER__
#define __JDAQPMTIDENTIFIER__

#include <istream>
#include <ostream>

#include "km3net-dataformat/online/JDAQModuleIdentifier.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * PMT identifier.
   */
  class JDAQPMTIdentifier :
    public JDAQModuleIdentifier
  {
  public:

    friend size_t getSizeof<JDAQPMTIdentifier>();
    friend JReader& operator>>(JReader&, JDAQPMTIdentifier&);
    friend JWriter& operator<<(JWriter&, const JDAQPMTIdentifier&);

    /**
     * Default constructor.
     */
    JDAQPMTIdentifier() :
      JDAQModuleIdentifier(),
      address            (0)
    {}
    

    /**
     * Constructor.
     *
     * \param  id             module identifier
     * \param  pmt_address    PMT address
     */
    JDAQPMTIdentifier(const JDAQModuleIdentifier& id,
		      const int                   pmt_address) :
      JDAQModuleIdentifier(id),
      address             (pmt_address)
    {}


    /**
     * Get PMT identifier.
     *
     * \return                PMT identifier
     */
    const JDAQPMTIdentifier& getPMTIdentifier() const 
    { 
      return *this; 
    }


    /**
     * Set PMT identifier.
     *
     * \param  pmt            PMT identifier
     */
    void setPMTIdentifier(const JDAQPMTIdentifier& pmt)
    { 
      *this = pmt;
    }


    /**
     * Get PMT identifier.
     *
     * \return                PMT identifier
     */
    int getPMTAddress() const 
    { 
      return address;
    }


    /**
     * Read PMT identifier from input.
     *
     * \param  in             input stream
     * \param  pmt            PMT identifier
     * \return                input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQPMTIdentifier& pmt)
    {
      in >> static_cast<JDAQModuleIdentifier&>(pmt);
      in >> pmt.address;

      return in;
    }


    /**
     * Write PMT identifier to output.
     *
     * \param  out           output stream
     * \param  pmt           PMT identifier
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQPMTIdentifier& pmt)
    {
      out << static_cast<const JDAQModuleIdentifier&>(pmt) << ' ';
      out << pmt.address;

      return out;
    }

 
    /**
     * Compare PMT identifiers.
     *
     * The comparison is applied to the module identifer and to the PMT address.
     * If the module identifier or PMT address is <tt>-1</tt>, the corresponding comparison evaluates to <tt>true</tt>.
     *
     * \param  first       PMT identifier
     * \param  second      PMT identifier
     * \result             true if first PMT equal second PMT; else false
     */
    static inline bool compare(const JDAQPMTIdentifier& first, const JDAQPMTIdentifier& second)
    {
      return ((first .getModuleIdentifier() == second.getModuleIdentifier() ||
	       first .getModuleIdentifier() == -1                           ||
	       second.getModuleIdentifier() == -1)                          &&
	      (first .getPMTAddress()       == second.getPMTAddress()       ||
	       first .getPMTAddress()       == -1                           ||
	       second.getPMTAddress()       == -1));
    }


    ClassDefNV(JDAQPMTIdentifier,1);


  protected:
    int      address;
  };

 
  /**
   * Less than operator for PMT identifiers.
   *
   * The less than operator is applied first to the module identifer and then to the PMT address.
   *
   * \param  first       PMT identifier
   * \param  second      PMT identifier
   * \result             true if first PMT lower than second PMT; else false
   */
  inline bool operator<(const JDAQPMTIdentifier& first, const JDAQPMTIdentifier& second)
  {
    if (first.getModuleIdentifier() == second.getModuleIdentifier())
      return first.getPMTAddress()       < second.getPMTAddress();
    else
      return first.getModuleIdentifier() < second.getModuleIdentifier();
  }

 
  /**
   * Equal operator for PMT identifiers.
   *
   * The equal operator is applied to the module identifer and to the PMT address.
   *
   * \param  first       PMT identifier
   * \param  second      PMT identifier
   * \result             true if first PMT equal second PMT; else false
   */
  inline bool operator==(const JDAQPMTIdentifier& first, const JDAQPMTIdentifier& second)
  {
    return (first.getModuleIdentifier() == second.getModuleIdentifier() &&
	    first.getPMTAddress()       == second.getPMTAddress());
  }


  /**
   * Not-equal operator for PMT identifiers.
   *
   * \param  first       PMT identifier
   * \param  second      PMT identifier
   * \result             true if first PMT identifier not equal to second; else false
   */
  inline bool operator!=(const JDAQPMTIdentifier& first,
			 const JDAQPMTIdentifier& second)
  {
    return !(first == second);
  }
}

#endif
