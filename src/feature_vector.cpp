

#include "feature_vector.h"
#include "config_handler.h"
#include "audio_recorder.h"
#include "json_generator.h"
#include "utils.h"


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


bool feature_vector::write( config_handler *ch ) {
  
  string mn = "write::";
  cout<<cn<<mn<<" Preparing to write feature vector ... "<<endl;
  bool res = true;
  

  cout<<cn<<mn<<" IMPLIMENT ME! "<<endl;
  // open file

  string filepath = ch->get_data_location();
  string filename = get_time_stamp() + ".dat";
  //  if( fpath != "" ) filepath = fpath;
  //  if( fname != "" ) filename = fname;
  
  string file =  filepath + filename;



  cout<<cn<<mn<<" Going to place json file to \""<<filename<<"\""<<endl;
  json_generator jg( file );
  
  // write stuff
  jg.open_object();

  jg.add_key(   "sample_rate" );
  jg.add_value( utils::number_to_string<int>(ch->get_samp_rate()) );

  jg.add_key(   "recording_number" );
  jg.add_value( utils::number_to_string<int>(ch->get_rec_number()) );

  jg.add_key(   "recording_duration" );
  jg.add_value( utils::number_to_string<int>(ch->get_rec_duration()) );

  jg.add_key(   "latitude" );
  jg.add_value( ch->get_latitude() );

  jg.add_key(   "longitude" );
  jg.add_value( ch->get_longitude() );

  jg.add_key(   "rpid" );
  jg.add_value( ch->get_longitude() );

  jg.add_key(   "rpid" );
  jg.add_value( ch->get_longitude() );

  jg.add_key(   "simulation" );
  jg.add_value( ch->get_simulate()? "yes":"no" );

  jg.add_key(   "filter" );
  jg.add_value( ch->get_filter()? "yes":"no" );

  jg.add_key(   "analysis" );
  jg.add_value( ch->get_analysis()? "yes":"no" );

  jg.add_key(   "config_file" );
  jg.add_value( ch->get_config_file() );

  jg.add_key(   "features" );
  std::vector<string > features;
  features.clear();
  features.push_back("Energy");
  features.push_back("PerceptualResolution");
  jg.add_array_value( features );


  jg.close_object(); //TEST
  
  cout<<cn<<mn<<" TEST: Sample JSON format:"<<endl;
  cout<<jg.get_contents_string()<<endl;

  //jg.write_to( file );


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

