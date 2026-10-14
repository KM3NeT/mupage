#ifndef __JDAQFRAME__
#define __JDAQFRAME__

#include <stdlib.h>
#include <string.h>
#include <string>
#include <iterator>
#include <algorithm>

#include "km3net-dataformat/online/JDAQHit.hh"
#include "km3net-dataformat/online/JDAQRoot.hh"
#include "km3net-dataformat/online/JDAQException.hh"


/**
 * \author mdejong
 */
namespace KM3NETDAQ {

  /**
   * Subset of data frame.
   **/
  class JDAQFrameSubset
  {
  public:

    typedef  const JDAQHit*                         const_iterator;
    typedef  std::reverse_iterator<const_iterator>  const_reverse_iterator;


    /**
     * Constructor.
     *
     * \param  number_of_hits   number of hits
     * \param  data             pointer to data
     */
    JDAQFrameSubset(const int number_of_hits, const JDAQHit* data) :
      numberOfHits(number_of_hits),
      buffer(data)
    {}


    const_iterator begin() const { return buffer; }
    const_iterator end()   const { return buffer + numberOfHits; }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

    bool empty() const { return numberOfHits == 0; }

    int size() const { return numberOfHits; }

    const JDAQHit* data() const { return buffer; }


  private:
    const int      numberOfHits;
    const JDAQHit* buffer;
  };


  /**
   * Data frame.
   */
  class JDAQFrame
  {
  public:

    typedef  const JDAQHit*                         const_iterator;
    typedef  JDAQHit*                               iterator;
    typedef  std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef  std::reverse_iterator<iterator>        reverse_iterator;

    friend size_t getSizeof(const JDAQFrame&);
    friend JReader& operator>>(JReader&, JDAQFrame&);
    friend JWriter& operator<<(JWriter&, const JDAQFrame&);

    /**
     * Default constructor.
     */
    JDAQFrame() :
      numberOfHits(0),
      buffer(NULL)
    {}


    /**
     * Copy constructor.
     *
     * \param  frame            frame
     */
    JDAQFrame(const JDAQFrame& frame) :
      numberOfHits(0),
      buffer(NULL)
    {
      add(frame);
    }


    /**
     * Move constructor.
     *
     * \param  frame            frame
     */
    JDAQFrame(JDAQFrame&& frame) :
      numberOfHits(frame.numberOfHits),
      buffer      (frame.buffer)
    {
      frame.numberOfHits = 0;
      frame.buffer       = NULL;
    }


    /**
     * Constructor.
     *
     * \param  number_of_hits   number of hits
     * \param  data             pointer to data
     */
    JDAQFrame(const int number_of_hits, const JDAQHit* data) :
      numberOfHits(0),
      buffer(NULL)
    {
      add(number_of_hits, data);
    }


    /**
     * Destructor.
     */
    virtual ~JDAQFrame()
    {
      clear();
    }


    /**
     * Assignment operator.
     *
     * \param  frame            frame
     * \return                  this frame
     */
    JDAQFrame& operator=(const JDAQFrame& frame)
    {
      add(frame);

      return *this;
    }


    /**
     * Clear data.
     */
    void clear()
    {
      if (buffer != NULL) {
	free(buffer);
      }

      numberOfHits = 0;
      buffer       = NULL;
    }


    const_iterator begin() const { return buffer; }
    const_iterator end()   const { return buffer + numberOfHits; }


    iterator begin() { return buffer; }
    iterator end()   { return buffer + numberOfHits; }


    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }


    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend()   { return reverse_iterator(begin()); }


    bool empty() const { return numberOfHits == 0; }

    int size() const { return numberOfHits; }

    const JDAQHit* data() const { return buffer; }
    JDAQHit*       data()       { return buffer; }


    /**
     * Get hit at given index.
     *
     * \param  index            index
     * \return                  hit
     */
    const JDAQHit& operator[](int index) const
    {
      return buffer[index];
    }

    /**
     * Get subset of data.
     *
     * \param  i1               first index of hit (included)
     * \param  i2               last  index of hit (excluded)
     * \return                  frame
     */
    JDAQFrameSubset subset(const int i1, const int i2) const
    {
      return JDAQFrameSubset(i2 - i1, buffer + i1);
    }


    /**
     * Add data.
     *
     * \param  frame            frame
     * \return                  this frame
     */
    JDAQFrame& add(const JDAQFrame& frame) 
    {
      return add(frame.numberOfHits, frame.buffer);
    }


    /**
     * Add data.
     *
     * \param  number_of_hits   number of hits
     * \param  data             pointer to data
     * \return                  this data frame
     */
    JDAQFrame& add(const int number_of_hits, const JDAQHit* data)
    {
      if (number_of_hits > 0) {

	resize(numberOfHits + number_of_hits);

	memcpy(buffer + numberOfHits - number_of_hits, data, number_of_hits * sizeof(JDAQHit));
      }

      return *this;
    }


    /**
     * Swap data.
     *
     * \param  frame            frame
     */
    void swap(JDAQFrame& frame) 
    {
      std::swap(numberOfHits, frame.numberOfHits);
      std::swap(buffer,       frame.buffer);
    }


    int      numberOfHits;        // following comment line is used by rootcint
    JDAQHit* buffer;              // [numberOfHits]


    ClassDef(JDAQFrame,1);


  protected:
    /**
     * Resize internal buffer.
     *
     * This method increases the size of the buffer if necessary.
     * Otherwise, the current size is maintained.
     *
     * \param  number_of_hits   total number of hits to allocate
     */
    void resize(const int number_of_hits)
    {
      if (number_of_hits > numberOfHits) {

	const int number_of_bytes = number_of_hits * sizeof(JDAQHit);

	if (buffer == NULL)
	  buffer = (JDAQHit*) malloc(number_of_bytes);
	else
	  buffer = (JDAQHit*) realloc(buffer, number_of_bytes);

	if (buffer == NULL) {
	  throw JDAQException("JDAQFrame::resize(): Memory exception - number of hits = " + std::to_string(number_of_hits));
	}

	numberOfHits = number_of_hits;
      }
    }
  };


  /**
   * Equal operator for DAQ frames.
   *
   * \param  first     frame
   * \param  second    frame
   * \result           true if first frame equal to second; else false
   */
  inline bool operator==(const JDAQFrame& first,
			 const JDAQFrame& second)
  {
    if (first.size() == second.size()) {
      
      for (JDAQFrame::const_iterator p = first.begin(), q = second.begin(); p != first.end(); ++p, ++q) {
	if (*p != *q) {
	  return false;
	}
      }
      
      return true;

    } else {

      return false;
    }
  }


  /**
   * Not-equal operator for DAQ frames.
   *
   * \param  first     frame
   * \param  second    frame
   * \result           true if first frame not equal to second; else false
   */
  inline bool operator!=(const JDAQFrame& first,
			 const JDAQFrame& second)
  {
    return !(first == second);
  }
}

#endif
