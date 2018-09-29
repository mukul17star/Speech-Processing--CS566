// LBG.cpp : Defines the entry point for the console application.
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
#include <ctime>
#include <cstdlib>

#define DEBUG(x) cout << '>' << #x << ':' << x << endl;
#define DEBUGV(x) cout << '>' << #x << ':' ;for (auto y:x) cout<< y <<' '; cout<< endl;

using namespace std;

vector<double> parse_line(string line, char delimit){
	int first = 0;
	vector<double> output;
	for (int i = 0; i < line.size(); i++){
		if (line[i] == delimit){
			output.push_back(stod(line.substr(first, i - first)));
			first = i + 1;
		}
		else if (i == line.size() - 1)
			output.push_back(stod(line.substr(first, i - first + 1)));
	}
	return output;
}


double euclidian_dist(vector<double> a, vector<double> b){
	if (a.size() != b.size()){
		cout << "Size of vetors for computing euclidian distance is note same A:";
		DEBUGV(a); DEBUGV(b);
		return -1;
	}

	double ans = 0;
	for (int i = 0; i<a.size(); i++)
		ans += (a[i] - b[i])*(a[i] - b[i]);

	return ans;
}

void classify(vector<double> x, vector<int> &labels, int index, vector<vector<double> > y){

	//Check which euclidian distance is minimum
	int allot_class = 0;
	double min_dist = euclidian_dist(x, y[0]);

	for (int i = 1; i < y.size(); i++){
		double ed = euclidian_dist(x, y[i]);
		if (ed < min_dist){
			min_dist = ed;
			allot_class = i;
		}
	}

	labels[index] = allot_class;
	// size_class[allot_class]++;

}

void update_y(vector<vector<double> >&y, vector<int> labels, vector<vector<double> >univ){
	int n_features = y[0].size();
	int n_classes = y.size();
	vector<vector<double> >sum(n_classes, vector<double>(n_features, 0));
	vector<double> size(n_classes, 0);
	
	for (int j = 0; j<n_features; j++){//for all features
		for (int i = 0; i<univ.size(); i++){//for all points
			sum[labels[i]][j] += univ[i][j]; //add feature in corresponding sum class
			if(j==0) size[labels[i]]++;
		}
		for (int i = 0; i < y.size(); i++)
			if(size[i]!=0)y[i][j] = sum[i][j] / size[i];
	}

	//for (int i = 0; i<y.size(); i++){// divide by size of elements in that class
	//	if (size[i] == 0)
	//		continue;
	//	for (int j = 0; j<n_features; j++)
	//		y[i][j] = sum[i][j] / size[i];
	//}
}

double distortion(vector<vector<double> >a, vector<vector<double> > b){
	_ASSERT_EXPR(a.size() == b.size(), "Size of vectors to calculate distortion is not same");
	double ans = 0;
	for (int i = 0; i < a.size(); i++){
		for (int j = 0; j < a[0].size(); j++)
			ans += pow((a[i][j] - b[i][j]), 2);
	}
	return ans;
}

void kMeans(vector<vector<double> > univ, vector<vector<double> >&y, vector<int>&labels){
	int d_size = univ.size();
	int n_classes = y.size(), n_features = 12;

	vector<vector<double> > y_prev;	
	double distor;
	for (int m = 0; m<30; m++){// run 30 iterations
		for (int i = 0; i<d_size; i++)// for all, classify class
			classify(univ[i], labels, i, y);

		y_prev = y;
		update_y(y, labels, univ);
		distor = distortion(y_prev, y);
		if ( distor< 1e-20){ cout << "Distortion less than 10^-20: " << distor << endl; break; }
		//cout << "Distortion " << distortion(y_prev, y) << endl;
	}

}

vector<double> stdev(vector<vector<double> >a){
	vector<double> ans(a[0].size(),0);
	vector<double> ans2(a[0].size(),0);
	
	for (int j = 0; j < a[0].size(); j++){
		for (int i = 0; i < a.size(); i++)
			ans[j] += a[i][j];
		
		ans[j] /= a.size();
	}
	
	for (int j = 0; j < a[0].size(); j++){
		for (int i = 0; i < a.size(); i++)
			ans2[j] += ( (a[i][j] - ans[j])*(a[i][j] - ans[j]) ) / (a.size() - 1) ;
		ans2[j] = sqrt(ans2[j]);
	}


	return ans2;

}

void split_y(vector<vector<double> >univ, vector<vector<double> >&y,vector<int> labels){
	int n = y.size();
	vector<vector<double> >y_prime;
	
	for (int i = 0; i < n; i++){
		vector<vector<double> > temp; // all points of a given class
		for (int j = 0; j < univ.size(); j++)
			if (labels[j] == i) temp.push_back(univ[j]);
		
		vector<double>	epsilon = stdev(temp); // stdev of a class points
		vector<double> y_eps1,y_eps2;
		for (int j = 0; j < y[0].size(); j++){
			y_eps1.push_back( y[i][j] - epsilon[j] / 1000 );
			y_eps2.push_back( y[i][j] + epsilon[j] / 1000 );
		}
		y_prime.push_back(y_eps1);
		y_prime.push_back(y_eps2);
	}

	y = y_prime;
}

void display(vector<vector<double> >a){
	for (int i = 0; i < a.size(); i++){
		cout << "Y" << i << " : ";
		for (int j = 0; j < a[0].size(); j++)
			cout << a[i][j] << ", ";
		cout << endl;
	}
}

void lbg(vector<vector<double> > univ){
	int n_features = 12;
	vector<vector<double> >y(1,vector<double>(12,0));
	for (int i = 0; i<univ.size(); i++){
		for (int j = 0; j<n_features; j++)
			y[0][j] += univ[i][j]; 
	}

	for (int j = 0; j<n_features; j++)
		y[0][j] = y[0][j] / univ.size();

	vector<int>labels(univ.size(), 0);
	
	display(y);
	int m = y.size();

	while (m < 8){
		cout << "Codebook size: " << m << endl;
		cout << "Splitting y...\n";
		split_y(univ, y, labels);
		display(y);
		kMeans(univ, y, labels);
		m = y.size();
	}

	string vowels = "aeiou";
	for (int j = 0; j < y.size(); j++){
		cout << "Letters alloted to Class" << j << ":\n";
		for (int i = 0; i < labels.size(); i++){
			if (labels[i] == j) cout << vowels[(i / 5) % 5] << " ";
		}
		cout << endl;
	}
	
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fp("myUniverse.txt");

	vector<vector<double> >univ;

	double x;
	string line;
	vector<double> temp;
	while (fp.good()){//Taking the data in the vector(data)
		getline(fp, line, '\n');
		if (line.size() == 0) break;
		temp = parse_line(line, ',');
		univ.push_back(temp);
	}
	fp.close();

	lbg(univ);
	return 0;
}


