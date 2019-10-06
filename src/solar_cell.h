#ifndef _SOLARCELL_H_
#define _SOLARCELL_H_

#include <random>

#include "atomic.h"
#include "VTime.h"


#define SOLAR_CELL_MODEL_NAME "SolarCell"


class SolarCell : public Atomic {
  public:
    
    SolarCell(const string &name = SOLAR_CELL_MODEL_NAME );
    virtual string className() const {  return SOLAR_CELL_MODEL_NAME ;}
  
  protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const CollectMessage & );

  private:

    const Port &rays;
    
    Port &obtained_energy;
    
    float rad;
    float deg;

    float conversion_factor;

    VTime frequency_time;
};

#endif
