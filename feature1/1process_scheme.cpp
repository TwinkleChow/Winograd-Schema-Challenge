#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

bool search_line(string keyword, string input, string output) {
	ifstream ins;
	ofstream outs;

	ins.open(input.c_str());
	outs.open(output.c_str());


	string line;
	bool found = false;
	while (getline(ins, line)) {
		if (line.find(keyword) != string::npos) {
			outs << line;
			outs << endl;
			found = true;
		}
	}

	ins.close();
	outs.close();

	return found;

}


int main() {
	ifstream ins;
	ifstream schemas;
	ofstream outs;

	ofstream out;

	ins.open("input.txt");
	schemas.open("schemas_out.txt");
	outs.open("sentence_processed.txt");
	out.open("out.txt");

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

	bool read_sentence = false;
	bool print_each = false;

	//binary feature
	int i1;
	int i2;
	vector<int>answers1;
	int sentence_number=0;

	int tokens;
	// loop for each line

	while ((getline(ins, line))&&(line.find("Coreference")==string::npos))
			{ // loop to read each sentence

		while (getline(ins, line)) {


			if (line.find("Sentence") != string::npos) {
				string str;
				start_position = line.find("(");
				end_position = line.find("tokens");
				if(end_position-start_position>0){
				str = line.substr(start_position+1,end_position-start_position);
				;}
				tokens = atoi( str.c_str() );
				if(tokens<8){
					break;
				}
				sentence_number++;
				cout<<"sentence number: "<<sentence_number<<endl;

				cout<<answers1.size()<<endl;


				if((count<4)&&(count)){
					cout<<"invalid type!"<<endl; //remember to mark the end of the file with "Sentence" in order to detect whether the last one is invalid
				i1=i2=0;
				outs<<"i1: "<<i1;
				outs<<"i2: "<<i2;
				outs<<endl;
				answers1.push_back(0);

				cout<<answers1.size()<<endl;


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
				mymap.clear();

				if (read_sentence == false) {
					read_sentence = true;
				} else {
					read_sentence = false;
					break;
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

				outs << line.substr(line.find("("));


				person = line.substr(line.find(",") + 2,
						line.find(")") - line.find(",") - 2);
				action = line.substr(line.find("(") + 1,
						line.find(",") - line.find("(") - 1);
				mysentence[action] = person;

				if (count == 1) {
					candidate1 = person;
					process_action = action.substr(0, action.length() - 2);
					candidate_event.push_back(process_action);
				} else if (count == 3) {
					target = person;
					target_event.push_back(action);
				} else if (person == target) {
					target_event.push_back(action);
				}

				outs << endl;

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

			outs << line.substr(line.find("("));
			outs << endl;

			person = line.substr(line.find(",") + 2,
					line.find(")") - line.find(",") - 2);
			action = line.substr(line.find("(") + 1,
					line.find(",") - line.find("(") - 1);
			mysentence[action] = person;
			if (count == 2) {
				candidate2 = person;
				process_action = action.substr(0, action.length() - 2);
			}
		}



		//resolve the target
		if ((count == 4) && (print_each == false)) {

			print_each = true;


			//pair the candidate event and target event
			search_result.clear();
			string search_candidate;






			for (std::vector<string>::iterator target = target_event.begin();
					target != target_event.end(); ++target) {
				if (search_line(*target, "schemas_out.txt",
						"search_result.txt")) {
					ifstream result;
					result.open("search_result.txt");

					while (getline(result, line)) {
						for (std::vector<string>::iterator candidate =
								candidate_event.begin();
								candidate != candidate_event.end();
								++candidate) {
							if (line.find(*candidate) != string::npos) {

								search_candidate = line.substr(
										line.find(*candidate),
										(*candidate).length()+2);
								search_result.push_back(search_candidate);
							}
						}

					}

				}

			}
			if(search_result.size()){

			string resolve = mysentence[search_result.front()];
			if(resolve == candidate1){
				i1=1;
				i2=0;
				outs<<"i1: "<<i1;
				outs<<"i2: "<<i2;
				answers1.push_back(1);


			}
			else{
				i1=0;
				i2=1;
				outs<<"i1: "<<i1;
				outs<<"i2: "<<i2;
				outs<<endl;
				answers1.push_back(2);
			}

		}
			else{
				cout<<"Not found!";
				i1=i2=0;
				outs<<"i1: "<<i1;
				outs<<"i2: "<<i2;
				outs<<endl;
				answers1.push_back(0);
			}
		}

	}

}

	for(std::vector<int>::iterator it = answers1.begin(); it != answers1.end(); ++it) {
	    out<<*it<<endl;
	}

ins.close();
outs.close();
out.close();

return 0;
}
