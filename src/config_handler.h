/*
 * Title:    config_handler
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2013
 * Edited:   4-SEP-2014
 * Notes:    Handles storage for config file options
 */

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

using namespace std;


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>  // std::setw
#include <fstream>
#include <sstream>


class config_handler {
 private:

  bool debug;
  string cn; //class name

  string config_file_name;
  string fv_path;
  string rec_file_name_prefix;
  string rec_location;
  string rec_extention;
  
  int samp_rate;
  int rec_dur;
  int rec_num;
  

  void init();  
  int numeric( string s );
  
  
 
 public:

  config_handler(string fname);

  void read_config(string fname);
  void print();
  void split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters);
  bool file_exists(const string& s); //helper
  string trim(std::string s); //helper

  bool set_config_file_name( string fname );
  bool set_fv_path( string fvpath );
  bool set_rec_file_name_prefix( string prefix );
  bool set_rec_location( string loc );
  bool set_rec_extention( string ext );
  bool set_samp_rate( int samprate );
  bool set_rec_duration( int recdur );  
  bool set_rec_number( int recnum );  
  
  
  string get_config_file_name();
  string get_fv_path();
  string get_rec_file_name_prefix();  
  string get_rec_location();
  string get_rec_extention();
  int get_samp_rate();
  int get_rec_duration();
  int get_rec_number();
  
 };


#endif
