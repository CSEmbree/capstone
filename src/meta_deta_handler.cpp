

#include "meta_data_handler.h"

/******************************************************************
 ** Method Implementations
 ******************************************************************/

string cn = " meta_data_handler::";

meta_data_handler::meta_data_handler( string fname = "" )
{
  string mn = "meta_data_handler:"; 
  cout<<cn<<mn<<" Constructor started for file \""<<fname<<"\"."<<endl;

  debug = true;
  init();

  cout<<cn<<mn<<" Constructor finished for file \""<<fname<<"\"."<<endl;
}


void meta_data_handler::init() {

  string mn = "init:";
  meta_data_file_info.clear();
  set_meta_data_file_name( "meta_data" );

  return;
}

bool meta_data_handler::clear() {
  
  string mn = "clear:";
  cout<<cn<<mn<<" Returning to init state."<<endl;
  init();

  return true; //TODO - impliment
}


bool meta_data_handler::write( string fname = "", string fpath = "" ) {

  string mn = "write:";
  ofstream recMetaData;

  if( fpath != "" ) set_meta_data_file_path( fpath );
  if( fname != "" ) set_meta_data_file_name( fname );
  string metaFilePath = get_meta_data_file_path();
  string metaFileName = get_meta_data_file_name();
  string metaFile = metaFilePath + metaFileName;

  cout<<cn<<mn<<" Creating meta data file \""<<metaFile<<"\"..."<<endl;


  recMetaData.open( metaFile.c_str() );
  
  for( int i = 0; i < meta_data_file_info.size(); i++) {
    recMetaData << meta_data_file_info.at(i);
  }
  
  recMetaData.close();  


  cout<<cn<<mn<<" Created meta data file \""<<metaFile<<"\"."<<endl

    return true; //TODO - impliment
}


void meta_data_handler::print() {
  //
  // Display internal settings set from reading steering
  //

  return;
}


//*****************
//*   ACCESSORS   *
//*****************

vector<string > meta_data_handler::get_meta_data_file_info() { return meta_data_file_info; };
string meta_data_handler::get_meta_data_file_path() { return meta_data_file_path; };
string meta_data_handler::get_meta_data_file_name() { return meta_data_file_info; };
string meta_data_handler::get_meta_data_file() { 

  string metaDataFile = "";

  if( meta_data_file == "" ) {

    if( get_meta_data_file_path() != "" ) meta_data_file += get_meta_data_file_path();
    else                                  meta_data_file += utils::get_base_dir()+"rec/fv_raw/";
    
    if( get_meta_data_file_name() != "" ) meta_data_file += get_meta_data_file_name();
    else                                  meta_data_file += "meta_data"; // TODO - make default

    set_meta_data_file( metaDataFile );
  }

  return metaDataFile;
};


//*****************
//*   MUTATORS    *
//*****************
bool meta_data_handler::add_data( string data ) {
  meta_data_file_info.push_back( data );
  return true; // TODO - impliment
}

bool meta_data_handler::set_meta_data_file_info( vector<string> *metadata ) {
  meta_data_file_info = (*meta_data);
  return tru; //TODO - impliment
}

bool meta_data_handler::set_meta_data_file_path( string fpath ) {
  meta_data_file_path = utils::pathify(fname);
  return true; // TODO - impliment
}

bool meta_data_handler::set_meta_data_file_name( string fname ) {
  meta_data_file_name = fname;
  return true; // TODO - impliment
}

bool meta_data_handler::set_meta_data_file( string pathwithname ) {
  meta_data_file = pathwithname;
  return true; // TODO - impliment
}
