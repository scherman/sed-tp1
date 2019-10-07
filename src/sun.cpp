#include <random>
#include <string>
#include <fstream>
#include <sstream>
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

	this->finRad.open("CSV_annual_daily_radiation.csv", ios::in);
	this->finDeg.open("NOAA_Solar_Elevation_Day.csv", ios::in);

	//no logre entender por que hay basura en la
	//primera linea, pero la descarto
	string line;
	getline(this->finDeg, line);

	string line1;
	getline(this->finRad, line1);

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

	//todo el codigo de lectura del archivo esta aca para probar, tiene que ir en las transiciones externes o Internas
	//donde se va a actualizar el valor actual de acuerdo a alguna condicion (no siempre van a cambiar ambos al mismo tiempo)
	vector<string> row;
	string rad, word, deg;

	if(getline(this->finRad, rad)) {
		row.clear();
	  stringstream s(rad);
	  while (std::getline(s, word, ',')) {
			row.push_back(word);
	  }

		if(getline(this->finDeg, deg)) {

			//row1 = mes, row2 = dia, row3 = valor en (kW-hr/m^2/day)
			//deg es el angulo
			sendOutput(msg.time(), radiation, Real(stoi(row[2])));
			sendOutput(msg.time(), degree, Real(std::stof(deg)));
			cout << "mes :" <<row[0] << " dia :" << row[1] << " radiacion :" << row[2] << endl;
			cout << "angulo :" << deg << endl;
			// sendOutput(msg.time(), degree, Real(random_int));
			return *this;
		}
	}
}
