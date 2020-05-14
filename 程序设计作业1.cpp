// 程序设计作业1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include"ROI.h"
using namespace std;

int main()
{
    string FilePath = "D:\\课程资料\\C++程序设计\\roi.txt";
    string ResultPath = "D:\\课程资料\\C++程序设计\\result.txt";
    string HDRPath = "C:\\Users\\sun-PC\\Desktop\\汉川HJ1A_原始影像.HDR";
    string ImgPath = "C:\\Users\\sun-PC\\Desktop\\汉川HJ1A_原始影像";
    string HDRPath_fl = "C:\\Users\\sun-PC\\Desktop\\fenlei.hdr";
    string ImgPath_fl = "C:\\Users\\sun-PC\\Desktop\\fenlei";
    string w_fenlei = "C:\\Users\\sun-PC\\Desktop\\w_fenlei_4";
    string MMDfenlei = "C:\\Users\\sun-PC\\Desktop\\MMDfenlei";
    string MMDfenleiHDR = "C:\\Users\\sun-PC\\Desktop\\MMDfenlei";
    CImg ObjImg;
    ObjImg.ReadFileHDR(HDRPath);
    ObjImg.ReadFileImg(ImgPath);
    ObjImg.ShowImg();
   /* n_ROI ObjRoi;
    Cfenlei ObjFL;
    ConductMMD ObjCM;
    ObjFL.ReadFileHDR(HDRPath);
    ObjFL.ReadFileImg(ImgPath);
    ObjFL.ReadFileHDR_fl(HDRPath_fl);
    ObjFL.ReadSVMImg(ImgPath_fl);
    //ObjFL.WriteFileImg(ImgPath_fl,w_fenlei);
    ObjRoi.ReadFile(FilePath);
    ObjRoi.GetClusterCenter();
    ObjCM.ReadFileHDR(HDRPath);
    ObjCM.ReadFileImg(ImgPath);
    ObjCM.fenlei(ObjRoi);
    ObjCM.WriteImg(MMDfenlei);
    ObjCM.WriteHDR(MMDfenleiHDR, ObjRoi);
    ObjCM.PresionEvaluation(ObjFL);
    //ObjRoi.GetRoiMean();
   // ObjRoi.PrintResult(ResultPath);*/

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
