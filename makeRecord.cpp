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


std::string makeCmd(std::string fileName) {
  stringstream cmdStream;

  //"arecord -D plughw:1 --duration=2 -f cd -vv ~/sounds/rec/recTest.wav"; 

  string recProg = "arecord -D plughw:1";
  string deviceName= "-D plughw:1";
  string duration = "--duration=2";
  string format = "-f cd";
  string extraOptions = "-vv";
  
  cmdStream << recProg
            << " " << deviceName
            << " " << duration
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
  string timeStamp = makeTimeStamp();
  string audioRecName = makeAudioFileName("~/sounds/rec/", timeStamp, ".wav");
  string recCmd = makeCmd(audioRecName);
  string macAddr = getMacAddress();

  //HARDCODED POSITIONS
  string lat = "33° 58' 34.3698\"";
  string lon = "-120° 6' 46.2198\"";

  
  //system call to make audio recording
  system(recCmd.c_str());


  //create output file of audio recording and Pi info
  string path(getenv("HOME"));  

  stringstream outRecFileName;
  outRecFileName << path 
		 << "/sounds/rec/"
		 << "rec_info_"
		 << timeStamp
		 << ".txt";
  
  cout << "TEST: Creating file: " << outRecFileName.str().c_str() << "\n";

  ofstream recFileData;
  recFileData.open(outRecFileName.str().c_str());
  recFileData << "REC: " << audioRecName << "\n"
	      << "CMD: " << recCmd << "\n"
	      << "LAT: " << lat << "\n"
	      << "LON: " << lon << "\n"
	      << "MAC: " << macAddr << "\n";

  recFileData.close();

  
  return 0;
}
