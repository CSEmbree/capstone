/*
 * Title:    raraa
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2014
 * Edited:   23-OCT-2014
 * Notes:    main recording and analysis program overseer. Here is 
 *             where worker classe are organised from. 
 */


#include "src/utils.h"
#include "src/filters.h"
//#include "src/raraa_exception.h"

#include "src/config_handler.h"
#include "src/audio_recorder.h"
#include "src/feature_vector.h"


//using namespace std;

string n = "**raraa::";


bool do_filter_feature_extraction( config_handler *ch, audio_recorder *ar ) {

  string mn = "filter_feature_extraction:";
  cout<<n<<mn<<" Performing filter feature extraction ... "<<endl;
  bool res = true; // TODO - only true if succesful extraction



  stringstream filterCmd;
  filterCmd << "yaafe.py "
	    << " -r " << ch->get_samp_rate()    	   
	    << " -c " << ch->get_fv_filter()
	    << " "    << ar->get_rec_file_name();
  //        << " -b " << ch.get_analysis_location(); // TODO - change output location ?
  
  //perform feature extraction
  //  system(filterCmd.str().c_str());
  

  int ret;
  if( !(ret = system(filterCmd.str().c_str())) ) {
    //SUCCESS - alert user to cration of feature vector elements
    cout<<n<<mn<<" Preliminary filter features here: "<<ch->get_analysis_location()<<endl;    
  } else {
    res = false;
  }



  cout<<n<<mn<<" Finished filter feature extraction."<<endl;

  return res; // TODO - only true if succesful extraction
}


bool rm_filter_features( config_handler *ch, audio_recorder *ar) {

  string mn = "rm_filter_features:";
  bool res = true;
  cout<<n<<mn<<" removing filter features ... "<<endl;
  

  string analysis_dir = ch->get_analysis_location();
  string audio_fname  = ar->get_rec_file_name_core() + ar->get_rec_extention(); 


  
  //CMD SYNTAX: find analysis_dir -type f -not -name audio_fname | xargs rm
  stringstream rmFiltersCmd;
  rmFiltersCmd << "bash -c 'find "
	       << analysis_dir
	       << " -type f -not -name "
	       << "'" << "dir.info" << "'"
	       << " -not -name "
	       << "'" << audio_fname<< "'"
	       << " | xargs rm'";
  

  //cout<<"TEST: "<<rmFiltersCmd.str()<<endl;

  
  int ret;
  if( !(ret = system(rmFiltersCmd.str().c_str())) ) {
    //SUCCESS - alert user to removal of filter features
    cout<<n<<mn<<" Finished filter feature removal ... "<<endl;
  } else {
    res = false;
  }

    
  return res;
}

bool do_feature_extraction( config_handler *ch, audio_recorder *ar ) {

  string mn = "do_feature_extraction:";
  bool res = true;
  cout<<n<<mn<<" Performing full feature extraction ... "<<endl;




  stringstream extractCmd;
  extractCmd << "yaafe.py "
	    << " -r " << ch->get_samp_rate()    	   
	    << " -c " << ch->get_fv_file()
	    << " "    << ar->get_rec_file_name();
  //        << " -b " << ch.get_analysis_location(); // TODO - change output location ?
  
  //perform FV element extraction
  //system(extractCmd.str().c_str());



  int ret;
  if( !(ret = system(extractCmd.str().c_str())) ) {
    //SUCCESS - alert user to audio feature extraction completion    
    cout<<n<<mn<<" Finished audio analysis ... "<<endl; 
  } else {
    res = false;
  }




  return res; 
}


bool move_features( config_handler *ch, audio_recorder *ar ) {

  string mn = "move_features:";
  bool res = true;
  cout<<n<<mn<<" Moving features to data deployment directory ... "<<endl;
  


  stringstream deployCmd;
  deployCmd << "bash -c 'cp " << ar->get_rec_file_name_base() << "* "
	    << " " << utils::pathify( ch->get_media_location() )
	    << "' "; 
  
  cout<<n<<mn<<" Copying analysis with: \""<<deployCmd.str().c_str()<<"\""<<endl;
  //system(deployCmd.str().c_str());



  int ret;
  if( !(ret = system(deployCmd.str().c_str())) ) {
    //SUCCESS - alert user to moving features
    cout<<n<<mn<<" Finished moving full feature(s) extracted."<<endl; 
  } else {
    res = false;
  }
  


  return res;
}


bool move_audio( config_handler *ch, audio_recorder *ar ) {

  string mn = "move_audio:";
  bool res = true;
  cout<<n<<mn<<" Moving audio to data deployment directory ... "<<endl;
  


  stringstream deployCmd;
  deployCmd << "bash -c 'mv " << ar->get_rec_file_name_base() << ch->get_rec_extention()
	    << " " << utils::pathify( ch->get_media_location() )
	    << "' "; 
  
  cout<<n<<mn<<" Copying audio with: \""<<deployCmd.str().c_str()<<"\""<<endl;
  //system(deployCmd.str().c_str());



  int ret;
  if( !(ret = system(deployCmd.str().c_str())) ) {
    //SUCCESS - alert user to moving features
    cout<<n<<mn<<" Finished moving audio extracted."<<endl; 
  } else {
    res = false;
  }
  


  return res; //TODO - only be true if copy was succesful
}



bool rm_audio( config_handler *ch, audio_recorder *ar ) {

  string mn = "rm_audio:";
  bool res = true;
  cout<<n<<mn<<" removing audio file  ... "<<endl;


  string audio = ch->get_analysis_location() + ar->get_rec_file_name_core() + ar->get_rec_extention();

  stringstream rmAudioCmd;
  rmAudioCmd << "bash -c 'rm " << audio << "'";


  int ret;
  if( !(ret = system(rmAudioCmd.str().c_str())) ) {
    //SUCCESS - alert user to removal of audio                                                                  
    cout<<n<<mn<<" Finished audio removal ... "<<endl;
  } else {
    res = false;
  }


  return res;
}




bool create_meta_data_file( string timeStamp, config_handler *ch, audio_recorder *ar ) {

  string mn = "create_meta_data_file:";
  cout<<n<<mn<<" Creating meta data file ... "<<endl;
  bool res = true;

  
  // create appropreate meta data file 
  stringstream outRecMetaFileName;
  outRecMetaFileName << ch->get_analysis_location()
		     << ch->get_rec_file_name_prefix()
		     << timeStamp
		     << ".mdat";
  string metaFileName = outRecMetaFileName.str();
  
  
  // Fill with recording data
  ofstream recMetaData;
  recMetaData.open( metaFileName.c_str() );
  if( !recMetaData ) {
    res = false;
  } else {
    recMetaData << "REC:  " << ar->get_rec_file_name()  << "\n"
		<< "TIME: " << timeStamp                << "\n"		
		<< "RPid: " << ch->get_rpid()           << "\n"
		<< "LAT:  " << ch->get_latitude()       << "\n"
		<< "LON:  " << ch->get_longitude()      << "\n"
		<< "MAC:  " << utils::get_mac_address() << endl;
  }
  
  recMetaData.close();
  
  
  cout<<n<<mn<<" Created meta data file \""<<metaFileName<<"\"."<<endl;
  
  return res;
}


bool simulate_run( config_handler *ch ) {

  string mn = "simulate_run:";
  bool res = true;
  cout<<n<<mn<<" Simulating a run ... "<<endl;


  string ext_media = "";
  string ext_mdata = "";

  // Choose which files get sent where by extention
  if ( ch->get_final_feature_format() == "WRAPPED" ) {
    ext_mdata = "*.dat";
    ext_media = "*.wav";
  } else if ( ch->get_final_feature_format() == "FILES" ) { 
    ext_mdata = "*.dat";
    ext_media = "{*.csv,*.wav}";
  }  else {
    ext_mdata = "*.dat";
    ext_media = "{*.csv,*.wav}";
  }


  // Simulate an actual run by copying the example data to the real data deployment directory
  stringstream copyExampleCmd;
  copyExampleCmd << "bash -c 'cp"
		 << " "
		 << utils::pathify(ch->get_simulate_dir()) + ext_mdata
		 << " " 
		 << ch->get_data_location()
		 <<"' ";

  string simulateRunCmd = copyExampleCmd.str();
  cout<<n<<mn<<" Simulated meta data copy command: \""<<simulateRunCmd<<"\""<<endl;

  // Warn if there is a problem
  int ret;
  if( !(ret = system(simulateRunCmd.c_str())) ) {
    //SUCCESS - alert user to cration of feature vector elements
    cout<<n<<mn<<" Simulation of meta data finished."<<endl; 
  } else {
    res = false;
  }


  
  copyExampleCmd.str(std::string()); // clear for next command


  copyExampleCmd << "bash -c 'cp"
		 << " "
		 << utils::pathify(ch->get_simulate_dir()) + ext_media
		 << " " 
		 << ch->get_media_location()
		 <<"' ";

  simulateRunCmd = copyExampleCmd.str();
  cout<<n<<mn<<" Simulated media data copy command: \""<<simulateRunCmd<<"\""<<endl;

  // Warn if there is a problem
  if( !(ret = system(simulateRunCmd.c_str())) ) {
    //SUCCESS - alert user to cration of feature vector elements
    cout<<n<<mn<<" Simulation of media data finished."<<endl; 
  } else {
    res = false;
  }



  // exit after copying example data as if an actual run has occured.
  return res;
}


bool clean_analysis_workspace( string timeStamp, config_handler *ch, audio_recorder *ar ) {

  string mn = "clean_analysis_workspace:";
  bool res = true;
  cout<<n<<mn<<" Cleaning up analysis workspace ... "<<endl;


  // create workspace file names to remove
  stringstream workspaceFileBase;
  workspaceFileBase << ch->get_analysis_location()
		    << ch->get_rec_file_name_prefix()
		    << timeStamp;
  string fileNameBase = workspaceFileBase.str();
  cout<<n<<mn<<" Will remove workspace files with base: \""<<fileNameBase<<"\""<<endl;


  // create clean cmd
  stringstream cleanWorkspaceCmd;
  cleanWorkspaceCmd << "bash -c '"
		    << "rm "
		    << fileNameBase
		    << "*"
		    << "' ";
  string cleanCmd = cleanWorkspaceCmd.str();
  cout<<n<<mn<<" Clean command is: \""<<cleanCmd<<"\""<<endl;
  //system(cleanCmd.c_str());



  int ret;
  if( !(ret = system(cleanCmd.c_str())) ) {
    //SUCCESS - alert user to cleaning finish  
    cout<<n<<mn<<" Analysis workspace clean. "<<endl;
  } else {
    res = false;
  }




  return res;
}


bool out_msg( int _type, string _data, bool _format, string _fname="", string _fpath="" ) {

  string mn = "out_msg:";
  bool res = true;


  res = utils::out_json( _type, _data, _format, _fname, _fpath );


  return res;
}


bool out_msg( string _data, string _fname, string _fpath, config_handler *ch ) {
  
  string mn = "out_msg:";
  bool res = true;
  
  int    type   = ch->get_output_type_id();
  string data   = _data;
  bool   format = ch->get_output_formatted();
  string fname  = _fname;
  string fpath  = _fpath;

  
  res = out_msg( type, data, format, fname, fpath );


  return res;
}


bool err_msg( string _data, config_handler *ch ) {

  string mn = "err_msg:";
  bool res = true;

  string unique_id = utils::gen_unique_id();
  string DEFAULT_FEXT  = ".dat";
  string DEFAULT_FNAME = "sound_script_error-" + unique_id + DEFAULT_FEXT;

  int    type   = ch->get_output_type_id();
  string data   = _data;
  bool   format = ch->get_output_formatted();
  string fpath  = ch->get_data_location();


  res = out_msg( type, data, format, DEFAULT_FNAME, fpath );
  

  return res;
}


bool err_msg( int _type, string _data, bool _format ) {

  string mn = "err_msg:";
  bool res = true;

  string unique_id = utils::gen_unique_id();
  string DEFAULT_FEXT  = ".dat";
  string DEFAULT_FNAME = "sound_script_error-" + unique_id + DEFAULT_FEXT;

  int    type   = _type;
  string data   = _data;
  bool   format = _format;
  string fpath  = utils::pathify( utils::get_home_dir()+"data" );
  
  res = out_msg( type, data, format, DEFAULT_FNAME, fpath );

  //fpath  = utils::pathify( utils::get_base_dir()+"data" );
  //res = out_msg( type, data, format, DEFAULT_FNAME, fpath );
  

  return res;
}



void err( string msg, config_handler *ch ) {
  err_msg( msg, ch );
  throw msg;
}


void warn( string msg, config_handler *ch ) {
  err_msg( msg, ch );
}




//////////////////////////////////////////////////////////////////////////////
// RARAA CORE
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {

  string mn = "main:";
  int DEFAULT_TYPE_ID = -1;

  
  try {
    int dur = -1;        //HARDCODED TEMP RECORD LENGTH IF NONE SELECTED BY USER
    int numRuns = -1;    //HARDCODED TEMP NUMBER OF RUNS
    
    // Have a way to track succes or failure of events and report them
    bool evres = true; //variable for saving the success or failure of an event (ex: result of creating a file). 
    string emsg = "";  //error message holder    
    

    
    //******************CONFIG PHASE******************
    
    //read config file for settings
    string configPath = utils::get_home_dir();//"/home/pi/sounds";
    string configFname = "cirainbow.conf";
    
    
    // allow user to choose a specifc config file by argument
    int numCheck = 0;
    if(argc >= 1) {
      
      for(int i = 1; i < argc; i++ ) {
	string curArg = argv[i];
	
	// last readable string is assumed to be a config file
	if ( utils::is_file_readable( curArg ) == true ) {
	  configPath = utils::get_cwd() + utils::get_directory_path( curArg );
	  configFname = utils::get_base( curArg );
	  
	} else if ( utils::is_pos_int( curArg ) == true && numCheck == 0 ) {
	  // first pos int found is assumed to be duration
	  dur = utils::string_to_number<int>( curArg );
	  numCheck++;
	  
	} else if ( utils::is_pos_int( curArg ) == true && numCheck == 1 ) {
	  // first pos int found is assumed to be number of recordings
	  numRuns = utils::string_to_number<int>( curArg );
	  numCheck++;
	}
      }
    }
    
    
    
    // We need a configuration file manager to ask questions about our recording state later
    cout<<n<<mn<<" Started reading config file ... "<<endl;
    config_handler ch( configPath, configFname );
    cout<<n<<mn<<" Finished reading config file."<<endl;
    
    
    // Running a simulation or in background handled first as they dictate later behavior
    if( ch.get_simulate()   == true ) { 
      if( simulate_run( &ch ) ) {
	// simulate finished!
	exit(0);
      } else {
	throw string("An error occured when running the simulation!");
      }
    } 
    if( ch.get_background() == true ) utils::daemonize();
    
    // User requested number of recordings and duration take presidence over config file settings
    if( dur     != -1 ) ch.set_rec_duration( dur );
    if( numRuns != -1 ) ch.set_rec_number( numRuns );
    
    
    // create an audio recorder with user's settings
    cout<<n<<mn<<" Creating audio recorder for '"<<ch.get_rec_number()
	<<"' runs, each '"<<ch.get_rec_duration()<<"' sec(s) ... "<<endl; 
    audio_recorder ar( ch );
    cout<<n<<mn<<" Created audio recorder."<<endl;
    
    
    //Alert user to starting of recording with desired duration
    cout<<n<<mn<<" Making '"<<ch.get_rec_number()
	<<"' recording(s) of '"<<ch.get_rec_duration()
	<<"' sec(s) each."<<endl;
    
    
    
    
    //******************RECORDING PHASE******************
    
    pid_t childpid   = -1; //pid for keeping track of children
    string timeStamp = ""; // audio recording time stamp
    int recDur       = 0;  //audio recording durration
    int recRunCount  = 0;  // run counter
    bool forever = false;
    
    
    // The number of recording can be finite or infinite (if zero)
    if( ch.get_rec_number() == 0 ) forever = true;
    if( ch.get_rec_number() < 0 ) { 
      emsg = " ERROR: Invalid number of recordings requested (found "
	+utils::number_to_string(ch.get_rec_number())+"). Check config file: \""
	+ch.get_config_file()+"\".";
            
      err( emsg, &ch );

      return 1; //invalid
    }
    
    
    //Make as many recordings as requested
    for( recRunCount=0; recRunCount < ch.get_rec_number() || forever ; recRunCount++ ) {
      
      
      // collect local info for recording
      timeStamp = utils::make_time_stamp();
      recDur = ch.get_rec_duration();
      if( recDur <=0 ) recDur = 1; //TODO - make more generic
      
      
      // make a recording
      evres = ar.record( timeStamp, recDur );

      if( evres == false ) {
	emsg = "ERROR: There was an issue making a recording! (pid:"
	  +utils::number_to_string((long)getpid())+"). ";
		
	err( emsg, &ch );
      }
      
      
      //spawn a child to extract features from audio for each recording and wrap data up
      childpid = fork();
      
      // ensure child exists to handle audio analysis
      if( childpid == -1 ) {
	string emsg = " ERROR: (fork) Failed to create a child for recoding analysis! (pid:"
	  +utils::number_to_string((long)getpid())+"). ";	  
	
	err( emsg, &ch );
	// return 1; //ERROR - fork failed - kill process?!
      }
      
      if( childpid == 0 ) {
	break; // Allow child to leave home and perform analysis
      }
      
      
    }
    
    
    
    //******************ANALYSIS PHASE******************
    
    // Child takes care of feature extraction
    
    if( childpid == 0 ) {
      
      bool interesting = true; // assume all audio is interesting unless we find otherwise
      
      
      //---------FILTER FEATURE EXTRACTION----------
      // Do minimal feature extractions for filtering requirments
      if( ch.get_filter() == true ) {
	

	cout<<n<<mn<<" Performing basic audio filtering ... "<<endl;
	evres = do_filter_feature_extraction( &ch, &ar );
	
	if( evres == true ) {
	  cout<<n<<mn<<" Extracted filter features (child pid \""<<(long)getpid()<<"\")."<<endl;
	} else {
	  string emsg = " ERROR: Failed to extract filter audio features! (child pid:"
	    +utils::number_to_string((long)getpid())+"). ";
	  
	  err( emsg, &ch );
	}
	
	
	//---------ANALYIZE FILTER FEATURES----------
	// Analyize the features for a filter to look for interesting features
	interesting = filters::perceptual_sharpness(ar.get_rec_file_name()+".ps.csv");    

	// Remove any filter features extracted now that we have determined if audio was interesting (or not)
	rm_filter_features( &ch, &ar );

	cout<<n<<mn<<" Completed filter analysis (child pid \""<<(long)getpid()<<"\")."
	    <<" Interesting: "<<(interesting? "YES!":"NO!")<<endl; 
      } else {
	cout<<n<<mn<<" No feature filtering is being performed. "<<endl;
      }    


      
      
      
      //---------EXTRACT MORE FEATURES IF INTERSTING----------
      // if a feature was interesting we need to extract more features and deploy them
      if( interesting == true && ch.get_analysis() == true ) {
	
	evres = do_feature_extraction( &ch, &ar );    
	
	if( evres == true ) {
	  cout<<n<<mn<<" Completed full feature extraction (child pid \""<<(long)getpid()<<"\")."<<endl;
	} else {
	  string emsg = " ERROR: Failed to complete feature extraction! (child pid:"
	    +utils::number_to_string((long)getpid())+"). ";
	  
	  err( emsg, &ch );
	}

      }
      
      
      
      //---------MOVE EXTRACTED FEATURES TO DEPLOYMENT----------
      // Move audio and features extracted to data deployment based on user's desired formats
      
      
      // Keeping or disgarding audio if we are told to keep it or a filter says it is intersting
      if( ch.get_save_rec() == true && interesting == true ) {
	
	evres = move_audio( &ch, &ar );

	if( evres == false ) {
	  string emsg = " ERROR: Failed to move audio file to data dir! (child pid:"
            +utils::number_to_string((long)getpid())+"). ";

	  err( emsg, &ch );
	}

      } else {
	
	evres = rm_audio( &ch, &ar );

	if( evres == false ) {
	  string emsg = " ERROR: Failed to remove audio file! (child pid:"
            +utils::number_to_string((long)getpid())+"). ";

	  err( emsg, &ch );
	}
      }
      


      //---------CREATE A Feature Vector & metadata from extracted features----------      
      if( interesting == true ) {

	// What we found was interesting (so worth saving)!


	// Create a feature vector as json foramtted file
	cout<<n<<mn<<" Creating a feature vector ... "<<endl;      
	feature_vector fv( timeStamp, &ch, &ar ); 
	
	
	// Write feature vector and meta data inforamtion
	evres = fv.write( &ch, &ar );
	
	if( evres == false ) {
	  string emsg = "ERROR: Failed to write feature vector meta data file! (pid:"
	    + utils::number_to_string( (long)getpid()) + ")";
	  
	  err( emsg, &ch );
	}
	
	
	
	// Save YAAFE files if user wants them
	if( ch.get_final_feature_format() == "FILES" && ch.get_analysis() == true) {
	  cout<<n<<mn<<" Moving features extracted to deployment ... "<<endl;
	  evres = move_features( &ch, &ar ); // Move YAAFE features extracted
	  
	  
	  if( evres == true ) {
	    cout<<" Finished moving features and meta data to data deployment. "
		<<"(child pid \""<<(long)getpid()<<"\")."<<endl;
	  } else {
	    string emsg = "ERROR: Failed to move YAAFE features to data dir! (child pid:"
	      + utils::number_to_string( (long)getpid()) + ")";
	    
	    err( emsg, &ch );
	  }
	}    
      }
      
      
      
      // always delete all tmp local files after user requested ones are moved to data deployment directory      
      if( ch.get_analysis() == true) {
	
	evres = clean_analysis_workspace( timeStamp, &ch, &ar );
	
	if(evres == true) {
	  cout<<n<<mn<<" Audio Analysis complete (child pid \""<<(long)getpid()<<"\")."<<endl;
	} else {
	  string emsg = "ERROR: Failed to clean workspace! (child pid:"
	    + utils::number_to_string( (long)getpid()) + ")";
	  
	  err( emsg, &ch );
	}
      }
      
      
      
    } //end child analysis process check
    
    
    //Handle child processes based on their job
    if(childpid == 0) {
      cout<<n<<mn<<" Analyizer & filter ("<<(recRunCount+1)<<" of "<<ch.get_rec_number()<<")"
	  <<" complete (child pid \""<<(long)getpid()<<"\")."<<endl;
      return 0; //kill child now that task is complete
    } else {
      cout<<n<<mn<<" ("<<(recRunCount)<<" of "<<ch.get_rec_number()<<")"
	  <<" Recording(s) complete (parent pid \""<<(long)getpid()<<"\")."<<endl;
      return 1;
    }
    
    
    cerr<<n<<mn<<" WARNING: Raraa core finished: This should never be seen! "<<endl;
    
  } catch ( const std::string& e ) {
    
    cerr<<n<<mn<<e<<endl;
    
  } catch (std::exception& e) {

    // Emergency (unsupported) error reporting here:
    // This is very bad, and should never happen, but we need to report to the backend if something unexpected occured.
    string error_msg = "ERROR: An unexpected error occured. Please contact support. Exception caught: "+string(e.what());
    bool error_format = true;
    int  error_type   = DEFAULT_TYPE_ID;

    cerr<<n<<mn<<error_msg<<endl;
    err_msg( error_type, error_msg, error_format );
  }

  
  return 0;
}
