/*
 * Title:    utils
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  5-SEP-2014
 * Edited:   6-SEP-2014
 * Notes:    Contains various generic helper functions needed in 
 *            multiple places. Also houses all includes.
 */


#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>  // std::setw
#include <sstream>
#include <fstream>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>


//#include "config_handler.h"
//#include "audio_recorder.h"

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif


using namespace std; 



class utils {

 private:
  //nothing here, this is a utility class
  
 public:


  //CITE: str->num from http://www.cplusplus.com/articles/D9j2Nwbp/
  template <typename T>
    static T string_to_number ( const std::string &Text ) {
      istringstream ss(Text);
      T result;
      return ss >> result ? result : 0;
    };


  //CITE: num->str from http://www.cplusplus.com/articles/D9j2Nwbp/
  template <typename T> 
    static string number_to_string ( T Number ) {
      ostringstream ss;
      ss << Number;
      return ss.str();
    };



  static void error_msg( const string err ) {
    ofstream myfile;
    string errorFileName = "logs/error.log";

    myfile.open ( errorFileName.c_str() );
    myfile << "raraa encountered the following error: " << endl;
    myfile << err <<endl;
    myfile.close();

    cout<<" An error log \""<<errorFileName<<"\"has been generated"<<endl;
  };



  static string make_time_stamp() {
    
    string mn = " utils::make_time_stamp:";
    //cout<<mn<<" Making time stamp ... "<<endl;
    
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
    
    
    //cout<<mn<<" Time Stamp is \""<<timeStmp.str()<<"\""<<endl;
    
    return timeStmp.str();
  }
  

  static string get_mac_address() {
    
    string mn = " utils::get_mac_address:";
    struct addrinfo *result;
    struct addrinfo *res;
    int error;
    
    // resolve the domain name into a list of addresses 
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
    
    // loop over all returned results and do inverse lookup 
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
    
    //cout<<mn<<" Found address: " << macAddr << endl;
    
    freeaddrinfo(result);
    
    
    return macAddr;
  };



  static string pathify( string oldPath ) {
    
    string mn = "pathify:";
    string path = oldPath;

    if( path.back() != '/' && 
	path.empty() == false && 
	path.find_first_not_of(' ') != std::string::npos ) {
      path += "/";
    }

    return path;
  };


  // CITE: Daemon creation process code borrowed and edited from:
  // http://www.thegeekstuff.com/2012/02/c-daemon-process/
  static int daemonize() {

    string mn = "daemonize:";
    
    pid_t pid = 0;
    pid_t sid = 0;
    
    // Create child process
    pid = fork();
    
    // Indication of fork() failure
    if ( pid < 0 ) {
      cout<<mn<<" fork failed! pid = \""<<pid<<"\""<<endl;
      
      // Return failure in exit status
      exit(1);
    }
    
    // PARENT PROCESS. Need to kill it be become free.
    if ( pid > 0 ) {
      cout<<mn<<" pid of child is \""<<pid<<"\""<<endl;
      
      // return success in exit status
      exit(0);
    }
    
    //unmask the file mode
    umask(0);
    
    
    //set new session
    sid = setsid();
    if( sid < 0 ) {
      // Return failure
      exit(1);
    }
    
    // Change the current working directory to root.
    chdir("/"); //TODO - make this the raraa working directory based on config
    
    
    // Close stdin. stdout and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    
    return 0;
  };
  

  //Retrieving the current working directory borrowed from
  // http://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
  static string get_cwd() {
    
    string mn = "get_cwd:";
    char cCurrentPath[FILENAME_MAX];
    
    if ( !getcwd( cCurrentPath, sizeof(cCurrentPath) ) ) {
      cerr<<mn<<" WARN: Current working directory not found! Assuming home directory!"<<endl;
      return ""; //TODO - this error can be handled better? 
    }
    
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
    string cwd = string(cCurrentPath);


    //cout<<mn<<" Current working directory is \""<<cwd<<"\""<<endl;
  
    return pathify(cwd);
  };


  static string get_base_dir() {

    string mn = "get_base_dir:";
    string bd = "";
    
    char* bdChar;
    bdChar = getenv( "SOUND_BASE_DIR" );
    if ( bdChar != NULL ) {
      bd = string(bdChar);
      //cout<<mn<<" Base dir env found: \""<<bd<<"\"."<<endl;
    } else {
      bd = get_cwd();
      cerr<<mn<<" WARN: Base dir env NOT found, assuming cwd: \""<<bd<<"\"."<<endl;
    }


    return pathify(bd);
  }
  

  /*
  static bool write_meta_data( config_handler ch, audio_recorder ar ) {

    string mn = "write_meta_data:";

    string lat            = ch.get_latitude();
    string lon            = ch.get_longitude();
    string rpid           = ch.get_rpid();
    string analysisPath   = ch.get_analysis_location();
    string analysisPrefix = ch.get_rec_file_name_prefix(); 
    string macAddr        = utils::get_mac_address(); 
    string audioRecName   = ar.get_rec_file_name();
    
    
    cout<<mn<<" Generating meta data file (child pid \""<<(long)getpid()<<"\") ..."<<endl; 
    
    // create appropreate meta data file 
    stringstream outRecMetaFileName;
    outRecMetaFileName << analysisPath
		       << analysisPrefix
		       << timeStamp
		       << ".mdat";
    string metaFileName = outRecMetaFileName.str();
    
    
    // Fill with recording data
    ofstream recMetaData;
    recMetaData.open( metaFileName.c_str() );
    
    if ( recMetaData.is_open() == false ) {
         cerr<<mn<<" ERROR: Could not open \""<<metaFileName<<"\"!"<<endl;
         res = false;
    }
    if ( recMetaData.bad() ) {
         cerr<<mn<<" ERROR: Could not save to \""<<metaFileName<<"\"!"<<endl;
	 res = false;
    }

    if( res == true ) {
      recMetaData << "REC:  " << audioRecName << "\n"
		  << "TIME: " << timeStamp    << "\n"		
		  << "RPid: " << rpid         << "\n"
		  << "LAT:  " << lat          << "\n"
		  << "LON:  " << lon          << "\n"
		  << "MAC:  " << macAddr      << endl;
    
      //Alert user to creation of meta data file
      cout<<mn<<" Created meta data file \""<<outRecMetaFileName.str().c_str()
	  <<" (child pid \""<<(long)getpid()<<"\")"<<endl;
    }
    

    recMetaData.close();
        
    return res;
  };
  */












};

#endif
