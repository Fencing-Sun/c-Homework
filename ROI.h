#pragma once
#include <fstream>
#include<iostream>
#include<vector>
#include<string>
#include<time.h>
#include <math.h>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;
typedef unsigned short Type;

//template<class T>
class CPoint
{
public:
	CPoint() {};
	~CPoint() {};
	int ID;
	int i_posi_x;
	int i_posi_y;
	int i_NumImgBand;
	Type*pd_ValuePixel;
};

//template<class T>
class CImg
{
public:
	CImg() {};
	~CImg() {};
	string s_Description;
	int i_Samples;
	int i_Lines;
	int i_NumBands;
	double d_HeaderOffset;
	string s_FileType;
	int i_DataType;
	string s_InterLeave;
	string s_SensorType;
	int i_ByteOrder;
	//vector<CPoint>v_Points;
	CPoint* pC_Points;
	void ReadFileHDR(const string& filepath);
	void ReadFileImg(const string& filepath);
	void ShowImg();
};

class Cfenlei: public CImg
{
public:
	//~Cfenlei();
	int i_Classes;//����Ӱ��������
	int i_Bands;//����Ӱ���HDR�ļ�ȡ�Ĳ�����������1��
	int i_DataType_fl;
	char ** ClassPixelValue;//��¼��ȡ�ķ���Ӱ������ֵ
	void ReadFileHDR_fl(const string& filepath);
	void ReadSVMImg(const string& filepath);
	void WriteFileImg(const string& filepath, const string& w_fenlei);
};

class ROI
{
public:
	ROI() {};
	//~ROI();
	string RoiName, RoiRgbValue;
	int npts;
	int Code;
	double * Position_x;
	double * Position_y;
	int NumImgBand;
	double* ValuePixel;
	double GetNumImgBand(){ return NumImgBand; }
};
class n_ROI
{
public:
	//~n_ROI();
	int NumROI;
	vector <ROI> MutilRoi;
	double* GetRoiMean();//�������ROI��������
	double** ClusterCenter;
	void GetClusterCenter();//������ROI�ľ������ĸ�ֵ��ClusterCenterָ��
	void ReadFile(const string & filepath);
	void PrintResult(const string& ResultPath);
};

class ConductMMD :public CImg
{
public:
	~ConductMMD();
	int NumClassification;
	int * ClassPixelCode;//��¼MMD�����и������ص�ķ������ֵ��0-4��
	double P0;//������ྫ��
	double Kappa;//Kappaϵ��
	void fenlei(n_ROI objRoi);//���ݼ���ĵõ��ľ�����࣬����ÿ����Ԫ�����ķ������
	void WriteImg(const string & Filename);
	void WriteHDR(const string& Filename, n_ROI objRoi);
	void PresionEvaluation(Cfenlei SVMclassfication);
	
};




