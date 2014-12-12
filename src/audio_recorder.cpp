

#include "audio_recorder.h"
#include "config_handler.h"

 

audio_recorder::audio_recorder( config_handler ch ) {

  init();
  string mn = "audio_recorder:";

  cout<<cn<<mn<<" Constructor starting with config data: "<<endl;
  ch.print();

  cout<<cn<<mn<<" Applying config settings ... "<<endl;
  apply_config_settings( ch );

  cout<<cn<<mn<<" Current recording settings ... "<<endl;
  this->print();

  cout<<cn<<mn<<" Constructor finished. "<<endl;
}


void audio_recorder::init() {

  cn = " audio_recorder::";
  string mn = "init:";
  cout<<cn<<mn<<" Performing init ... "<<endl;


  set_rec_file_name       ( "" );
  set_rec_file_name_base  ( "" );
  set_rec_file_name_core  ( "" );
  set_rec_file_name_prefix( "" );
  set_time_stamp          ( utils::make_time_stamp() );
  set_rec_duration        ( 1 ); //each recording is 1 sec by default

  cout<<cn<<mn<<" init complete."<<endl;

  return;
}


void audio_recorder::apply_config_settings( config_handler ch ) {

  string mn = "apply_config_settings:";
  cout<<cn<<mn<<" Applying configuration settings ..."<<endl;

  set_rec_file_name_prefix( ch.get_rec_file_name_prefix() );
  set_rec_location( ch.get_analysis_location() );
  set_rec_extention( ch.get_rec_extention() );
  set_rec_duration( ch.get_rec_duration() );

  cout<<cn<<mn<<" Applied Configuration settings."<<endl;
}


bool audio_recorder::record( string ts, int dur) {

  string mn = "record:";
  bool res = true;
  cout<<cn<<mn<<" Making a recording ... "<<endl;


  // collect recording details for recording name and system call
  string timeStamp = ts;
  int duration = dur;
  if( timeStamp == "" )  timeStamp = utils::make_time_stamp();
  if( duration == -1 )   duration = get_rec_duration();


  // remember the time stamp used for recording
  res = set_time_stamp( timeStamp );


  // create recording file name from config and env details
  string audioRecName = make_audio_file_name( timeStamp );
  res = set_rec_file_name( audioRecName );


  // create a system command to make the recording
  string recCmd = make_rec_cmd( audioRecName, duration );

      

  //make audio recording
  int ret;
  if( !(ret = system(recCmd.c_str())) ) {
    //SUCCESS - alert user to creation of audio 
    cout<<cn<<mn<<" Finished a recording called \""<<audioRecName<<"\"."<<endl;   
  } else {
    res = false;
  }



  return res;
}


std::string audio_recorder::make_rec_cmd( const string fileName, const string dur ) {
  return make_rec_cmd( fileName, (int) utils::string_to_number<int>(dur) );
}


std::string audio_recorder::make_rec_cmd( const string fileName, const int dur ) {

  string mn = "make_rec_cmd:";
  cout<<cn<<mn<<" Making record system command ... "<<endl;


  stringstream cmdStream;
  
  //"arecord -D plughw:1 --duration=2 -f cd -vv ~/sounds/rec/recTest.wav"; 
  
  //various necessary parameters to make an ARECORD command line call
  string recProg = "arecord -D plughw:1";
  string deviceName= "-D plughw:1";
  string duration = "--duration="; //no duration set by default - will use 2 secs if not overwrittn
  string format = "-f cd";
  string extraOptions = "-vv";
  
  cmdStream << recProg
            << " " << deviceName
            << " " << duration << dur //duration of recording
            << " " << format
            << " " << extraOptions
            << " " << fileName;
  
  string cmd = cmdStream.str();

  cout<<cn<<mn<<" Record system command is \""<<cmd<<"\""<<endl;  


  cout<<cn<<mn<<" Finished record system command."<<endl;
  
  return cmd;
}

std::string audio_recorder::make_audio_file_name_core(string timeStamp, string recPrefix ) {
  
  string mn = "make_audio_file_name_core:";
  cout<<cn<<mn<<" Making record audio file name ... "<<endl;
  

  // Generate file name info if user does not provide
  if(recPrefix == "") recPrefix = get_rec_file_name_prefix();
  cout<<cn<<mn<<" Making audio file name core with: TIMESTAMP:"<<timeStamp<<", PREFIX:"<<recPrefix<<endl;
  


  // make a descriptive/unique recording file name
  stringstream stream;
  stream << recPrefix 
	 << timeStamp;

  string fileNameCore = stream.str();
  set_rec_file_name_core( fileNameCore );


  cout<<cn<<mn<<" Constructed recording name core titled \""<<get_rec_file_name_core()<<"\""<<endl;

  return fileNameCore;
}

std::string audio_recorder::make_audio_file_name_base(string timeStamp, string recPrefix, string dirPath ) {
  
  string mn = "make_audio_file_name:";
  cout<<cn<<mn<<" Making record audio file name ... "<<endl;
  

  // Generate file name info if user does not provide
  if(dirPath == "") dirPath = utils::pathify(get_rec_location());
  string fname_core = make_audio_file_name_core( timeStamp, recPrefix );
  cout<<cn<<mn<<" Making audio file name base with: CORE:"<<fname_core<<", DIR:"<<dirPath<<endl;


  // make a descriptive/unique recording file name
  stringstream stream;
  stream << dirPath 
	 << fname_core;


  string fileNameBase = stream.str();


  cout<<cn<<mn<<" Constructed recording name base titled \""<<fileNameBase<<"\""<<endl;

  return fileNameBase;
}

std::string audio_recorder::make_audio_file_name(string timeStamp, string recPrefix, string dirPath, string fExt) {
  
  string mn = "make_audio_file_name:";
  cout<<cn<<mn<<" Making record audio file name ... "<<endl;

  // Generate file name info if user does not provide
  if(fExt == "") fExt = get_rec_extention();


  set_rec_file_name_base( make_audio_file_name_base( timeStamp, recPrefix, dirPath ) );  
  string fullFileName = get_rec_file_name_base() + fExt;


  cout<<cn<<mn<<" Made recording name titled \""<<fullFileName<<"\""<<endl;

  return fullFileName;
}


void audio_recorder::print() {
  //
  // Display internal settings set from reading steering
  //
  string mn = "print:";
  string def = "<default>";

  int w1=25; //arbitrary spacing size that makes formatting look pretty for data names
  int w2=50; //arbitrary spacing size that makes formatting look pretty for data options


  cout<<cn<<mn<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w1)<<"DEBUG: "                <<setw(w2)<<(debug? "ON":"OFF")
      <<"\n"<<setw(w1)<<"Rec File Name Core: "   <<setw(w2)<<"\""<<get_rec_file_name_core()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Name: "        <<setw(w2)<<"\""<<get_rec_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Name Base: "   <<setw(w2)<<"\""<<get_rec_file_name_base()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Name Prefix: " <<setw(w2)<<"\""<<get_rec_file_name_prefix()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Location: "    <<setw(w2)<<"\""<<get_rec_location()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Extention: "   <<setw(w2)<<"\""<<get_rec_extention()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Duration: "    <<setw(w2)<<"\""<<get_rec_duration()<<"\""
      <<"\n"<<setw(w1)<<"Rec Time Stamp: "       <<setw(w2)<<"\""<<get_time_stamp()<<"\""<<endl;
  cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  
  return;
}



bool audio_recorder::set_time_stamp( string timestamp ) {

  string mn = "set_time_stamp:";
  bool res = false;

  if ( timestamp != "" ) {
    time_stamp = timestamp;
    res = true;
  } else {
    cerr<<cn<<mn<<" WARN: Time stamp could not be set!"<<endl;
    res = true;
  }
  
  return res;
}


bool audio_recorder::set_rec_file_name_prefix( string prefix ) {

  string mn = "set_rec_file_name_prefix:";
  bool res = false;

  if ( prefix != "" ) {
    rec_file_name_prefix = prefix;
    res = true;
  } else {
    cerr<<cn<<mn<<" WARN: Recordings will not have a file name prefix!"<<endl;
    res = true;
  }

  return res;
}


bool audio_recorder::set_rec_file_name_base( string fnamebase ) {

  string mn = "set_rec_file_name_base:";
  bool res = false;

  if( fnamebase != "" ) {
    rec_file_name_base = fnamebase;
    res = true;
  } else {
    string def = "audio";
    cerr<<cn<<mn<<" WARN: A recording must have a file name! Using \""<<def<<"\"."<<endl;
    rec_file_name = def;
  }

  return res;
}


bool audio_recorder::set_rec_file_name_core( string fnamecore ) {

  string mn = "set_rec_file_name_core:";
  bool res = false;

  if( fnamecore != "" ) {
    rec_file_name_core = fnamecore;
    res = true;
  } else {
    cerr<<cn<<mn<<" WARN: Audio file name core set is set to empty!"<<endl;
  }

  return res;
}


bool audio_recorder::set_rec_file_name( string fname ) {

  string mn = "set_rec_file_name:";
  bool res = false;

  if( fname != "" ) {
    rec_file_name = fname;
    res = true;
  } else {
    string def = "audio";
    cerr<<cn<<mn<<" WARN: A recording must havea file name! Using \""<<def<<"\"."<<endl;
    rec_file_name = def;
  }

  return res;
}


bool audio_recorder::set_rec_location( string loc ) {
  
  string mn = "set_rec_location:";
  bool res = false;

  if( loc != "" ) {
    rec_location = utils::pathify(loc);
    res = true;
  } else {
    rec_location = utils::get_cwd();
    cerr<<cn<<mn<<" ERROR: A recording file location not specified! "
	<<"Recordings will be saved to local working directory \""<<rec_location<<"\""<<endl;
  }

  return res;
}


bool audio_recorder::set_rec_extention( string ext ) { 

  string mn = "set_rec_extention:";
  bool res = false;

  if( ext != "" ) {
    rec_extention = ext;
    res = true;
  } else {
    cerr<<cn<<mn<<" ERROR: A recording must have an extention. Using \".wav\"."<<endl;
    rec_extention = ".wav";
  }

  return res;
}


bool audio_recorder::set_rec_duration( int dur ) {
  
  string mn = "set_rec_duration:";
  bool res = false;
  
  if( dur > 0 ) { 
    rec_duration = dur;
    res = true;
  } else {
    cerr<<cn<<mn<<" ERROR: A recording cannot have duration '"<<dur<<"'."
	<<" Duration remains '"<<get_rec_duration()<<"'"<<endl;
    //utils::error_msg();
  }

  return res;
}


string audio_recorder::get_time_stamp() {
  return time_stamp;
}


string audio_recorder::get_rec_file_name_base() {
  return rec_file_name_base;
}


string audio_recorder::get_rec_file_name_core() {
  return rec_file_name_core;
}


string audio_recorder::get_rec_file_name_prefix() {
  return rec_file_name_prefix;
}


string audio_recorder::get_rec_file_name() {
  return rec_file_name;
}


string audio_recorder::get_rec_location() {
  return rec_location;
}


string audio_recorder::get_rec_extention() {
  return rec_extention;
}

int audio_recorder::get_rec_duration() {
  return rec_duration;
}
