/*
 * integration.cpp
 *
 *  Created on: 21 Nov 2016
 *      Author: twinkle
 */
//integrate the results of all the three features and make decisions

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;



int main(){
	ifstream ins;
	ofstream outs;
	ifstream ins1;
	ifstream ins2;
	ifstream ins3;

	string line;
	ins.open("challenge.txt");
	outs.open("challenge_processed.txt");
	ins1.open("input1.txt");
	ins2.open("input2.txt");
	ins3.open("input3.txt");

	vector<int>answers;
	while(getline(ins,line)){
		if(line.length()==0){
			getline(ins,line);
			outs<<line;
			outs<<endl;
		}
		if(line.find("Correct Answer: A ")!=string::npos){
			answers.push_back(1);
		}
		else if(line.find("Correct Answer: B ")!=string::npos)
			answers.push_back(2);
	}


	 int total =0;
	 int unresolved = 0;
	 int correct =0;
	 string line1;
	 string line2;
	 string line3;
	 bool line3_end = false;
	 while(getline(ins1,line1)&&getline(ins2,line2)){
		 line3_end = !getline(ins3,line3);
		 if((line1=="0")&&((line2)=="0")){
			if((!line3_end)&&(line3!="0")){
				if(line3=="1"){
					if(answers[total]==1){
						correct++;
					}
				}
				else if(line3=="2"){
					if(answers[total]==2){
						correct++;
					}
				}

			}
			else{
			 unresolved++;
			}
		 }
		 else if((line1=="1") or (line2=="1")){
			 if(answers[total]==1){
				 correct++;
			 }
		 }
		 else if((line1=="2") or (line2=="2")){
					 if(answers[total]==2){
						 correct++;
					 }
				 }
		 total++;
	 }
	 cout<<"total: "<<total<<endl;
	 cout<<"correct: "<<correct<<endl;
	 cout<<"unresolved: "<<unresolved<<endl;

	return 0;
}



