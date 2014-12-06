/*
 * Title:    config_handler
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2014
 * Edited:   23-OCT-2014
 * Notes:    Handles storage for config file options
 */

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include "utils.h"


class config_handler {
 private:

  static const string DEF_CONFIG_NAME;
  static const string DEF_FILTER_NAME;
  static const string DEF_FEATURE_NAME;
  static const string DEF_REC_PREFIX;
  static const string DEF_REC_EXT;
  static const string DEF_FINAL_FORMAT;
  static const string DEF_LAT;
  static const string DEF_LON;
  static const string DEF_RPID;
  static const int    DEF_SOUND_ID;
  static const string DEF_DATA_DIR;
  static const string DEF_ANALYSIS_DIR;
  static const string DEF_MEDIA_DIR;


  bool debug;
  string cn; //class name

  string config_file_path;
  string config_file_name;
  string config_file;
  
  string fv_filter_path;
  string fv_filter_name;
  string fv_filter;

  string fv_file_path;
  string fv_file_name;
  string fv_file;

  string rec_file_name_prefix;
  string media_location;
  string rec_extention;
  
  string data_location;
  string analysis_location;

  int samp_rate;
  int rec_dur;
  int rec_num;
  
  bool background;
  bool filter;
  bool analysis;
  bool simulate;
  bool save_rec;
  bool output_formatted;

  string simulate_dir;

  string latitude;
  string longitude;
  string rpid;

  string final_feature_format;
  int output_type_id;

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

  bool set_fv_filter_path( string fvpath );
  bool set_fv_filter_name( string fvname );
  bool set_fv_filter( string fvfilter );

  bool set_fv_file_path( string fvpath );
  bool set_fv_file_name( string fvname );
  bool set_fv_file( string fvfile );

  bool set_rec_file_name_prefix( string prefix );
  bool set_media_location( string loc );
  bool set_rec_extention( string ext );

  bool set_data_location( string loc );
  bool set_analysis_location( string loc );

  bool set_samp_rate( int samprate );
  bool set_rec_duration( int recdur );  
  bool set_rec_number( int recnum );  
  bool set_background( bool status );

  bool set_latitude( string lat );
  bool set_longitude( string lon );
  bool set_rpid( string id );

  bool set_filter( bool status );
  bool set_analysis( bool status );
  bool set_save_rec( bool status );

  bool set_simulate( bool status );
  bool set_simulate_dir( string dir );

  bool set_output_formatted( bool status );

  bool set_final_feature_format( string format );
  bool set_output_type_id( int outputid );
  


  string get_config_file_path();
  string get_config_file_name();
  string get_config_file();

  string get_fv_filter_path();
  string get_fv_filter_name();
  string get_fv_filter();

  string get_fv_file_path();
  string get_fv_file_name();
  string get_fv_file();

  string get_rec_file_name_prefix();  
  string get_media_location();
  string get_rec_extention();

  string get_data_location();
  string get_analysis_location();

  int get_samp_rate();
  int get_rec_duration();
  int get_rec_number();
  bool get_background();
  
  string get_latitude();
  string get_longitude();
  string get_rpid();

  bool get_filter();
  bool get_analysis();
  bool get_simulate();
  bool get_output_formatted();
  bool get_save_rec();

  string get_simulate_dir();

  string get_final_feature_format();
  int get_output_type_id();

 };


#endif
