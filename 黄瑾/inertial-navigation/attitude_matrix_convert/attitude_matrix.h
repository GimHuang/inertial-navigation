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
	Vector3f value = Eigen::Vector3f::Zero();
	Euler_angle() { value = Eigen::Vector3f::Zero(); }
	Euler_angle(Euler_angle& input) { value = input.value; }	// �����ʼ��
	Euler_angle(float phi, float theta, float gamma);	// ��ֵ��ʼ��
	Euler_angle(Vector3f input_vector);	// Vector3f��ʼ��
	//transform function
	Matrix3f EA2DCM();	// ŷ���� --> ����������
	Vector4f EA2QV();	// ŷ���� --> ��Ԫ��
	Vector3f EA2ERV();	// ŷ���� --> ��Ч��תʸ��
	// caculate function
	Matrix3f EA2ssm();	//Skew-symmetric matrix	ŷ���� --> ���Գ���ssm
	// Basic function
	void show();
	void initialize() { value =  Eigen::Vector3f::Zero(); };		//��ʼ��
	void operator<<(Vector3f vector3);			// ����<<������������ֵ
	void operator=(Euler_angle input);	// ����=�����ڶ���丳ֵ
	//Euler_angle operator*(Euler_angle& input);	// ����*�����ڽ��г˷�����
	Euler_angle operator+(Euler_angle& input);
};
// ����������
class Direct_cosine_matrix
{
public:
	Matrix3f value = Eigen::Matrix3f::Zero();
	Direct_cosine_matrix() { value = Eigen::Matrix3f::Zero(); }
	Direct_cosine_matrix(Direct_cosine_matrix& input) { value = input.value; }	// �����ʼ��
	Direct_cosine_matrix(Matrix3f input_matrix);
	//transform function
	Vector3f DCM2EA();	// ���������� --> ŷ����
	Vector4f DCM2QV();	// ���������� --> ��Ԫ��
	Vector3f DCM2ERV();	// ���������� --> ��Ч��תʸ��
	// Basic function
	void show();
	void initialize() { value = Eigen::Matrix3f::Zero(); };		//��ʼ��
	void operator<<(Matrix3f matrix3);			//����<<������������ֵ
	void operator=(Direct_cosine_matrix input);	//����=�����ڶ���丳ֵ
};

// ��Ԫ��
class Quaternion_vector
{
public:
	Vector4f value = Eigen::Vector4f::Zero();
	Quaternion_vector() {value = Eigen::Vector4f::Zero();}
	Quaternion_vector(Quaternion_vector& input) { value = input.value; }	// �����ʼ��
	Quaternion_vector(Vector4f &input);
	//transform function
	Vector3f QV2EA();	//��Ԫ�� --> ŷ����
	Matrix3f QV2DCM();	//��Ԫ�� --> ����������
	Vector3f QV2ERV();	//��Ԫ�� --> ��Ч��תʸ��
	// Basic function
	void show();
	void initialize() { value = Eigen::Vector4f::Zero(); };		//��ʼ��
	void operator<<(Vector4f &matrix3);		//����<<������������ֵ
	void operator=(Quaternion_vector &input);//����=�����ڶ���丳ֵ
	Quaternion_vector operator*(Quaternion_vector& input);	//������Ԫ�����
	
};

// ��Ч��תʸ��
class Equivalent_rotation_vector
{
public:
	Vector3f value = Eigen::Vector3f::Zero();
	Equivalent_rotation_vector(){ value = Eigen::Vector3f::Zero(); }
	Equivalent_rotation_vector(Equivalent_rotation_vector& input) { value = input.value; }	// �����ʼ��
	Equivalent_rotation_vector(Vector3f input);
	//transform function
	Vector4f ERV2QV();	//��Ч��תʸ�� --> ��Ԫ��
	Matrix3f ERV2DCM();	//��Ч��תʸ�� --> ����������
	Vector3f ERV2EA();	//��Ч��תʸ�� --> ŷ����
	// Basic function
	void show();
	void initialize() { value = Eigen::Vector3f::Zero(); };		//��ʼ��
	void operator<<(Vector3f &vector3);							//����<<������������ֵ
	void operator=(Equivalent_rotation_vector &input);		//����=�����ڶ���丳ֵ
};

