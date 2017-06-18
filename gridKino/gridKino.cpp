/**
   main.cpp
   @author Dmytro Haponov
   @version 1.0.0.2 18/06/17

   Purpose: find empty cells in matrix
*/

#include "stdafx.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <numeric>
#include <string>
#include <stdio.h>

int n=0, m=0, k=0;
int current_street=0, street_begin=0, street_end=0;

std::map<int, std::vector<int>> reels_map;

void add_street(int& cur_street)
{
	std::vector<int> street(m);
	std::iota(street.begin(), street.end(), 1);
	reels_map.insert(std::make_pair(cur_street, std::move(street)));
}

void process_line_from_file(std::string& line, int& first_int, int& sec_int, int& third_int)
{
	std::vector<std::string> vec_of_str(3);
	boost::split(vec_of_str, line, boost::is_any_of(" "));
	first_int = std::stoi(vec_of_str.at(0));
	sec_int = boost::lexical_cast<int>(vec_of_str.at(1)); // can be used as well
	third_int = std::stoi(vec_of_str.at(2));
}

void process_reel(int& current_street, int& street_begin, int& street_end)
{
	std::vector<int> current_reel(street_end-street_begin+1), diff_vec;
	std::iota(current_reel.begin(), current_reel.end(), street_begin);

	std::set_difference(reels_map.at(current_street).begin(), reels_map.at(current_street).end(),
		current_reel.begin(), current_reel.end(),
		std::back_inserter(diff_vec));
	reels_map[current_street] = std::move(diff_vec);

#ifdef _DEBUG
	std::cout<<"created reel at "<<current_street<<std::endl;
	std::for_each(current_reel.begin(), current_reel.end(), [](const int& n) { std::cout<<n<<" "; });
	std::cout<<std::endl;

	std::cout<<"resulting difference is ";
	std::for_each(reels_map.at(current_street).begin(), reels_map.at(current_street).end(), [](const int& n) { std::cout << n << " "; });
	std::cout<<std::endl;
#endif
}

int main(int argc, char* argv[]) 
{
	if(argc != 2) 
	{
		std::cerr << "please type filename as program parameter" << std::endl;
		return 1;
	}

	std::string in_file_str(argv[1]);
	std::cout << "trying to open and read: " << in_file_str << std::endl;
	std::ifstream in_stream (argv[1]);
	if (!in_stream.is_open())
		perror(("error while opening file " + in_file_str).c_str());

	std::string line_from_file;
	//! process first line
	getline(in_stream, line_from_file);
	process_line_from_file(line_from_file, n, m, k); 

	//! process all reels
	while(getline(in_stream, line_from_file)) 
	{
		process_line_from_file(line_from_file, current_street, street_begin, street_end);
		if (!reels_map.count(current_street))
			add_street(current_street);
		process_reel(current_street, street_begin, street_end);
	}

	if (in_stream.bad())
		perror(("error while reading file " + in_file_str).c_str());
	in_stream.close();

	int ans=0;
	for (auto& map_pair : reels_map)
		ans += map_pair.second.size();

	//! check streets WO reels
	int streets_with_reels = reels_map.size();
	if (n > streets_with_reels)
		ans += m * (n - streets_with_reels);

	std::cout<<"answer is "<<ans<<std::endl;

	//! write answer to file
	std::string out_file_str;
	std::size_t in_found = in_file_str.rfind("in");
	if (in_found != std::string::npos)
		out_file_str = in_file_str.substr(0, in_found) + "out.txt";
	else
		out_file_str	 = in_file_str.substr(0, in_file_str.rfind(".")) + "ANSWER.txt";
	std::cout << "trying to open and write to: " << out_file_str << std::endl;
	std::ofstream o_stream(out_file_str);
	if (!o_stream.is_open())
		perror(("cannot write to file " + out_file_str).c_str());
	o_stream << ans;
	o_stream.close();

	return 0;
}