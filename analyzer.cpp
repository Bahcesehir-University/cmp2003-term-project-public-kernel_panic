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
   ZoneBasedMap.clear();
    std::ifstream file(csvPath);
    if(!file.is_open()){
    
        return;
    }
    
    std::string line;
std::getline(file, line);
    



 
 

    while(std::getline(file,line)){


size_t CommaLocation1 = line.find(',');
size_t CommaLocation2 = line.find(',',CommaLocation1 + 1);
size_t CommaLocation3 = line.find(',',CommaLocation2 + 1);
size_t CommaLocation4 = line.find(',',CommaLocation3 + 1);
size_t CommaLocation5 = line.find(',',CommaLocation4 + 1);




    std::string TripID =line.substr(0,CommaLocation1);
    std::string PickupZoneID = line.substr(CommaLocation1 +1,CommaLocation2 - CommaLocation1 - 1);
    std::string DropoffZoneID =line.substr(CommaLocation2 +1,CommaLocation3 - CommaLocation2 - 1);
    std::string PickupDateTime=line.substr(CommaLocation3 +1,CommaLocation4 - CommaLocation3 - 1);
    std::string DistanceKm=line.substr(CommaLocation4 +1,CommaLocation5 - CommaLocation4 - 1);
    std::string FareAmount = line.substr(CommaLocation5 + 1);
    

 if (CommaLocation1==std::string::npos || CommaLocation2==std::string::npos || CommaLocation3==std::string::npos || CommaLocation4==std::string::npos || CommaLocation5==std::string::npos) {
 
    continue;
}



 if(TripID.empty()){
    continue;
}



  

 
if(PickupZoneID.empty()){

    continue;
}

 
 


 if(DropoffZoneID.empty()){

    continue;
}

 


  

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



  
   if(DistanceKm.empty()){

    continue;
}


  

 
 if(FareAmount.empty()){

    continue;
}
   

 
  


 
 
 

 
 
 
 







std::string key = PickupZoneID + "|" + std::to_string(int_JustTheHour); 
    
    


CounterMap[key]++;
ZoneBasedMap[PickupZoneID]++;

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
