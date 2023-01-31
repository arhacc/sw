#include <iostream>
#include <fstream>
#include<string>
#include <map>
#include<vector>
#include<sstream>

using namespace std;





string modify_string(string s) 
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '`')
		{
			s.erase(s.begin() + i);
		}
	}

	return s;
}



int get_digit(string s) 
{
	int val = 0;

	for (int i = 0; i < s.length(); i++)
	{
		if (isdigit(s[i]) && s[i-1]=='+')
		{
			val = s[i] - '0';
		}
	}

	return val;
}



string get_string(string s) 
{
	int poz = 0;

	for (int i = 1; i < s.length(); i++)
	{
		if (s[i] == '+')
		{
			poz = i;
		}
	}

	if (poz == 0) 
	{
		s.pop_back();
	}
	else
	{
		s.erase(s.begin() + poz, s.end());
	}

	s.erase(0, 1);
	return s;
}

int main() {
	map<string, int> mymap;
	vector<string>macro_elem;
	ifstream file("../../../hw/sources/design/01_isa.sv");																				// change path here as needed
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) 
		{
			if (line.find("`define") != std::string::npos && line.find('(') != std::string::npos) 
			{
				line.erase(line.begin(), line.begin() + 7);
				line = modify_string(line);
				vector<string> words_from_line;
				string word, elem_from_map;
				stringstream iss(line);

				while (iss >> word)
				{
					words_from_line.push_back(word);
				}

				string first_term=get_string(words_from_line[1]);
				mymap.insert(pair<string,int>(words_from_line[0],mymap[first_term]+get_digit(words_from_line[1])));

				elem_from_map += words_from_line[0] + " " + to_string(mymap[words_from_line[0]]);
				macro_elem.push_back(elem_from_map);
			}
		}

		file.close();
	}

	ofstream MyFile("isa_macros_expanded.txt");

	for (int i = 0; i < macro_elem.size(); i++)
	{
		MyFile << macro_elem[i] << endl;
	}

	MyFile.close();

	
	
	return 0;
}















