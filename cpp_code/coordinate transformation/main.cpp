#include"Transformation.h"
#include<iostream>
using namespace std;


int main()
{
	//���->����
	geodeticToGeocentric func0;
	Geocentric c(20.0, 30.0, 10000, func0);
	Vector v0(c.getCoordinate());
	cout << "(X,Y,Z)" << endl;
	for (auto i : v0)
	{
		cout << i << endl;
	}
	cout << endl;
	//����->���
	cout << "(lamda,L,h)" << endl;
	geocentricToGeodetic func1;
	Geodetic g(27000, 11000, 10000, func1);
	Vector v(g.getCoordinate());
	for (auto i:v)
	{
		cout << i << endl;
	}

	system("pause");
	return 0;
}