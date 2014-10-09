/*
 * Title:    raraa
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2014
 * Edited:   30-SEP-2014
 * Notes:    main recording and analysis program overseer. Here is 
 *             where worker classe are organised from. 
 */


#include "src/utils.h"
#include "src/filters.h"

#include "src/config_handler.h"
#include "src/audio_recorder.h"


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
  
  //perform FV element extraction
  system(filterCmd.str().c_str());
  
  //alert user to cration of feature vector elements
  cout<<n<<mn<<" Preliminary filter features here: "<<ch->get_analysis_location()<<endl;    


  cout<<n<<mn<<" Finished filter feature extraction."<<endl;

  return res; // TODO - only true if succesful extraction
}


bool do_feature_extraction( config_handler *ch, audio_recorder *ar ) {

  string mn = "do_feature_extraction:";
  cout<<n<<mn<<" Performing full feature extraction ... "<<endl;
  bool res = true; // TODO - only true if succesful extraction



  stringstream extractCmd;
  extractCmd << "yaafe.py "
	    << " -r " << ch->get_samp_rate()    	   
	    << " -c " << ch->get_fv_file()
	    << " "    << ar->get_rec_file_name();
  //        << " -b " << ch.get_analysis_location(); // TODO - change output location ?
  
  //perform FV element extraction
  system(extractCmd.str().c_str());

  cout<<n<<mn<<" Finished addtional audio analysis ... "<<endl;



  cout<<n<<mn<<" Moving interesting analysis to data deployment directory ... "<<endl;
  
  stringstream deployCmd;
  deployCmd << "cp " << ar->get_rec_file_name_base()<<"* "
	    << " " << ch->get_data_location(); 
  
  cout<<n<<mn<<" Copying analysis with: \""<<deployCmd.str().c_str()<<"\""<<endl;
  system(deployCmd.str().c_str());
  
  cout<<n<<mn<<" Finished moving full feature(s) extracted."<<endl;



  return res; 
}


bool create_meta_data_file( string timeStamp, config_handler *ch, audio_recorder *ar ) {

  string mn = "create_meta_data_file:";
  cout<<n<<mn<<" Creating meta data file ... "<<endl;
  bool res = true;

  
  /*
  string lat            = ch.get_latitude();
  string lon            = ch.get_longitude();
  string rpid           = ch.get_rpid();
  string analysisPath   = ch.get_analysis_location();
  string analysisPrefix = ch.get_rec_file_name_prefix(); 
  string audioRecName   = ar.get_rec_file_name();
  string macAddr        = utils::get_mac_address(); 
  */

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


void simulate_run( config_handler *ch ) {

  string mn = "simulate_run:";
  cout<<n<<mn<<" Simulating a run ... "<<endl;


  // Simulate an actual run byt copying the example data to the real data deployment directory
  stringstream copyExampleCmd;
  copyExampleCmd << "bash -c 'cp"
		 << " "
		 << utils::pathify(ch->get_simulate_dir())+"{*.wav,*.dat}"
		 << " " 
		 << ch->get_data_location()
		 <<"' ";
  string simulateRunCmd = copyExampleCmd.str();

  cout<<n<<mn<<" simulated data copy command: \""<<simulateRunCmd<<"\""<<endl;

  system(simulateRunCmd.c_str());
  cout<<n<<mn<<" simulation finished."<<endl;


  // exit after copying example data as if an actual run has occured.
  exit(0);
}


int main(int argc, char **argv) {

  string mn = "main:";
  int dur = 5;         //HARDCODED TEMP RECORD LENGTH IF NONE SELECTED BY USER
  int numRuns = 1;     //HARDCODED TEMP NUMBER OF RUNS


  //******************CONFIG PHASE******************

  //read config file for settings
  string configPath = utils::get_base_dir();//"/home/pi/sounds";
  string configFname = "config";

  cout<<n<<mn<<" Started reading config file ... "<<endl;
  config_handler ch( configPath, configFname );
  cout<<n<<mn<<" Finished reading config file."<<endl;


  if(ch.get_simulate() == true) simulate_run( &ch ); 

  if( ch.get_background() == true ) utils::daemonize();


  cout<<n<<mn<<" Creating audio recorder for '"<<ch.get_rec_number()
      <<"' runs, each '"<<ch.get_rec_duration()<<"' sec(s) ... "<<endl; 
  
  audio_recorder ar( ch );
  cout<<n<<mn<<" Created audio recorder."<<endl;


  
  // allow user to overload number of runs and run duration
  if(argc >= 3) {
    
    cout<<n<<mn<<" Overloading audio recording settings...";

    numRuns = atoi(argv[1]); // arg1 - user num of runs ('0' is inf runs (arecord(1)).
    dur     = atoi(argv[2]); // arg2 - user duration for recording 
    ch.set_rec_number( numRuns );
    ch.set_rec_duration( dur );
  }

  
  
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

  if( ch.get_rec_number() == 0 ) forever = true;
  //if( ch.get_rec_number() < 0 ) exit(0);

  //Make as many recordings as requested
  for( recRunCount=0; recRunCount < ch.get_rec_number() || forever ; recRunCount++ ) {

    
    // collect local info for recording
    timeStamp = utils::make_time_stamp();
    recDur = ch.get_rec_duration();
    if( recDur <=0 ) recDur = 1; //TODO - make more generic
    

    // make a recording
    ar.record( timeStamp, recDur );


    //spawn a child to extract features from audio for each recording
    childpid = fork();

    // ensure child exists to handle audio analysis
    if( childpid == -1 ) {

      string emsg = " ERROR: (fork) Failed to create a child for recoding analysis!";

      cerr<<n<<mn<<emsg<<endl;
      utils::error_msg( emsg );

      return 1; //ERROR - fork failed - kill process?!
    }
    
    if( childpid == 0 ) {
      break; // Allow child to leave home and perform analysis
    }

  }
  
  
  
  //******************ANALYSIS PHASE******************

  // Child takes care of feature extraction
  if(childpid == 0) {
    cout<<n<<mn<<" Generating meta data file (child pid \""<<(long)getpid()<<"\") ..."<<endl; 
    

    //---------CREATE A METDA DATA FILE FOR A RECORDING----------
    if( create_meta_data_file( timeStamp, &ch, &ar ) == false ) {
      cerr<<n<<mn<<" ERROR: A problem occured creating a meta data file at \""<<timeStamp<<"\""<<endl;
    }

    // Alert user to creation of meta data file
    cout<<n<<mn<<" Created meta data file (child pid \""<<(long)getpid()<<"\")."<<endl;
    
  
        
    //---------FILTER FEATURE EXTRACTION----------
    // Do minimal feature extractions for filtering requirments
    cout<<n<<mn<<" Performing basic audio filtering ... "<<endl;
    do_filter_feature_extraction( &ch, &ar );
    
    // Alert user to filter feature completion
    cout<<n<<mn<<" Extracted filter features (child pid \""<<(long)getpid()<<"\")."<<endl;


    
    //---------ANALYIZE FILTER FEATURES----------
    // Analyize the features for a filter to look for interesting features
    bool interesting = filters::perceptual_sharpness(ar.get_rec_file_name()+".ps.csv");    

    // Alert user to filter analysis completion
    cout<<n<<mn<<" Completed filter analysis (child pid \""<<(long)getpid()<<"\")."
	<<" Interesting: "<<(interesting? "YES!":"NO!")<<endl; 
    


    //---------EXTRACT MORE FEATURES MOVE TO DATA----------
    // if a feature was interesting we need to extract more features and deploy them

    if( interesting == true ) {
      do_feature_extraction( &ch, &ar );    

      // Alert user to full feature extraction completion
      cout<<n<<mn<<" Completed full feature extraction (child pid \""<<(long)getpid()<<"\")."<<endl;
    } 
    
    cout<<n<<mn<<" Audio Analysis complete (child pid \""<<(long)getpid()<<"\")."<<endl;
  }
  
  

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


  cout<<n<<mn<<" ERROR: This should never be seen! "<<endl;


  return 0;
}
