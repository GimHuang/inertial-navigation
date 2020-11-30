//===============================================================
//����ʵ���� ��Ԫ��������������ŷ����֮����໥ת��				*
//																*
//	First edit:		09/11/2020									*
//	Last edit:		10/11/2020									*
//	Author:			GimHuang									*
//	Org:			SDUST										*
//	e-mail:			kim.huang.j@qq.com							*
//																*
//===============================================================
#pragma once
#include<iostream>
#include<Eigen\Dense>
#include<math.h>
using namespace Eigen;
using namespace std;

//������
class Euler_angle;
class Direct_cosine_matrix;
class Quaternion_vector;
class Equivalent_rotation_vector;

// ŷ����
class Euler_angle
{
public:
	Vector3d value = Eigen::Vector3d::Zero();
	Euler_angle() { value = Eigen::Vector3d::Zero(); }
	Euler_angle(Euler_angle& input) { value = input.value; }	// �����ʼ��
	Euler_angle(double phi, double theta, double gamma);	// ��ֵ��ʼ��
	Euler_angle(Vector3d input_vector);	// Vector3d��ʼ��
	//transform function
	Matrix3d EA2DCM();	// ŷ���� --> ����������
	Vector4d EA2QV();	// ŷ���� --> ��Ԫ��
	Vector3d EA2ERV();	// ŷ���� --> ��Ч��תʸ��
	// caculate function
	Matrix3d EA2ssm();	//Skew-symmetric matrix	ŷ���� --> ���Գ���ssm
	// Basic function
	void show();
	void initialize() { value =  Eigen::Vector3d::Zero(); };		//��ʼ��
	void operator<<(Vector3d vector3);			// ����<<������������ֵ
	void operator=(Euler_angle input);	// ����=�����ڶ���丳ֵ
	//Euler_angle operator*(Euler_angle& input);	// ����*�����ڽ��г˷�����
	Euler_angle operator+(Euler_angle& input);
};
// ����������
class Direct_cosine_matrix
{
public:
	Matrix3d value = Eigen::Matrix3d::Zero();
	Direct_cosine_matrix() { value = Eigen::Matrix3d::Zero(); }
	Direct_cosine_matrix(Direct_cosine_matrix& input) { value = input.value; }	// �����ʼ��
	Direct_cosine_matrix(Matrix3d input_matrix);
	//transform function
	Vector3d DCM2EA();	// ���������� --> ŷ����
	Vector4d DCM2QV();	// ���������� --> ��Ԫ��
	Vector3d DCM2ERV();	// ���������� --> ��Ч��תʸ��
	// Basic function
	void show();
	void initialize() { value = Eigen::Matrix3d::Zero(); };		//��ʼ��
	void operator<<(Matrix3d matrix3);			//����<<������������ֵ
	void operator=(Direct_cosine_matrix input);	//����=�����ڶ���丳ֵ
};

// ��Ԫ��
class Quaternion_vector
{
public:
	Vector4d value = Eigen::Vector4d::Zero();
	Quaternion_vector() {value = Eigen::Vector4d::Zero();}
	Quaternion_vector(Quaternion_vector& input) { value = input.value; }	// �����ʼ��
	Quaternion_vector(Vector4d &input);
	//transform function
	Vector3d QV2EA();	//��Ԫ�� --> ŷ����
	Matrix3d QV2DCM();	//��Ԫ�� --> ����������
	Vector3d QV2ERV();	//��Ԫ�� --> ��Ч��תʸ��
	// Basic function
	void show();
	void initialize() { value = Eigen::Vector4d::Zero(); };		//��ʼ��
	void operator<<(Vector4d &matrix3);		//����<<������������ֵ
	void operator=(Quaternion_vector &input);//����=�����ڶ���丳ֵ
	Quaternion_vector operator*(Quaternion_vector& input);	//������Ԫ�����
	
};

// ��Ч��תʸ��
class Equivalent_rotation_vector
{
public:
	Vector3d value = Eigen::Vector3d::Zero();
	Equivalent_rotation_vector(){ value = Eigen::Vector3d::Zero(); }
	Equivalent_rotation_vector(Equivalent_rotation_vector& input) { value = input.value; }	// �����ʼ��
	Equivalent_rotation_vector(Vector3d input);
	//transform function
	Vector4d ERV2QV();	//��Ч��תʸ�� --> ��Ԫ��
	Matrix3d ERV2DCM();	//��Ч��תʸ�� --> ����������
	Vector3d ERV2EA();	//��Ч��תʸ�� --> ŷ����
	// Basic function
	void show();
	void initialize() { value = Eigen::Vector3d::Zero(); };		//��ʼ��
	void operator<<(Vector3d &vector3);							//����<<������������ֵ
	void operator=(Equivalent_rotation_vector &input);		//����=�����ڶ���丳ֵ
};

