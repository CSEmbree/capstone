/*
 * Title:    audio_recorder
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  5-SEP-2014
 * Edited:   6-SEP-2014
 * Notes:    Makes a recording based on config file provided recording 
 *             inforamtion (if avaialble) and/or user data at runtime.
 */


#ifndef   AUDIO_RECORDER_H
#define   AUDIO_RECORDER_H


#include "utils.h"
#include "config_handler.h"



using namespace std;

class audio_recorder {

 private:

  bool debug;
  string cn; //class name
  
  string rec_file_name;
  string rec_file_name_base;
  string rec_file_name_core;
  string rec_file_name_prefix;
  string rec_location;
  string rec_extention;
  int rec_duration;
  string time_stamp;

  void init();
  bool set_rec_file_name_prefix( string recfilenameprefix );
  bool set_rec_file_name_base( string recfilenamebase );
  bool set_rec_file_name_core( string recfilenamecore );
  bool set_rec_file_name( string recfilename );
  bool set_rec_location( string loc );
  bool set_rec_extention( string ext );
  bool set_rec_duration( int dur );
  bool set_time_stamp( string timestamp );

  
 public:

  audio_recorder( config_handler ch );

  void apply_config_settings( config_handler ch );
  bool record( string ts="", int dur=-1 ); //default recording length is 1 second
  string make_rec_cmd( const string fileName, const int dur=-1 );
  string make_rec_cmd( const string fileName, const string dur="-1" );
  string make_audio_file_name_core(string timeStamp, string recPrefix="" );
  string make_audio_file_name_base(string timeStamp, string recPrefix="", string dirPath="" );  
  string make_audio_file_name( string timeStamp, string recPrefix="", string dirPath="", string fExt="" );

  void print();
  string get_rec_file_name_prefix();
  string get_rec_file_name();
  string get_rec_file_name_base();
  string get_rec_file_name_core();
  string get_rec_location();
  string get_rec_extention();
  int get_rec_duration();
  string get_time_stamp();


};

#endif
