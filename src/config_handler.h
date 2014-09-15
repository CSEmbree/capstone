/*
 * Title:    config_handler
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2014
 * Edited:   4-SEP-2014
 * Notes:    Handles storage for config file options
 */

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include "utils.h"


class config_handler {
 private:

  bool debug;
  string cn; //class name

  string config_file_path;
  string config_file_name;
  string config_file;

  string fv_file_path;
  string fv_file_name;
  string fv_file;

  string rec_file_name_prefix;
  string rec_location;
  string rec_extention;
  
  string analysis_location;

  int samp_rate;
  int rec_dur;
  int rec_num;
  
  bool background;

  string latitude;
  string longitude;
  string rpid;

  void init();  
  
 
 public:

  config_handler(string fpath, string fname );

  void read_config(string fname);
  void print();
  void split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters);
  bool file_exists(const string& s); //helper
  string trim(std::string s); //helper

  bool set_config_file_path( string fpath );
  bool set_config_file_name( string fname );
  bool set_config_file( string configfile );

  bool set_fv_file_path( string fvpath );
  bool set_fv_file_name( string fvname );
  bool set_fv_file( string fvfile );

  bool set_rec_file_name_prefix( string prefix );
  bool set_rec_location( string loc );
  bool set_rec_extention( string ext );

  bool set_analysis_location( string loc );

  bool set_samp_rate( int samprate );
  bool set_rec_duration( int recdur );  
  bool set_rec_number( int recnum );  
  bool set_background( bool status );

  bool set_latitude( string lat );
  bool set_longitude( string lon );
  bool set_rpid( string id );



  
  string get_config_file_path();
  string get_config_file_name();
  string get_config_file();

  string get_fv_file_path();
  string get_fv_file_name();
  string get_fv_file();

  string get_rec_file_name_prefix();  
  string get_rec_location();
  string get_rec_extention();

  string get_analysis_location();

  int get_samp_rate();
  int get_rec_duration();
  int get_rec_number();
  bool get_background();
  
  string get_latitude();
  string get_longitude();
  string get_rpid();

 };


#endif
