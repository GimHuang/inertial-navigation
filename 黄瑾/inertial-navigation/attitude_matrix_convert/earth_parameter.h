/*
*************************************************************************************
*file:	erath_parameter.h
*@Description:	��ŵ�����в���
*creator��Jin Huang
*organization:sdust
*e-mail:kim.huang.j@qq.com
*datetime��20/11/2020
*************************************************************************************
*/
#pragma once
#include<math.h>
#define PI atan(-1)	//����PI
//�������
struct Earth_parameter
{
public:
    const double R_e = 6378160.0;		        // ����뾶
    const double f = 1 / 298.3;			        // �������
    const double R_p = (1 - f) * R_e;			// �������
    const double e1 = sqrt(R_e * R_e - R_p * R_p) / R_e;	// �����һƫ����
    const double e2 = sqrt(R_e * R_e - R_p * R_p) / R_p;	// ����ڶ�ƫ����
    const double phi_ie = 7.2921151467e-5;  // ������ת������
    const double g0 = 9.7803267714; // ������������
}earth;