#include <random>
#include <string>

#include "message.h"
#include "parsimu.h"
#include "real.h"
#include "tuple_value.h"

#include "sun.h"

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


Sun::Sun(const string &name) :
	Atomic(name),
	radiation(addOutputPort("radiation")),
	degree(addOutputPort("degree")),
	frequency_time(0,0,1,0),
	dist(0,100),
	rng(random_device()())
{
}


Model &Sun::initFunction()
{
	holdIn(AtomicState::active, this->frequency_time);
	return *this;
}


Model &Sun::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif
	return *this;
}


Model &Sun::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	holdIn(AtomicState::active, this->frequency_time);

	return *this ;
}


Model &Sun::outputFunction(const CollectMessage &msg)
{
	auto random_int = this->dist(this->rng);
//	Tuple<Real> out_value{Real(random_int), 0, 1};
	sendOutput(msg.time(), radiation, Real(random_int));
	sendOutput(msg.time(), degree, Real(random_int));
	return *this ;
}
