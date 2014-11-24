/*
 * Title:    json_generator
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  25-OCT-2014
 * Edited:   25-OCT-2014
 * Notes:    Handles dumb boarderline-hardcoded creation of a json formatted file
 */

#ifndef JSON_GENERATOR_H
#define JSON_GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;



class json_generator {
 private:

  bool db;
  string cn; //class name
  
  string eol;
  string eoe;
  string sep;
  string ind;
  string aro;
  string arc;
  string spa;
  char els;
  string objo;
  string objc;
  std::vector<string > contents;
  std::vector<string > organisation;
 
  string pos( int pos );
  void init(); 
  
 
 public:

  //constructor
  json_generator( string fname );
  void print();

  // business
  bool write_to( string fname, string fpath, bool formatted );
  //template <typename T> bool add_key( T key );
  //template <typename T> bool add_value( T key );
  //template <typename T, typename Y> bool add( T key, Y value );
  
  void add_pair( string key, string value );
  void add_pair( string key, long int value );
  void add_pair( string key, int value );
  void add_pair( string key, double value );
  void add_pair( string key, std::vector<string> value );  
  void add_pair( string key, std::vector<int> value );  
  void add_pair( string key, std::vector<double> value );  


  bool add_key( string key );
  bool add_value( string key );
  bool add_value( double key );
  bool add_value( int key );
  bool add_value( long int key );
  bool add_value( std::vector<string > values );
  bool add_value( std::vector<int > values );
  bool add_value( std::vector<double > values );
  bool add_array_value( string key );
  bool add_array_value( std::vector<string > values );
  bool add_array_value( std::vector<int > values );
  bool add_array_value( std::vector<double > values );
  bool add( string key, string value );
  bool newline( );
  bool open_object();
  bool close_object();
  bool open_array();
  bool close_array();
  bool format( std::vector<string > *data );


  // mutators
  bool set_contents( std::vector<string > newcontents );
  bool debug_statements( bool status );
  bool debug_on( );
  bool debug_off( );
  bool clear_contents();

  // accessors
  std::vector<string > get_contents();
  string get_contents_string( bool formatted );
  std::vector<string > get_organisation();
  string get_organisation_string();
  bool get_debug();

};


#endif
