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
	rotation_val(addOutputPort("rotation_val")),
	rays_val(addOutputPort("rays_val")),
	frequency_time(0,0,1,0)
{
}


Model &Controller::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "tolerance"));
	param_str >> tolerance;
	std::cout << "[Controller] Tolerance: " << tolerance << std::endl;

	passivate();
	return *this;
}


Model &Controller::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == radiation)
	{
		this->_radiation = std::stof(msg.value()->asString());
		holdIn(AtomicState::active, VTime(0));
	}
	else if(msg.port() == degree)
	{
		this->_degree = std::stof(msg.value()->asString());
		holdIn(AtomicState::active, VTime(0));
	}

	return *this;
}


Model &Controller::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	passivate();

	return *this ;
}


Model &Controller::outputFunction(const CollectMessage &msg)
{
	float difference = abs(current_degree - _degree);
	if (difference > tolerance) {
		std::cout << "[Controller] current_degree=" << current_degree << ", degree=" << _degree << ", tolerance=" << tolerance << "=> current_degree := degree = " << _degree << std::endl;
		current_degree = _degree;
		sendOutput(msg.time(), rotation_val, Real(difference));	
	} else {
		std::cout << "[Controller] current_degree=" << current_degree << ", degree=" << _degree << ", tolerance=" << tolerance << "=> Don't move" << std::endl;;
		sendOutput(msg.time(), rotation_val, Real(0));	
	}
	
	sendOutput(msg.time(), rays_val, Real(abs(_radiation/_degree)));
	return *this ;
}
