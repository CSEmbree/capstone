

#include "config_handler.h"

/******************************************************************
 ** Method Implementations
 ******************************************************************/


config_handler::config_handler( string fpath, string fname = "" )
{
  string mn = "config_handler:"; 
  cout<<mn<<" Constructor started for file \""<<fname<<"\"."<<endl;

  debug = true;
  init();


  if( fpath == "" ) fpath = utils::get_home_dir();
  if( fname == "" ) utils::get_base( fpath );
  if( fname == "" ) fname = "cirainbow.conf";

  // make sure default or custom config file is found
  // User coice, home directory "cirainbow.conf", local directory "sound_settings.conf"
  if( utils::is_file_readable( utils::pathify(fpath) + fname ) == false ) {

    cout<<cn<<mn<<" Initial config file not accesable at: \""<<utils::pathify(fpath)+fname<<"\"."<<endl;
    fpath = utils::get_base_dir();
    fname = "sound_settings.conf";
  }

  //cout<<cn<<mn<<" Config file here: \""<<utils::pathify(fpath)+fname<<"\"."<<endl; //TEST
  //exit( 0 ); //TEST


  set_config_file_path( fpath );
  set_config_file_name( fname );

  string configFile = get_config_file_path() + get_config_file_name();
  set_config_file( configFile );

  cout<<cn<<mn<<" Config file here: \""<<get_config_file()<<"\"."<<endl;


  read_config( configFile );


  cout<<cn<<mn<<" Constructor finished for file \""<<fname<<"\"."<<endl;
}


void config_handler::init() {

  //  const std::string MyClass::valid = "something else again";

  const string DEF_CONFIG_NAME  = "cirainbow.conf";
  const string DEF_FILTER_NAME  = "featureplan_filter";
  const string DEF_FEATURE_NAME = "featureplan";
  const string DEF_REC_PREFIX   = "rec_";
  const string DEF_REC_EXT      = ".wav";
  const string DEF_FINAL_FORMAT = "WRAPPED";
  const string DEF_LAT          = "0.0000 N";
  const string DEF_LON          = "0.0000 W";
  const string DEF_RPID         = "-1";
  const int    DEF_SOUND_ID     = 0; // TODO - What should this be by default?

  const string DEF_DATA_DIR     = "data/";
  const string DEF_ANALYSIS_DIR = "analysis/";
  const string DEF_MEDIA_DIR    = "media/";



  cn = " config_handler::";
  set_config_file_path ( utils::get_home_dir() );
  set_config_file_name ( DEF_CONFIG_NAME ); // TODO - make hardcoded elsewhere 
  set_config_file      ( get_config_file_path() + get_config_file_name() ); 

  set_fv_filter_path   ( utils::get_base_dir() ); 
  set_fv_filter_name   ( DEF_FILTER_NAME ); 
  set_fv_filter        ( get_fv_filter_path() + get_fv_filter_name() ); 
  
  set_fv_file_path     ( utils::get_base_dir() ); 
  set_fv_file_name     ( DEF_FEATURE_NAME ); 
  set_fv_file          ( get_fv_file_path() + get_fv_file_name() ); 
  
  //  set_data_location       ( utils::get_base_dir() + DEF_DATA_DIR ); //TODO - make hardcoded elsewhere
  //  set_analysis_location   ( utils::get_base_dir() + DEF_ANALYSIS_DIR ); // TODO - make hardcoded elsewhere 
  set_data_location       ( utils::get_home_dir() + DEF_DATA_DIR );
  set_analysis_location   ( utils::get_base_dir() + DEF_ANALYSIS_DIR );

  set_rec_file_name_prefix( DEF_REC_PREFIX ); // TODO - make hardcoded elsewhere 
  set_media_location      ( get_data_location() + DEF_MEDIA_DIR ); // TODO - make hardcoded elsewhere 
  set_rec_extention       ( DEF_REC_EXT ); // TODO - make hardcoded elsewhere 
  
  set_samp_rate    ( -1 ); 
  set_rec_number   ( -1 ); 
  set_rec_duration ( -1 ); 

  set_latitude( DEF_LAT ); // TODO - make hardcoded elsewhere 
  set_longitude( DEF_LON ); // TODO - make hardcoded elsewhere 
  set_rpid( DEF_RPID );

  set_final_feature_format( DEF_FINAL_FORMAT ); //Foramts: WRAPPED, FILES
  set_output_type_id( DEF_SOUND_ID ); //Numeric represetning the type of data this is. TODO - WHAT SHOULD THIS BE?

  set_simulate( false ); //simulate a run (dont actually do one).
  set_simulate_dir( "" ); //default sim data dir

  set_background( false ); //run in forground by default
  set_filter( false ); //filtering is ONLY done if requested
  set_analysis( false ); //feature extraction only done if user wants it
  set_save_rec( true ); //Always save recordings by default
  set_output_formatted( true ); //It makes sense to always format output

  return;
}

void config_handler::read_config( string fname="" ) {
  //
  // read steering from file fname
  //
  string mn = "read_config:"; //Method name, for printing
  bool sound_section = false; //flag for only parsing the sound related sections

  string confFileName = fname;
  if( fname == "" ) confFileName = get_config_file(); 


  cout<<cn<<mn<<" Searching for config file \""<<confFileName<<"\"..."<<endl;
  
  //ensure config provided is found and readable
  ifstream infile( confFileName.c_str(), ios::in );
  if( !infile ){ // Check open
    cerr<<cn<<mn<<" ERROR: Can't open \""<<confFileName<<"\""<<endl;
    infile.close();
    exit(1);
  } else {

    if( confFileName != get_config_file() ) {
      this->set_config_file( confFileName );
    }
    
    if (debug) cout<<cn<<mn<<" Reading config file \""<<confFileName<<"\""<<endl;
  }

  
  // prepare to read
  string curLine;
  string optionName;
  string optionValue;
  int w = 20; //arbitrary width units that make formatting nice


  // read all options from text file
  while (infile.good()) {
    
    // get one whole line at a time
    getline(infile, curLine);    
    curLine = trim(curLine); //remove leading and trailing white space
    

    // check for correct config file section as we go through all of the config file
    if( curLine.front() == '[' && curLine.back() == ']' ) {
      string section_name = curLine.substr( curLine.find_last_of("[")+1, curLine.find_last_of("]")-1 );

      if( section_name == "NODE_INFO" || section_name == "SOUND" ) {
	sound_section = true;
      } else {
	sound_section = false;
      }

      cout<<cn<<mn<<" Section name: \""<<section_name<<"\". This is a sound section? "<<(sound_section? "YES":"NO")<<endl;
    }

     
    
    // Only parse the relevnt sound sections
    if( sound_section == true ) {
      
      // retrieve the option name and it's value seperatly for further parsing
      int optionSep = curLine.find('='); //optioname and value are seperated by aan equals sign
      optionName  = trim(curLine.substr(0, optionSep));
      optionValue = trim(curLine.substr(optionSep+1, curLine.size())); //'optionValue' could be broken up further if needed
      
      
      if (debug) {
	cout<<cn<<mn<<" Read in:>>>>>>>>>>>"
	    <<"\n"<<setw(w)<<"currentLine: \""<<curLine<<"\""
	    <<"\n"<<setw(w)<<" optionName: \""<<optionName<<"\""
	    <<"\n"<<setw(w)<<"optionValue: \""<<optionValue<<"\""
	    <<"\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
      }
      
      
      
      if(curLine[0] != '#' && !curLine.empty() ) { //ignore lines beginning with comments - could be done better
	
	if( optionName == optionValue && optionName != "debug" ) {
	  cerr<<cn<<mn<<" ERROR: config option \""<<optionName<<"\" found but no value set!"<<endl;
	  
	} else if ( optionName == "debug" ) {
	  debug=true;
	  cout<<cn<<mn<<" Debug turned on!"<<endl;
	  
	} else if ( optionName == "recordingduration" ) {
	  set_rec_duration( utils::string_to_number<int>( optionValue ) );
	  
	} else if ( optionName == "recordingnumber" ) {
	  set_rec_number( utils::string_to_number<int>( optionValue ) );
	  
	} else if ( optionName == "recordingprefix" ) {
	  set_rec_file_name_prefix( optionValue );
	  
	} else if ( optionName == "samplerate" ) {
	  set_samp_rate( utils::string_to_number<int>( optionValue ) );
	  
	} else if ( optionName == "featureplanpath" ) {
	  set_fv_file_path( optionValue );
	  
	} else if ( optionName == "featureplanname" ) {
	  set_fv_file_name( optionValue );
	  
	} else if ( optionName == "filterplanpath" ) {
	  set_fv_filter_path( optionValue );
	  
	} else if ( optionName == "filterplanname" ) {
	  set_fv_filter_name( optionValue );
	  
	} else if ( optionName == "recordingextention" ) {
	  set_rec_extention( optionValue );
	  
	} else if ( optionName == "medialocation" ) {
	  set_media_location( optionValue );
	  
	} else if ( optionName == "datalocation" ) {
	  set_data_location( optionValue );
	  
	} else if ( optionName == "analysislocation" ) {
	  set_analysis_location( optionValue );
	  
	} else if ( optionName == "latitude" ) {
	  set_latitude( optionValue );
	  
	} else if ( optionName == "longitude" ) {
	  set_longitude( optionValue );
	  
	} else if ( optionName == "rasberrypiid" ) {
	  set_rpid( optionValue );
	  
	} else if ( optionName == "simulationdirectory" ) {
	  set_simulate_dir( optionValue );
	  
	} else if ( optionName == "outputform" ) {
	  set_final_feature_format( optionValue );

	} else if ( optionName == "outputtypeid" ) {
	  set_output_type_id( utils::string_to_number<int>( optionValue ) );
	  
	} else if ( optionName == "outputhumanreadable" ) {
	  if( optionValue == "on"  ) set_output_formatted( true  );
	  else                       set_output_formatted( false );

	} else if ( optionName == "simulate" ) {
	  if( optionValue == "on"  ) set_simulate( true  );
	  else                       set_simulate( false );
	  
	} else if ( optionName == "background" ) {
	  if( optionValue == "on"  ) set_background( true  );
	  else                       set_background( false );
	  
	} else if ( optionName == "filter" ) {
	  if( optionValue == "on"  ) set_filter( true  );
	  else                       set_filter( false );

	} else if ( optionName == "analysis" ) {
	  if( optionValue == "on"  ) set_analysis( true  );
	  else                       set_analysis( false );

	} else if ( optionName == "saverecording" ) {
	  if( optionValue == "on"  ) set_save_rec( true  );
	  else                       set_save_rec( false );
	  
	} else {
	  cerr<<cn<<mn<<" WARN: Invalid config option found '"<<curLine<<"'"<<endl;
	  //exit(0); //stop if an invalid option found, it's probably an error!
	}	
      }
    }
  }  
  

  // feature plans can be listed a few different ways, ensure they are appropratly formatted
  string file = "";
  
  if( fv_filter_path != "" ) file =  get_fv_filter_path();
  else                       file =  utils::get_base_dir();
  if( fv_filter_name != "" ) file += get_fv_filter_name();
  else                       file += "featureplan_filter";  
  set_fv_filter( file );


  if( fv_file_path != "" ) file =  get_fv_file_path();
  else                     file =  utils::get_base_dir();
  if( fv_file_name != "" ) file += get_fv_file_name();
  else                     file += "featureplan";
  set_fv_file( file );
  
  
  // set default directoryies for simulations, if none were provided
  string simDataDir = utils::pathify( utils::get_base_dir() + "test/" );

  if( get_simulate() == true ) {

    if( get_simulate_dir() == "" ) {

      if( get_final_feature_format() == "WRAPPED" )    simDataDir += "data_fv/";
      else if( get_final_feature_format() == "FILES" ) simDataDir += "data_yaafe/";
  
      set_simulate_dir( simDataDir ); 
    }
  }
  

  if(debug) { 
    cout<<cn<<mn<<" Finished reading config. REPORT:"<<endl;
    this->print();
  }
  
  infile.close(); //cleanup

  return;
}


void config_handler::print() {
  //
  // Display internal settings set from reading steering
  //
  string mn = "print:";
  string def = "<default>";

  int w1=30; //arbitrary spacing size that makes formatting look pretty for data names
  int w2=30; //arbitrary spacing size that makes formatting look pretty for data options

  
  cout<<cn<<mn<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w1)<<"DEBUG: "                       <<setw(w2)<<(debug? "ON":"OFF")
      <<"\n"<<setw(w1)<<"RUN IN BACKGROUND: "           <<setw(w2)<<(get_background()? "ON":"OFF")
      <<"\n"<<setw(w1)<<"SIMULATION: "                  <<setw(w2)<<(get_simulate()? "ON":"OFF")
      <<"\n"<<setw(w1)<<"FILTER FIRST: "                <<setw(w2)<<(get_filter()? "ON":"OFF")
      <<"\n"<<setw(w1)<<"SAVE RECORDINGS: "             <<setw(w2)<<(get_save_rec()? "ON":"OFF")
      <<"\n"<<setw(w1)<<"HUMAN READABLE OUTPUT: "       <<setw(w2)<<(get_output_formatted()? "ON":"OFF")
      <<"\n"<<setw(w1)<<"Output Type ID: "              <<setw(w2)<<"\""<<get_output_type_id()<<"\""
      <<"\n"<<setw(w1)<<"RPi Latitude: "                <<setw(w2)<<"\""<<get_latitude()<<"\""
      <<"\n"<<setw(w1)<<"RPi Longitude: "               <<setw(w2)<<"\""<<get_longitude()<<"\""
      <<"\n"<<setw(w1)<<"RPi ID: "                      <<setw(w2)<<"\""<<get_rpid()<<"\""
      <<"\n"<<setw(w1)<<"Simulation Data Dir: "         <<setw(w2)<<"\""<<get_simulate_dir()<<"\""
      <<"\n"<<setw(w1)<<"Config Path: "                 <<setw(w2)<<"\""<<get_config_file_path()<<"\""
      <<"\n"<<setw(w1)<<"Config Name: "                 <<setw(w2)<<"\""<<get_config_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Config File: "                 <<setw(w2)<<"\""<<get_config_file()<<"\""
      <<"\n"<<setw(w1)<<"Final Feature Format: "        <<setw(w2)<<"\""<<get_final_feature_format()<<"\""
      <<"\n"<<setw(w1)<<"Filter Vector Path: "          <<setw(w2)<<"\""<<get_fv_filter_path()<<"\""
      <<"\n"<<setw(w1)<<"Filter Vector Name: "          <<setw(w2)<<"\""<<get_fv_filter_name()<<"\""
      <<"\n"<<setw(w1)<<"Filter Vector File: "          <<setw(w2)<<"\""<<get_fv_filter()<<"\""
      <<"\n"<<setw(w1)<<"Feature Vector Path: "         <<setw(w2)<<"\""<<get_fv_file_path()<<"\""
      <<"\n"<<setw(w1)<<"Feature Vector Name: "         <<setw(w2)<<"\""<<get_fv_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Feature Vector File: "         <<setw(w2)<<"\""<<get_fv_file()<<"\""
      <<"\n"<<setw(w1)<<"Analysis File(s) Path: "       <<setw(w2)<<"\""<<get_analysis_location()<<"\""
      <<"\n"<<setw(w1)<<"Data File(s) Path: "           <<setw(w2)<<"\""<<get_data_location()<<"\""
      <<"\n"<<setw(w1)<<"Media Location File Path: "    <<setw(w2)<<"\""<<get_media_location()<<"\""
      <<"\n"<<setw(w1)<<"Recording Extention: "         <<setw(w2)<<"\""<<get_rec_extention()<<"\""
      <<"\n"<<setw(w1)<<"Recording File Name Prefix: "  <<setw(w2)<<"\""<<get_rec_file_name_prefix()<<"\""
      <<"\n"<<setw(w1)<<"Recording Rate: "              <<setw(w2)<<"\""<<get_samp_rate()<<"\""
      <<"\n"<<setw(w1)<<"Recording Duration: "          <<setw(w2)<<"\""<<get_rec_duration()<<"\""
      <<"\n"<<setw(w1)<<"Recording Number: "            <<setw(w2)<<"\""<<get_rec_number()<<"\""<<endl;
  cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  
  return;
}



void config_handler::split_string(std::string str, std::vector<std::string>& split_results, std::string delimiters) {
  //
  // Helper to split strings by a string delimter 
  //

  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      split_results.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }

  return;
}


bool config_handler::file_exists(const string& s) {
  //
  // Helper to make sure a file exists
  //

  if ( FILE* testfile=fopen(s.c_str(),"r") ) {
    fclose(testfile);
    return true;
  }
  else return false;
}


std::string config_handler::trim(std::string s) {
  // 
  // Helper to remove leading and trailing white space from steering file reading
  //  credit to: http://www.toptip.ca/2010/03/trim-leading-or-trailing-white-spaces.html
  //
  std::string reducedS = s;

  size_t p = reducedS.find_first_not_of(" \t");
  reducedS.erase(0, p);
  
  p = reducedS.find_last_not_of(" \t");
  if (string::npos != p) reducedS.erase(p+1);

  return reducedS;
}



//*****************
//*   ACCESSORS   *
//*****************

string config_handler::get_config_file_path() { return utils::pathify(config_file_path); }
string config_handler::get_config_file_name() { return config_file_name; }
string config_handler::get_config_file() { 

  if( config_file == "" ) {
    if( get_config_file_path() != "" ) config_file += get_config_file_path();
    else                               config_file += utils::get_base_dir();
    
    if( get_config_file_name() != "" ) config_file += get_config_file_name();
    else                               config_file += "config"; // TODO - make default
  }    
  
  return config_file; 
}
string config_handler::get_fv_filter_path() { return utils::pathify(fv_filter_path); }
string config_handler::get_fv_filter_name() { return fv_filter_name; }
string config_handler::get_fv_filter() { 

  if( fv_filter == "" ) {
    if( get_fv_filter_path() != "" ) fv_filter += get_fv_filter_path();
    else                             fv_filter += utils::get_base_dir();
    
    if( get_fv_filter_name() != "" ) fv_filter += get_fv_filter_name();
    else                             fv_filter += "featureplan_filter"; // TODO - make default
  }

  return fv_filter; 
}
string config_handler::get_fv_file_path() { return utils::pathify(fv_file_path); }
string config_handler::get_fv_file_name() { return fv_file_name; }
string config_handler::get_fv_file() { 

  if( fv_file == "" ) {
    if( get_fv_file_path() != "" ) fv_file += get_fv_file_path();
    else                           fv_file += utils::get_base_dir();
    
    if( get_fv_file_name() != "" ) fv_file += get_fv_file_name();
    else                           fv_file += "featureplan"; // TODO - make default
  }

  return fv_file; 
}
string config_handler::get_rec_file_name_prefix() { return rec_file_name_prefix; }
string config_handler::get_media_location() { return media_location; };
string config_handler::get_rec_extention() { return rec_extention; };
string config_handler::get_data_location() { return data_location; };
string config_handler::get_analysis_location() { return analysis_location; };
int config_handler::get_samp_rate() { return samp_rate; }
int config_handler::get_rec_number() { return rec_num; }
int config_handler::get_rec_duration() { return rec_dur; }
bool config_handler::get_background() { return background; };
bool config_handler::get_simulate() { return simulate; };
bool config_handler::get_output_formatted() { return output_formatted; };
bool config_handler::get_filter() { return filter; };
bool config_handler::get_analysis() { return analysis; };
bool config_handler::get_save_rec() { return save_rec; };
string config_handler::get_simulate_dir() { return utils::pathify(simulate_dir); };
string config_handler::get_latitude() { return latitude; };
string config_handler::get_longitude() { return longitude; };
string config_handler::get_rpid() { return rpid; };
string config_handler::get_final_feature_format() { return final_feature_format; };
int config_handler::get_output_type_id() { return output_type_id; };



//*****************
//*   MUTATORS    *
//*****************
bool config_handler::set_config_file_name( string fname ) {
  config_file_name = fname;
  return true; // TODO - impliment
}


bool config_handler::set_config_file_path( string fpath ) {
  config_file_path = utils::pathify(fpath);
  return true; // TODO - impliment
}


bool config_handler::set_config_file( string configfile ) {
  config_file = configfile;
  return true; // TODO - impliment
}


bool config_handler::set_fv_filter_path( string fvpath ) {
  fv_filter_path = utils::pathify(fvpath);
  return true; // TODO - impliment
}


bool config_handler::set_fv_filter_name( string fvname ) {
  fv_filter_name = fvname;
  return true; // TODO - impliment
}


bool config_handler::set_fv_filter( string fvfilter ) {
  fv_filter = fvfilter;
  return true; // TODO - impliment
}


bool config_handler::set_fv_file_path( string fvpath ) {
  fv_file_path = utils::pathify(fvpath);
  return true; // TODO - impliment
}


bool config_handler::set_fv_file_name( string fvname ) {
  fv_file_name = fvname;
  return true; // TODO - impliment
}


bool config_handler::set_fv_file( string fvfile ) {
  fv_file = fvfile;
  return true; // TODO - impliment
}


bool config_handler::set_rec_file_name_prefix( string prefix ) {
  rec_file_name_prefix = prefix;
  return true; // TODO - impliment
}


bool config_handler::set_media_location( string loc ) {
  media_location = utils::pathify(loc);
  return true; // TODO - impliment
}


bool config_handler::set_rec_extention( string ext ) {
  rec_extention = ext;
  return true; // TODO - impliment
}


bool config_handler::set_data_location( string loc ) {
  data_location = utils::pathify(loc);
  return true; // TODO - impliment
}


bool config_handler::set_analysis_location( string loc ) {
  analysis_location = utils::pathify(loc);
  return true; // TODO - impliment
}


bool config_handler::set_samp_rate( int samprate ) {
  samp_rate = samprate;
  return true; // TODO - impliment
}


bool config_handler::set_rec_duration( int recdur ) {
  rec_dur = recdur;
  return true; // TODO - impliment
}


bool config_handler::set_rec_number( int recnum ) {
  rec_num = recnum;
  return true; // TODO - impliment
}


bool config_handler::set_background( bool status ) {
  background = status;
  return true; // TODO - impliment
}


bool config_handler::set_simulate( bool status ) {
  simulate = status;
  return true; // TODO - impliment
}


bool config_handler::set_output_formatted( bool status ) {
  output_formatted = status;
  return true; // TODO - impliment
}


bool config_handler::set_filter( bool status ) {
  filter = status;
  return true; // TODO - impliment
}


bool config_handler::set_save_rec( bool status ) {
  save_rec = status;
  return true; // TODO - impliment
}


bool config_handler::set_analysis( bool status ) {
  analysis = status;
  return true; // TODO - impliment
}


bool config_handler::set_simulate_dir( string dir ) {
  simulate_dir = dir;
  return true; // TODO - impliment
}


bool config_handler::set_latitude( string lat ) {
  latitude = lat;
  return true; // TODO - impliment
}


bool config_handler::set_longitude( string lon ) {
  longitude = lon;
  return true; // TODO - impliment
}


bool config_handler::set_rpid( string id ) {
  rpid = id;
  return true; // TODO - impliment
}


bool config_handler::set_final_feature_format( string feature ) {
  final_feature_format = feature;
  return true; // TODO - impliment
}


bool config_handler::set_output_type_id( int outputid ) {
  output_type_id = outputid;
  return true; // TODO - impliment
}

