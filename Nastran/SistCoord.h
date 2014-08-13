#ifndef SistCoord_H
#define SistCoord_H

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;

class SistCoord{

	public:
	SistCoord::SistCoord(int i, int tipo);
	const int SistCoord::VerTipo();
	const int SistCoord::VerId();

	private:
	int id;
	int Tipo;	//0 es rect, 1 es cil y 3 es esferico

};

#endif