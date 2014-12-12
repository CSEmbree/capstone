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
#include <cmath>
#include <dirent.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include "json_generator.h"

#include "config_handler.h"
//#include "audio_recorder.h"

#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif

#define DEBUG
#ifdef DEBUG
#define msg(x) std::cout << x
#else
#define msg(x) 
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

  
  static bool out_json( int type, string msg, bool format, string fname="", string path="" ) {
    
    bool res = true;
    string DEFAULT_FNAME = "sound_script_msg";
    string DEFAULT_FPATH = pathify( get_home_dir() );

    int    msg_type   = type;
    string msg_data   = msg;
    bool   format_out = true;
    string file_name  = fname;
    string file_path  = pathify( path );

    // ensure optional arguments are valid
    if ( file_name == "" ) file_name = DEFAULT_FNAME;
    if ( file_path == "" ) file_path = DEFAULT_FPATH;

    string file = file_path + file_name;



    // set up JSON formatted file with user's info
    json_generator jg( file );
    
    jg.open_object();
    
    jg.add_pair( "date", utils::get_current_time() );
    jg.add_pair( "type", msg_type );
    jg.add_pair( "data", msg_data );
    
    jg.close_object();


    
    res = jg.write_to( file_name, file_path, format_out );

    
    return res;
  }


  static bool is_pos_int( const string &line ) {
    bool res = true;

    if (line[0] == '0')  {
      res = true;
    } else  {
      res = atoi( line.c_str() );
    }

    return res;
  }


  static string gen_unique_id() {
    std::srand( std::time( 0 ) ); // use current time as seed for random generator
    int ran_num = std::rand() % 10000;
    
    return number_to_string( ran_num );
  }

  /*  
  static void error_msg( const string err ) {
    ofstream myfile;
    string errorFileName = "logs/error.log";

    myfile.open ( errorFileName.c_str() );
    myfile << "raraa encountered the following error: " << endl;
    myfile << err <<endl;
    myfile.close();

    cout<<" An error log \""<<errorFileName<<"\"has been generated"<<endl;
  };
  */


  static long int get_current_time() {
    
    struct timeval tp;
    gettimeofday( &tp, NULL );
    long int res = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    return res;
  }


  // CITE: http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
  static const string get_current_date_time() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &tstruct);

    return buf;
  }


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


  static bool is_file_readable( string file ) {
    
    string mn = "is_file_readable:";
    bool res = true; // file is readable

    ifstream my_file( file );
    if ( my_file.good() == false ) {
      cout<<mn<<" WARNING: File \""<<file<<"\" could not be opened!"<<endl;
      res = false; // file was not readable
    }

    return res;
  }


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


  static string trim( string s , string pat="") {
    // 
    // Helper to remove leading and trailing white space from steering file reading
    //  credit to: http://www.toptip.ca/2010/03/trim-leading-or-trailing-white-spaces.html
    //
    std::string reducedS = s;
    std::string pattern = " \t"; //default are remove space and tab
    if( pat != "" ) pattern = pat;
    
    //    size_t p = reducedS.find_first_not_of(" \t");
    size_t p = reducedS.find_first_not_of( pattern );
    reducedS.erase(0, p);
    
    //    p = reducedS.find_last_not_of(" \t");
    p = reducedS.find_last_not_of( pattern );
    if (string::npos != p) reducedS.erase(p+1);
    
    return reducedS;
  }
  

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

  
  static string get_home_dir() {
    
    string mn = "get_home_dir:";
    string hd = "";
    
    char* hdChar;

    // Home directory order is CIRAINBOWPATH then HOME then cwd
    hdChar = getenv( "CIRAINBOWPATH" );
    if ( hdChar != NULL ) {
      hd = string(hdChar);
    } else {
      /*      
      hdChar = getenv( "HOME" );

      
      if ( hdChar != NULL ) {
	hd = string(hdChar);
      } else {
	hd = get_cwd();
	cerr<<mn<<" WARN: Home directory env var not found! Assuming cwd: \""<<hd<<"\"."<<endl;
      }
      */

      hd = get_base_dir();
    }    

    //cout<<mn<<" Home env found: \""<<hd<<"\"."<<endl;
    
    return pathify(hd);
  }
  

  static string get_base( string str ) {

    string mn = "get_base";
    string res = "";

    if( str.size() > 0 ) {
      const size_t last_slash_index = str.rfind('/');
      if( std::string::npos != last_slash_index ) {
	
	// get the last directory or file in path
	if( last_slash_index+1 != str.size() ) {
	  // FILE
	  res = str.substr( last_slash_index+1, str.size() );
	} else {
	  // DIRECTORY
	  string reducedS = str.substr( 0, last_slash_index );
	  const size_t second_last_slash_index = reducedS.rfind('/');
	  
	  if( std::string::npos != second_last_slash_index ) {
	    res = reducedS.substr( second_last_slash_index+1, last_slash_index);
	  } else {
	    res = reducedS.substr( 0, reducedS.size());
	  }
	  
	}
      } else {
	res = str;
      }
    } else {
      //cerr<<mn<<" WARNING: Problem getting base of '"<<str<<"'. It  may be malformed?"<<endl;
    }
    
    
    return res;
  }
  

  static string get_directory_path( string str ) {
    string dir = "";
    const size_t last_slash_index = str.rfind('/');

    if ( std::string::npos != last_slash_index ) {
      dir = str.substr(0, last_slash_index);
    }
   

    return dir;
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
