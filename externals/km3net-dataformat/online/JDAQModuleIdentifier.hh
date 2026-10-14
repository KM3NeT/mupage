#ifndef __JDAQMODULEIDENTIFIER__
#define __JDAQMODULEIDENTIFIER__

#include <istream>
#include <ostream>

#include "km3net-dataformat/online/JDAQRoot.hh"


/**
 * \author mdejong
 */

namespace KM3NETDAQ {

  /**
   * Module identifier.
   */
  class JDAQModuleIdentifier
  {
  public:

    friend size_t getSizeof<JDAQModuleIdentifier>();
    friend JReader& operator>>(JReader&, JDAQModuleIdentifier&);
    friend JWriter& operator<<(JWriter&, const JDAQModuleIdentifier&);

    /**
     * Default constructor.
     */
    JDAQModuleIdentifier() :
      id(-1)
    {}
    

    /**
     * Constructor.
     *
     * \param  __id           module identifier
     */
    JDAQModuleIdentifier(const int __id) :
      id(__id)
    {}


    /**
     * Get Module identifier.
     *
     * \return                Module identifier
     */
    const JDAQModuleIdentifier& getModuleIdentifier() const 
    { 
      return *this; 
    }


    /**
     * Set Module identifier.
     *
     * \param  module         Module identifier
     */
    void setModuleIdentifier(const JDAQModuleIdentifier& module)
    { 
      *this = module;
    }

    
    /**
     * Get module identifier.
     *
     * \return                module identifier
     */
    int getModuleID() const 
    { 
      return id;
    }


    /**
     * Read DAQ ModuleIdentifier from input.
     *
     * \param  in             input stream
     * \param  module         module identifier
     * \return                input stream
     */
    friend inline std::istream& operator>>(std::istream& in, JDAQModuleIdentifier& module)
    {
      in >> module.id;

      return in;
    }


    /**
     * Write DAQ ModuleIdentifier to output.
     *
     * \param  out           output stream
     * \param  module        module identifier
     * \return               output stream
     */
    friend inline std::ostream& operator<<(std::ostream& out, const JDAQModuleIdentifier& module)
    {
      out << module.id;

      return out;
    }


    ClassDefNV(JDAQModuleIdentifier,1);


  protected:
    int id;
  };


  /**
   * Equal operator.
   *
   * \param  first     first  module identifier
   * \param  second    second module identifier
   * \return           true if identifiers are equal; else false
   */
  inline bool operator==(const JDAQModuleIdentifier& first, const JDAQModuleIdentifier& second)
  { 
    return first.getModuleID() == second.getModuleID(); 
  }


  /**
   * Not equal operator.
   *
   * \param  first     first  module identifier
   * \param  second    second module identifier
   * \return           true if identifiers are different; else false
   */
  inline bool operator!=(const JDAQModuleIdentifier& first, const JDAQModuleIdentifier& second)
  { 
    return first.getModuleID() != second.getModuleID();
  }


  /**
   * Equal operator.
   *
   * \param  module    module identifier
   * \param  id        identifier
   * \return           true if identifiers are equal; else false
   */
  inline bool operator==(const JDAQModuleIdentifier& module, const int id)
  { 
    return module.getModuleID() == id; 
  }


  /**
   * Not equal operator.
   *
   * \param  module    module identifier
   * \param  id        identifier
   * \return           true if identifiers are different; else false
   */
  inline bool operator!=(const JDAQModuleIdentifier& module, const int id)
  { 
    return module.getModuleID() != id; 
  }
  

  /**
   * Less than operator.
   *
   * \param  first     first  module identifier
   * \param  second    second module identifier
   * \return           true if first identifier less than second eidentifier; else false
   */
  inline bool operator<(const JDAQModuleIdentifier& first, const JDAQModuleIdentifier& second)
  { 
    return first.getModuleID() < second.getModuleID();
  }


  /**
   * Less than operator.
   *
   * \param  module    module identifier
   * \param  id        identifier
   * \return           true if module identifier less than identifier; else false
   */
  inline bool operator<(const JDAQModuleIdentifier& module, const int id)
  { 
    return module.getModuleID() < id;
  }


  /**
   * Auxiliary class to compare modules.
   */
  struct JDAQModuleComparator {
    /*
     * Default constructor.
     */
    JDAQModuleComparator()
    {}

    
    /**
     * Compare modules.
     *
     * \param  first           first  module
     * \param  second          second module
     * \return                 true if first module identifier less than that of second; else false
     */
    bool operator()(const JDAQModuleIdentifier& first, const JDAQModuleIdentifier& second) const
    {
      return first.getModuleID() < second.getModuleID();
    }
  };
}

#endif
