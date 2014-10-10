

#include "feature_vector.h"
#include "config_handler.h"
#include "audio_recorder.h"


/******************************************************************
 ** Method Implementations
 ******************************************************************/


feature_vector::feature_vector( string timeStamp, config_handler *ch, audio_recorder *ar ) {
  string mn = "feature_vector:"; 
  cout<<mn<<" Constructor started. "<<endl;
  
  
  debug = true;
  init();
  
  setup_feature_vector( timeStamp, ch, ar );
  
  
  cout<<cn<<mn<<" Constructor finished. "<<endl;
}


void feature_vector::init() {
  
  cn = " feature_vector::";
  
  feature_file_names.clear();
  set_time_stamp( "" );
  set_id( "" );
  set_lat( "" );
  set_lon( "" );
  set_macaddr( "" );
  set_fv_ext( ".dat" );
  
  return;
}


bool feature_vector::setup_feature_vector( string timeStamp, config_handler *ch, audio_recorder *ar ) {
  
  string mn = "setup_feature_vector:";
  cout<<cn<<mn<<" Setting up feature vector basics ... "<<endl;
  
  
  // prep basics
  set_time_stamp( timeStamp );
  set_id( ch->get_rpid() );
  set_lat( ch->get_latitude() );
  set_lon( ch->get_longitude() );
  set_macaddr( utils::get_mac_address() );
  
  set_fv_file_name( ar->get_rec_file_name_base() + get_fv_ext() );

  // find all feature vector element file names
  find_feature_vector_files();
  

  cout<<cn<<mn<<" Finished basic setup."<<endl;
  
  return true;
}



bool feature_vector::find_feature_vector_files( ) {

  string mn = "find_feature_vector_files:";
  cout<<cn<<mn<<" gathering yaafe extracted features ... "<<endl;
  bool res = true;

  
  cout<<cn<<mn<<" IMPLIMENT ME! "<<endl;


  cout<<cn<<mn<<" Found all yaafe extracted features. "<<endl;

  return res;
}


bool feature_vector::write( string fname="" ) {
  
  string mn = "write::";
  cout<<cn<<mn<<" Preparing to write feature vector ... "<<endl;
  bool res = true;
  

  cout<<cn<<mn<<" IMPLIMENT ME! "<<endl;
  // open file
  
  // write stuff
  
  // close file
  
  
  cout<<cn<<mn<<" Preparing to write feature vector ... "<<endl;
  
  return res;
}



void feature_vector::print() {
  //
  // Display internal settings set from reading steering
  //
  string mn = "print:";
  string def = "<default>";
  
  int w1=30; //arbitrary spacing size that makes formatting look pretty for data names
  int w2=30; //arbitrary spacing size that makes formatting look pretty for data options
  
  
  cout<<cn<<mn<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w1)<<"DEBUG: "                    <<setw(w2)<<(debug? "ON":"OFF")
      <<"\n"<<setw(w1)<<"Feature Vector File Name: " <<setw(w2)<<"\""<<get_fv_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Feature Vector Extention: " <<setw(w2)<<"\""<<get_fv_ext()<<"\""
      <<"\n"<<setw(w1)<<"Time: "                     <<setw(w2)<<"\""<<get_time_stamp()<<"\""
      <<"\n"<<setw(w1)<<"ID: "                       <<setw(w2)<<"\""<<get_id()<<"\""
      <<"\n"<<setw(w1)<<"Latitude: "                 <<setw(w2)<<"\""<<get_lat()<<"\""
      <<"\n"<<setw(w1)<<"Longitude: "                <<setw(w2)<<"\""<<get_lon()<<"\""
      <<"\n"<<setw(w1)<<"Mac Address: "              <<setw(w2)<<"\""<<get_macaddr()<<"\""<<endl;
  cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  
  return;
}



//*****************
//*   ACCESSORS   *
//*****************


string feature_vector::get_time_stamp() { return time_stamp; }
string feature_vector::get_id() { return id; }
string feature_vector::get_lat() { return latitude; }
string feature_vector::get_lon() { return longitude; }
string feature_vector::get_macaddr() { return mac_addr; }
string feature_vector::get_fv_file_name() { return fv_file_name; }
string feature_vector::get_fv_ext() { return fv_extention; }

//*****************
//*   MUTATORS    *
//*****************


bool feature_vector::set_time_stamp( string timeStamp ) {
  time_stamp = timeStamp;
  return true; //TODO - impliment
}

bool feature_vector::set_id( string rpid ) {
  id = rpid;
  return true; //TODO - impliment
}

bool feature_vector::set_lat( string lat ) {
  longitude = lat;
  return true; //TODO - impliment
}

bool feature_vector::set_lon( string lon ) {
  longitude = lon;
  return true; //TODO - impliment
}

bool feature_vector::set_macaddr( string macAddr ) {
  mac_addr = macAddr;
  return true; //TODO - impliment
}

bool feature_vector::set_fv_file_name( string fname ) { 
  fv_file_name = fname;
  return true; //TODO - impliment
}

bool feature_vector::set_fv_ext( string ext ) { 
  fv_extention = ext;
  return true; //TODO - impliment
}
