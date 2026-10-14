#ifndef __JDAQEXCEPTION__
#define __JDAQEXCEPTION__

#include <exception>
#include <ostream>


/**
 * \author mdejong
 */

namespace KM3NETDAQ {


  /**
   * General exception
   */
  class JDAQException : public std::exception { 
  public:
    /**
     * Constructor.
     *
     * \param  error    error message
     */
    JDAQException(const std::string& error) :
      std::exception(),
      buffer(error)
    {}


    /**
     * Destructor.
     */
    ~JDAQException() throw()
    {}


    /**
     * Get error message.
     *
     * \return          error message
     */
    virtual const char* what() const throw() 
    { 
      return buffer.c_str();
    }


    /**
     * Print error message of JDAQException.
     *
     * \param  out        output stream
     * \param  exception  exception
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQException& exception)
    {
      return out << exception.what();
    }

  private:
    const std::string buffer;
  };
}

#endif
