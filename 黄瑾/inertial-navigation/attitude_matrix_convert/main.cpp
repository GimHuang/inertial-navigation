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
#include<iomanip>
#include"attitude_matrix.h"		// ��̬����
#include"coordinate_convert.h"	// ����ת��
#include"earth_parameter.h"		// �������
#include"read_imr_file.h"		// ��ȡimr�ļ�
#include"updating.h"	// ��̬����
using namespace std;
using namespace Eigen;

//#define attitude_matrix
//#define transformation
#define read_imr_data
#define updating
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

#ifdef updating
	// ��ʼ����������
	Euler_angle begin_rotation(0, 0, 0);
	Direct_cosine_matrix begin_dcm(begin_rotation.EA2DCM());
	// m,m-1 �β���delta����
	Vector3f temp1; temp1 << data.file_data[1].gx, data.file_data[1].gy, data.file_data[1].gz;
	Vector3f temp2; temp2 << data.file_data[2].gx, data.file_data[2].gy, data.file_data[2].gz;
	vector<Vector3f> delta_theta; delta_theta.push_back(temp1); delta_theta.push_back(temp2);
	// ��ʼ�����m-2,m-1ʱ�̵��ٶ���vector[1], vecotr[0]
	temp1 << 0, 0, 0;
	temp2 << 1, 1, 0;
	vector<Vector3f> speed; speed.push_back(temp1); speed.push_back(temp2);
	// m -1�� m-2 ʱ�̵�λ������
	temp1 << -2590726.191, 4469381.673, 3728381.517;	// m-1
	temp2 << -2590726.191, 4469381.673, 3728381.517;	// m-2
	vector<Vector3f> position; position.push_back(temp1), position.push_back(temp2);

	//���m , m - 1ʱ�̵��ٶ�������vector[1], vector[0]
	temp1 << data.file_data[2].ax, data.file_data[2].ay, data.file_data[2].az;
	temp2 << data.file_data[1].ax, data.file_data[1].ay, data.file_data[1].az;
	vector<Vector3f> velocity; velocity.push_back(temp2); velocity.push_back(temp1);

	// ��Ų�������
	double T = data.file_data[2].Time;
	
	update update_process(begin_dcm, speed, delta_theta, position, velocity, T);
	update_process.attitude_update();
	update_process.speed_update();
	update_process.position_update();
	double x = update_process.position_data[1].position(0);
	double y = update_process.position_data[1].position(1);
	double z = update_process.position_data[1].position(2);
	std::cout << setprecision(12) << x << endl << y << endl << z << endl;

#endif // updating
	
	system("pause");
}