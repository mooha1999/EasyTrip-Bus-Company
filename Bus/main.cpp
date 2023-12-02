#include <iostream>
#include "Company.h"
using namespace std;
int main() {
	Company* company = new Company();
	company->readInputFile("input.txt");
	company->startSimulation();
	return 0;
}