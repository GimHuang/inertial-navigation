/*
*************************************************************************************
*file:attitude_updating.h
*@Description:ʵ�ֵ�������ϵ(g����ϵ)�µ���̬����
*creator��Jin Huang
*organization:sdust
*e-mail:kim.huang.j@qq.com
*datetime��20/11/2020
*************************************************************************************
*/
#pragma once
#include<iostream>
#include<math.h>
#include<Eigen/Dense>
#include"attitude_matrix.h"
#include"earth_parameter.h"
using namespace std;
using namespace Eigen;

// ����������
class gyroscope
{
public:
	Vector3f omega_ibb = Eigen::Vector3f::Zero();
	//Matrix3f _x();
};

//Matrix3f gyroscope::_x()
//{
//	Matrix3f res = Eigen::Matrix3f::Zero();
//	res(0, 0) = 0; res(0,1) = 
//}

Direct_cosine_matrix attitude_update(Quaternion_vector Qnb,Vector3f delta_theta1,Vector3f delta_theta2, Vector3f position1, Vector3f Velocity, float T)
{
    /*
    *************************************************************************************
    *func: ������̬����
    *@Param:    Qnb             ����ϵb����ڵ�������ϵn����̬��ת��Ԫ��
    *@Param:    delta_theta1    ��һ�ν���������
    *@Param:    delta_theta2    �ڶ��ν���������
    *@Param:    position1       [L,B,H]��ʼλ�õ�γ�ȡ����ȡ���ظ�
    *@Param:    Velocity        �����˶������ٶ� [Vx, Vy, 0] ���� [Ve, Vn, 0]
    *@Param:    T               ��̬��������
    *@Param:
    *@Return:
    *@Note:
    *creator��Jin Huang
    *organization:sdust
    *e-mail:kim.huang.j@qq.com
    *************************************************************************************
    */
    // ͨ�ñ�������
    float sinl = sin(position1(0)), cosl = cos(position1(0)), tanl = tan(position1(0));    // �������γ��Ϊsin cos tan
    float Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    float Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    float Rmh = Rm + position1(2); // ʵ�ʳ��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    float Rnh = Rn + position1(2); // ʵ�ʳ��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    

    // ���򣨵�������ϵ������ת����ת�����Ĺ�������ϵ�£��Ľ��ٶ��ڵ�������ϵ�µ�ͶӰ
    Euler_angle omega_nie(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   
    // ����ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת
    Euler_angle omega_nen(-Velocity(1) / Rmh, Velocity(0) / Rnh, Velocity(0) / Rnh * tanl); 
    // ���nϵ�൱��iϵ����ת
    Euler_angle omega_nin(omega_nie + omega_nen);

	// ���õ�Ч��תʸ���������㷨�����ڸ��������� ��������ϵ����ڵ��Ĺ�������ϵ�ı仯bib ���Ե�Ч��תʸ����ʽ��ʾ��
	Euler_angle Phi_bib(delta_theta1 + delta_theta2 + 2.0 / 3 * delta_theta1.cross(delta_theta2));

    // ���㵼������ϵ����ڵ��Ĺ�������ϵ�ı仯�������ٶȺ�λ�������omega_nin�ı仯��С��
    // ��˿����ó�ֵ���棬��仯������һ��ʱ�̵ı仯�ʳ��Բ�������
    Euler_angle Phi_nin(omega_nie.value * T);

    // ========�α��㷨
    Direct_cosine_matrix Cnm_nm_1(Phi_nin.EA2DCM().transpose());
    Direct_cosine_matrix Cbm_1_bm(Phi_bib.EA2DCM());
    Direct_cosine_matrix Cnm_bm(Cnm_nm_1.value);
    Direct_cosine_matrix Cnb_m_1(Qnb.QV2DCM());     // ����ϵb����ڵ�������ϵn����̬��,Ҳ������t(m-1)ʱ����bϵת��Ϊnϵ����̬��������
    Direct_cosine_matrix Cnb_m(Cnm_nm_1.value * Cnb_m_1.value * Cbm_1_bm.value);    // ��t(m)ʱ�̵Ľ�����̬����
    return Cnb_m;
    //// ========����ʦmatlab�㷨
    //// �仯���Ч��תʸ��
    //Euler_angle ea(Phi_bib.value - Cnb.value.transpose() * Phi_nin.value);
    //// ���仯��ĵ�Ч��תʸ���任Ϊ��Ԫ����ʾ
    //Quaternion_vector qv(ea.EA2QV());
    //// ������̬����
    //Quaternion_vector Qnb(qv * Qnb);
    //return Qnb;
}
