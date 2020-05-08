#pragma once
#include <fstream>
#include<iostream>
#include<vector>
#include<string>
#include<time.h>
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
};

class Cfenlei: public CImg
{
public:
	//~Cfenlei();
	int i_Classes;//分类影像的类别数
	int i_Bands;//分类影像读HDR文件取的波段数（等于1）
	int i_DataType_fl;
	char ** ClassPixelValue;//记录读取的分类影像的类别值
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
	double* GetRoiMean();//计算各个ROI聚类中心
	double** ClusterCenter;
	void GetClusterCenter();//将各个ROI的聚类中心赋值给ClusterCenter指针
	void ReadFile(const string & filepath);
	void PrintResult(const string& ResultPath);
};

class ConductMMD :public CImg
{
public:
	~ConductMMD();
	int NumClassification;
	int * ClassPixelCode;//记录MMD分类中各个像素点的分类代码值（0-4）
	double P0;//总体分类精度
	double Kappa;//Kappa系数
	void fenlei(n_ROI objRoi);//根据计算的得到的距离分类，返回每个像元所属的分类代码
	void WriteImg(const string & Filename);
	void WriteHDR(const string& Filename, n_ROI objRoi);
	void PresionEvaluation(Cfenlei SVMclassfication);
	
};




