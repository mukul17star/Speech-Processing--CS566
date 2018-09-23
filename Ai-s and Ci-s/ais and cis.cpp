// ais and cis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

vector<double> calc_Ri(vector<double> &data ,int p=12){
	int n = data.size();
	vector<double> R;

	for(int i=0;i<=p;i++){//iterating on the interval p
		double ri=0;
		for(int j=0;j+i<n;j++)
			ri+=data[j]*data[j+i];
		R.push_back(ri);
	}
	return R;
}

vector<double> calc_Ai(vector<double> &R){
	// Implementation of Durbin's Algorithm
	int p = R.size()-1;
	vector<double> E;
	E.push_back(R[0]);

	vector<vector<double> > alpha(p + 1, vector<double>(p+1));
	for(int i=1;i<=p;i++){
		double k=R[i];
		for(int j=1;j<i;j++)
			k-=alpha[i-1][j]*R[i-j];
		k/=E[i-1];

		alpha[i][i] = k;
		for(int j=1;j<=i-1;j++)
			alpha[i][j] = alpha[i-1][j] - k*alpha[i-1][i-j];
		E.push_back( (1-k*k)*E[i-1]);
	}
	vector<double> A;
	for(int i=1;i<=p;i++)
		A.push_back(alpha[p][i]);
	return A;
}

vector <double> calc_Ci(vector<double> &A, double R_0){
	int p = A.size();
	vector<double> C;
	 C.push_back(log2(R_0));

	for(int i=1;i<=p;i++){
		double x = A[i-1];
		for(int k=1;k<=i-1;k++)
			x+=((double)k/i)*C[k]*A[i-k-1];
		C.push_back(x);
	}

	return C;
}

void hamming_window(vector<double> &A){
	int n = A.size();
	for (int i = 0; i < n; i++){
		A[i] *= (0.56 - 0.46*cos(2 * M_PI*i / n));
	}
}

int main(int argc, _TCHAR* argv[])
{
		ifstream fp;
	// //string file_name;
	// double rec_time;
	// cout << "Enter the duration of recording path(recommended 2): ";
	// cin >> rec_time;

	// string cmnd = "Recording_Module.exe " + to_string(rec_time) + " \"recording.wav\" " + "\"recording.txt\" ";

	// const char *command = cmnd.c_str();
	// system(command);
	fp.open("trimmed.txt");
	//C:\\Users\\Mukul\\Documents\\Visual Studio 2013\\Projects\\150101038_program3\\saved_recording.txt
	double x, i = 0;
	vector <double> data;
	string s;

	double MAX_ABS_VAL = 0, NORM_VAL = 5000, DC_SHIFT = 0; //setting value of DC Shift and Normalization_Value
	while (!fp.eof()){//Taking the data in the vector(data)
		fp >> x;
		data.push_back(x);

		MAX_ABS_VAL = max(abs(x), MAX_ABS_VAL);
	}
	fp.close();

	int data_size = data.size();
	//DC Shift
	for (int i = 0; i < data_size; i++)
		data[i] -= DC_SHIFT;

	//Normalization
	 for (int i = 0; i < data_size; i++)
	 	data[i] *= NORM_VAL / MAX_ABS_VAL;

	int stride=320,wind_size=320;
	vector<double> splitx[5];
	for(int i=0;i<5;i++){
		splitx[i]=(vector<double>(data.begin()+i*stride,data.begin()+i*stride+wind_size));
		//hamming_window(splitx[i]);
	}
	


	vector<vector<double> >R,A,C;
	for(int i=0;i<5;i++){
		R.push_back(calc_Ri(splitx[i]));
		A.push_back(calc_Ai(R[i]));
		C.push_back(calc_Ci(A[i],R[i][0]));
	}

	int p = R[0].size()-1;
	// for (int i = 0; i <= p; i++)
	// 	cout << R[i] << " ";
	// cout << endl;
	cout<<"Ai's:\n";
	for(int j=0;j<5;j++){
		for (int i = 0; i < p; i++)
			cout << A[j][i] << " ";
		cout << endl;
	}

	cout<<"\nCi's\n";
	for(int j=0;j<5;j++){
		for (int i = 0; i <= p; i++)
			cout << C[j][i] << " ";
		cout << endl;
	}
	return 0;
}

