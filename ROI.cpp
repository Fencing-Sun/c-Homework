#include "ROI.h"
#include<iomanip>
using namespace std;

//template<class T>
void CImg::ReadFileHDR(const string& filepath)
{
	ifstream fin;
	string s_tmp;
	int i_NumLines;
	unsigned int ul_Index = 0;
	vector<string> vs_tmp;
	fin.open(filepath, ios_base::in);
	if (!fin)
		cout << "file can't open!" << endl;
	while (getline(fin, s_tmp))
	{
		vs_tmp.push_back(s_tmp);
	}
	fin.close();
	//读description
	i_NumLines = 1;
	ul_Index = vs_tmp[i_NumLines].find("=");
	//s_tmp = vs_tmp[i_NumLines].substr(ul_Index + 1) + vs_tmp[i_NumLines+1] + vs_tmp[i_NumLines+2];
	s_tmp = vs_tmp[i_NumLines + 1];
	s_Description = s_tmp;
	//读samples
	i_NumLines = 4;
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_Samples = stoi(s_tmp);
	//读lines
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_Lines = stoi(s_tmp);
	//读bands
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_NumBands = stoi(s_tmp);
	//读header offset
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	d_HeaderOffset = stoi(s_tmp);
	//读file type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_FileType = s_tmp;
	//读data type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_DataType = stoi(s_tmp);
	//读interleave
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_InterLeave = s_tmp;
	//读sensor type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_SensorType = s_tmp;
	//读byte order
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_ByteOrder = stoi(s_tmp);
	if (i_NumLines == vs_tmp.size())
		cout << "HDRFile read succeeded!" << endl;
	else
		cout << "HDRFile read failed!" << endl;
}

//template<class T>
void CImg::ReadFileImg(const string& filepath)
{
	ifstream fin;
	fin.open(filepath, ios_base::in | ios_base::binary);
	if (!fin)
		cout << "file can't open!" << endl;

	//将数据读入二维数组p2us_tmp;
	Type** p2us_tmp;
	p2us_tmp = new Type * [i_NumBands];
	for (int i = 0;i < i_NumBands;i++)
	{
		p2us_tmp[i] = new Type[i_Lines * i_Samples];
	};
	for (int i = 0;i < i_NumBands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			fin.read(reinterpret_cast<char*>(&p2us_tmp[i][j]), sizeof(Type));
		}
	fin.close();
	//将数据添加到类中并赋每个像素点的波段值；
	pC_Points = new CPoint[i_Lines * i_Samples];
	for (int i = 0;i < i_Lines * i_Samples;i++)
		pC_Points[i].pd_ValuePixel = new Type[i_NumBands];
	for (int j = 0;j < i_Lines;j++)
		for (int k = 0;k < i_Samples;k++)
		{
			for (int i = 0;i < i_NumBands;i++)
			{
				pC_Points[j * i_Samples + k].ID = j * i_Samples + k;
				pC_Points[j * i_Samples + k].i_NumImgBand = i_NumBands;
				pC_Points[j * i_Samples + k].i_posi_x = k + 1;
				pC_Points[j * i_Samples + k].i_posi_y = j + 1;
				pC_Points[j * i_Samples + k].pd_ValuePixel[i] = p2us_tmp[i][j * i_Samples + k];
			};
		}
	/*ofstream fout;//将读取的数据写入文件
	fout.open("C:\\Users\\sun-PC\\Desktop\\resule1.txt", ios_base::out);
	fout << setiosflags(ios::right);
	fout << setw(7) << "ID"
		<< setw(7) << "X"
		<< setw(7) << "Y";
	for (int i = 0;i < i_NumBands;i++)
		fout << setw(5) << "B" << i+1;
	fout << endl;

	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		fout << setw(7) << pC_Points[i].ID << setw(7) << pC_Points[i].i_posi_x << setw(7) << pC_Points[i].i_posi_y;
		for (int j = 0;j < i_NumBands;j++)
		{
			fout << setw(7) << pC_Points[i].pd_ValuePixel[j];
		};
		fout << endl;
	}
	fout.close();
	cout << "Write Succeed! " << endl;
	//计算灰度值
	double d_DNmax = pC_Points[0].pd_ValuePixel[0];
	double d_DNmin = pC_Points[0].pd_ValuePixel[0];
	for (int i = 0;i < i_Lines * i_Samples;i++)
		for (int j = 0;j < i_NumBands;j++)
		{
			if (d_DNmax < pC_Points[i].pd_ValuePixel[j])
				d_DNmax = pC_Points[i].pd_ValuePixel[j];
			if (d_DNmin > pC_Points[i].pd_ValuePixel[j])
				d_DNmin = pC_Points[i].pd_ValuePixel[j];
		};
	//将DN值转化为灰度值
	for (int i = 0;i < i_Lines * i_Samples;i++)
		for (int j = 0;j < i_NumBands;j++)
		{
			pC_Points[i].pd_ValuePixel[j] = int((pC_Points[i].pd_ValuePixel[j] - d_DNmin) / (d_DNmax - d_DNmin) * 255);
		};
	//将灰度值写入result2文件
	fout.open("C:\\Users\\sun-PC\\Desktop\\resule2.txt", ios_base::out);
	fout << setw(7) << "ID"
		<< setw(7) << "X"
		<< setw(7) << "Y";
	for (int i = 0;i < i_NumBands;i++)
		fout << setw(5) << "B" << i;
	fout << endl;

	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		fout << setw(7) << pC_Points[i].ID << setw(7) << pC_Points[i].i_posi_x << setw(7) << pC_Points[i].i_posi_y;
		for (int j = 0;j < i_NumBands;j++)
		{
			fout << setw(7) << pC_Points[i].pd_ValuePixel[j];
		};
		fout << endl;
	}
	fout.close();
	cout << "Write Succeed! " << endl;*/

	//释放空间
	for (int i = 0;i < i_NumBands;i++)
		delete[]p2us_tmp[i];
	delete[]p2us_tmp;

}
void CImg::ShowImg()
{
	//计算灰度值
	double d_DNmax = pC_Points[0].pd_ValuePixel[0];
	double d_DNmin = pC_Points[0].pd_ValuePixel[0];
	for (int i = 0;i < i_Lines * i_Samples;i++)
		for (int j = 0;j < i_NumBands;j++)
		{
			if (d_DNmax < pC_Points[i].pd_ValuePixel[j])
				d_DNmax = pC_Points[i].pd_ValuePixel[j];
			if (d_DNmin > pC_Points[i].pd_ValuePixel[j])
				d_DNmin = pC_Points[i].pd_ValuePixel[j];
		};
	//将DN值转化为灰度值
	/*for (int i = 0;i < i_Lines * i_Samples;i++)
		{
			pC_Points[i].pd_ValuePixel[0] = int((pC_Points[i].pd_ValuePixel[0] - d_DNmin) / (d_DNmax - d_DNmin) * 255);
		};*/
	Type ** tmp;
	tmp = new Type * [i_Lines];
	for (int i = 0;i < i_Lines;i++)
	{
		tmp[i] = new Type[ i_Samples];
	};
	for (int j = 0;j < i_Lines;j++)
		for (int k = 0;k < i_Samples;k++)
			//tmp[j][k]=pC_Points[j * i_Samples + k].pd_ValuePixel[2] ;
			tmp[j][k] = int((pC_Points[j * i_Samples + k].pd_ValuePixel[0] - d_DNmin) / (d_DNmax - d_DNmin) * 255);
	Mat Img(i_Lines, i_Samples, CV_8UC1);
	for (int i = 0; i < Img.rows; ++i)
		for (int j = 0; j < Img.cols; ++j)
			Img.at<uchar>(i, j) = tmp[i][j];
	//namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", Img); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window*/
	
}
;

	
void n_ROI::ReadFile(const string& filepath)
{
	ifstream fin;
	string tmp;
	ROI tmp_ROI;
	int index, index1;
	fin.open(filepath, ios_base::in);//读取文件
	if (!fin)
		cout << "file can't open!" << endl;
	getline(fin, tmp);
	fin >> tmp;//;
	fin >> tmp;//Number
	fin >> tmp;//of
	fin >> tmp;//ROI:
	fin >> NumROI;
	getline(fin, tmp);//读此行剩余数据
	getline(fin, tmp);//读图像大小
	for (int i = 0;i < NumROI;i++)
	{
		getline(fin, tmp);//读空行
		//getline(fin, tmp_ROI.RoiName);
		getline(fin, tmp);
		index = tmp.find("#");
		index1 = tmp.find(":");
		tmp_ROI.RoiName = tmp.substr(index1+2, index-index1-3);//读ROIname
		tmp_ROI.Code=stoi(tmp.substr(index+1));//读对应的类别代码
		getline(fin, tmp);
		index = tmp.find("{");
		index1 = tmp.find("}");
		tmp_ROI.RoiRgbValue= tmp.substr(index+1, index1 - index-1);//读RGB值
		fin >> tmp;//读;
		fin >> tmp;//读ROI
		fin >> tmp;//读npts:
		fin >> tmp_ROI.npts;//读取ROI点数
		getline(fin, tmp);//读该行剩余数据
		MutilRoi.push_back(tmp_ROI);
	};
	int tmp_NumImgBand = 0;
	for (int i = 0;i < 1000;i++)
	{
		fin >> tmp;
		tmp_NumImgBand++;
		if (tmp == "1")
			break;
	}

	/*for (int i = 0;i < MutilRoi.size();i++)//对每个ROI循环
	{
		cout << MutilRoi[i].Code << endl;
		cout << MutilRoi[i].RoiName << endl;
		cout << MutilRoi[i].RoiRgbValue << endl;
	}*/

	for (int i = 0;i < MutilRoi.size();i++)//对每个ROI循环
	{
		MutilRoi[i].NumImgBand = tmp_NumImgBand - 5;
		MutilRoi[i].Position_x = new double[MutilRoi[i].npts];
		MutilRoi[i].Position_y = new double[MutilRoi[i].npts];
		MutilRoi[i].ValuePixel = new double [MutilRoi[i].NumImgBand * MutilRoi[i].npts];
		for(int j=0;j< MutilRoi[i].npts;j++)//对每个POI中的每一行数据循环
		{
			fin>>MutilRoi[i].Position_x[j];
			fin>>MutilRoi[i].Position_y[j];
			for (int k = 0;k < MutilRoi[i].NumImgBand;k++)
			{
				fin >> MutilRoi[i].ValuePixel[j * MutilRoi[i].NumImgBand + k];
			};
			fin >> tmp;//读取ID；
		}


	}
	fin.close();
};

/*n_ROI::~n_ROI()
{
	for (int i = 0;i < NumROI;i++)
		delete[]ClusterCenter[i];
	delete[]ClusterCenter;
	//delete[]result;
}*/

double* n_ROI::GetRoiMean()
{
	int NumImgBand = MutilRoi[0].GetNumImgBand();
	int npts = MutilRoi[0].npts;
	double* result = new double[NumROI * NumImgBand];
	double tmp = 0;	
	for (int i = 0;i < NumROI;i++)
	{
		for (int j = 0;j < NumImgBand;j++)
		{
			for (int k = 0;k < MutilRoi[i].npts;k++)
			{
				tmp = tmp+MutilRoi[i].ValuePixel[k * NumImgBand + j];
			}
			result[i * NumImgBand + j] = tmp / MutilRoi[i].npts;
			tmp = 0;
		}
	}
	return result;
};

void n_ROI::GetClusterCenter()
{
	int NumImgBand = MutilRoi[0].NumImgBand;
	ClusterCenter = new double* [NumROI];
	for (int i = 0;i < NumROI;i++)
		ClusterCenter[i] = new double[NumImgBand];
	for (int i = 0;i < NumROI;i++)
	{
		for (int j = 0;j < NumImgBand;j++)
		{
			ClusterCenter[i][j] = GetRoiMean()[i * NumImgBand + j];
			//cout << ClusterCenter[i][j] << " ";
		}
		//cout << endl;
	}
}



void n_ROI::PrintResult(const string & ResultPath)
{
	double* result = GetRoiMean();
	ofstream fout;//将读取的数据写入文件
	fout.open(ResultPath, ios_base::out);
	for (int i = 0;i < NumROI * MutilRoi[0].NumImgBand;i++)
	{
		if (i % MutilRoi[0].NumImgBand == 0)
			fout << endl;
		fout << result[i] << "   ";
		   
	}
	fout.close();
	cout <<"Succeeded!!!" << endl;
}


void Cfenlei::ReadFileHDR_fl(const string& filepath)
{
	ifstream fin;
	string s_tmp;
	int i_NumLines;
	fin.open(filepath, ios_base::in);
	if (!fin)
		cout << "file can't open!" << endl;
	while (getline(fin, s_tmp))
	{
		if (s_tmp.substr(0, 7) == "samples")
		{
			i_Samples = atoi((s_tmp.substr(10, 13)).c_str());
		}
		if (s_tmp.substr(0, 5) == "lines")
		{
			i_Lines = atoi((s_tmp.substr(10, 13)).c_str());
		}
		if (s_tmp.substr(0, 5) == "bands")
		{
			i_Bands = atoi((s_tmp.substr(10, 11)).c_str());
		}
		if (s_tmp.substr(0, 10) == "interleave")
		{
			s_InterLeave = s_tmp.substr(13, 16);
		}
		if (s_tmp.substr(0, 9) == "data type")
		{
			i_DataType_fl = atoi((s_tmp.substr(12)).c_str());
		}
		if (s_tmp.substr(0, 7) == "classes")
		{
			i_Classes = atoi((s_tmp.substr(10)).c_str());
		}
	}
	/*cout << i_Samples << endl;
	cout << i_Lines << endl;
	cout << i_Classes << endl;*/

	fin.close();
}
void Cfenlei::ReadSVMImg(const string& filepath)
{
	ifstream fin;
	fin.open(filepath, ios_base::in | ios_base::binary);
	if (!fin)
		cout << "file can't open!" << endl;
	ClassPixelValue = new char* [i_Bands];
	for (int i = 0;i < i_Bands;i++)
	{
		ClassPixelValue[i] = new char[i_Lines * i_Samples];
	};
	for (int i = 0;i < i_Bands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			fin.read(reinterpret_cast<char*>(&ClassPixelValue[i][j]), sizeof(char));
		}
	fin.close();
}
;

void Cfenlei::WriteFileImg(const string& filepath, const string& w_fenlei)
{
	int** p2i_tmp;//存放将char转换成int类型的数据
	p2i_tmp = new int* [i_Bands];
	for (int i = 0;i < i_Bands;i++)
	{
		p2i_tmp[i] = new int[i_Lines * i_Samples];
	};
	for (int i = 0;i < i_Bands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			p2i_tmp[i][j] = int(ClassPixelValue[i][j]);
		};
	Type** p2us_tmp2;
	p2us_tmp2 = new Type * [i_NumBands];
	for (int i = 0;i < i_NumBands;i++)
	{
		p2us_tmp2[i] = new Type[i_Lines * i_Samples];
	};
	//全部初始化为0
	for (int i = 0;i < i_NumBands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			p2us_tmp2[i][j] = 0;
		}
	int i_type;
	cout << "请输入要输出的分类代码：";
	cin>> i_type;
	for (int i = 0;i < i_Bands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			if (p2i_tmp[i][j] == i_type)
				p2us_tmp2[i][j] = pC_Points[j].pd_ValuePixel[i];
		}
	ofstream fout;
	fout.open(w_fenlei + ".dat", ios::out | ios::binary);
	for (int i = 0;i < i_NumBands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			fout.write((char*)(&p2us_tmp2[i][j]), sizeof(Type));
		};
	fout.close();
	//构造hdr头文件
	fout.open(w_fenlei + ".hdr", ios::out);
	fout << "ENVI" << endl;
	fout << "description = " << s_Description << endl;
	fout << "samples = " << i_Samples << endl;
	fout << "lines   = " << i_Lines << endl;
	fout << "bands   = " << i_NumBands << endl;
	fout << "header offset = " << d_HeaderOffset << endl;
	fout << "file type = " << s_FileType << endl;
	fout << "data type = " << i_DataType << endl;
	fout << "interleave = " << s_InterLeave << endl;
	fout << "sensor type = " << s_SensorType << endl;
	fout << "byte order = " << i_ByteOrder << endl;


};

ConductMMD::~ConductMMD()
{
	delete[]ClassPixelCode;
}

void ConductMMD::fenlei(n_ROI objRoi)
{
	int NumROI = objRoi.NumROI;
	NumClassification = NumROI + 1;//多一类unclassification
	int NumImgBand = objRoi.MutilRoi[0].NumImgBand;
	double ** distance;
	distance = new double * [NumROI];
	for (int i = 0;i < NumROI;i++)
	{
		distance[i] = new double[i_Lines * i_Samples];
	};
	for (int i = 0;i < NumROI;i++)
		for(int j=0;j< i_Lines * i_Samples;j++)
		{
			distance[i][j] = 0;;//距离赋初值
		};
	ClassPixelCode = new int[i_Lines * i_Samples];
	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		ClassPixelCode[i] = 1000;//ClassPixelCode赋初值
	};
	for (int j = 0;j < i_Lines * i_Samples;j++)
	{ 
		for (int i = 0;i < NumROI;i++)
		{
			for (int k = 0;k < NumImgBand;k++)
			{
				//distance[i][j] += pow((pC_Points[j].pd_ValuePixel[k] - objRoi.ClusterCenter[i][k]),2);
				distance[i][j] += fabs(pC_Points[j].pd_ValuePixel[k] - objRoi.ClusterCenter[i][k]);//计算每个像素点到每个ROI聚类中心的距离
			}
			//distance[i][j] = sqrt(distance[i][j]);

		}
	};
	double tmp_distance;
	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		tmp_distance = distance[0][i];//初始距离赋值为距离第一类的距离
		//tmp_distance = 99999999999999;
		ClassPixelCode[i] = 1;//暂时分为第一类
		for (int j = 0;j < NumROI;j++)
		{
			if (distance[j][i] <=tmp_distance)
			{
				tmp_distance = distance[j][i];
				//ClassPixelCode[i] = j + 1;//更新类别值
				ClassPixelCode[i] = objRoi.MutilRoi[j].Code;
			}


		};
	};
	/*for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		if (i % 256 == 0)
			cout << endl;
		cout << ClassCode[i] << " ";
	}*/
	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		if (ClassPixelCode[i] == 0)
			cout << i << endl;
	}
	for (int i = 0;i < NumROI;i++)//释放distance所占的内存
		delete[]distance[i];
	delete[]distance;

}

void ConductMMD::WriteImg(const string& Filename)
{
	ofstream fout;
	fout.open(Filename + ".dat", ios::out | ios::binary);
		for (int i= 0;i < i_Lines * i_Samples;i++)
		{
			fout.write((char*)(&ClassPixelCode[i]), sizeof(char));
		};
	fout.close();
}

void ConductMMD::WriteHDR(const string& Filename, n_ROI objRoi)
{
	//获取系统时间  
	time_t now_time = time(NULL);
	//获取本地时间  
	tm* t_tm = localtime(&now_time);
	//转换为年月日星期时分秒结果，如图：  
	//printf("local time is    : %s\n", asctime(t_tm));
	ofstream fout;
	fout.open(Filename + ".hdr", ios::out);
	fout << "ENVI" << endl;
	fout << "description = " << "{" << endl;
	fout<<"MMD Classfication Result["<< asctime(t_tm)<<"]}"<< endl;
	fout << "samples = " << i_Samples << endl;
	fout << "lines   = " << i_Lines << endl;
	fout << "bands   = " << 1 << endl;
	fout << "header offset = " << d_HeaderOffset << endl;
	fout << "file type = " << "ENVI Classification" << endl;
	fout << "data type = " << 1 << endl;
	fout << "interleave = " << "bsq" << endl;
	fout << "sensor type = " << s_SensorType << endl;
	fout << "classes = " << NumClassification << endl;
	fout << "class lookup = {" << endl;
	fout << "   0,   0,   0";
	for (int i = 0;i < objRoi.MutilRoi.size();i++)//对每个ROI循环
		fout << ","<<objRoi.MutilRoi[i].RoiRgbValue;
	fout << "}" << endl;
	fout << "class names = {" << endl;
	fout << "Unclassified";
	for (int i = 0;i < objRoi.MutilRoi.size();i++)//对每个ROI循环
		fout << ", " << objRoi.MutilRoi[i].RoiName << " #" << objRoi.MutilRoi[i].Code;
	fout << "}" << endl;
	fout << "byte order = " << i_ByteOrder << endl;
	fout << "wavelength units = " << "Unknown" << endl;
	fout << "band names = {" << endl;
	fout << "MMD(" << s_Description <<"原始影像"<< ")}";
	fout.close();

}

void ConductMMD::PresionEvaluation(Cfenlei SVMclassfication)
{
	double NumTotal;//样本总数
	double NumCorClassfication;//正确分类数
	NumCorClassfication = 0;
	double Pe;
	NumTotal = i_Samples * i_Lines;
	int* NumMMDEveryClass;//MMD分类中各类别数目
	int* NumCorEveryClass;//参考影像中各个类别数目
	NumMMDEveryClass = new int[NumClassification];
	NumCorEveryClass = new int[SVMclassfication.i_Classes];
	for (int i = 0;i < NumClassification;i++)
		NumMMDEveryClass[i] = 0;//初始化为0
	for (int i = 0;i < SVMclassfication.i_Classes;i++)
		NumCorEveryClass[i] = 0;//初始化为0
	int* ReferenceImg;//存放参考分类影像的值
	ReferenceImg = new int[NumTotal];
	for (int i = 0;i < SVMclassfication.i_Bands;i++)
		for (int j = 0;j < SVMclassfication.i_Lines * SVMclassfication.i_Samples;j++)
		{
			ReferenceImg[j] = int(SVMclassfication.ClassPixelValue[i][j]);//分类影像的波段数只能为1，故i=1，外层循环只有一次。
		}
	for (int j = 0;j < i_Lines * i_Samples;j++)
	{
		if (ReferenceImg[j] == ClassPixelCode[j])
			NumCorClassfication+=1;
	}
	cout << NumTotal << endl;
	cout << NumCorClassfication << endl;
	P0 = NumCorClassfication / NumTotal;
	cout << NumCorClassfication / NumTotal << endl;
	for (int i = 0;i < i_Lines * i_Samples;i++)//计算MMD分类法中各个类别的总数
		for (int j = 0;j < NumClassification;j++)
		{
			if (ClassPixelCode[i] == j)
			{
				NumMMDEveryClass[j]++;
			}
		};
	for (int i = 0;i < i_Lines * i_Samples;i++)//计算参考分类影像中各个类别的总数
		for (int j = 0;j < SVMclassfication.i_Classes;j++)
		{
			if (ReferenceImg[i] == j)
			{
				NumCorEveryClass[j]++;
			}
		};
	double tmp=0;
	for (int j = 0;j < NumClassification;j++)
		tmp += NumCorEveryClass[j] * NumMMDEveryClass[j];
	Pe = tmp / (NumTotal * NumTotal);//计算Pe
	cout << Pe;
	Kappa = (P0 - Pe) / (1 - Pe);//Kappa=(P0-Pe)/(1-Pe)
	cout << "计算得总体分类精度为：" << P0 << endl;
	cout << "计算得Kappa系数为：" << Kappa << endl;


}

/*ROI::~ROI()
{
	delete[] Position_x;
	delete[] Position_y;
	delete[] ValuePixel;
}*/
