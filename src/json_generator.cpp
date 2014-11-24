

#include "json_generator.h"



/******************************************************************
 ** Method Implementations
 ******************************************************************/


json_generator::json_generator( string fname ) {
  string mn = "json_generator:"; 
  
  init();  
  
  if(db) cout<<cn<<mn<<" Constructor finished. "<<endl;
}




void json_generator::init() {
  
  cn = " json_generator::";


  eoe = ",";  // end of element char
  eol = "\n"; // end of line char
  sep = ": "; // seperator char
  objo = "{"; // object open char
  objc = "}"; // object close char
  spa = " ";  //space char
  aro = "[";  // array open char
  arc = "]";  //array close char
  spa = " ";   // indent amount, updates as things are added
  els = '"';
    
  debug_statements( false );
  contents.clear();
  organisation.clear();

  
  return;
}




void json_generator::add_pair( string key, string value ) {
  add_key( key );
  add_value( value );
}

void json_generator::add_pair( string key, long int value ) {
  add_key( key );
  add_value( value );
}

void json_generator::add_pair( string key, int value ) {
  add_key( key );
  add_value( value );
}

void json_generator::add_pair( string key, double value ) {
  add_key( key );
  add_value( value );
}

void json_generator::add_pair( string key, std::vector<string> value ) {
  add_key( key );
  add_array_value( value );
}

void json_generator::add_pair( string key, std::vector<int> value ) {
  add_key( key );
  add_array_value( value );
}

void json_generator::add_pair( string key, std::vector<double> value ) {
  add_key( key );
  add_array_value( value );
}



//template <typename T> bool json_generator::add_key( T key ) {
bool json_generator::add_key( string key ) {

  string mn = "add_key:";
  bool res = true;


  // add a End Of Line char above us if we are not the first element
  if( contents.size() > 0 ) {

    //except when the key we add is the first element of an object
    if( contents.at( contents.size()-1 ).back() != objo.back() ) { 
      contents.at( contents.size()-1 ) = contents.at( contents.size()-1 ) + eoe;
    }
  }
  
  std::ostringstream oss;
  oss << els << key << els << sep;

  contents.push_back( oss.str() );
  organisation.push_back( pos(contents.size()-1) + mn );


  if(db) cout<<cn<<mn<<" Added key: \""<<key<<"\""<<endl;

  return res;
}


//template <typename T> bool json_generator::add_value( T value ) {
bool json_generator::add_value( string value ) {

  string mn = "add_value:";
  bool res = true;


  std::ostringstream oss;
  oss << els << value << els;

  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  if(db) cout<<cn<<mn<<" Added value: \""<<value<<"\""<<endl;

  return res;
}


bool json_generator::add_value( int value ) {
  bool res = true;

  long int val = value;
  add_value( val );

  return res;
}


bool json_generator::add_value( long int value ) {
  bool res = true;

  double val = value;
  add_value( val );

  return res;
}


bool json_generator::add_value( double value ) {

  string mn = "add_value:";
  bool res = true;


  std::ostringstream oss;
  oss << value;

  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  if(db) cout<<cn<<mn<<" Added value: \""<<value<<"\""<<endl;

  return res;
}


bool json_generator::add_array_value( std::vector<string > values ) {

  string mn = "add_value:";
  bool res = true;

  open_array(); // open array for multile values


  // create single string of array values
  std::ostringstream oss;
  unsigned int vi = 0;

  // first n-1 elements are element seperated
  for( vi = 0; vi < values.size()-1; vi++ ) {
    oss << els << values.at(vi) << els << eoe << spa;
  }
  // last n'th element does not need an element seperator
  oss << els << values.at(vi) << els << spa;

  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  close_array(); // close array for multile values

  return res;
}


bool json_generator::add_array_value( std::vector<int > values ) {
  
  string mn = "add_values:";
  bool res = true;

  // simply convert ints to doubles and treat them the same
  std::vector<double > dvalues;
  dvalues.clear();

  for( unsigned int i = 0; i < values.size(); i++ ) {
    dvalues.push_back( values.at(i) );
  }

  res = add_value( dvalues );


  return res;
}


bool json_generator::add_array_value( std::vector<double > values ) {

  string mn = "add_value:";
  bool res = true;

  open_array(); // open array for multile values


  // create single string of array values
  std::ostringstream oss;
  unsigned int vi = 0;

  // first n-1 elements are element seperated
  for( vi = 0; vi < values.size()-1; vi++ ) {
    oss << values.at(vi) << eoe << spa;
  }
  // last n'th element does not need an element seperator
  oss << values.at(vi) << spa;
  
  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  close_array(); // close array for multile values

  return res;
}


bool json_generator::add_value( std::vector<string > values ) {

  string mn = "add_value:";
  bool res = true;

  // assume array is already open


  // create single string of array values
  std::ostringstream oss;
  unsigned int vi = 0;

  // first n-1 elements are element seperated
  for( vi = 0; vi < values.size()-1; vi++ ) {
    oss << els << values.at(vi) << els << eoe << spa;
  }
  // last n'th element does not need an element seperator
  oss << els << values.at(vi) << els << spa;

  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  // assume array will be closed manually later

  return res;
}


bool json_generator::add_value( std::vector<int > values ) {
  
  string mn = "add_values:";
  bool res = true;

  // simply convert ints to doubles and treat them the same
  std::vector<double > dvalues;
  dvalues.clear();

  for( unsigned int i = 0; i < values.size(); i++ ) {
    dvalues.push_back( values.at(i) );
  }

  res = add_value( dvalues );


  return res;
}


bool json_generator::add_value( std::vector<double > values ) {

  string mn = "add_value:";
  bool res = true;

  // assume array is already open


  // create single string of array values
  std::ostringstream oss;
  unsigned int vi = 0;

  // first n-1 elements are element seperated
  for( vi = 0; vi < values.size()-1; vi++ ) {
    oss << values.at(vi) << eoe << spa;
  }
  // last n'th element does not need an element seperator
  oss << values.at(vi) << spa;
  
  int last_index = contents.size() - 1;
  contents.at(last_index) = contents.back() + oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  // assume array will be closed manually later

  return res;
}


bool json_generator::newline( ) {

  string mn = "newline:";
  bool res = true;


  contents.push_back( eol );
  organisation.push_back( pos(contents.size()-1) + mn );


  return res;
}


bool json_generator::open_object() {

  string mn = "open_object:";
  bool res = true;

  
  if( contents.size() > 0 ) {
    if ( contents.at( contents.size()-1 ).back() == objc.back() ) {
      contents.at( contents.size()-1 ) += eoe;
    }
  }   

  contents.push_back( objo );

  organisation.push_back( pos(contents.size()-1) + mn );


  if(db) cout<<cn<<mn<<" Added: \""<<contents.back()<<"\""<<endl;

  return res;
}


bool json_generator::close_object() {

  string mn = "close_object:";
  bool res = true;


  contents.push_back( objc );
  organisation.push_back( pos(contents.size()-1) + mn );

  
  if(db) cout<<cn<<mn<<" Added: \""<<contents.back()<<"\""<<endl;

  return res;
}


bool json_generator::open_array() {

  string mn = "open_array:";
  bool res = true;


  std::ostringstream oss;
  oss << contents.back() << aro << spa;

  contents.at( contents.size()-1 ) = oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  return res;
}


bool json_generator::close_array() {

  string mn = "close_array:";
  bool res = true;


  std::ostringstream oss;
  oss << contents.back() << arc;

  contents.at( contents.size()-1 ) = oss.str();
  organisation.push_back( pos(contents.size()-1) + mn );


  return res;
}


bool json_generator::clear_contents() {

  string mn = "clear_contents:";
  bool res = true;


  contents.clear();
  organisation.clear();


  return res;
}


bool json_generator::write_to( string fname, string fpath, bool formatted ) {
 
  string mn = "write_to:";
  bool res = true;


  string filename = "";
  if( fpath != "" ) filename = fpath;
  if( fname != "" ) filename += fname;

  // if a filename is valid, then write formatted contents
  if( filename != "" ) {

    ofstream outfile;
    outfile.open( filename );

    outfile << get_contents_string( formatted );

    outfile.close();

  } else {
    cout<<cn<<mn<<" ERROR: file name \""<<filename<<"\" is invalid!"<<endl;
  }

  
  return res;
}


bool json_generator::format( std::vector<string > *data ) {

  string mn = "format:";
  bool res = true;
  string new_indent = "";
  string indent = "";

  // add an end of line element to each
  for( unsigned int i = 0; i < data->size(); i++ ) {


    if( data->at(i).back() != eol.back() ) {

      // update indent status for new run
      if( new_indent != indent ) {
	indent = new_indent;
      }
      
      // Add future spacing as needed
      if( data->at(i).back() == objo.back() ) {
	new_indent = indent + spa + spa;
      }
      
      // Remove future spacing as neeed
      if( data->at(i).front() == objc.front() ) {	
	// only remove two spaces chars if there are two to remove!
	if( indent.size() >= 2 ) {
	  new_indent = indent.substr( 0, indent.size()-2 );
	  indent = new_indent; //special case
	} else {
	  cout<<cn<<mn<<" ERROR: Synatx error!"
	      <<" Object CLOSING char '"<<objo
	      <<"' is not paired with an Object OPENING char '"<<objc<<"'"<<endl;
	}
      }

      // Format with correct spacing and end of line char
      data->at(i) = indent + data->at(i) + eol;
    }
  }


  return res;
}



void json_generator::print() {
  //
  // Display internal settings set from reading steering
  //
  string mn = "print:";
  string def = "<default>";
  
  int w1=30; //arbitrary spacing size that makes formatting look pretty for data names
  int w2=30; //arbitrary spacing size that makes formatting look pretty for data options
  
  
  cout<<cn<<mn<<" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
      <<"\n"<<setw(w1)<<"DEBUG: "                  <<setw(w2)<<(db? "ON":"OFF")
      <<"\n"<<setw(w1)<<"Object Open Char: "       <<setw(w2)<<"'"<<objo<<"'"
      <<"\n"<<setw(w1)<<"Object Close Char: "      <<setw(w2)<<"'"<<objc<<"'"
      <<"\n"<<setw(w1)<<"Array Open Char: "        <<setw(w2)<<"'"<<aro<<"'"
      <<"\n"<<setw(w1)<<"Array CLose Char: "       <<setw(w2)<<"'"<<arc<<"'"
      <<"\n"<<setw(w1)<<"End of Element Char: "    <<setw(w2)<<"'"<<eoe<<"'"
      <<"\n"<<setw(w1)<<"End of Line Char: "       <<setw(w2)<<"'"<<"\\n"<<"'" //<--- not sure how to fix this yet...
      <<"\n"<<setw(w1)<<"Key/Val Seperator Char: " <<setw(w2)<<"'"<<sep<<"'"
      <<"\n"<<setw(w1)<<"Space Char: "             <<setw(w2)<<"'"<<spa<<"'"

      <<"\n"<<setw(w1)<<"Contents: "               <<setw(w2)<<contents.front()<<endl;
  cout<<cn<<mn<<" <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;            
  

  return;
}


string json_generator::pos( int pos ) {

  string mn = "pos:";
  string open = "[";
  string close = "]"; 

  std::ostringstream oss;
  oss << open << pos << close;

  return oss.str();
}



//*****************
//*   ACCESSORS   *
//*****************


string json_generator::get_contents_string( bool formatted ) {

  string mn = "get_contents_string:";


  // construct one string with correctly formated contents
  std::vector<string > data = contents;
  if( formatted == true ) format( &data );

  std::ostringstream oss;
  for( unsigned int i = 0; i < data.size(); i++ ) {
    oss << data.at(i);
  }


  return oss.str();
}

string json_generator::get_organisation_string() {
  
  string mn = "get_organisation_string:";
 

  // construct a csv of organisation structure
  std::ostringstream oss;
  for( unsigned int i = 0; i < organisation.size(); i++ ) {
    oss << organisation.at(i);
  }
 
  
  return oss.str();
}


std::vector<string > json_generator::get_contents() { return contents; }
std::vector<string > json_generator::get_organisation() { return organisation; }

bool json_generator::get_debug() { return db; }


//*****************
//*   MUTATORS    *
//*****************


bool json_generator::set_contents( std::vector<string > new_contents ) {
  contents.clear();
  contents = new_contents;
  return true; //TODO - impliment
}

bool json_generator::debug_statements( bool status ) {

  db = status;
  return true; //TODO - impliment
}

bool json_generator::debug_on() {
  debug_statements( true );
  return true; //TODO - impliment
}

bool json_generator::debug_off() {
  debug_statements( false );
  return true; //TODO - impliment
}

