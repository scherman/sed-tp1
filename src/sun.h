#ifndef _SUN_H_
#define _SUN_H_

#include <random>

#include "atomic.h"
#include "VTime.h"


#define SUN_MODEL_NAME "Sun"

class Sun : public Atomic {
  public:

    Sun(const string &name = SUN_MODEL_NAME );
    virtual string className() const {  return SUN_MODEL_NAME ;}

  protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const CollectMessage & );

  private:

    // Output ports
    Port &radiation;
    Port &degree;

    // Internal
    VTime frequency_time;
    std::uniform_int_distribution<int> dist;
    std::mt19937 rng;
    fstream finRad;
    fstream finDeg;
};

#endif
