/*
 * AUTHOR: CAMERON EMBREE
 */


#include "src/utils.h"

#include "src/config_handler.h"
#include "src/audio_recorder.h"
 



using namespace std;

/*
std::string makeTimeStamp()
{
  time_t ltime;
  struct tm *Tm;
 
  ltime=time(NULL);
  Tm=localtime(&ltime);
 
  stringstream timeStmp;
  timeStmp << "D-" << Tm->tm_mday
	   << "-" << Tm->tm_mon
	   << "-" << Tm->tm_year
	   << "_T-" << Tm->tm_sec
	   << "-" << Tm->tm_min
	   << "-" << Tm->tm_hour;

  std::cout << "timeStamp:: " << timeStmp.str() << "\n";


  return timeStmp.str();
}


//TEMP METHOD - TODO - Could be used as an identifier for Meta data file?
std::string getMacAddress() {
  struct addrinfo *result;
  struct addrinfo *res;
  int error;
 
  // resolve the domain name into a list of addresses 
  error = getaddrinfo("www.example.com", NULL, NULL, &result);
  if (error != 0) {   
    if (error == EAI_SYSTEM) {
      perror("getaddrinfo");
    }
    else {
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
    }   
    exit(EXIT_FAILURE);
  }   
 
  char hostname[NI_MAXHOST] = "";
 
  // loop over all returned results and do inverse lookup 
  for (res = result; res != NULL; res = res->ai_next) {   
      error = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0); 
      
      if (error != 0) {
	fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(error));
	continue;
      }
  }   
  
  

  stringstream mac;
  mac << hostname;
  string macAddr = mac.str();

  std::cout << "macAddress:: Found address: " << macAddr << "\n";
  
  freeaddrinfo(result);


  return macAddr;
}


std::string makeRecCmd(std::string fileName, int dur) {
  stringstream cmdStream;

  //"arecord -D plughw:1 --duration=2 -f cd -vv ~/sounds/rec/recTest.wav"; 

  //various necessary parameters to make an ARECORD command line call
  string recProg = "arecord -D plughw:1";
  string deviceName= "-D plughw:1";
  string duration = "--duration="; //no duration set by default - will use 2 secs if not overwrittn
  string format = "-f cd";
  string extraOptions = "-vv";
  
  cmdStream << recProg
            << " " << deviceName
            << " " << duration << dur //duration of recording
            << " " << format
            << " " << extraOptions
            << " " << fileName;

  string cmd = cmdStream.str();


  return cmd;
}


std::string makeAudioFileName(std::string dirPath, std::string timeStamp, std::string fExt) {
  string basicName = "rec_";

  stringstream stream;

  stream << dirPath 
	 << basicName 
	 << timeStamp 
	 << fExt;

  string fullFileName = stream.str();

  return fullFileName;
}
*/


string n = "**raraa::";

string make_time_stamp() {

  string mn = "make_time_stamp:";
  cout<<n<<mn<<" Making time stamp ... "<<endl;

  time_t ltime;
  struct tm *Tm;
 
  ltime=time(NULL);
  Tm=localtime(&ltime);
 
  stringstream timeStmp;
  timeStmp << "D-"  << Tm->tm_mday
	   << "-"   << Tm->tm_mon
	   << "-"   << Tm->tm_year
	   << "_T-" << Tm->tm_sec
	   << "-"   << Tm->tm_min
	   << "-"   << Tm->tm_hour;


  cout<<n<<mn<<" Time Stamp is \""<<timeStmp.str()<<"\""<<endl;

  return timeStmp.str();
}


int main(int argc, char **argv) {

  string mn = "main:";
  int dur = 5; //HARDCODED TEMP RECORD LENGTH IF NONE SELECTED BY USER
  int numRuns = 1; //HARDCODED TEMP NUMBER OF RUNS



  //read config file for settings
  string configPath = "/home/pi/sounds";
  
  cout<<n<<mn<<" Started reading config file ... "<<endl;
  config_handler ch( configPath );
  cout<<n<<mn<<" Finished reading config file."<<endl;



  cout<<n<<mn<<" Creating audio recorder for '"<<ch.get_rec_number()
      <<"' runs, each '"<<ch.get_rec_duration()<<"' sec(s) ... "<<endl; 
  
  audio_recorder ar( ch );
  cout<<n<<mn<<" Created audio recorder."<<endl;


  
  //EXAMPLE RUN: ./mr 2 5
  //Make two recordings of length 5
  if(argc >= 3) {
    
    cout<<n<<mn<<" Overloading audio recording settings...";

    numRuns = atoi(argv[1]); // user num of runs ('0' is infinity runs (arecord(1)).
    dur = atoi(argv[2]); // user duration for recording 

    ch.set_rec_number( numRuns );
    ch.set_rec_duration( dur );
  }

  
  

  
  //Alert user to starting of recording with desired duration
  cout<<n<<mn<<" Making '"<<ch.get_rec_number()
      <<"' recording(s) of '"<<ch.get_rec_duration()
      <<"' sec(s) each."<<endl;


  pid_t childpid; //pid for keeping track of children


  //Make as many recordings as requested
  for(int recRunCount=0; recRunCount < ch.get_rec_number(); recRunCount++) {

    string timeStamp = make_time_stamp();
    int recDur = ch.get_rec_duration();
    ar.record( timeStamp, recDur );

    //spawn a child to extract features from audio for each recording
    childpid = fork();
    
    if( childpid == -1 ) {
      cerr<<mn<<" ERROR: Failed to (fork) create a child for recoding analysis!"<<endl;
      return 1; //ERROR - fork failed - kill process?!
    }
    
    if( childpid == 0 ) {
      break; //child moves to perform analysis
    }
  }
  

  /*
  //Child process takes care of feature extraction
  if(childpid == 0) {
    cout<<cn<<mn<<"Analyzing audio (Child process \""<<(long)getpid()<<"\")..."<<endl; 
    

    //---------META DATA----------
    //create Metadata output file of audio recording and Pi info
    string path(getenv("HOME"));  
    
    stringstream outRecMetaFileName;
    outRecMetaFileName << path 
		       << "/sounds/rec/fv_raw/"
		       << "rec_"
		       << timeStamp
		       << ".mdat";
    
    //create meta data file with approprate info
    ofstream recMetaData;
    recMetaData.open(outRecMetaFileName.str().c_str());
    recMetaData << "REC: " << audioRecName << "\n"
		<< "CMD: " << recCmd << "\n"
		<< "LAT: " << lat << "\n"
		<< "LON: " << lon << "\n"
		<< "MAC: " << macAddr << "\n";
    
    recMetaData.close();
    
    //Alert user to creation of meta data file
    cout << "META DATA FILE: " << outRecMetaFileName.str().c_str() << "\n";
    
    
    
    //---------FEATURE EXTRACTION----------
    //sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c featureplan -r 44100 rec_D-13-2-114_T-20-55-6.wav -b $(pwd)/test -v -p MetaData=True
    
    
    //BUG?: There is an issue where the output file is based on the ENTIRE input file. To avoid creating directories based on input file location
    // we are cding to the correct directory and then doing the extraction from there...
    
    //TEMP WORKING COMMAND
    //cd rec/audio_raw/ && sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c ../../featureplan -r 44100 rec_D-29-2-114_T-43-21-0.wav -b ../fv_raw/
    
    stringstream extCmd;
    extCmd << "cd rec/audio_raw/ && sudo -E PYTHONPATH=$PYTHONPATH /usr/local/bin/yaafe.py -c"
	   << " ../../featureplan -r 44100 "
	   << makeAudioFileName("", timeStamp, ".wav")
	   << " -b ../fv_raw/";
    
    //perform FV element extraction
    //system(extCmd.str().c_str());
    
    //alert user to cration of feature vector elements
    //cout << "FEATURE VEC FILE(s): " << extCmd.str().c_str() << "\n"; //TODO - make builder for feature vector command line call 
    cout << "FEATURE VEC FILE(s) LOC: " << "~/sounds/rec/fv_raw/" << "\n";
    
    
    
    //---------CHECK FOR NOISE----------
    //check if FV contains information we are interested in sending to the server
    
    //open feature vector element 
    
    
    //look for something other than just background noise
    
    
    //If not just background noise, move audio and fv data w/ meta data to './deploy/'
    
  }
  
  //Temporary Check to see when tasks are completed 
  if(childpid == 0) {
    printf("TEST: Audio Analizer (Child, pid:%ld) finished. \n", (long)getpid());
    exit(0); //kill child process
  } else {
    printf("TEST: Audio recorder (Parent, pid:%ld) finished. \n", (long)getpid());
    return 1;
  }

  printf("test - this should never be seen!");
  */

  return 0;
}
