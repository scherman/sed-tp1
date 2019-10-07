#include <random>
#include <string>

#include "message.h"
#include "parsimu.h"
#include "real.h"
#include "tuple_value.h"

#include "engine.h"

using namespace std;


#define VERBOSE true

#define PRINT_TIMES(f) {\
	VTime timeleft = nextChange();\
	VTime elapsed  = msg.time() - lastChange();\
	VTime sigma    = elapsed + timeleft;\
	cout << f << "@" << msg.time() <<\
		" - timeleft: " << timeleft <<\
		" - elapsed: " << elapsed <<\
		" - sigma: " << sigma << endl;\
}


Engine::Engine(const string &name) :
	Atomic(name),
	rotation_val(addInputPort("rotation_val")),
	consumed_energy(addOutputPort("consumed_energy")),
	frequency_time(0,0,1,0)
{
}


Model &Engine::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "consumption_factor"));
	param_str >> consumption_factor;
	std::cout << "[Engine] Consumption factor: " << consumption_factor << std::endl;
	passivate();
	
	return *this;
}


Model &Engine::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == rotation_val)
	{
		holdIn(AtomicState::active, VTime(0));
	}

	return *this;
}


Model &Engine::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	// holdIn(AtomicState::active, this->frequency_time);
	passivate();

	return *this ;
}


Model &Engine::outputFunction(const CollectMessage &msg)
{
	sendOutput(msg.time(), consumed_energy, Real(18));
	return *this ;
}

