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
	rotation_p(addInputPort("rotation")),
	consumed_energy_p(addOutputPort("consumed_energy"))
{
}


Model &Engine::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "consumption_factor"));
	param_str >> consumption_factor;
	std::cout << "[Engine] params := {consumption_factor: " << consumption_factor << "}" << std::endl;

	consumed_energy = 0;
	rotation = 0;

	passivate();

	return *this;
}


Model &Engine::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == rotation_p)
	{
		rotation = std::stof(msg.value()->asString());
		holdIn(AtomicState::active, VTime(0));
		consumed_energy = consumption_factor * rotation;
	}

	return *this;
}


Model &Engine::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	consumed_energy = 0;
	rotation = 0;
	passivate();

	return *this ;
}


Model &Engine::outputFunction(const CollectMessage &msg)
{
	std::cout << "[Engine] rotation = " << rotation << " => consumed_energy := " << consumed_energy << std::endl; 
	sendOutput(msg.time(), consumed_energy_p, Real(consumed_energy));
	return *this ;
}

