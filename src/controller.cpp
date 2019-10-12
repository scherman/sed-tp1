#include <random>
#include <string>

#include "message.h"
#include "parsimu.h"
#include "real.h"
#include "tuple_value.h"
#include "math.h"

#include "controller.h"

using namespace std;

#define _USE_MATH_DEFINES

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

#define MIN_BATTERY_TO_MOVE 10


Controller::Controller(const string &name) :
	Atomic(name),
	// input proveniente del sol
	radiation_p(addInputPort("radiation")),
	degree_p(addInputPort("degree")),
	// input proveniente del motor y celda solar (para actualizar bateria) 
	obtained_energy_p(addInputPort("obtained_energy")),
	consumed_energy_p(addInputPort("consumed_energy")),
	
	rotation_p(addOutputPort("rotation")),
	received_energy_p(addOutputPort("received_energy")),
	battery_p(addOutputPort("battery"))
{
}


Model &Controller::initFunction()
{
	std::stringstream param_str(ParallelMainSimulator::Instance().getParameter(this->description(), "tolerance"));
	param_str >> tolerance;
	std::cout << "[Controller] params := {tolerance: " << tolerance << "}" << std::endl;
	this->radiation = 0;
	this->degree = 0;
	this->current_degree = 0;
	this->received_radiation = false;
	this->received_degree = false;
	this->rotation = 0;
	this->received_energy = 0;
	this->battery = 0;
	this->battery_update = false;
	this->obtained_energy = 0;
	this->consumed_energy = 0;

	passivate();
	return *this;
}


Model &Controller::externalFunction(const ExternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dext");
#endif

	if(msg.port() == radiation_p) {
		this->radiation = std::stof(msg.value()->asString());
		received_radiation = true;
	} else if(msg.port() == degree_p) {
		this->degree = std::stof(msg.value()->asString());
		received_degree = true;
	} else if(msg.port() == obtained_energy_p) {
		this->battery_update = true;
		obtained_energy = std::stof(msg.value()->asString());
		battery += obtained_energy;
	} else if(msg.port() == consumed_energy_p) {
		this->battery_update = true;
		consumed_energy = std::stof(msg.value()->asString());
		battery -= consumed_energy; 
	}
	holdIn(AtomicState::active, VTime(0));

	if (received_radiation && received_degree) {
		received_radiation = false;
		received_degree = false;

		if ( 0 <= degree && degree <= 180 ) {
			//esta es la logica de como afecta el angulo de recepcion de la radiacion a la energia generada,
			//aca vamos a tener que invstigar un poco mas para poner alguna ecuacion copada

			/*
			E = A * r * H * PR
			E = Energy(kWh)
			A = TotalsolarpanelArea(m²)
			r = solarpanelyield(%)
			H = Annual average solar radiation_p on tilted panels (shadings not included)
			PR = Performance ratio, coefficient for losses (range between 0.5 and 0.9, default value = 0.75)

			r is the yield of the solar panel given by the ratio : electrical power (in kWp)
			 of one solar paneldivided by the area of one panelExample : the solar panel yield
			  of a PV module of 250 Wp with an area of 1.6 m² is 15.6%Be aware that this nominal
				 ratio is given for standard test conditions (STC) : radiation_p=1000 W/m²,
				 cell temperature=25 °C, Wind speed=1 m/s, AM=1.5 The unit of the nominal power
				 of thephotovoltaic panel in these conditions is called "Watt-peak"
				 (Wp or kWp=1000 Wp orMWp=1000000 Wp).
			 */
			int altura = 10;
			int ancho = 6;

			//eliminamos PR que es un factor de la red que de momento no nos interesa analizar
			//tomamos r 0.5 como valor inicial, puede ser un parametro de testeo
			//el lado del area visible del panel que se modifica con el angulo del sol
			// va a ser el coseno del valor absoluto del angulo de elevacion - 90º
			// multiplicado por el lado real del panel

			float anchoIrradiado = cos((abs(90-this->degree)) * M_PI / 180) * ancho;
			//	cout <<"ancho final " << anchoIrradiado << endl;
			//cout << "del angulo " << degree << endl;
			//cout << "no radian " << (90 - degree) << " radian " << ((90 - this->degree) * M_PI / 180) << " abs " << ((abs(90 - this->degree)) * M_PI / 180) << " con coseno " << cos((abs(90-this->degree)) * M_PI / 180) << endl;
			float area = anchoIrradiado * altura;
			received_energy = area * 0.5 * this->radiation;
			

			float difference = abs(current_degree - degree);
			if (difference > tolerance && battery > MIN_BATTERY_TO_MOVE) {
				current_degree = degree;
				rotation = difference;
			} 
		}
	} 

	return *this;
}


Model &Controller::internalFunction(const InternalMessage &msg)
{
#if VERBOSE
	PRINT_TIMES("dint");
#endif
	this->rotation = 0;
	this->received_energy = 0;
	this->battery_update = false;
	this->obtained_energy = 0;
	this->consumed_energy = 0;
	passivate();

	return *this ;
}


Model &Controller::outputFunction(const CollectMessage &msg) {
	if (battery_update) {
		std::cout << "[Controller] obtained_energy = " << obtained_energy << ", consumed_energy = " << consumed_energy << " => battery := " << battery << std::endl; 
		sendOutput(msg.time(), battery_p, Real(battery)); // a panel solar
	} else {
		std::cout << "[Controller] current_degree = " << current_degree << ", degree =" << degree << std::endl;

		std::cout << "[Controller] rotation = " <<  rotation << " => current_degree :=" << current_degree <<  std::endl;
		sendOutput(msg.time(), rotation_p, Real(rotation));	 // a motor

		std::cout << "[Controller] received_energy := " <<  received_energy << std::endl;
		sendOutput(msg.time(), received_energy_p, Real(received_energy)); // a celda solar	
	}

	return *this ;
}