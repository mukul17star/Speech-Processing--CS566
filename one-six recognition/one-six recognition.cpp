// one-six recognition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	//ifstream fp;
	////string file_name;
	//double rec_time;
	//cout << "Enter the duration of recording path(recommended 2): ";
	//cin >> rec_time;

	//string cmnd = "Recording_Module.exe " + to_string(rec_time) + " \"recording.wav\" " + "\"recording.txt\" ";
	//
	//const char *command = cmnd.c_str();
	//system(command);
	//fp.open("recording.txt");

	//double x, i = 0;
	//vector <double> data;
	//string s;
	////Discarding the strating 5 lines
	//while (i++<5){
	//	getline(fp, s);
	//}

	ifstream fp;
	string file_name;
	//You should eneter the path of file(eg D:\recordings\150101038\150101038_6_5) which is in COOLEDIT format
	/*cout << "Enter the path of file: ";
	cin >> file_name;
*/
	fp.open("D:\\recordings\\vowels\\150101038_vowel_data\\150101038_i_1.txt");

	double x, i = 0;
	vector <double> data;
	string s;
	//Discarding the strating 5 lines
	/*while (i++<5){
		getline(fp, s);
	}*/

	double MAX_ABS_VAL=0, NORM_VAL = 5000, DC_SHIFT = 0; //setting value of DC Shift and Normalization_Value
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

	// Extracting voice from signal
	double ENRGY_THRSHLD = 0;
	int wind_s = 500;
	for (int i = 0; i < wind_s; i++)
		ENRGY_THRSHLD += data[i] * data[i];
	

	int start_d = -1, end_d = data_size-1;
	double sumsq = 0;
	for (int i = 0; i < data_size; i+=100){
		if (i + wind_s < data_size){
			for (int j = i; j < i + wind_s; j++)
				sumsq += data[i] * data[i];
			if (sumsq >= 40*ENRGY_THRSHLD){
				start_d = i;
				sumsq = 0;
				break;
			}
			sumsq = 0;
		}
	}

	/*cout << start_d<<endl;
	cout << "Energy Threshold" << ENRGY_THRSHLD << endl;*/

	if (start_d == -1){
		cout << "No voice found\n";
		return 0;
	}
	else{
		for (int i = start_d; i < data_size; i += 100){
			sumsq = 0;
			if (i + wind_s < data_size){
				for (int j = i; j < i + wind_s; j++)
					sumsq += data[i] * data[i];
				if (sumsq < ENRGY_THRSHLD/100){
					end_d = i + wind_s;
					break;
				}
				sumsq = 0;
			}
		}
	}

	vector <double> voice;
	ofstream myfile;
	myfile.open("saved_recording.txt");
	for (int i = start_d; i < end_d; i++){
			voice.push_back(data[i]);
			myfile << data[i] << endl;
	}
	myfile.close();

	//Resizing Window to starting part of one/six
	int window = voice.size();

	//Calculate ZCR
	int ZCR = 0,prev_sign = 1;
	for (int i = 0; i < window; i++){
		if (voice[i] < 0 && prev_sign == 1){
			ZCR++;
			prev_sign = -1;
		}
		else if (voice[i] > 0 && prev_sign == -1){
			ZCR++;
			prev_sign = 1;
		}
	}

	cout << "ZCR: " << ZCR << endl;
	// Comparing ZCR value with 500
	if (ZCR < 150){
		cout << "Speak Loudly/Clearly\n";
	}
	else if (ZCR < 1000){
		cout << "The number is ONE\n";
	}
	else{
		cout << "The number is SIX\n";
	}

	return 0;
}