

#include "config_handler.h"

/******************************************************************
 ** Method Implementations
 ******************************************************************/

string cn = " config_handler::";

config_handler::config_handler( string fname )
{

  string mn = "config_handler:";
  cout<<cn<<mn<<" Constructor started for file \""<<fname<<"\"."<<endl;

  debug = true;
  init();

  set_config_file_name( fname );
  read_config( get_config_file_name()+"/config" );

  cout<<cn<<mn<<" Constructor finished for file \""<<fname<<"\"."<<endl;
}


void config_handler::init() {

  cn = " config_handler::";
  set_config_file_name    ( "" ); 
  set_fv_path             ( "" ); 
  set_rec_file_name_prefix( "" );
  
  set_samp_rate    ( 0 ); 
  set_rec_number   ( 0 ); 
  set_rec_duration ( 0 ); 

  set_background( true );
  
  return;
}

void config_handler::read_config( string fname ) {
  //
  // read steering from file fname
  //
  string mn = "read_config:"; //Method name, for printing


  cout<<cn<<mn<<" Searching for config file \""<<fname<<"\"..."<<endl;
  
  //ensure config provided is found and readable
  ifstream infile(fname.c_str(), ios::in);
  if(!infile){ // Check open
    cerr<<cn<<mn<<" ERROR: Can't open \""<<fname<<"\""<<endl;
    infile.close();
    exit(1);
  } else {
    this->set_config_file_name( fname );
    if (debug) cout<<cn<<mn<<" Reading config file \""<<fname<<"\""<<endl;
  }

  
  // prepare to read
  string curLine;
  string optionName;
  string optionValue;
  int w = 20; //arbitrary width units that make formatting nice


  // read all options from text file
  while (infile.good()) {
    getline(infile, curLine);

    
    curLine = trim(curLine); //remove leading and trailing white space
    int optionSep = curLine.find(' '); //optioname ends after first space


    // retrieve the option name and it's value seperatly for further parsing
    optionName  = curLine.substr(0, optionSep);
    optionValue = trim(curLine.substr(optionSep+1, curLine.size())); //'optionValue' could be broken up further if needed


    
    if (debug) {
      cout<<cn<<mn<<" Read in:>>>>>>>>>>>"
	  <<"\n"<<setw(w)<<"currentLine: \""<<curLine<<"\""
	  <<"\n"<<setw(w)<<" optionName: \""<<optionName<<"\""
	  <<"\n"<<setw(w)<<"optionValue: \""<<optionValue<<"\""
	  <<"\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
    }
    
    
    
    if(curLine[0] != '#' && !curLine.empty() ) { //ignore lines beginning with comments - could be done better

      if ( optionName == "debug" ) {
	debug=true;
	cout<<cn<<mn<<" Debug turned on!"<<endl;
      } else if ( optionName == "REC_DUR" ) {
	set_rec_duration( utils::string_to_number<int>( optionValue ) );
      } else if ( optionName == "REC_NUM" ) {
	set_rec_number( utils::string_to_number<int>( optionValue ) );
      } else if ( optionName == "REC_PREFIX" ) {
	set_rec_file_name_prefix( optionValue );
      } else if ( optionName == "SAMP_RATE" ) {
	set_samp_rate( utils::string_to_number<int>( optionValue ) );
      } else if ( optionName == "FV_PATH" ) {
	set_fv_path( optionValue );
      } else if ( optionName == "REC_EXT" ) {
	set_rec_extention( optionValue );
      } else if ( optionName == "REC_LOC" ) {
	set_rec_location( optionValue );
      } else if ( optionName == "BACKGROUND" ) {
	if( optionValue == "ON"  ) set_background( true  );
	if( optionValue == "OFF" ) set_background( false );
      } else {
	cerr<<cn<<mn<<" WARN: Invalid config option found '"<<curLine<<"'"<<endl;
	//exit(0); //stop if an invalid option found, it's probably an error!
      }

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
      <<"\n"<<setw(w1)<<"Config File: "                 <<setw(w2)<<"\""<<get_config_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Feature Vector Path: "         <<setw(w2)<<"\""<<get_fv_path()<<"\""
      <<"\n"<<setw(w1)<<"Recording Location File Path: "<<setw(w2)<<"\""<<get_rec_location()<<"\""
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

string config_handler::get_config_file_name() { return config_file_name; }
string config_handler::get_fv_path() { return fv_path; }
string config_handler::get_rec_file_name_prefix() { return rec_file_name_prefix; }
string config_handler::get_rec_location() { return rec_location; };
string config_handler::get_rec_extention() { return rec_extention; };
int config_handler::get_samp_rate() { return samp_rate; }
int config_handler::get_rec_number() { return rec_num; }
int config_handler::get_rec_duration() { return rec_dur; }
bool config_handler::get_background() { return background; };


//*****************
//*   MUTATORS    *
//*****************
bool config_handler::set_config_file_name( string fname ) {
  config_file_name = fname;

  return true; // TODO - impliment
}


bool config_handler::set_fv_path( string fvpath ) {
  fv_path = fvpath;
  return true; // TODO - impliment
}


bool config_handler::set_rec_file_name_prefix( string prefix ) {
  rec_file_name_prefix = prefix;
  return true; // TODO - impliment
}


bool config_handler::set_rec_location( string loc ) {
  rec_location = loc;
  return true; // TODO - impliment
}


bool config_handler::set_rec_extention( string ext ) {
  rec_extention = ext;
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


