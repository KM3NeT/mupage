#ifndef __JDAQPRINT__
#define __JDAQPRINT__

/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * Get DAQ print option.
   *
   * \return            print option
   */
  inline bool& getDAQLongprint()
  {
    static bool option;

    return option;
  }


  /**
   * Set DAQ print option.
   *
   * \param  option     print option
   */
  inline void setDAQLongprint(const bool option)
  {
    getDAQLongprint() = option;
  }
}

#endif
