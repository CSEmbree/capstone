/*
 * Title:    feature_vector
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  9-OCT-2014
 * Edited:   9-OCT-2014
 * Notes:    Handles creation of a json formatted feature vector
 */

#ifndef FEATURE_VECTOR_H
#define FEATURE_VECTOR_H

#include "utils.h"
#include "config_handler.h"
#include "audio_recorder.h"


class feature_vector {
 private:

  bool debug;
  string cn; //class name
  
  string time_stamp;
  string id;
  string latitude;
  string longitude;
  string mac_addr;
  string fv_file_name;
  string fv_extention;
  

  vector<string > feature_fnames;
  vector<string > feature_fnames_base;
  vector<pair< vector<pair<string,string> >, vector<double > > > features;

  bool extract_feature_vector_file_names( );
  bool setup_feature_vector( string timeStamp, config_handler *ch, audio_recorder *ar );
  bool find_feature_vector_files( string timeStamp, config_handler *ch, audio_recorder *ar );
  bool read_features( std::vector<string > *fnames=NULL );
  bool read_feature( string fname );

  void init();  
  
 
 public:

  feature_vector( string timeStamp, config_handler *ch, audio_recorder *ar );
  
  bool write( config_handler *ch, audio_recorder *ar, bool formatOutput = true );
  //bool write( string fname );
  void print();


  bool set_time_stamp( string timeStamp );
  bool set_id( string rpid );
  bool set_lat( string lat );
  bool set_lon( string lon );
  bool set_macaddr( string macAddr );
  bool set_fv_file_name( string fname );  
  bool set_fv_ext( string ext );  


  std::vector<string > get_feature_fnames();
  string get_time_stamp();
  string get_id();
  string get_lat();
  string get_lon();
  string get_macaddr( ); 
  string get_fv_file_name();
  string get_fv_ext();
 };


#endif
