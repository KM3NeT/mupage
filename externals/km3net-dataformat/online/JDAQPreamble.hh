#ifndef __JDAQPREAMBLE__
#define __JDAQPREAMBLE__

#include <ostream>
#include <iomanip>

#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQDataTypes.hh"
#include "km3net-dataformat/online/JDAQAbstractPreamble.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * DAQ preamble.
   *
   * The data type is determined by the return value of method getDataType().\n
   * This method should be overloaded so that each data type is uniquely mapped to an integer value.
   *
   * The size and type information of this object can be used for RTTI.
   */
  class JDAQPreamble :
    public JDAQAbstractPreamble,
    public TObject 
  {
  protected:
    
    /**
     * Constructor.
     *
     * \param  type           data type of derived class
     */
    template<class T>
    JDAQPreamble(const JDAQType<T>& type) :
      JDAQAbstractPreamble(type)
    {}

  public:

    friend size_t getSizeof<JDAQPreamble>();
    friend JReader& operator>>(JReader&, JDAQPreamble&);
    friend JWriter& operator<<(JWriter&, const JDAQPreamble&);

    /**
     * Default constructor.
     */
    JDAQPreamble() :
      JDAQAbstractPreamble()
    {}


    /**
     * Virtual destructor.
     */
    virtual ~JDAQPreamble()
    {}


    /**
     * Get DAQ preamble.
     *
     * This method should be used for binary I/O to get the actual data for the given object.\n
     * To this end, the following method should be overloaded for the corresponding data type.
     * <pre>
     *    size_t  getSizeof(const T&);
     * </pre>
     *
     * \param  object         object
     * \return                preamble
     */
    template<class T>
    static JDAQPreamble getDAQPreamble(const T& object)
    {
      static JDAQPreamble preamble;

      preamble.length = getSizeof(object);
      preamble.type   = KM3NETDAQ::getDataType<T>();

      return preamble;
    }


    /**
     * Print DAQ preamble.
     *
     * \param  out           output stream
     * \param  preamble      JDAQPreamble
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQPreamble& preamble)
    {
      using namespace std;

      out << setw(8) << preamble.length;
      out << ' ';
      out << setw(6) << preamble.type;

      return out;
    }


    ClassDef(JDAQPreamble,1);
  };
}

#endif
