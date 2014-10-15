/*
 * Title:    filters
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  26-SEP-2014
 * Edited:   26-SEP-2014
 * Notes:    Contains various filters 
 */


#ifndef FILTERS_H
#define FILTERS_H

#include "utils.h"


class filters {

 private:
  //nothing here, this is a utility class
  
  typedef struct basic_stats{
    int    count;
    double sum;
    double max;
    double min;
    double avg;
    double max_diff;
    double avg_diff;
    double sum_diff;
  } BASIC_STATS;
  
  
 public:

  static bool filter( string fName, string feature ) {

    string mn = "filter: ";
    cout<<mn<<" Performing filter analysis of \""<<feature<<"\" on file \""<<fName<<"\""<<endl;

    bool res = false;

    //myfile.open ( fName.c_str() );
    //myfile.close();

    if( feature == "PerseptualSharpness" ) {
      res = perceptual_sharpness(fName);
    }

    
    cout<<mn<<" Finished filter analysis. Filter:"<<(res? "y":"n")<<endl;
    
    return res;
  };

  static bool perceptual_sharpness( string fName ) {

    string mn = "perceptual_sharpness:";
    cout<<mn<<" Starting PS filer..."<<endl;
    bool res = true;

    //TODO - parse file
    BASIC_STATS bs = get_basic_stats( fName );

    cout<<mn<<" max_diff="<<bs.max_diff<<endl; 
        


    cout<<mn<<" Finished PS filter. Res:"<<(res? "y":"n")<<endl;

    return res;
  }

  static void clear ( BASIC_STATS *bs ) {

    bs->count    = 0;
    bs->sum      = 0.0;
    bs->max      = 0.0;
    bs->min      = 0.0;
    bs->avg      = 0.0;
    bs->max_diff = 0.0;
    bs->avg_diff = 0.0;
    bs->sum_diff = 0.0;
  }


  static void reset ( BASIC_STATS *bs ) {

    bs->count    = -1;
    bs->sum      = -1.0;
    bs->max      = -1.0;
    bs->min      = -1.0;
    bs->avg      = -1.0;
    bs->max_diff = -1.0;
    bs->avg_diff = -1.0;
    bs->sum_diff = -1.0;
  }


  static BASIC_STATS get_basic_stats( string fname ) {

    string mn = "get_basic_stats:";
    cout<<mn<<" Getting basic stats for: \""<<fname<<"\"."<<endl;
    BASIC_STATS bs;
    reset( &bs );


    if( utils::is_file_readable( fname ) == true) {
      std::ifstream data( fname );    
      std::string line;
      bool first = true;
      std::vector<double > diffs;
      double prev_cval = 0;

      
      while( std::getline( data, line) ) {
	std::stringstream lineStream( line ); //each line
	std::string cell; //each element of a line
	
	while( std::getline( lineStream, cell, ',' ) ) {
	  
	  if( cell.front() != '%' ) {
	    
	    double cval = (double) utils::string_to_number<double>( cell );
	    //cout<<"TEST: Have element: '"<<cell<<"' as '"<<cval<<"'."<<endl;
	    
	    if( first == true ) {
	 
	      clear( &bs );
	      bs.min = cval;
	      bs.max = cval;
	      prev_cval = cval;
	      
	      first = false;
	    } else {
	  
	      if( cval < bs.min ) bs.min = cval;
	      if( cval > bs.max ) bs.max = cval;
	      diffs.push_back( prev_cval - cval );
	    }

	    bs.sum += cval;
	    bs.count++;
	    
	    prev_cval = cval; //previous was the old current
	  }	  
	}
      }

      
      bs.avg = bs.sum  / ( (double) bs.count );
      bs.max_diff = std::abs( bs.max - bs.min );

      for( size_t i = 0; i < diffs.size(); i++ ) bs.sum_diff += std::abs( diffs.at(i) );
      
      bs.avg_diff = bs.sum_diff / ( (double) bs.count );

    } else {
      cout<<mn<<" ERROR: Could not retreive file \""<<fname<<"\"."<<endl;
    }
  
      
    cout<<mn<<" TEST Found: "
	<<"\n\t count: "<<bs.count
	<<"\n\t sum: "<<bs.sum
	<<"\n\t max: "<<bs.max
	<<"\n\t min: "<<bs.min
	<<"\n\t avg: "<<bs.avg
	<<"\n\t max_diff: "<<bs.max_diff
	<<"\n\t avg_diff: "<<bs.avg_diff
	<<"\n\t sum_diff: "<<bs.sum_diff<<endl;
    

    return bs;
  }
  
  
};

#endif
