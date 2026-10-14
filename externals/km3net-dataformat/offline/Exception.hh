#ifndef EXCEPTION_HH_INCLUDED
#define EXCEPTION_HH_INCLUDED

#include <exception>
#include <string>
#include <ostream>
#include <sstream>
#include <functional>


/**
 * General exception
 */
class Exception : public std::exception {
public:
  /**
   * Constructor.
   *
   * \param  error    error message
   */
  Exception(const std::string& error) :
    std::exception(),
    buffer(error)
  {}


  /**
   * Destructor.
   */
  ~Exception() throw()
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
   * Print error message of JException.
   *
   * \param  out        output stream
   * \param  exception  exception
   */
  friend inline std::ostream& operator<<(std::ostream& out, const Exception& exception)
  {
    return out << exception.what();
  }

private:
  const std::string buffer;
};


/**
 * Make exception.
 *
 * \param  JException_t    exception
 * \param  A               message
 */
#ifndef MAKE_EXCEPTION
#define MAKE_EXCEPTION(JException_t, A) std::invoke( [&]() { std::ostringstream out; out << __FILE__ << ':' << __LINE__ << std::endl << A; return JException_t(out.str()); } )
#endif

/**
 * Marco for throwing exception with std::ostream compatible message.
 *
 * \param  Exception_t     exception
 * \param  A               message
 */
#ifndef THROW
#define THROW(JException_t, A) do { throw MAKE_EXCEPTION(JException_t, A); } while(0)
#endif

#endif
