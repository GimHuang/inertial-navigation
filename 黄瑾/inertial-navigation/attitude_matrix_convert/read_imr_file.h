#pragma once
/*
*************************************************************************************
*file:	read_imr_file.h
*@Description: ����imr��ʽ���ݹ��ɣ���ȡIMR��ʽ�������ļ����ļ�ͷ�������壩
*creator��Jin Huang
*organization:sdust
*e-mail:kim.huang.j@qq.com
*datetime��22/11/2020
*************************************************************************************
*/
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

// �������ݸ�ʽ
#define int8_t unsigned char
#define int32_t int
#define unit8_t char
//#define time_type long double

// ����ṹ�壬�����ṹ�峤��Ϊ512�ֽ�
struct IMR_Header
{
	char szHeader[8];				// "$IMURAW\0"
	int8_t bIsIntelOrMotorola;		// 0 ��ʾС�ֽڣ� 1��ʾ���ֽ�
	double dVersionNumber;			// ������Դ����������汾��
	int32_t bDeltaTheta;			// 0 �����������ݽ��Ա仯�ĽǶ����ʶ�ȡ
									// 1 ��Ĭ�ϣ������������ݽ�����ȡΪdelta����������
	int32_t bDeltaVelocity;			// 0 �����������ݽ��Ա仯�ļ��ٶȶ�ȡ
									// 1 ��Ĭ�ϣ������������ݽ�����ȡΪ�ٶȣ����ٶ�����
	double dDataRateHz;				// IMU �Ĳ����� (Hz) 
	double dGyroScaleFactor;		// bDeltaTheta == 0 �������ǵĲ���ֵ���Ը�ֵ�õ� ��/��
									// bDeltaTheta == 1 �������ǵĲ���ֵ���Ը�ֵ�õ� �ȣ�Ȼ�����dDataRateHz�õ� ��/��
	double dAccelScaleFactor;		// bDeltaVelocity == 0 ��ʵ�����ݳ��Ը�ֵ�õ� m/s^2
									// bDeltaVelocity == 1 ��ʵ�����ݳ��Ը�ֵ�õ� m/s�� �ٳ���dDataRateHz�͵õ� m/s^2
	int32_t iUtcOrGpsTime;			// ��ʱ���Ƕ���Ϊһ�ܵ�GPS��UTC��
									// iUtcOrGpsTime == 0 δ֪��Ĭ��ΪGPS
									// iUtcOrGpsTime == 1 ʱ���ǩ��ÿ�ܵ�UTC��
									// iUtcOrGpsTime == 2 ʱ���ǩ��ÿ�ܵ�GPS��
	int32_t iRcvTimeOrCorrTime;		// ����ʱ�����Ƿ��ڵڶ�����ƵĶ�����Ϊ���ջ�ʱ��ƫ�����У��
	double dTimeTagBias;			// GPS��IMUʱ���ǩ֮���ƫ��
	char szImuName[32];				// ʹ�õ�IMU����
	unit8_t reserved1[4];			// ���ޣ�����
	char szProgramName[32];			// ���ó�������
	char tCreate[12];				// �����ļ�ʱ��
	bool bLeverArmValid;			// ���IMU����GNSS���ߵĸ˱۴洢�ڴ˱�ͷ�У���Ϊ����ֵΪ��
	int32_t lXoffset;				// �ܸ˱�X mm
	int32_t lYoffset;				// �ܸ˱�Y mm
	int32_t lZoffset;				// �ܸ˱�Z mm
	int8_t Reserved[354];			// ����δ��ʹ��
};

// The single header, which is a total of 512 bytes long, 
// is followed by a structure of the following type for each IMU measurement epoch


struct IMR_Record
{
	double Time;		// ��ǰ�۲��ʱ��
	int32_t gx;			// ����IMU x����������ݲ�����
	int32_t gy;			// ����IMU y����������ݲ�����
	int32_t gz;			// ����IMU z����������ݲ�����
	int32_t ax;			// ����IMU x������ż��ٲ���
	int32_t ay;			// ����IMU y������ż��ٲ���
	int32_t az;			// ����IMU z������ż��ٲ���
};

struct adj_IMR_Record
{
	double Time;		// ��ǰ�۲��ʱ��
	double gx;			// ����IMU x����������ݲ�����
	double gy;			// ����IMU y����������ݲ�����
	double gz;			// ����IMU z����������ݲ�����
	double ax;			// ����IMU x������ż��ٲ���
	double ay;			// ����IMU y������ż��ٲ���
	double az;			// ����IMU z������ż��ٲ���
};


void read_imr_header(fstream &imrfile, IMR_Header *imrheader)
{
	/*
	*************************************************************************************
	*func:	read_imr_header
	*@Param:	fstream &imrfile �ļ���
	*@Param:	IMR_Header *imrheader �ṹ��ָ�룬����ļ�ͷ����
	*@Return:
	*@Note:	��ȡimrͷ�ļ�
	*creator��Jin Huang
	*organization:sdust
	*e-mail:kim.huang.j@qq.com
	*************************************************************************************
	*/
	for (int i = 0; i < 8; i++)
	{
		imrfile.read((char*)&imrheader->szHeader[i], sizeof(char));
	}
	imrfile.read((char*)&imrheader->bIsIntelOrMotorola, sizeof(int8_t));
	imrfile.read((char*)&imrheader->dVersionNumber, sizeof(double));
	imrfile.read((char*)&imrheader->bDeltaTheta, sizeof(int32_t));
	imrfile.read((char*)&imrheader->bDeltaVelocity, sizeof(int32_t));
	imrfile.read((char*)&imrheader->dDataRateHz, sizeof(double));
	imrfile.read((char*)&imrheader->dGyroScaleFactor, sizeof(double));
	imrfile.read((char*)&imrheader->dAccelScaleFactor, sizeof(double));
	imrfile.read((char*)&imrheader->iUtcOrGpsTime, sizeof(int32_t));
	imrfile.read((char*)&imrheader->iRcvTimeOrCorrTime, sizeof(int32_t));
	imrfile.read((char*)&imrheader->dTimeTagBias, sizeof(double));
	for (int i = 0; i < 32; i++)
	{
		imrfile.read((char*)&imrheader->szImuName[i], sizeof(char));
	}
	for (int i = 0; i < 4; i++)
	{
		imrfile.read((char*)&imrheader->reserved1[i], sizeof(unit8_t));
	}
	for (int i = 0; i < 32; i++)
	{
		imrfile.read((char*)&imrheader->szProgramName[i], sizeof(char));
	}
	for (int i = 0; i < 12; i++)
	{
		imrfile.read((char*)&imrheader->tCreate[i], sizeof(char));
	}
	
	imrfile.read((char*)&imrheader->bLeverArmValid, sizeof(bool));
	imrfile.read((char*)&imrheader->lXoffset, sizeof(int32_t));
	imrfile.read((char*)&imrheader->lYoffset, sizeof(int32_t));
	imrfile.read((char*)&imrheader->lZoffset, sizeof(int32_t));
	for (int i = 0; i < 354; i++)
	{
		imrfile.read((char*)&imrheader->Reserved[i], sizeof(int8_t));
	}
	//imrfile.close();
}



void read_imr_data(fstream& imrfile, vector<adj_IMR_Record> &adj_data, IMR_Header imr_header)
{

	/*
	*************************************************************************************
	*func:		read_imr_data
	*@Param:	fstream& imrfile �ļ���
	*@Param:	vector<adj_IMR_Record> &adj_data ����vector ��ŵ�У����imr���ݵĽṹ������
	*@Param:	IMR_Header imr_header imr�����ļ�ͷ
	*@Return:
	*@Note:		��ȡimr���ݲ��֣���ͨ��imr_header ����У����ͨ��vector<adj_IMR_Record> &adj_data ����
	*creator��Jin Huang
	*organization:sdust
	*e-mail:kim.huang.j@qq.com
	*************************************************************************************
	*/
	imrfile.seekg(512, ios::beg);	// �����ļ�ͷ����
	IMR_Record* imr_data = new IMR_Record;
	adj_IMR_Record* temp_data = new adj_IMR_Record;
	while (!imrfile.eof())
	{
		imrfile.read((char*)imr_data, sizeof(IMR_Record));
		temp_data->Time = imr_data->Time;
		if (!imr_header.bDeltaTheta)	// �������ǲ�ĵ����������Ը���
		{
			temp_data->gx = imr_data->gx * imr_header.dGyroScaleFactor;
			temp_data->gy = imr_data->gy * imr_header.dGyroScaleFactor;
			temp_data->gz = imr_data->gz * imr_header.dGyroScaleFactor;
		}
		else
		{
			temp_data->gx = imr_data->gx * imr_header.dGyroScaleFactor * imr_header.dDataRateHz;
			temp_data->gy = imr_data->gy * imr_header.dGyroScaleFactor * imr_header.dDataRateHz;
			temp_data->gz = imr_data->gz * imr_header.dGyroScaleFactor * imr_header.dDataRateHz;
		}
		if (!imr_header.bDeltaVelocity)
		{
			temp_data->ax = imr_data->ax * imr_header.dAccelScaleFactor;
			temp_data->ay = imr_data->ay * imr_header.dAccelScaleFactor;
			temp_data->az = imr_data->az * imr_header.dAccelScaleFactor;
		}
		else
		{
			temp_data->ax = imr_data->ax * imr_header.dAccelScaleFactor * imr_header.dDataRateHz;
			temp_data->ay = imr_data->ay * imr_header.dAccelScaleFactor * imr_header.dDataRateHz;
			temp_data->az = imr_data->az * imr_header.dAccelScaleFactor * imr_header.dDataRateHz;
		}
		adj_data.push_back(*temp_data);
	}
	imrfile.close();
}
