/*
*************************************************************************************
*file:  updating.h
*@Description:ʵ�ֵ�������ϵ(g����ϵ)�µ���̬���ٶȡ�λ�ø���
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
#include"read_imr_file.h"
using namespace std;
using namespace Eigen;

// �����ṹ����ڱ�����̬���½��
struct attitude_updating_data
{
    
    Euler_angle omega_nie;  // ���򣨵�������ϵ������ת����ת�����Ĺ�������ϵ�£��Ľ��ٶ��ڵ�������ϵ�µ�ͶӰ
    Euler_angle omega_nen;  // �ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת
    Euler_angle omega_nin;  // ���nϵ�൱��iϵ����ת
    Euler_angle Phi_bib;    // ��������ϵ����ڵ��Ĺ�������ϵ�ı仯bib ���Ե�Ч��תʸ����ʽ��ʾ��
    Euler_angle Phi_nin;    //��������ϵ����ڵ��Ĺ�������ϵ�ı仯
    Direct_cosine_matrix Cnb_m; // ��t(m)ʱ�̵Ľ�����̬����
};

struct speed_updating_data
{
    Vector3f v_nem; // �����ڵ���ϵ�µļ����˶��ٶ�
};

struct position_updating_data
{
    Vector3f position;
};

struct input_data
{
    Direct_cosine_matrix Dnb;   // ��һʱ�̵���bϵ��nϵ�ķ���������
    //vector<Vector3f> delta_theta1;      // ��һ�β���
    vector<Vector3f> delta_theta;      // ���β�����������ֻ���õ��������
    vector<Vector3f> position;          // δ�仯ǰλ��
    vector<Vector3f> Velocity;          // �����˶����ٶ�[E N U]
    float T;                    // �����������
};


// ===========================��̬���ٶȡ�λ�ø���===========================
class update
{
public:
    //  ���m-1ʱ�̵ķ��������󣬴��m��m-1ʱ�̵Ĳ������ݷֱ���vector[2],vecotr[1]�����m-1ʱ�̣�m-2ʱ�̵�λ��������vector[1],vector[0]�����m-1,m-2ʱ�̵��ٶ�������vector[1],vector[0], ��Ų�������
    update(Direct_cosine_matrix a1, vector<Vector3f> a2, vector<Vector3f> a3, vector<Vector3f> a4, vector<Vector3f> a5, double a7);
    input_data inputdata;   // ��������
    attitude_updating_data attitude_data;           // ��̬���½��
    vector<speed_updating_data> speed_data;         // �ٶȸ��½����ͨ����������ٶȸ��½����[0] m-2 ʱ�̵��ٶȸ��£�[1]��m -1 ʱ�̵��ٶȸ��£�[2]��m ʱ�̵��ٶȸ���
    vector<position_updating_data> position_data;   // λ�ø��½����ͨ���������λ�ø��½����[0] m-1 ʱ�̵�λ�ã�[1]��m ʱ�̵�λ��
    void attitude_update(); // ��̬����
    void speed_update();    // �ٶȸ���
    void position_update(); // λ�ø���
    // ����ͨ�ñ���
private:
    float sinl, cosl, tanl;
    float Rm;
    float Rn;
    float Rmh;
    float Rnh;
};


// ͨ������Ϊ1*3��vector����������vector[0]��ʾm-2ʱ�̵�����,vector[1]��ʾm-1ʱ�̵�����,vector[2]��ʾ��ǰmʱ�̵�����
update::update(Direct_cosine_matrix a1,vector<Vector3f> a2, vector<Vector3f> a3, vector<Vector3f> a4, vector<Vector3f> a5, double a7)
{
    // ��ʼ�����ݴ�С
    inputdata.delta_theta.resize(3);    
    inputdata.position.resize(3);
    inputdata.Velocity.resize(3);
    speed_data.resize(2);   // ���������������pushback
    position_data.resize(1);// ��2�����������pushback
    // ����������ת����������
    // ���m - 1ʱ�̵ķ���������
    inputdata.Dnb = a1;
    // ���m��m - 1ʱ�̵Ĳ������ݷֱ���vector[2], vecotr[1]
    inputdata.delta_theta[2] = a3[1];
    inputdata.delta_theta[1] = a3[0];
    // ��ʼ�����m-2,m-1ʱ�̵��ٶ���vector[1], vecotr[0]
    speed_data[1].v_nem = a2[1];
    speed_data[0].v_nem = a2[0];
    // ���m - 1ʱ�̣�m - 2ʱ�̵�λ��������vector[1], vector[0]
    inputdata.position[1] = a4[1];
    inputdata.position[0] = a4[0];
    // ���m , m - 1ʱ�̵��ٶ�������vector[1], vector[0]
    inputdata.Velocity[2] = a5[1];
    inputdata.Velocity[1] = a5[0];
    // ��ų�ʼλ��
    position_data[0].position = a4[1];
    // ��Ų�������
    inputdata.T = a7;
}


void update::attitude_update()
{
    /*
    *************************************************************************************
    *func: ������̬����
    *@Param:    Dnb             ����ϵb����ڵ�������ϵn�ķ���������
    *@Param:    delta_theta1    ��һ�ν���������
    *@Param:    delta_theta2    �ڶ��ν���������
    *@Param:    inputdata.position       [L,B,H]��ʼλ�õ�γ�ȡ����ȡ���ظ�
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
    // m ʱ��
    sinl = sin(inputdata.position[2](0)); cosl = cos(inputdata.position[2](0)); tanl = tan(inputdata.position[2](0));    // ����mʱ�̵ĵ���γ��Ϊsin cos tan
    Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    Rmh = Rm + inputdata.position[2](2); // ʵ��mʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Rnh = Rn + inputdata.position[2](2); // ʵ��mʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    double g_m = earth.g0 * (1 + 5.27094e-3 * sinl * sinl + 2.32718e-5 * sinl * sinl * sinl * sinl) - 3.086e-6 * inputdata.position[2](2);    // ����mʱ�̵ĵ����������ٶ�
    Euler_angle omega_nie_m(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   // ����mʱ�̵�w_nie��������ת����ĵ���ϵ��ת
    Euler_angle omega_nen_m(-inputdata.Velocity[2](1) / Rmh, inputdata.Velocity[2](0) / Rnh, inputdata.Velocity[2](0) / Rnh * tanl);    //����mʱ�̹ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת

    // ���򣨵�������ϵ������ת����ת�����Ĺ�������ϵ�£��Ľ��ٶ��ڵ�������ϵ�µ�ͶӰ
    Euler_angle omega_nie(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   
    attitude_data.omega_nie = omega_nie;
    // ����ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת
    Euler_angle omega_nen(-inputdata.Velocity[2](1) / Rmh, inputdata.Velocity[2](0) / Rnh, inputdata.Velocity[2](0) / Rnh * tanl);
    attitude_data.omega_nen = omega_nen;
    // ���nϵ�൱��iϵ����ת
    Euler_angle omega_nin(omega_nie + omega_nen);
    attitude_data.omega_nin = omega_nin;

	// ���õ�Ч��תʸ���������㷨�����ڸ��������� ��������ϵ����ڵ��Ĺ�������ϵ�ı仯bib ���Ե�Ч��תʸ����ʽ��ʾ��
	Euler_angle Phi_bib(inputdata.delta_theta[1] + inputdata.delta_theta[2] + 2.0 / 3 * inputdata.delta_theta[1].cross(inputdata.delta_theta[2]));
    attitude_data.Phi_bib = Phi_bib;

    // ���㵼������ϵ����ڵ��Ĺ�������ϵ�ı仯�������ٶȺ�λ�������omega_nin�ı仯��С��
    // ��˿����ó�ֵ���棬��仯������һ��ʱ�̵ı仯�ʳ��Բ�������
    Euler_angle Phi_nin(omega_nie.value * inputdata.T);
    attitude_data.Phi_nin = Phi_nin;

    // ========�α��㷨
    Direct_cosine_matrix Cnm_nm_1(Phi_nin.EA2DCM().transpose());
    Direct_cosine_matrix Cbm_1_bm(Phi_bib.EA2DCM());
    Direct_cosine_matrix Cnm_bm(Cnm_nm_1.value);
    Direct_cosine_matrix Cnb_m_1(inputdata.Dnb.value);     // ����ϵb����ڵ�������ϵn����̬��,Ҳ������t(m-1)ʱ����bϵת��Ϊnϵ����̬��������
    Direct_cosine_matrix Cnb_m(Cnm_nm_1.value * Cnb_m_1.value * Cbm_1_bm.value);    // ��t(m)ʱ�̵Ľ�����̬����
    attitude_data.Cnb_m = Cnb_m;
}


void update::speed_update()
{
    /*
    *************************************************************************************
    *func:  speed_update
    *@Param:
    *@Param:
    *@Return:
    *@Note: �����ٶȸ��¼���
    *creator��Jin Huang
    *organization:sdust
    *e-mail:kim.huang.j@qq.com
    *************************************************************************************
    */
    // ��������ʱ�̵��������ٶ�
    // m ʱ��
    //sinl = sin(inputdata.position[2](0)); cosl = cos(inputdata.position[2](0)); tanl = tan(inputdata.position[2](0));    // ����mʱ�̵ĵ���γ��Ϊsin cos tan
    //Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    //Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    //Rmh = Rm + inputdata.position[2](2); // ʵ��mʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    //Rnh = Rn + inputdata.position[2](2); // ʵ��mʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    //double g_m = earth.g0 * (1 + 5.27094e-3 * sinl * sinl + 2.32718e-5 * sinl * sinl * sinl * sinl) - 3.086e-6 * inputdata.position[2](2);    // ����mʱ�̵ĵ����������ٶ�
    //Euler_angle omega_nie_m(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   // ����mʱ�̵�w_nie��������ת����ĵ���ϵ��ת
    //Euler_angle omega_nen_m(-inputdata.Velocity[2](1) / Rmh, inputdata.Velocity[2](0) / Rnh, inputdata.Velocity[2](0) / Rnh * tanl);    //����mʱ�̹ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת
    
    // m-1 ʱ��
    sinl = sin(inputdata.position[1](0)); cosl = cos(inputdata.position[1](0)); tanl = tan(inputdata.position[1](0));    // �������γ��Ϊ m-1 ʱ�̵� sin cos tan
    Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    Rmh = Rm + inputdata.position[1](2); // ʵ��m-1ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Rnh = Rn + inputdata.position[1](2); // ʵ��m-1ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    double g_m_1 = earth.g0 * (1 + 5.27094e-3 * sinl * sinl + 2.32718e-5 * sinl * sinl * sinl * sinl) - 3.086e-6 * inputdata.position[1](2);    // ����m-1ʱ�̵ĵ����������ٶ�
    Euler_angle omega_nie_m_1(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   // ����m-1ʱ�̵�w_nie��������ת����ĵ���ϵ��ת
    Euler_angle omega_nen_m_1(-inputdata.Velocity[1](1) / Rmh, inputdata.Velocity[1](0) / Rnh, inputdata.Velocity[1](0) / Rnh * tanl);    //����m-1ʱ�̹ߵ�ϵͳ�ڵ�����渽���ƶ��������������������nϵ��ת

                                                                             
    // m-2 ʱ��
    sinl = sin(inputdata.position[0](0)); cosl = cos(inputdata.position[0](0)); tanl = tan(inputdata.position[0](0));    // �������γ��Ϊ m-2 ʱ�̵� sin cos tan
    Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    Rmh = Rm + inputdata.position[0](2); // ʵ��m-2ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Rnh = Rn + inputdata.position[0](2); // ʵ��m-2ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    double g_m_2 = earth.g0 * (1 + 5.27094e-3 * sinl * sinl + 2.32718e-5 * sinl * sinl * sinl * sinl) - 3.086e-6 * inputdata.position[0](2);    // ����m-2ʱ�̵ĵ����������ٶ�
    Euler_angle omega_nie_m_2(0, earth.phi_ie * cosl, earth.phi_ie * sinl);   // ����m-2ʱ�̵�w_nie��������ת����ĵ���ϵ��ת
    Euler_angle omega_nen_m_2(-inputdata.Velocity[1](1) / Rmh, inputdata.Velocity[1](0) / Rnh, inputdata.Velocity[1](0) / Rnh * tanl);    //����m-2ʱ�̹ߵ�ϵͳ�ڵ�����渽���ƶ��������������������
    

    // ���Ʒ����� �ĸ�����
    Euler_angle omega_nie_m_05((3 * omega_nie_m_1.value - omega_nie_m_2.value) / 2);
    Euler_angle omega_nen_m_05((3 * omega_nen_m_1.value - omega_nen_m_2.value) / 2);
    Vector3f v_n_m_05 = ((3 * speed_data[1].v_nem - speed_data[0].v_nem) / 2);
    Vector3f g_m_05 = Eigen::Vector3f::Zero();
    g_m_05(2) = -(3 * g_m_1 - g_m_2) / 2;
    
    // �����к����ٶȵ��ٶ�����
    Vector3f delta_v_ncorg = (-(2 * omega_nie_m_05.value + omega_nen_m_05.value).cross(v_n_m_05) + g_m_05) * inputdata.T;

    // �����ٶ���������

    // �����ٶȵ���ת������delta_v_brot
    Vector3f delta_v_brot = 0.5 * attitude_data.omega_nin.value.cross(inputdata.Velocity[2] + inputdata.Velocity[1]);   // delta v �����β����ı仯֮�ͱ�ʾ
    // ���㻮��������delta_v_bscul
    Vector3f delta_v_bscul = 2.0 / 3 * (inputdata.delta_theta[1].cross(inputdata.Velocity[2]) + inputdata.Velocity[1].cross(inputdata.delta_theta[2]));
    //Vector3f delta_v_bscul = 
    // �������������ٶȱ仯delta_v_nsf
    Matrix3f I = Eigen::Matrix3f::Identity();
    Vector3f delta_v_nsf = (I - inputdata.T / 2 * (omega_nie_m_05 + omega_nen_m_05).EA2ssm()) * attitude_data.Cnb_m.value * (inputdata.Velocity[2] + delta_v_brot + delta_v_bscul);

    // inputdata.T / 2 �Ƿ���ȷ������������

    //�����������ڵ���ϵ�µļ����˶��ٶ�
    Vector3f v_nm = speed_data[0].v_nem + delta_v_nsf + delta_v_ncorg;
    speed_updating_data v_nm_m; v_nm_m.v_nem = v_nm;
    speed_data.push_back(v_nm_m);   // �� m ʱ�̵��ٶȸ��½�����ӵ��ڴ���
}


void update::position_update()
{
    /*
    *************************************************************************************
    *func:  position_update
    *@Param:
    *@Param:
    *@Return:
    *@Note: ������̬���¼���
    *creator��Jin Huang
    *organization:sdust
    *e-mail:kim.huang.j@qq.com
    *************************************************************************************
    */
    // ����M_pv
    // m-1 ʱ��
    sinl = sin(inputdata.position[1](0)); cosl = cos(inputdata.position[1](0)); tanl = tan(inputdata.position[1](0));    // �������γ��Ϊ m-1 ʱ�̵� sin cos tan
    Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    Rmh = Rm + inputdata.position[1](2); // ʵ��m-1ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Rnh = Rn + inputdata.position[1](2); // ʵ��m-1ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Matrix3f M_pv_m_1 = Eigen::Matrix3f::Zero();
    M_pv_m_1(0, 1) = 1.0 / Rmh;
    M_pv_m_1(1, 0) = 1.0 / (cosl * Rnh);
    M_pv_m_1(2, 2) = 1;

    // m-2 ʱ��
    sinl = sin(inputdata.position[0](0)); cosl = cos(inputdata.position[0](0)); tanl = tan(inputdata.position[0](0));    // �������γ��Ϊ m-2 ʱ�̵� sin cos tan
    Rn = earth.R_e * (1 * earth.e1 * sinl * sinl);   // ����î��Ȧ�����ʰ뾶
    Rm = earth.R_e * (1 - 2 * earth.e1 + 3 * earth.e1 * sinl * sinl);    // ��������Ȧ�����ʰ뾶
    Rmh = Rm + inputdata.position[0](2); // ʵ��m-2ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Rnh = Rn + inputdata.position[0](2); // ʵ��m-2ʱ�̳��� ����Ȧ���ʰ뾶 + ��ظ���������ľ���
    Matrix3f M_pv_m_2 = Eigen::Matrix3f::Zero();
    M_pv_m_2(0, 1) = 1.0 / Rmh;
    M_pv_m_2(1, 0) = 1.0 / (cosl * Rnh);
    M_pv_m_2(2, 2) = 1;

    // ���Ʒ�����M_pv_05m
    Matrix3f M_pv_05m = Eigen::Matrix3f::Zero();
    M_pv_05m = (3 * M_pv_m_1 - M_pv_m_2) / 2;
    position_updating_data p_m;
    p_m.position = position_data[0].position + M_pv_05m * (speed_data[0].v_nem + speed_data[1].v_nem) * inputdata.T;    // ������º�λ��
    // �����º�λ�ô���
    position_data.push_back(p_m);
}
