#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <random>

#include "atomic.h"
#include "VTime.h"


#define ENGINE_MODEL_NAME "Engine"


class Engine : public Atomic {
  public:
    
    Engine(const string &name = ENGINE_MODEL_NAME );
    virtual string className() const {  return ENGINE_MODEL_NAME ;}
  
  protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const CollectMessage & );

  private:

    const Port &rotation;
    
    Port &consumed_energy;
    
    float consumption_factor;

    VTime frequency_time;
};

#endif
