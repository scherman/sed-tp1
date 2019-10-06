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
    const Port &radiation;
    const Port &degree;

    // Output ports
    Port &rotation_val;
    Port &rays_val;

    // Internal
    float _radiation;
    float _degree;

    VTime frequency_time;
};

#endif
