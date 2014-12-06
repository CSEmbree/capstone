/*
 * Title:    raraa_exception
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-DEC-2014
 * Edited:   4-DEC-2014
 * Notes:    Handles various exceptions where we need configuration info
 *
 */


#ifndef RARAA_EXCEPTION_H
#define RARAA_EXCEPTION_H

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
#include "audio_recorder.h"




//using namespace std; 


class raraa_exception {
  std::string msg;
  config_handler ch;

 public:
  raraa_exception( string s_, config_handler *ch_ ) { 
    msg = s_;
    ch = ch_;
  };
  
  string get_msg() const { return msg; };
  config_handler* get_config_handler() const { return config_handler; };
};


#endif
