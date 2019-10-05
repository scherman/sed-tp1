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

    const Port &radiation;
    const Port &degree;

    Port &rotation;
    Port &rays;

    float rad;
    float deg;

    VTime frequency_time;
};

#endif
