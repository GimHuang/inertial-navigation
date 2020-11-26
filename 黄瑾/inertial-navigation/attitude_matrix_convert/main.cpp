/*
*************************************************************************************
*file:	main.cpp
*@Description:	������̬��������������
*creator��Jin Huang
*organization:sdust
*e-mail:kim.huang.j@qq.com
*datetime��09/11/2020
*************************************************************************************
*/
#include <iostream>
#include <Eigen\Dense>
#include"attitude_matrix.h"		// ��̬����
#include"coordinate_convert.h"	// ����ת��
#include"earth_parameter.h"		// �������
#include"read_imr_file.h"		// ��ȡimr�ļ�
#include"updating.h"	// ��̬����
#include"speed_updating.h"		// �ٶȸ���
using namespace std;
using namespace Eigen;

//#define attitude_matrix
//#define transformation
#define read_imr_data
//#define attitude_updating
void main()
{
	//��֤��̬��ʾ������ת��
#ifdef attitude_matrix
	// test  
	Vector3f initional_data;
	initional_data << 1, 1, 1;
	//Euler Angle
	Euler_angle ea(initional_data);
	cout << "## Euler angle ##" << endl;
	ea.show();
	cout << endl << "## Euler 2 Direct cosine matrix ##" << endl;
	Direct_cosine_matrix dcm(ea.EA2DCM());
	dcm.show();
	cout << endl << "## Eulor 2 Quaternion ##" << endl;
	Quaternion_vector qv(ea.EA2QV());
	qv.show();
	cout << endl << "## Eulor 2 Equivalent rotation vector ##" << endl;
	Equivalent_rotation_vector erv(ea.EA2ERV());
	erv.show();

	cout << endl << endl << "## Direct cosine matrix ##" << endl;
	dcm.show();
	cout << endl << "## Direct cosine matrix 2 Euler angle ##" << endl;
	ea << dcm.DCM2EA();
	ea.show();
	cout << endl << "## Direct cosine matrix 2 Quaternion ##" << endl;
	qv << dcm.DCM2QV();
	qv.show();
	cout << endl << "## Direct cosine matrix 2 Equivalent rotation vector ##" << endl;
	erv << dcm.DCM2ERV();
	erv.show();

	cout << endl << endl << "## Quaternion vector ##" << endl;
	qv.show();
	cout << endl << "## Quaternion 2 Euler angle ##" << endl;
	ea << qv.QV2EA();
	ea.show();
	cout << endl << "## Quaternion 2 Direct cosine matrix ##" << endl;
	dcm << qv.QV2DCM();
	dcm.show();
	cout << endl << "## Quaternion 2 Equivalent rotation vector ##" << endl;
	erv << qv.QV2ERV();
	erv.show();

	cout << endl << endl << "## Equivalent_rotation_vector ##" << endl;
	erv.show();
	cout << endl << "## Equivalent_rotation_vector 2 Quaternion ##" << endl;
	qv << erv.ERV2QV();
	qv.show();
	cout << endl << "## Equivalent_rotation_vector 2 Euler angle ##" << endl;
	ea << erv.ERV2EA();
	ea.show();
	cout << endl << "## Equivalent_rotation_vector 2 Direct cosine matrix ##" << endl;
	dcm << erv.ERV2DCM();
	dcm.show();



#endif // attidtude_matrix
	//��֤������������ֱ�������ת��
#ifdef transformation
	Vector3f BLH(deg2rad(20), deg2rad(30), 10000);
	cout << BLH << endl;
	Vector3f xyz = gcs2gc(BLH);
	cout << xyz << endl;
	BLH = gc2gcs(xyz);
	cout << BLH << endl;
#endif // transformation
	//Vector4f x;
	//x << 1, 2, 3, 4;
	//Quaternion_vector qv1(x);
	//qv1.show();
	//Quaternion_vector qv2(qv1 * qv1);
	//qv2.show();

#ifdef read_imr_data
	// ==========��ȡimr�ļ�===========
	char* file_path = "..\\imr_data\\20200911_095111_001.imr";
	imr_data data;
	data.read_data(file_path);
	
	// ==========���imr��ȡ===========
#endif // read_imr_data

#ifdef attitude_updating
	// ������̬����
	// ������̬���¾���
	int data_num = 0;	//ѡ����������
	Direct_cosine_matrix au;
	// ����ǰһʱ�̵�QV Qnb
	Euler_angle ea(1, 1, 1);
	Quaternion_vector Qnb(ea.EA2QV());
	// ���� ��һ�Ρ��ڶ��β��� v3f 
	Vector3f theta1 = Eigen::Vector3f::Zero();
	Vector3f theta2 = Eigen::Vector3f::Zero();
	theta1 << adj_data[data_num].gx, adj_data[data_num].gy, adj_data[data_num].gz;
	theta2 << adj_data[data_num + 1].gx, adj_data[data_num + 1].gy, adj_data[data_num + 1].gz;
	// �����ʼλ�� v3f
	Vector3f pos = Eigen::Vector3f::Zero();
	pos << -2590726.191, 4469381.673, 3728381;
	// �����ٶ� v3f
	Vector3f vec = Eigen::Vector3f::Zero();
	vec << adj_data[data_num].ax, adj_data[data_num].ay, adj_data[data_num].az;
	// ���������� float
	float T = 1.0 / data_header->dDataRateHz;
	//au = attitude_update(Qnb, theta1, theta2, pos, vec, T);
	//au.show();

#endif // attitude_updating
	


	system("pause");
}