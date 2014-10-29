

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
  
  feature_fnames.clear();
  set_time_stamp( "" );
  set_id( "" );
  set_lat( "" );
  set_lon( "" );
  set_macaddr( "" );
  set_fv_ext( ".dat" ); //feature vector json extention
  

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
  
  set_fv_file_name( ar->get_rec_file_name_core() + get_fv_ext() );

  // find all feature vector element file names
  find_feature_vector_files( timeStamp, ch, ar );


  // read all of the feature files found
  read_features();


  
  

  cout<<cn<<mn<<" Finished basic setup."<<endl;
  
  return true;
}



bool feature_vector::find_feature_vector_files( string timeStamp, config_handler *ch, audio_recorder *ar ) {

  string mn = "find_feature_vector_files:";
  cout<<cn<<mn<<" gathering yaafe extracted features ... "<<endl;
  bool res = true;

  
  cout<<cn<<mn<<" IMPLIMENT ME! "<<endl;

  cout<<cn<<mn<<" Retreiving specific featrues from \""<<ch->get_analysis_location()<<"\" "<<endl;
  std::vector<string > found_feature_files;
  found_feature_files.clear();

  //audio info
  ar->print();


  DIR* d = opendir( ch->get_analysis_location().c_str() );
  if (d == NULL) {
    cout<<cn<<mn<<" ERROR: Coudl not open directory \""<<ch->get_analysis_location()<<"\"."<<endl;
    return EXIT_FAILURE;
  }



  string fname = "";
  string full_fname = "";
  string audio_fname_core = ar->get_rec_file_name_core();
  string audio_fname = audio_fname_core + ch->get_rec_extention();
  for(struct dirent *de = NULL; (de = readdir(d)) != NULL; ) {
    
    //printf("%s/%s\n", ch->get_analysis_location().c_str(), de->d_name);
    fname = string(de->d_name);
    full_fname = ch->get_analysis_location() + fname;
    cout<<cn<<mn<<" Checking file: \""<<full_fname<<"\"."<<endl;
    

    if( fname.find( audio_fname_core ) != string::npos && fname != audio_fname ) {
      cout<<cn<<mn<<" Contains what we want: \""<<full_fname<<"\""<<endl;
      found_feature_files.push_back( full_fname );
    }
  }


  cout<<cn<<mn<<" List of Feature Files from YAAFE:"<<endl;
  for( unsigned int i=0; i<found_feature_files.size(); i++) {
    cout<<"  "<<found_feature_files.at(i)<<endl;
  }

  closedir(d);

  
  feature_fnames = found_feature_files;
  cout<<cn<<mn<<" Found all yaafe extracted features for recording \""<<audio_fname<<"\"."<<endl;

  return res;
}


bool feature_vector::read_features( std::vector<string > *fnames ) {

  string mn = "read_features:";
  bool res = true;
  cout<<cn<<mn<<" Reading features ... "<<endl;

  // decide what we are reading features from. Allow explicit pass by parameter.
  std::vector<string > feature_files = feature_fnames; //default to global but allow user to pass explicit features;
  if( fnames != NULL ) feature_files = *fnames;
 

  // read each feature
  for( unsigned int i = 0; i < feature_files.size(); i++ ) {
    read_feature( feature_files.at(i) );
    //    break; //TEST - TODO - REMOVE ME
  }

  
  cout<<cn<<mn<<" Finished reading features."<<endl;
  
  return res;
}


bool feature_vector::read_feature( string fname ) {

  string mn = "read_feature:";
  bool res = true;
  cout<<cn<<mn<<" Reading extracted feature \""<<fname<<"\""<<endl;



  std::vector<pair<string,string> > feature_details;
  feature_details.clear();
  std::vector<double > feature_data;
  feature_data.clear();



  if( utils::is_file_readable( fname ) == true) {
    std::ifstream data( fname );    
    std::string line;
    std::string key = "", value = "";
    
    while( std::getline( data, line) ) {
      std::stringstream lineStream( line ); //each line
      std::string cell; //each element of a line

      
      while( std::getline( lineStream, cell, ',' ) ) {

	
	if( cell.front() == '%' ) {
	  string trimmed_line = utils::trim( cell, " \t%" ); //clean up
	  
	  // retrieve the option name and it's value seperatly for further parsing
	  int optionSep = trimmed_line.find('='); //optioname and value are seperated by aan equals sign
	  key  = utils::trim( trimmed_line.substr(0, optionSep) );
	  value = utils::trim( trimmed_line.substr(optionSep+1, trimmed_line.size()) );
	  
	  pair<string, string> data_detail( key, value );
	  
	  feature_details.push_back( data_detail );	
	  
	} else {
	  double val = (double) utils::string_to_number<double>( cell );
	  
	  feature_data.push_back( val );
	}
      }
    }
    

    /*
    cout<<cn<<mn<<" TEST: Feature Details:"<<endl;
    for( unsigned int i = 0; i < feature_details.size(); i ++) {
      cout<<"  Pair "<<i<<": "<<feature_details.at(i).first<<", "<<feature_details.at(i).second<<endl;
    }

    cout<<cn<<mn<<" TEST: Feature Data:"<<endl;
    for( unsigned int i = 0; i < feature_data.size(); i ++) {
      cout<<"  index "<<i<<": "<<feature_data.at(i)<<endl;
    }
    */

    //any number of details paired with any number of data
    pair< vector<pair<string,string> >, vector<double> > feature_info( feature_details, feature_data );
    
    features.push_back( feature_info );
  }
  
  
  
  cout<<cn<<mn<<" Finished reading extracted feature \""<<fname<<"\""<<endl;
  
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

  jg.add_key( "features" );
  jg.open_object();


  
  // get all feature names
  vector<string > feature_names;
  feature_names.clear();
  
  for( unsigned int feat_i = 0; feat_i < features.size(); feat_i++ ) {
    
    //get the name of the feature
    vector<pair<string, string> > details = features.at(feat_i).first;
    string feature_name = "<missing>";
    bool found = false;
    
    for( unsigned int det_i = 0; det_i < details.size() && found == false; det_i++ ) {
      if( details.at(det_i).first == "yaafedefinition" ) {
	feature_name = details.at(det_i).second;
	found = true;
      }	
    }
    
    feature_names.push_back( feature_name );
  }


  // add feature names with details and data to json
  for( unsigned int feat_i = 0; feat_i < features.size(); feat_i++ ) {
    
    vector<pair<string, string> > details = features.at(feat_i).first;

    // open an individual feature with that name
    jg.add_key( feature_names.at(feat_i) );
    jg.open_object();
        
    // add key,vals for each detail of the feature
    for( unsigned int det_i = 0; det_i < details.size(); det_i++ ) {
      jg.add_key( details.at(det_i).first );
      jg.add_value( details.at(det_i).second );
    }	


    
    // add a key,val for the data extracted for our individual feature
    jg.add_key( "data" );

    // Add each value of the data extracted to the individual feature
    jg.add_array_value( features.at(feat_i).second );
    


    //close individual feature
    jg.close_object();
    
  }
  
  jg.close_object(); //close features
  
  jg.close_object(); //close whole
  

  //TEST
  cout<<cn<<mn<<" TEST: Sample JSON format:"<<endl;
  cout<<jg.get_contents_string()<<endl;


  // write json file out
  string out_fpath = utils::pathify( ch->get_data_location() );
  string out_fname = get_fv_file_name();

  cout<<cn<<mn<<" writing to: \""<<out_fpath+out_fname<<"\"."<<endl;

  jg.write_to( out_fname, out_fpath );

  
  cout<<cn<<mn<<" JSON formatted feature vector written to: \""<<out_fpath+out_fname<<"\"."<<endl;
  
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
std::vector<string > feature_vector::get_feature_fnames() { return feature_fnames; }

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

