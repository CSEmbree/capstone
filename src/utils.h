/*
 * Title:    utils
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  5-SEP-2013
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
#include <sstream>
#include <fstream>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>




using namespace std; 


class utils {

 private:
  //nothing here, this is a utility class

 public:
  //CITE: Str->Num, num->str from http://www.cplusplus.com/articles/D9j2Nwbp/
  template <typename T>
    static T string_to_number ( const string &Text )
    {
      istringstream ss(Text);
      T result;
      return ss >> result ? result : 0;
    };
  
  template <typename T> 
    static string number_to_string ( T Number )
    {
      ostringstream ss;
      ss << Number;
      return ss.str();
    };
  
};

#endif
