// NAME: Cameron Embree
// DATE: 5/11/14

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <stdlib.h>

using namespace std;


// struct for vector of config data
struct ConfigData {
  string type;
  double value;
};


class ConfigDataParser
{
  string _featureVectorPath;
  int _recDuration;
  int _recNumber;
  double _recSampleRate;
  vector< ConfigData> settings;

public:
  // Constructor
  ConfigDataParser( string  configFilePath ); 
  // Destructor  
  ~ConfigDataParser(){}; 

  // Accessors
  string GetFeatureVectorPath();
  int GetRecDuration();
  int GetRecNumber();
  double GetRecSampleRate();
  string ToString();
  bool Equals( ConfigDataParser cd );

private:
  // Mutators
  void SetDefaultState();
  void SetFeatureVectorPath( string featureVectorPath );
  void SetRecDuration( int recDuration );
  void SetRecNumber( int recNumber );  
  void SetRecSampleRate( double sampleRate );

  // Business methods
  bool FileExistCheck( string fileName );
  std::vector<ConfigData> ExtractValidData( string fileName );
  void ParseSettings( std::vector< ConfigData> settings );
}; 




//IMPLIMENTATIONS
// Constructor
ConfigDataParser::ConfigDataParser( string configFilePath ) {

  SetDefaultState();

  if( FileExistCheck( configFilePath ) == true ) {

    settings = ExtractValidData( configFilePath );

    ParseSettings( settings );

  } else {
    cout<<"ERROR: config file could not be found at: '"<<configFilePath<<"'"<<endl;
  }

}


// Accessors
string ConfigDataParser::GetFeatureVectorPath() {
  return _featureVectorPath;
}

int ConfigDataParser::GetRecDuration() {
  return _recDuration;
}

int ConfigDataParser::GetRecNumber() {
  return _recNumber;
}

double ConfigDataParser::GetRecSampleRate() {
  return _recSampleRate;
}

string ConfigDataParser::ToString() {
  std::stringstream ss;

  ss << "CONFIGURATION FILE DATA: " << endl
     << "\t FeatureVectorPath = \"" << ConfigDataParser::GetFeatureVectorPath() << "\"" << endl
     << "\t RecDuration = " << ConfigDataParser::GetRecDuration() << endl
     << "\t RecNumber = " << ConfigDataParser::GetRecNumber() << endl
     << "\t RecSampleRate = " << ConfigDataParser::GetRecSampleRate() << endl
     << "\tSETTINGS: " << endl;

  for (unsigned i=0; i < settings.size(); i++) {
    ss << "\t " << settings.at(i).type << " = " << settings.at(i).value << endl;
  }
  

  return ss.str();
}


// Mutators
void ConfigDataParser::SetDefaultState(){
  
  int defaultNum = -1;
  string defaultString = "";

  SetFeatureVectorPath( defaultString );
  SetRecDuration( defaultNum );
  SetRecNumber( defaultNum );
  SetRecSampleRate( defaultNum );
}

void ConfigDataParser::SetFeatureVectorPath( string featureVectorPath ) {
  _featureVectorPath = featureVectorPath;
}
void ConfigDataParser::SetRecDuration( int recDuration ) {
  _recDuration = recDuration;
}
void ConfigDataParser::SetRecNumber( int recNumber ) {
  _recNumber = recNumber;
}
void ConfigDataParser::SetRecSampleRate( double recSampleRate ) {
  _recSampleRate = recSampleRate;
}


// Business Methods
bool ConfigDataParser::FileExistCheck( string fileName ) {
  std::ifstream infile( fileName.c_str(), std::ifstream::in );

  return infile.good();
}

std::vector<ConfigData> ConfigDataParser::ExtractValidData( string fileName ) {
  std::ifstream inFile( fileName.c_str() );
  std::string line;  
  std::string type, val;
  std::vector<ConfigData> vec;

  while (std::getline(inFile, line)) {
    
    if(line.size() > 0) {
      if(line.at(0) != '#') {

	std::istringstream configData(line);
	
	std::string type, val;
	configData >> type >> val;
	//cout << "DEBUG: TEST: "<<" type: "<<type<<", val: "<<val<<endl; 
	
	struct ConfigData setting;
	setting.type = type;
	setting.value = atof(val.c_str());

	vec.push_back(setting);
      }
    }
  }
  
  return vec;
}


void ConfigDataParser::ParseSettings( std::vector< ConfigData> settings ) {
  
  string type;
  double value;

  for (unsigned i=0; i < settings.size(); i++) {
    type = settings.at(i).type;
    value = settings.at(i).value;

    //if( settings. )
    
    
    //ss << "\t " << settings.at(i).type << " = " << settings.at(i).value << endl;
  }
  
}

int main( int argc, const char* argv[] ) 
{
  ConfigDataParser cdp("config");
  cout << cdp.ToString();

  return 0;
}
