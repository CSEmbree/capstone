/*
 * Title:    audio_recorder
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  5-SEP-2013
 * Edited:   6-SEP-2014
 * Notes:    Makes a recording based on config file provided recording 
 *             inforamtion (if avaialble) and/or user data at runtime.
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>

#include "config_handler.h"


#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif


#ifndef   AUDIO_RECORDER_H
#define   AUDIO_RECORDER_H


using namespace std;

class audio_recorder {

 private:

  bool debug;
  string cn; //class name
  
  string rec_file_name;
  string rec_file_name_prefix;
  string rec_location;
  string rec_extention;
  int rec_duration;

  void init();
  bool set_rec_file_name_prefix( string recfilenameprefix );
  bool set_rec_file_name( string recfilename );
  bool set_rec_location( string loc );
  bool set_rec_extention( string ext );
  bool set_rec_duration( int dur );



  
 public:

  audio_recorder( config_handler ch );

  void apply_config_settings( config_handler ch );
    void record( string ts="", int dur=-1 ); //default recording length is 1 second
  string make_time_stamp();
  string get_mac_address();
  string make_rec_cmd( const string fileName, const int dur=-1 );
  string make_rec_cmd( const string fileName, const string dur="-1" );
  string make_audio_file_name( string timeStamp, string recPrefix="", string dirPath="", string fExt="" );

  void print();
  string get_rec_file_name_prefix();
  string get_rec_file_name();
  string get_rec_location();
  string get_rec_extention();
  int get_rec_duration();


};

#endif
