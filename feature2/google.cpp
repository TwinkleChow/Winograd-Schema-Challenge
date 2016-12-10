#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <algorithm>

//this file modifies subjectivity.cpp to construct query

using namespace std;

//this function is modified to search the polarity of the keyword
bool search_line(string keyword, string input, string output) {
	ifstream ins;
	ofstream outs;

	ins.open(input.c_str());
	outs.open(output.c_str());

	string line;
	bool found = false;
	bool found_polarity = false;
	string word;
	string polarity;

	while ((ins >> word) && (found == false)) {

		if (word == "word1=" + keyword) {
			found = true;
			while ((ins >> word) && (found_polarity == false)) {
				if (word.find("polarity") != std::string::npos) {
					polarity = word.substr(word.find("polarity=") + 9,
							word.length() - word.find("polarity="));
					found_polarity = true;
				}
			}

		}
	}

	ins.close();
	outs.close();

	return found;

}

int main() {
	ifstream ins;
	ofstream outs;

	ins.open("input.txt");
	outs.open("queries.txt");

	if (!ins.is_open() || !outs.is_open()) { //always check the following cerr << “file(s) cannot be open\n”;
		exit(-1);
	}

	string line;
	string word;

	int start_position;
	int end_position;
	int position;
	int count = 0;

	std::map<std::string, std::string> mymap; //a map to store the present form of verb

	std::map<std::string, std::string> mysentence; //a map to find the actor

	std::vector<std::string> candidate_event;
	std::vector<std::string> target_event;

	std::vector<std::string> search_result;

	string key;
	string value;

	string candidate1;
	string candidate2;
	string target;

	string person;
	string action;
	string process_action;

	string target_polarity;
	string candidate_polarity;

	//variables to process cop
	string be;
	string verb;

	//variables to process comparative adv
	string adv_modifier;
	string verb_modified;

	//variables to process negation
	bool negate = false;
	string verb_negated = "";

	//variables to process discourse connective
	bool discourse_exist = false;
	vector<string>connective;
	//initialize it with negative connectives
	connective.push_back("although");
	connective.push_back("but");
	string entity;




	bool read_sentence = false;
	bool print_each = false;



	// loop for each line

	while (getline(ins, line)) { // loop to read each sentence

		while (getline(ins, line)) {

			if (line.find("Sentence") != string::npos) {

				if ((count < 4) && (count)) {
					cout << "invalid type!"; //remember to mark the end of the file with "Sentence" in order to detect whether the last one is invalid

				}

				//initialize the count at each sentence analysis
				count = 0;
				mysentence.clear();
				candidate1 = "";
				candidate2 = "";
				target = "";
				print_each = false;
				candidate_event.clear();
				target_event.clear();
				target_polarity = "";
				candidate_polarity = "";

				be="";
				verb="";

				adv_modifier="";
				verb_modified="";

				negate = false;
				verb_negated = "";

				discourse_exist = false;

				if (read_sentence == false) {
					read_sentence = true;
				} else {
					read_sentence = false;
					break;
				}

			}
			//search for negative connectives
			if ((line.find("PartOfSpeech=IN") != string::npos) or(line.find("PartOfSpeech=CC") != string::npos)){
				start_position = line.find("Text");
				end_position = line.find("CharacterOffsetBegin");
				entity = line.substr(start_position + 5,
						end_position - start_position - 6);

				if(find(connective.begin(), connective.end(), entity) != connective.end()){
					discourse_exist = true;
				}

			}



			if (line.find("PartOfSpeech=VB") != string::npos) {
				start_position = line.find("Text");
				end_position = line.find("CharacterOffsetBegin");
				key = line.substr(start_position + 5,
						end_position - start_position - 6);

				start_position = line.find("Lemma");
				end_position = line.find("NamedEntityTag");

				value = line.substr(start_position + 6,
						end_position - start_position - 7);

				if (key != "." && (key.length() != 0)) {
					mymap[key] = value;
				}

			}

			if ((line.find("nsubj") != string::npos)
					or (line.find("xsubj") != string::npos)) {
				count++;

				line.replace(line.length() - 3, 3, ")");

				//restore the form of verb
				for (std::map<string, string>::iterator it = mymap.begin();
						it != mymap.end(); ++it) {

					// first is the first key
					if (line.find(it->first) != string::npos) {
						position = line.find(it->first);
						line.replace(position, it->first.length() + 2,
								mymap.at(it->first) + "-s");

					}
				}


				person = line.substr(line.find(",") + 2,
						line.find(")") - line.find(",") - 2);
				action = line.substr(line.find("(") + 1,
						line.find(",") - line.find("(") - 1);

				process_action = action.substr(0, action.length() - 2);

				mysentence[action] = person;

				if (count == 1) {
					candidate1 = person;
					candidate_event.push_back(process_action);
				} else if (count == 2) {
					if (person == candidate1) {
						count--;
					}
				} else if (count == 3) {
					target = person;
					target_event.push_back(
							process_action.substr(0,
									process_action.length() - 1));
				} else if (person == target) {
					target_event.push_back(
							process_action.substr(0,
									process_action.length() - 1));
				}


			}

			if ((line.find("dobj") != string::npos)
					or (line.find("nmod") != string::npos)) {
				count++;
				line.replace(line.length() - 3, 3, ")");

				//restore form of verb
				for (std::map<string, string>::iterator it = mymap.begin();
						it != mymap.end(); ++it) {
					// ent1.first is the first key
					if (line.find(it->first) != string::npos) {
						position = line.find(it->first);
						line.replace(position, it->first.length() + 2,
								mymap.at(it->first) + "-o");

					}
				}


				person = line.substr(line.find(",") + 2,
						line.find(")") - line.find(",") - 2);
				action = line.substr(line.find("(") + 1,
						line.find(",") - line.find("(") - 1);
				mysentence[action] = person;
				if (count == 2) {
					candidate2 = person;
					process_action = action.substr(0, action.length() - 2);

				}
				else if((count==3)&&(person!=candidate2)){
					count--;
				}

			}
			if(line.find("cop") != string::npos){
				count++;
				line.replace(line.length() - 3, 3, ")");

				//restore form of verb
				for (std::map<string, string>::iterator it = mymap.begin();
						it != mymap.end(); ++it) {
					// ent1.first is the first key
					if (line.find(it->first) != string::npos) {
						position = line.find(it->first);
						line.replace(position, it->first.length() + 2,
								mymap.at(it->first) + "-o");

					}
				}



				be = line.substr(line.find(",") + 2,
						line.find(")") - line.find(",") - 2);
				verb = line.substr(line.find("(") + 1,
						line.find("-") - line.find("(")-1);

				//read another line to find the adv modifier
				getline(ins,line);
				if(line.find("advmod") != string::npos){
					line.replace(line.length() - 3, 3, ")");

					//restore form of verb
					for (std::map<string, string>::iterator it = mymap.begin();
							it != mymap.end(); ++it) {
						// ent1.first is the first key
						if (line.find(it->first) != string::npos) {
							position = line.find(it->first);
							line.replace(position, it->first.length() + 2,
									mymap.at(it->first) + "-o");

						}
					}



					adv_modifier = line.substr(line.find(",") + 2,
							line.find(")") - line.find(",") - 2);
					verb_modified = line.substr(line.find("(") + 1,
							line.find("-") - line.find("(") - 1);

					cout<<"adv: "<<adv_modifier<<endl;
					cout<<"verb: "<<verb_modified<<endl;


				}



			}

			if(line.find("neg")!=string::npos){

				//change verb form
				for (std::map<string, string>::iterator it = mymap.begin();
						it != mymap.end(); ++it) {
					// ent1.first is the first key
					if (line.find(it->first) != string::npos) {
						position = line.find(it->first);
						line.replace(position, it->first.length() + 2,
								mymap.at(it->first) + "-o");

					}
				}


				verb_negated = line.substr(line.find("(") + 1,
						line.find("-") - line.find("(")-1);

				cout<<"verb_negated: "<<verb_negated<<endl;
				cout<<"taget.front: "<<target_event.front()<<endl;

				if(verb_negated==target_event.front()){
					negate = true;
					if(be.length()==0){
					count++;
					cout<<"count after negation: "<<count<<endl;}
				}
			}




			//resolve the target
			if ((count == 4) && (print_each == false)) {

				print_each = true;

				//pair the candidate event and target event
				search_result.clear();
				string search_candidate;

				cout << "target";
				cout << endl;
				for (std::vector<string>::iterator target =
						target_event.begin(); target != target_event.end();
						++target) {
					cout << *target;
				}

				cout << "candidate";
				cout<<endl;
				for (std::vector<string>::iterator target =
						candidate_event.begin();
						target != candidate_event.end(); ++target) {
					cout << *target;
				}
				cout << endl;

				//process queries

				string query = "";
                                        if(target_event.size()){
						query = candidate1+" "+target_event.front();
						outs<<query;
						outs<<endl;



						query = candidate2+" "+target_event.front();
						outs<<query;
						outs<<endl;}





				//find the polarity of target event
				/*for (std::vector<string>::iterator target =
						target_event.begin(); target != target_event.end();
						++target) {
					if (search_line(*target, "subjectivity.txt",
							"search_result.txt")) {
						ifstream result;
						result.open("search_result.txt");
						result >> target_polarity;

					}
				}

				//find the polarity of candidate event
				for (std::vector<string>::iterator target =
						candidate_event.begin();
						target != candidate_event.end(); ++target) {
					if (search_line(*target, "subjectivity.txt",
							"search_result.txt")) {
						ifstream result;
						result.open("search_result.txt");
						result >> candidate_polarity;

					}
				}

				cout << "target :" << target_polarity << endl;
				cout << "candidate :" << candidate_polarity<<endl;

				//reverse the polarity of target if case 2: comparative adv is presented


				if((adv_modifier=="less")&&(verb_modified==verb)){
					if(target_polarity=="positive"){
						target_polarity="negative";
					}
					else if(target_polarity=="negative"){
						target_polarity="positive";
					}

				}
				//case 1: nagation
				cout<<"negation: "<<negate<<endl;
				if(negate){
					if(target_polarity=="positive"){
						target_polarity="negative";
					}
					else if(target_polarity=="negative"){
						target_polarity="positive";
					}

				}

				//case 3: discourse connective
				cout<<"discourse: "<<discourse_exist<<endl;
				if(discourse_exist){
					if(target_polarity=="positive"){
						target_polarity="negative";
					}
					else if(target_polarity=="negative"){
						target_polarity="positive";
					}

				}

				cout << "target :" << target_polarity << endl;
				cout << "candidate :" << candidate_polarity<<endl;

				//resolve according to the polarity
				if ((target_polarity.length() != 0)
						&& (candidate_polarity.length() != 0)) {
					if (target_polarity == candidate_polarity) {
						i1=1;
						i2=0;
						cout<<i1<<endl<<i2;

					} else {
						i1=0;
						i2=1;
						cout<<i1<<endl<<i2;
					}

				} else {
					i1=i2=0;
					cout<<i1<<endl<<i2;
				}*/
			}

		}

	}

	ins.close();
	outs.close();

	return 0;
}

