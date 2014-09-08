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
#include <string>
#include <vector>
#include <iomanip>  // std::setw
#include <sstream>
#include <fstream>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>


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
    static T string_to_number ( const std::string &Text )
    {
      istringstream ss(Text);
      T result;
      return ss >> result ? result : 0;
    };


  //CITE: num->str from http://www.cplusplus.com/articles/D9j2Nwbp/
  template <typename T> 
    static string number_to_string ( T Number )
    {
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
  
};

#endif
