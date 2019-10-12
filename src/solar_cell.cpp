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
	obtained_energy(addOutputPort("obtained_energy"))
{
}


Model &SolarCell::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "conversion_factor"));
	param_str >> conversion_factor;
	std::cout << "[Solar Cell] params := {conversion_factor: " << conversion_factor << "}" << std::endl;

	_obtained_energy = 0;

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
		rays_val = std::stof(msg.value()->asString());
		holdIn(AtomicState::active, VTime(0));
		_obtained_energy = conversion_factor * rays_val;
	}

	return *this;
}


Model &SolarCell::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	_obtained_energy = 0;
	passivate();

	return *this ;
}


Model &SolarCell::outputFunction(const CollectMessage &msg)
{
	std::cout << "[SolarCell] rays_val=" << rays_val << " => obtained_energy := " << conversion_factor * rays_val << std::endl;
	sendOutput(msg.time(), obtained_energy, Real(_obtained_energy));
	return *this ;
}
