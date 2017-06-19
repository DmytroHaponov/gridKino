/**
main.cpp
@author Dmytro Haponov
@version 1.0.0.3 18/06/17

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

//! interpretation of city matrix, it has ONLY parts of city that have rails
//! which is important considering n < 10^9 while k < 10^3
typedef std::map<int, std::vector<int>> MinusRailMap;

//! creates street in MinusRailMap by filling vector of size m with int's in order
//! of arithmetic progression (each next +1)
void add_street (MinusRailMap& map, const int& street_length, const int& cur_street)
{
	std::vector<int> street(street_length);
	std::iota(street.begin(), street.end(), 1); //! numeration starts from 1
	map.insert(std::make_pair(cur_street, std::move(street)));
}

//! parse each line from file, get values for needed integer parameters
bool process_line_from_file (std::string& line, int& first_int, int& sec_int, int& third_int, int& line_num)
{
	std::vector<std::string> vec_of_str; 
	boost::split(vec_of_str, line, boost::is_any_of(" "));
	if(vec_of_str.size() != 3) //! each line must have 3 numbers
	{
		std::cerr<<"there are NOT 3 integers (maybe more than 3) on line number "<<line_num<<": \""<<line<<"\"";
		return 0;
	}
	try
	{
		first_int = std::stoi(vec_of_str.at(0));
		sec_int = std::stoi(vec_of_str.at(1));
	}
	catch (std::invalid_argument&)
	{
		std::cerr << "cannot convert string to int\n"
				  << "line number "<< line_num<<": \""<<line<<"\""<< std::endl;
		return 0;
	}
	catch(std::out_of_range&)
	{
		std::cerr << "result of converting string to int is off the limits\n"
				  << "line number "<< line_num<<": \""<<line<<"\""<< std::endl;
		return 0;
	}
	try
	{	
		third_int = boost::lexical_cast<int>(vec_of_str.at(2)); // boost can be used as well
	}
	catch(boost::bad_lexical_cast const& e)
	{
		std::cout << "couldn't boost lexical cast: " << e.what() 
		    		  << "line number "<< line_num<<": \""<<line<<"\""<< std::endl;
		return 0;
	}
	return 1;
}

//! main idea is here: take difference between street and rail,
//! storing only free cells.
//! REMINDER -- streets WO rails, that is completely free streets, are not stored!
void process_rail (MinusRailMap& map, int& current_street, int& street_begin, int& street_end)
{
	std::vector<int> current_rail(street_end - street_begin + 1), 
		diff_vec; //! vector to store difference between street and rail
	std::iota(current_rail.begin(), current_rail.end(), street_begin);

	std::set_difference(map.at(current_street).begin(), 
						map.at(current_street).end(),
						current_rail.begin(),
						current_rail.end(),
						std::back_inserter(diff_vec));
	map[current_street] = std::move(diff_vec);

#ifdef _DEBUG
	std::cout<<"created rail at "<<current_street<<std::endl;
	std::for_each(current_rail.begin(), current_rail.end(), [](const int& n) { std::cout<<n<<" "; });
	std::cout<<std::endl;

	std::cout<<"resulting difference is ";
	std::for_each(map.at(current_street).begin(), map.at(current_street).end(), 
									[](const int& n) { std::cout << n << " "; });
	std::cout<<std::endl;
#endif
};

int main(int argc, char* argv[]) 
{
	if(argc != 2) 
	{
		std::cerr << "please type filename as program parameter" << std::endl;
		return 1;
	}
	std::string in_file_str(argv[1]);
	std::cout << "trying to open and read: " << in_file_str << std::endl;
	std::ifstream in_stream (in_file_str);
	if (!in_stream.is_open())
		perror(("error while opening file " + in_file_str).c_str());

	int n=0, m=0, k=0; //! from task

	//! street is line in matrix of "city"
	int current_street=0, street_begin=0, street_end=0;
	MinusRailMap line_minus_rails_map;

	//! line read from file
	std::string line_from_file;

	//! process first line
	int line_number = 1; //! for user debugging problems with config file
	getline(in_stream, line_from_file);
	if(!process_line_from_file(line_from_file, n, m, k, line_number)) 
		return 1;
	line_number++;

	//! process all rails
	while(getline(in_stream, line_from_file)) 
	{
		if(!process_line_from_file(line_from_file, current_street, street_begin, street_end, line_number))
			return 1;
		line_number++;
		if (!line_minus_rails_map.count(current_street))
			add_street(line_minus_rails_map, m, current_street);
		process_rail(line_minus_rails_map, current_street, street_begin, street_end);
	}

	if (in_stream.bad())
		perror(("error while reading file " + in_file_str).c_str());
	in_stream.close();

	int ans=0;
	for (auto& map_pair : line_minus_rails_map)
		ans += map_pair.second.size();

	//! check streets WO rails
	int streets_with_rails = line_minus_rails_map.size();
	if (n > streets_with_rails)
		ans += m * (n - streets_with_rails); //! add all cells of streets WO rails

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