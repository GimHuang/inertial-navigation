/*
*************************************************************************************
*file:
*@Description:
*creator��Jin Huang
*organization:sdust
*e-mail:kim.huang.j@qq.com
*datetime��16/11/2020
*************************************************************************************
*/
#pragma once
#include<iostream>
#include<math.h>
#include<Eigen\Dense>
#include"earth_parameter.h"
using namespace std;
using namespace Eigen;

// ����
Vector3d gcs2gc(double lambda, double L, double h);         // �������ת��Ϊ����ֱ������
Vector3d gc2gcs(double x, double y, double z);              // ����ֱ������ת��Ϊ�������
Vector3d gcs2gc(Vector3d BLH);                              // �������ת��Ϊ����ֱ������
Vector3d gc2gcs(Vector3d XYZ);                              // ����ֱ������ת��Ϊ�������
Vector3d gc2gcs(double x, double y, double z);              // ����ֱ������ת��Ϊ�������
double acu_tan_l(double t, double x, double y, double z);   // �������� ���γ��L

// geodetic coordinate system to geocentric cartesian
//�������ϵת��Ϊ���Ŀռ�ֱ������ϵ
Vector3d gcs2gc(double lambda, double L, double h)
{
    double R_N = earth.R_e / sqrt(1 - earth.e1 * earth.e1 * sin(L) * sin(L));  // ����î��Ȧ���ʰ뾶
    double x = (R_N + h) * cos(L) * cos(lambda);
    double y = (R_N + h) * cos(L) * sin(lambda);
    double z = (R_N * (1 - earth.e1 * earth.e1) + h) * sin(L);
    Vector3d res = Eigen::Vector3d::Zero();
    res << x, y, z;
    return res;
}

// geocentric cartesian to geodetic coordinate system
// ������ֱ������ϵת��Ϊ�������ϵ
Vector3d gc2gcs(double x, double y, double z)
{
    double lambda = atan2(y, x);
    double L = acu_tan_l(0, x, y, z);
    double R_N = earth.R_e / sqrt(1 - earth.e1 * earth.e1 * sin(L) * sin(L));
    double h = sqrt(x * x + y * y) / cos(L) - R_N;
    Vector3d res = Eigen::Vector3d::Zero();
    res << lambda, L, h;
    return res;
}
// �������ת��Ϊ����ֱ������
Vector3d gcs2gc(Vector3d BLH)          
{
    double lambda = BLH(0), L = BLH(1), h = BLH(2);
    double R_N = earth.R_e / sqrt(1 - earth.e1 * earth.e1 * sin(L) * sin(L));  // ����î��Ȧ���ʰ뾶
    double x = (R_N + h) * cos(L) * cos(lambda);
    double y = (R_N + h) * cos(L) * sin(lambda);
    double z = (R_N * (1 - earth.e1 * earth.e1) + h) * sin(L);
    Vector3d res = Eigen::Vector3d::Zero();
    res << x, y, z;
    return res;
}
// ����ֱ������ת��Ϊ�������
Vector3d gc2gcs(Vector3d XYZ)
{
    double x = XYZ(0), y = XYZ(1), z = XYZ(2);
    double lambda = atan2(y, x);
    double L = acu_tan_l(0, x, y, z);
    double R_N = earth.R_e / sqrt(1 - earth.e1 * earth.e1 * sin(L) * sin(L));
    double h = sqrt(x * x + y * y) / cos(L) - R_N;
    Vector3d res = Eigen::Vector3d::Zero();
    res << lambda, L, h;
    return res;
}


// ��������γ��L
double acu_tan_l(double t, double x, double y, double z)
{
    double ti = (1 / sqrt(x * x + y * y)) * (z + earth.R_e * earth.e1 * earth.e1 * t / sqrt(1 + (1 - earth.e1 * earth.e1) * t * t));
    if (abs(ti - t) <= 1e-20)
        return atan(ti);
    else
        return acu_tan_l(ti, x, y, z);
}

double deg2rad(double degree)
{
    return degree * PI / 180.0;
}

double rad2deg(double rad)
{
    return rad * 180 / PI;
}