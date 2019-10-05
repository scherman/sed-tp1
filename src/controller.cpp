#include <random>
#include <string>

#include "message.h"
#include "parsimu.h"
#include "real.h"
#include "tuple_value.h"

#include "controller.h"

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


Controller::Controller(const string &name) :
	Atomic(name),
	radiation(addInputPort("radiation")),
	degree(addInputPort("degree")),
	rotation(addOutputPort("rotation_val")),
	rays(addOutputPort("rays_val")),
	frequency_time(0,0,1,0)
{
}


Model &Controller::initFunction()
{
	holdIn(AtomicState::active, this->frequency_time);
	return *this;
}


Model &Controller::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == radiation)
	{
		this->rad = 5;
		holdIn(AtomicState::active, this->frequency_time);
	}
	else if(msg.port() == degree)
	{
		this->deg = 5;
		passivate();
	}
	holdIn(AtomicState::active, this->frequency_time);

	return *this;
}


Model &Controller::internalFunction(const InternalMessage &msg)
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


Model &Controller::outputFunction(const CollectMessage &msg)
{
	sendOutput(msg.time(), rotation, Real(3));
	sendOutput(msg.time(), rays, Real(rad*deg));
	return *this ;
}
