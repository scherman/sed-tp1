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
	frequency_time(0,6,0,0),
	dist(0,100),
	rng(random_device()())
{
}


Model &Sun::initFunction()
{

	std::cout << "Opening data files..." << std::endl;
	this->finRad.open("./CSV_annual_daily_radiation.csv", ios::in);
	this->finDeg.open("./NOAA_Solar_Elevation_Day.csv", ios::in);
	std::cout << "Data files opened successfully" << std::endl;

	getline(this->finDeg, this->deg);

	getline(this->finRad, this->rad);
	this->row.clear();
	stringstream s(rad);
	while (std::getline(s, word, ',')) {
		this->row.push_back(word);
	}

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
	if(this->anglesCount < 239) {
		this->anglesCount++;
	} else {
		this->finDeg.clear();
		this->finDeg.seekg(0, ios::beg);
		this->anglesCount = 0;
	}
	getline(this->finDeg, this->deg);
	if(this->anglesCount == 0) {
		getline(this->finRad, this->rad);
		this->row.clear();
	  stringstream s(rad);
		while (std::getline(s, word, ',')) {
			this->row.push_back(word);
		}
		//row1 = mes, row2 = dia, row3 = valor en (kW-hr/m^2/day)
		//deg es el angulo
	}
	holdIn(AtomicState::active, this->frequency_time);
	return *this ;
}


Model &Sun::outputFunction(const CollectMessage &msg)
{
	string month = row[0];
	string day = row[1];
	std::cout << "[Sun] " << month << "/" << day << " - emiting with degree := " << deg << ", radiation := " <<  (stof(row[2])) << std::endl;

	// auto random_int = this->dist(this->rng);
	sendOutput(msg.time(), radiation, Real(stof(row[2])));
	sendOutput(msg.time(), degree, Real(stof(deg)));
	// cout << "mes :" <<row[0] << " dia :" << row[1] << " radiacion :" <<  << endl;
	// cout << "angulo :" << deg << endl;
	// sendOutput(msg.time(), degree, Real(random_int));
	return *this;
}
