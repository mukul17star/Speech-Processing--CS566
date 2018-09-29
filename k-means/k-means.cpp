// k-means.cpp : Defines the entry point for the console application.
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

void generate_initial(vector<vector<double> >univ , vector<vector<double> > &y, vector<int>&labels){
	
	int wind_size = 50;
	vector<vector<double> > splitx[5];
	for (int i = 0; i<5; i++){
		splitx[i] = (vector<vector<double> >(univ.begin() + i*wind_size, univ.begin() + (i+1)*wind_size));
		for (int j = i*wind_size; j < (i + 1)*wind_size; j++)
			labels[j] = i;
	}

	//vector<double> cmaxs(12, INT_MIN), cmins(12, INT_MAX);
	//for (int i = 0; i < univ.size(); i++){
	//	for (int j = 0; j < 12; j++){
	//		cmaxs[j] = max(cmaxs[j], univ[i][j]);
	//		cmins[j] = min(cmins[j], univ[i][j]);
	//	}
	//}
	//for(int i=0;i<5;i++){
	//	for(int j=0;j<12;j++){
	//		double r = ((float)rand()) / (float)RAND_MAX;
	//		//DEBUG(r);
	//		y[i][j] = cmins[j]+ r * (cmaxs[j]-cmins[j]);
	//		cout << y[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < splitx[i].size(); j++){
			for (int k = 0; k < 12; k++)
				y[i][k] += splitx[i][j][k];
		}
		cout << "Y" << i<<": ";
		for (int k = 0; k < 12; k++){
			y[i][k] = y[i][k] / splitx[i].size();
			cout << y[i][k] << ", ";
		}
		cout << endl;
	}

}

double euclidian_dist(vector<double> a, vector<double> b){
	if(a.size()!=b.size()){
		cout<<"Size of vetors for computing euclidian distance is note same A:";
		DEBUGV(a);DEBUGV(b);
		return -1;
	}

	double ans=0;
	for(int i=0;i<a.size();i++)
		ans+=(a[i]-b[i])*(a[i]-b[i]);

	return ans;
}

void classify(vector<double> x, vector<int> &labels, int index, vector<vector<double> > y){
	
	//Check which euclidian distance is minimum
	int allot_class = 0;
	double min_dist =  euclidian_dist(x,y[0]);

	for(int i=1;i<=4;i++){
		double ed = euclidian_dist(x,y[i]);
		if(ed < min_dist){
			min_dist = ed;
			allot_class = i;
		}
	}

	labels[index] = allot_class;
	// size_class[allot_class]++;

}

void update_y(vector<vector<double> >&y, vector<int> labels, vector<vector<double> >univ, int n_classes, int n_features){
	vector<vector<double> >sum(n_classes,vector<double>(n_features,0));
	vector<double> size(n_classes,0);

	for(int i=0;i<univ.size();i++){//for all points
		for(int j=0;j<n_features;j++){//for all features
			sum[labels[i]][j]+=univ[i][j]; //add feature in corresponding sum class
		}
		size[labels[i]]++;
	}

	for(int i=0;i<y.size();i++){// divide by size of elements in that class
		if(size[i]==0)
			continue;
		for(int j=0;j<n_features;j++)
			y[i][j]=sum[i][j]/size[i];
	}
}

double distortion(vector<vector<double> >a, vector<vector<double> > b){
	_ASSERT_EXPR(a.size() == b.size(),"Size of vectors to calculate distortion is not same");
	double ans = 0;
	for (int i = 0; i < a.size(); i++){
		for (int j = 0; j < a[0].size(); j++)
			ans += pow((a[i][j] - b[i][j]),2);
	}
	return ans;
}

void kMeans(vector<vector<double> > univ){
	int d_size = univ.size();
	int n_classes = 5 ,n_features = 12;
	
	//Initialize original
	vector<vector<double> > y(n_classes, vector<double>(n_features, 0));
	vector<vector<double> > y_prev;
	vector<int> labels(d_size, -1);//labels given to each data point
	generate_initial(univ,y,labels);

	
	for(int m=0;m<30;m++){// run 20 iterations
		for(int i=0;i<d_size;i++)// from start to current m, classify class
			classify(univ[i],labels,i,y);

		y_prev = y;
		update_y(y,labels,univ,n_classes,n_features);
		if (distortion(y_prev, y) < 1e-20){ cout << "Distortion less than 10^-20: " << distortion(y_prev, y)<<endl; break; }
		cout << "Distortion " << distortion(y_prev, y) << endl;
	}

	string vowels = "aeiou";
	for (int j = 0; j < 5; j++){
		cout << "Letters alloted to Class"<<j<<":\n";
		for (int i = 0; i < labels.size(); i++){
			if (labels[i]==j) cout << vowels[(i / 5) % 5] <<" ";
		}
		cout << endl;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fp ("myUniverse.txt");

	vector<vector<double> >univ;

	double x;
	string line;
	vector<double> temp;
	while (fp.good()){//Taking the data in the vector(data)
		getline(fp,line,'\n');
		if (line.size() == 0) break;
		temp = parse_line(line, ',');
		univ.push_back(temp);
	}
	fp.close();

	kMeans(univ);
	return 0;
}

