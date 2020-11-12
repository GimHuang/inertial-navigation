#pragma once
#include<iostream>
#include<math.h>
#include<Eigen\Dense>
using namespace std;
using namespace Eigen;
#define PI 3.1415927
//�������
struct Earth_parameter
{
	double radius = 6378160;		// ����뾶
	double f = 1 / 298.3;			// �������
	double a = 6378137.0;			// ������
	double b = 6356752.314;			// �������
	double e1 = sqrt(2 * f - f*f);	// ����ƫ����
	double e2 = e1*e1;
}earth;

//�������ϵת��Ϊ���Ŀռ�ֱ������ϵ
Vector3f convert_BLH2XYZ(Vector3f input)
{
	float B = B*PI / 180;
	float L = L*PI / 180;
}
