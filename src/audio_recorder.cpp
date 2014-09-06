/*
 * AUTHOR: CAMERON EMBREE
 */


#include "utils.h"

#include "audio_recorder.h"
#include "config_handler.h"

 
#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif
 


//using namespace std;

audio_recorder::audio_recorder( config_handler ch ) {

  init();
  string mn = "audio_recorder:";

  cout<<cn<<mn<<" Constructor starting... "<<endl;
  
  ch.print();

  apply_config_settings( ch );

  cout<<cn<<mn<<" Constructor finished. "<<endl;
}

void audio_recorder::init() {

  cn = " audio_recorder::";
  set_rec_file_name       ( "" );
  set_rec_file_name_prefix( "" );
  set_rec_duration        ( 1 ); //each recording is 1 sec by default

  return;
}

void audio_recorder::apply_config_settings( config_handler ch ) {

  string mn = "apply_config_settings:";
  cout<<cn<<mn<<" Applying configuration settings ..."<<endl;

  set_rec_file_name_prefix( ch.get_rec_file_name_prefix() );
  set_rec_location( ch.get_rec_location() );
  set_rec_extention( ch.get_rec_extention() );
  set_rec_duration( ch.get_rec_duration() );

  cout<<cn<<mn<<" Applied Configuration settings."<<endl;
}



void audio_recorder::record( string ts, int dur) {

  string mn = "record:";
  cout<<cn<<mn<<" Making a recording ... "<<endl;


  // collect recording details for recording name and system call
  string timeStamp = ts;
  int duration = dur;
  if( timeStamp == "" )  timeStamp = make_time_stamp();
  if( duration == -1 )   duration = get_rec_duration();


  // create recording name and system command from rec details
  string audioRecName = make_audio_file_name( timeStamp );
  string recCmd = make_rec_cmd( audioRecName, duration );

      
  //make audio recording
  system(recCmd.c_str());
    

  cout<<cn<<mn<<" Finished a recording called \""<<audioRecName<<"\"."<<endl;
}

std::string audio_recorder::make_time_stamp() {

  string mn = "make_time_stamp:";
  cout<<cn<<mn<<" Making time stamp ... "<<endl;

  time_t ltime;
  struct tm *Tm;
 
  ltime=time(NULL);
  Tm=localtime(&ltime);
 
  stringstream timeStmp;
  timeStmp << "D-"  << Tm->tm_mday
	   << "-"   << Tm->tm_mon
	   << "-"   << Tm->tm_year
	   << "_T-" << Tm->tm_sec
	   << "-"   << Tm->tm_min
	   << "-"   << Tm->tm_hour;


  cout<<cn<<mn<<" Time Stamp is \""<<timeStmp.str()<<"\""<<endl;

  return timeStmp.str();
}


//TEMP METHOD - TODO - Could be used as an identifier for Meta data file?
std::string audio_recorder::get_mac_address() {

  string mn = "get_mac_address:";
  cout<<cn<<mn<<" Getting mac address ... "<<endl;

  struct addrinfo *result;
  struct addrinfo *res;
  int error;
 
  /* resolve the domain name into a list of addresses */
  error = getaddrinfo("www.example.com", NULL, NULL, &result);
  if (error != 0) {   
    if (error == EAI_SYSTEM) {
      perror("getaddrinfo");
    }
    else {
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
    }   
    exit(EXIT_FAILURE);
  }   
 
  char hostname[NI_MAXHOST] = "";
 
  /* loop over all returned results and do inverse lookup */
  for (res = result; res != NULL; res = res->ai_next) {   
      error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
      
      if (error != 0) {
	fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
	continue;
      }
  }   
  
  
  
  stringstream mac;
  mac << hostname;
  string macAddr = mac.str();
  
  freeaddrinfo(result);
  
  cout<<cn<<mn<<" Mac address is \""<<macAddr<<"\""<<endl;
  
  return macAddr;
}


std::string audio_recorder::make_rec_cmd( const string fileName, const string dur ) {
  make_rec_cmd( fileName, (int) utils::string_to_number<int>(dur) );
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


std::string audio_recorder::make_audio_file_name(string timeStamp, string recPrefix, string dirPath, string fExt) {
  
  string mn = "make_audio_file_name:";
  cout<<cn<<mn<<" Making record audio file name ... "<<endl;
  

  // Generate file name info if user does not provide
  if(recPrefix == "") recPrefix = get_rec_file_name_prefix();
  if(dirPath == "")   dirPath = get_rec_location();
  if(fExt == "")      fExt = get_rec_extention();



  // make a descriptive/unique recording file name
  stringstream stream;
  stream << dirPath 
	 << recPrefix 
	 << timeStamp 
	 << fExt;

  string fullFileName = stream.str();


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
      <<"\n"<<setw(w1)<<"Rec File Name: "        <<setw(w2)<<"\""<<get_rec_file_name()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Name Prefix: " <<setw(w2)<<"\""<<get_rec_file_name_prefix()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Location: "    <<setw(w2)<<"\""<<get_rec_location()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Extention: "   <<setw(w2)<<"\""<<get_rec_extention()<<"\""
      <<"\n"<<setw(w1)<<"Rec File Duration: "    <<setw(w2)<<"\""<<get_rec_duration()<<"\""<<endl;
  cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  
  return;
}



bool audio_recorder::set_rec_file_name_prefix( string recfilenameprefix ) {
  
  rec_file_name_prefix = recfilenameprefix;
 
  return true;
}

bool audio_recorder::set_rec_file_name( string recfilename ) {
  
  rec_file_name = recfilename;
 
  return true;
}

bool audio_recorder::set_rec_location( string loc ) {
  
  rec_location = loc;
 
  return true;
}

bool audio_recorder::set_rec_extention( string ext ) {
  
  rec_extention = ext;
 
  return true;
}

bool audio_recorder::set_rec_duration( int dur ) {

  rec_duration = dur;

  return true;
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
