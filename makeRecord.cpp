/*
 * AUTHOR: CAMERON EMBREE
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/types.h>
 
#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif
 
#include <fstream>



using namespace std;

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
 
  /* resolve the domain name into a list of addresses */
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
 
  /* loop over all returned results and do inverse lookup */
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



int main(int argc, char **argv) {

  int dur = 2; //HARDCODED TEMP RECORD LENGTH IF NONE SELECTED BY USER
  int numRuns = 1; //HARDCODED TEMP NUMBER OF RUNS


  //EXAMPLE RUN: ./mr 2 5
  //Make two recordings of length 5

  if(argc >= 3) {
    numRuns = atoi(argv[1]); //user chosen number of runs (assumed valid) //Value of '0' means infinity (arecord(1)).
    dur = atoi(argv[2]); //user chosen duration for recording (assumed valid)
  }

  //Alert user to starting of recording with desired duration
  printf("Making '%d' records of duration '%d'\n", numRuns, dur);


  pid_t childpid; //pid for keeping track of children
  string timeStamp = "null";
  string audioRecName = "null";
  string recCmd = "null";
  string macAddr = "null";
  string lat = "null";
  string lon = "null";

  //Make as many recordings as requested
  for(int runCount=0; runCount<numRuns; runCount++) {

    //---------AUDIO RECORD----------
    //various meta data for the record
    timeStamp = makeTimeStamp();
    audioRecName = makeAudioFileName("~/sounds/rec/audio_raw/", timeStamp, ".wav");
    recCmd = makeRecCmd(audioRecName, dur);
    macAddr = getMacAddress();
    
    //HARDCODED Meta data for GPA POSITIONS
    lat = "33° 58' 34.3698\"";
    lon = "-120° 6' 46.2198\"";
    
    //make audio recording
    system(recCmd.c_str());
    
    //alert user to creation of audio file
    cout << "AUDIO FILE: " << audioRecName << "\n";
    
    
    //spawn a child to extract features from audio for each recording
    childpid = fork();
    
    if(childpid == -1) {
      perror("Failed to Fork");
      return 1; //ERROR - fork failed!!
    }
    
    if(childpid ==0) {
      break;
    }
  }
  

  //Child process takes care of feature extraction
  if(childpid == 0) {
    printf("Child process (%ld) analyzing audio... \n", (long)getpid());
    
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
    system(extCmd.str().c_str());
    
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
    printf("TEST: Child (pid:%ld) finished. \n", (long)getpid());
    exit(0); //kill child process
  } else {
    printf("TEST: Parent (pid:%ld) finished. \n", (long)getpid());
    return 1;
  }

  printf("test - this should never be seen!");

  return 0;
}
