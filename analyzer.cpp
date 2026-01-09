#include "analyzer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>


void TripAnalyzer::ingestFile(const std::string& csvPath) {
  CounterMap.clear();
   
    std::ifstream file(csvPath);
    if(!file.is_open()){
    
        return;
    }
    
    std::string line;
std::getline(file, line);
    




    size_t CommaLocation = 0;
    std::string TripID;
    std::string PickupZoneID;
    std::string DropoffZoneID;
    std::string PickupDateTime;
    std::string DistanceKm;
    std::string FareAmount;
    
 
 

    while(std::getline(file,line)){


        int CommaCounter = 0;
        for(int i = 0; i < line.length();i++){
            if(line[i] == ','){

            CommaCounter++;
            }
        } 

 if (CommaCounter != 5) {
 
    continue;
}

  CommaLocation = line.find(",");
  TripID = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());


 if(TripID.empty()){
    continue;
}



  

  CommaLocation = line.find(",");
  PickupZoneID = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());
 
if(PickupZoneID.empty()){

    continue;
}

 
 

  CommaLocation = line.find(",");
  DropoffZoneID = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());

 if(DropoffZoneID.empty()){

    continue;
}

 


  CommaLocation = line.find(",");
  PickupDateTime = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());

if(PickupDateTime.length() < 16 ){


    continue;
}

  std::string JustTheTime = PickupDateTime.substr(PickupDateTime.length() - 5); 

  if(JustTheTime[2] != ':'){
 
    continue;
  }
  if(!isdigit(JustTheTime[0]) || !isdigit(JustTheTime[1]) || !isdigit(JustTheTime[3]) || !isdigit(JustTheTime[4])){
  
    continue;
  }
  int int_JustTheHour   = stoi(JustTheTime.substr(0, 2)); 
  int int_JustTheMinute = stoi(JustTheTime.substr(3, 2)); 


  
  if(int_JustTheHour < 0 || int_JustTheHour> 23 || int_JustTheMinute < 0 || int_JustTheMinute > 59 ){
  
    continue;
  }




  CommaLocation = line.find(",");
  DistanceKm = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());
   if(DistanceKm.empty()){

    continue;
}


  CommaLocation = line.find(",");
  FareAmount = line.substr(0, CommaLocation);
  line = line.substr(CommaLocation + 1,line.length());

 
 if(FareAmount.empty()){

    continue;
}
   

 
  


 
 
 

 
 
 
 







std::string key = PickupZoneID + "|" + std::to_string(int_JustTheHour); 
    
    


CounterMap[key]++;

     }
}


struct ZoneCmp {
    bool operator()(const ZoneCount& a, const ZoneCount& b) const {
        if (a.count != b.count) {
            
            return a.count < b.count;
        }

        
        return a.zone > b.zone;
    }
};

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
   

   





    std::unordered_map<std::string,int>ZoneBasedMap;

 
    for (auto i = CounterMap.begin(); i != CounterMap.end(); ++i) {

    const std::string& key = i->first;
    int count = i->second;

    size_t  OrLocation = key.find('|');
    std::string zone = key.substr(0, OrLocation);


    ZoneBasedMap[zone] += count;
}
    

 std::priority_queue<ZoneCount, std::vector<ZoneCount>, ZoneCmp> heap;

    for (auto i = ZoneBasedMap.begin(); i != ZoneBasedMap.end(); ++i) {
    
 heap.push(ZoneCount{ i->first, i->second });

}

    std::vector<ZoneCount> result;

  
    while (k-- > 0 && !heap.empty()) {
        result.push_back(heap.top()); 
        heap.pop();
    }

    return result;
    
    



    
}



struct SlotCmp{ bool operator()(const SlotCount& a, const SlotCount& b) const {


 if (a.count != b.count) {
            return a.count < b.count;  
        }

if (a.zone != b.zone) {
            return a.zone > b.zone;
        }
return a.hour > b.hour;

    }
};



std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {

    
  
  std::priority_queue<SlotCount, std::vector<SlotCount>, SlotCmp> heap; 

 for (auto i = CounterMap.begin(); i != CounterMap.end(); ++i) {

const std::string& key = i->first; 
int count = i->second;
  
size_t  OrLocation = key.find('|');

std::string zone = key.substr(0, OrLocation); 

int hour = std::stoi(key.substr(OrLocation + 1));

heap.push(SlotCount{zone,hour,count});

}

std::vector<SlotCount> result;

while (k-- > 0 && !heap.empty()) {
        result.push_back(heap.top()); 
        heap.pop();
    }

    return result;
   
}
