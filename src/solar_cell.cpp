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
	holdIn(AtomicState::active, this->frequency_time);
	return *this;
}


Model &SolarCell::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == rays)
	{
		holdIn(AtomicState::active, this->frequency_time);
	}
	holdIn(AtomicState::active, this->frequency_time);

	return *this;
}


Model &SolarCell::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif

//	if(this->on)
//		holdIn(AtomicState::active, this->frequency_time);
//	else
//		passivate();

	holdIn(AtomicState::active, this->frequency_time);

	return *this ;
}


Model &SolarCell::outputFunction(const CollectMessage &msg)
{
	sendOutput(msg.time(), obtained_energy, Real(17));
	return *this ;
}
