/*
 * Title:    raraa
 * Author:   C. Embree
 * Contact:  cse@cameronembree.com
 * Created:  4-SEP-2014
 * Edited:   7-SEP-2014
 * Notes:    main recording and analysis program overseer. Here is 
 *             where worker classe are organised from. 
 */


#include "src/utils.h"

#include "src/config_handler.h"
#include "src/audio_recorder.h"



//using namespace std;

string n = "**raraa::";


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
  
  string lat            = ch.get_latitude();
  string lon            = ch.get_longitude();
  string rpid           = ch.get_rpid();
  string analysisPath   = ch.get_analysis_location();
  string analysisPrefix = ch.get_rec_file_name_prefix(); 
  string audioRecName   = ar.get_rec_file_name();
  string macAddr        = utils::get_mac_address(); 


  // Child takes care of feature extraction
  if(childpid == 0) {
    cout<<n<<mn<<" Generating meta data file (child pid \""<<(long)getpid()<<"\") ..."<<endl; 
    
    // create appropreate meta data file 
    stringstream outRecMetaFileName;
    outRecMetaFileName << analysisPath
		       << analysisPrefix
		       << timeStamp
		       << ".mdat";
    string metaFileName = outRecMetaFileName.str();


    // Fill with recording data
    ofstream recMetaData;
    recMetaData.open( metaFileName.c_str() );
    recMetaData << "REC:  " << audioRecName << "\n"
		<< "TIME: " << timeStamp    << "\n"		
		<< "RPid: " << rpid         << "\n"
		<< "LAT:  " << lat          << "\n"
		<< "LON:  " << lon          << "\n"
		<< "MAC:  " << macAddr      << endl;
    
    recMetaData.close();
    

    //Alert user to creation of meta data file
    cout<<n<<mn<<" Created meta data file \""<<outRecMetaFileName.str().c_str()
	<<" (child pid \""<<(long)getpid()<<"\")"<<endl;
    

    cout<<n<<mn<<" Performing audio filtering ... "<<endl;
  
        
    //---------FILTER FEATUURE EXTRACTION----------
    //sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c featureplan -r 44100 rec_D-13-2-114_T-20-55-6.wav -b $(pwd)/test -v -p MetaData=True
    
    
    //BUG?: There is an issue where the output file is based on the ENTIRE input file. To avoid creating directories based on input file location
    // we are cding to the correct directory and then doing the extraction from there...
    
    //TEMP WORKING COMMAND
    //cd rec/audio_raw/ && sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c ../../featureplan -r 44100 rec_D-29-2-114_T-43-21-0.wav -b ../fv_raw/

    //old working
    //stringstream extCmd;
    //extCmd << "cd rec/audio_raw/ && sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c"
    //	   << " ../../featureplan -r 44100 "
    //	   << makeAudioFileName("", timeStamp, ".wav")
    //	   << " -b ../fv_raw/";


    //EXAMPLE: yaafe.py -r 44100 -c /home/pi/sounds/featureplan_fin /home/pi/data/rec_D-26-8-114_T-16-41-1.wav
    stringstream extCmd;
    extCmd << "yaafe.py "
	   << " -r " << ch.get_samp_rate()    	   
	   << " -c " << ch.get_fv_file()
	   << " "    << audioRecName;
      //<< " -b " << ch.get_analysis_location();

    //perform FV element extraction
    system(extCmd.str().c_str());
    
    //alert user to cration of feature vector elements
    cout<<n<<mn<<" Preliminary filter features here: "<<ch.get_analysis_location()<<endl;    
    
    
    //---------ANALYIZE FILTER - MOVE TO DEPLOY IF RELEVANT----------
    //check if FV contains information we are interested in sending to the server
    
    //open feature vector element 
    
    
    //look for something other than just background noise
    
    
    //If not just background noise, move audio and fv data w/ meta data to 'deploy/'
    
    

    cout<<n<<mn<<" Audio filtering complete. "<<endl;

  }
  
  

  //Handle child processes based on their job
  if(childpid == 0) {
    cout<<n<<mn<<" Analyizer & filter ("<<(recRunCount+1)<<" of "<<ch.get_rec_number()<<")"
	<<" complete (Child, pid \""<<(long)getpid()<<"\")."<<endl;
    return 0; //kill child now that task is complete
  } else {
    cout<<n<<mn<<" ("<<(recRunCount)<<" of "<<ch.get_rec_number()<<")"
	<<" Recording(s) complete (Parent, pid \""<<(long)getpid()<<"\")."<<endl;
    return 1;
  }


  cout<<n<<mn<<" ERROR: This should never be seen! "<<endl;


  return 0;
}
