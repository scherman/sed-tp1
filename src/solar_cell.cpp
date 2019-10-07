#include <random>
#include <string>

#include "message.h"
#include "parsimu.h"
#include "real.h"
#include "tuple_value.h"

#include "solar_cell.h"

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


SolarCell::SolarCell(const string &name) :
	Atomic(name),
	rays(addInputPort("rays_val")),
	obtained_energy(addOutputPort("obtained_energy")),
	frequency_time(0,0,1,0)
{
}


Model &SolarCell::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "conversion_factor"));
	param_str >> conversion_factor;
	std::cout << "[Solar Cell] Conversion factor: " << conversion_factor << std::endl;

	// holdIn(AtomicState::active, this->frequency_time);
	passivate();
	return *this;
}


Model &SolarCell::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == rays)
	{	
		// std::cout << "asd " << std::stof(AbstractValue::to_value_ptr(msg.value())) << std::endl;
		holdIn(AtomicState::active, VTime(0));
	}

	return *this;
}


Model &SolarCell::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	passivate();

	return *this ;
}


Model &SolarCell::outputFunction(const CollectMessage &msg)
{
	sendOutput(msg.time(), obtained_energy, Real(conversion_factor * 10));
	return *this ;
}
