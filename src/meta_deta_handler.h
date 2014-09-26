/*
 * Title:    meta_data_handler
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  16-SEP-2014
 * Edited:   18-SEP-2014
 * Notes:    Handles writing meta data files
 */

#ifndef META_DATA_HANDLER_H
#define META_DATA_HANDLER_H

#include "utils.h"


class meta_data_handler {
 private:

  string meta_data_file_path;
  string meta_data_file_name;
  vector<string > meta_data_file_info;

  void init();  
  
 
 public:

  meta_data_handler( string fname )

  bool clear();
  bool write( string fname, string fpath );
  bool add_data;
  bool set_meta_data_file_info( vector<string > *metadata );

  bool set_meta_data_file_path( string fpath );
  bool set_meta_data_file_name( string fname );
  bool set_meta_data_file( string pathwithname );  

  vector<string > get_meta_data_file_info();
  string get_meta_data_file_path();
  string get_meta_data_file_name();
  string get_meta_data_file();

 };


#endif
