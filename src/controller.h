#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <random>

#include "atomic.h"
#include "VTime.h"


#define CONTROLLER_MODEL_NAME "Controller"


class Controller : public Atomic {
  public:
    
    Controller(const string &name = CONTROLLER_MODEL_NAME );
    virtual string className() const {  return CONTROLLER_MODEL_NAME ;}
  
  protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const CollectMessage & );

  private:

    // Input ports
    const Port &radiation_p;
    const Port &degree_p;

    // Output ports
    Port &rotation_p;
    Port &received_energy_p;

    // Parameters
    float tolerance;

    // Internal
    float radiation;
    float degree;
    float current_degree;
    
    bool received_radiation;
    bool received_degree;

    float rotation;
    float received_energy;
    
    VTime frequency_time;
};

#endif
