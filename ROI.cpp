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
	//��description
	i_NumLines = 1;
	ul_Index = vs_tmp[i_NumLines].find("=");
	//s_tmp = vs_tmp[i_NumLines].substr(ul_Index + 1) + vs_tmp[i_NumLines+1] + vs_tmp[i_NumLines+2];
	s_tmp = vs_tmp[i_NumLines + 1];
	s_Description = s_tmp;
	//��samples
	i_NumLines = 4;
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_Samples = stoi(s_tmp);
	//��lines
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_Lines = stoi(s_tmp);
	//��bands
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_NumBands = stoi(s_tmp);
	//��header offset
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	d_HeaderOffset = stoi(s_tmp);
	//��file type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_FileType = s_tmp;
	//��data type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	i_DataType = stoi(s_tmp);
	//��interleave
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_InterLeave = s_tmp;
	//��sensor type
	ul_Index = vs_tmp[i_NumLines].find("=");
	s_tmp = vs_tmp[i_NumLines++].substr(ul_Index + 1);
	s_SensorType = s_tmp;
	//��byte order
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

	//�����ݶ����ά����p2us_tmp;
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
	//��������ӵ����в���ÿ�����ص�Ĳ���ֵ��
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
	/*ofstream fout;//����ȡ������д���ļ�
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
	//����Ҷ�ֵ
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
	//��DNֵת��Ϊ�Ҷ�ֵ
	for (int i = 0;i < i_Lines * i_Samples;i++)
		for (int j = 0;j < i_NumBands;j++)
		{
			pC_Points[i].pd_ValuePixel[j] = int((pC_Points[i].pd_ValuePixel[j] - d_DNmin) / (d_DNmax - d_DNmin) * 255);
		};
	//���Ҷ�ֵд��result2�ļ�
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

	//�ͷſռ�
	for (int i = 0;i < i_NumBands;i++)
		delete[]p2us_tmp[i];
	delete[]p2us_tmp;

};

	
void n_ROI::ReadFile(const string& filepath)
{
	ifstream fin;
	string tmp;
	ROI tmp_ROI;
	int index, index1;
	fin.open(filepath, ios_base::in);//��ȡ�ļ�
	if (!fin)
		cout << "file can't open!" << endl;
	getline(fin, tmp);
	fin >> tmp;//;
	fin >> tmp;//Number
	fin >> tmp;//of
	fin >> tmp;//ROI:
	fin >> NumROI;
	getline(fin, tmp);//������ʣ������
	getline(fin, tmp);//��ͼ���С
	for (int i = 0;i < NumROI;i++)
	{
		getline(fin, tmp);//������
		//getline(fin, tmp_ROI.RoiName);
		getline(fin, tmp);
		index = tmp.find("#");
		index1 = tmp.find(":");
		tmp_ROI.RoiName = tmp.substr(index1+2, index-index1-3);//��ROIname
		tmp_ROI.Code=stoi(tmp.substr(index+1));//����Ӧ��������
		getline(fin, tmp);
		index = tmp.find("{");
		index1 = tmp.find("}");
		tmp_ROI.RoiRgbValue= tmp.substr(index+1, index1 - index-1);//��RGBֵ
		fin >> tmp;//��;
		fin >> tmp;//��ROI
		fin >> tmp;//��npts:
		fin >> tmp_ROI.npts;//��ȡROI����
		getline(fin, tmp);//������ʣ������
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

	/*for (int i = 0;i < MutilRoi.size();i++)//��ÿ��ROIѭ��
	{
		cout << MutilRoi[i].Code << endl;
		cout << MutilRoi[i].RoiName << endl;
		cout << MutilRoi[i].RoiRgbValue << endl;
	}*/

	for (int i = 0;i < MutilRoi.size();i++)//��ÿ��ROIѭ��
	{
		MutilRoi[i].NumImgBand = tmp_NumImgBand - 5;
		MutilRoi[i].Position_x = new double[MutilRoi[i].npts];
		MutilRoi[i].Position_y = new double[MutilRoi[i].npts];
		MutilRoi[i].ValuePixel = new double [MutilRoi[i].NumImgBand * MutilRoi[i].npts];
		for(int j=0;j< MutilRoi[i].npts;j++)//��ÿ��POI�е�ÿһ������ѭ��
		{
			fin>>MutilRoi[i].Position_x[j];
			fin>>MutilRoi[i].Position_y[j];
			for (int k = 0;k < MutilRoi[i].NumImgBand;k++)
			{
				fin >> MutilRoi[i].ValuePixel[j * MutilRoi[i].NumImgBand + k];
			};
			fin >> tmp;//��ȡID��
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
	ofstream fout;//����ȡ������д���ļ�
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

/*Cfenlei::~Cfenlei()
{
	for (int i = 0;i < i_Bands;i++)
		delete [] ClassPixelValue[i];
	delete[] ClassPixelValue;
}*/

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
	int** p2i_tmp;//��Ž�charת����int���͵�����
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
	//ȫ����ʼ��Ϊ0
	for (int i = 0;i < i_NumBands;i++)
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			p2us_tmp2[i][j] = 0;
		}
	int i_type;
	cout << "������Ҫ����ķ�����룺";
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
	//����hdrͷ�ļ�
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
	NumClassification = NumROI + 1;
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
			distance[i][j] = 0;;//���븳��ֵ
		};
	ClassPixelCode = new int[i_Lines * i_Samples];
	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		ClassPixelCode[i] = 1000;//ClassPixelCode����ֵ
	};
	for (int i = 0;i < NumROI;i++)
	{
		for (int j = 0;j < i_Lines * i_Samples;j++)
		{
			for (int k = 0;k < NumImgBand;k++)
			{
				distance[i][j] += fabs(pC_Points[j].pd_ValuePixel[k] - objRoi.ClusterCenter[i][k]);//����ÿ�����ص㵽ÿ��ROI�������ĵľ���
			}

		}
	};
	double tmp_distance;
	for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		tmp_distance = distance[0][i];//��ʼ���븳ֵΪ�����һ��ľ���
		ClassPixelCode[i] = 1;//��ʱ��Ϊ��һ��
		for (int j = 0;j < NumROI;j++)
		{
			if (distance[j][i] < tmp_distance)
			{
				tmp_distance = distance[j][i];
				ClassPixelCode[i] = j + 1;//�������ֵ
			}


		};
	};
	/*for (int i = 0;i < i_Lines * i_Samples;i++)
	{
		if (i % 256 == 0)
			cout << endl;
		cout << ClassCode[i] << " ";
	}*/
	for (int i = 0;i < NumROI;i++)//�ͷ�distance��ռ���ڴ�
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
	//��ȡϵͳʱ��  
	time_t now_time = time(NULL);
	//��ȡ����ʱ��  
	tm* t_tm = localtime(&now_time);
	//ת��Ϊ����������ʱ����������ͼ��  
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
	for (int i = 0;i < objRoi.MutilRoi.size();i++)//��ÿ��ROIѭ��
		fout << ","<<objRoi.MutilRoi[i].RoiRgbValue;
	fout << "}" << endl;
	fout << "class names = {" << endl;
	fout << "Unclassified";
	for (int i = 0;i < objRoi.MutilRoi.size();i++)//��ÿ��ROIѭ��
		fout << ", " << objRoi.MutilRoi[i].RoiName << " #" << objRoi.MutilRoi[i].Code;
	fout << "}" << endl;
	fout << "byte order = " << i_ByteOrder << endl;
	fout << "wavelength units = " << "Unknown" << endl;
	fout << "band names = {" << endl;
	fout << "MMD(" << s_Description <<"ԭʼӰ��"<< ")}";
	fout.close();

}

void ConductMMD::PresionEvaluation(Cfenlei SVMclassfication)
{
	double NumTotal=0;//��������
	double NumCorClassfication=0;//��ȷ������
	double Pe;
	NumTotal = i_Samples * i_Lines;
	int* NumMMDEveryClass;//MMD�����и������Ŀ
	int* NumCorEveryClass;//�ο�Ӱ���и��������Ŀ
	NumMMDEveryClass = new int[NumClassification];
	NumCorEveryClass = new int[SVMclassfication.i_Classes];
	for (int i = 0;i < NumClassification;i++)
		NumMMDEveryClass[i] = 0;//��ʼ��Ϊ0
	for (int i = 0;i < SVMclassfication.i_Classes;i++)
		NumCorEveryClass[i] = 0;//��ʼ��Ϊ0
	int* ReferenceImg;//��Ųο�����Ӱ���ֵ
	ReferenceImg = new int[NumTotal];
	for (int i = 0;i < SVMclassfication.i_Bands;i++)
		for (int j = 0;j < SVMclassfication.i_Lines * SVMclassfication.i_Samples;j++)
		{
			ReferenceImg[j] = int(SVMclassfication.ClassPixelValue[i][j]);//����Ӱ��Ĳ�����ֻ��Ϊ1����i=1�����ѭ��ֻ��һ�Ρ�
		}
	for (int j = 0;j < i_Lines * i_Samples;j++)
	{
		if (ReferenceImg[j] == ClassPixelCode[j])
			NumCorClassfication++;
	}
	P0 = NumCorClassfication / NumTotal;
	for (int i = 0;i < i_Lines * i_Samples;i++)//����MMD���෨�и�����������
		for (int j = 0;j < NumClassification;j++)
		{
			if (ClassPixelCode[i] == j)
			{
				NumMMDEveryClass[j]++;
			}
		};
	for (int i = 0;i < i_Lines * i_Samples;i++)//����ο�����Ӱ���и�����������
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
	Pe = tmp / (NumTotal * NumTotal);//����Pe
	Kappa = (P0 - Pe) / (1 - Pe);//Kappa=(P0-Pe)/(1-Pe)
	cout << "�����������ྫ��Ϊ��" << P0 << endl;
	cout << "�����Kappaϵ��Ϊ��" << Kappa << endl;


}

/*ROI::~ROI()
{
	delete[] Position_x;
	delete[] Position_y;
	delete[] ValuePixel;
}*/
