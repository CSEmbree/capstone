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
    
    cout<<mn<<" Finished PS filter. Res:"<<(res? "y":"n")<<endl;

    return res;
  }

  
  
};

#endif
