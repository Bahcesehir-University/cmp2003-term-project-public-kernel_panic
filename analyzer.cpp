#include "analyzer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>
// Students may use ANY data structure internally


void TripAnalyzer::ingestFile(const std::string& csvPath) {
CounterMap.clear();
    // TODO:
    // - open file
    // analyzer.ingestFile("SmallTrips.csv"); main.cppde mevcut
   
std::ifstream file(csvPath);
if(!file.is_open()){

return;
}
    // görebildiğimiz test dosyasında header satır yok ama yine de skip header diyo diye atliyom 
    // belki local testte eksik bir şey patlar ilk satırdan dolayı
    // - skip header
    
std::string line;
std::getline(file, line);


// header varsa skipliyo yoksa bos zaten



size_t CommaLocation = 0;
std::string TripID;
std::string PickupZoneID;
std::string DropoffZoneID;
std::string PickupDateTime;
std::string DistanceKm;
std::string FareAmount;
    // - skip malformed rows

     // TripID, PickupZoneID, DropoffZoneID, PickupDateTime, DistanceKm, FareAmount
     //Example Record:
     // 1 1000001, ZONE034, ZONE102, 2023-01-10 08:42, 7.4, 82.5

      // https://www.youtube.com/watch?v=S2pvOeWyqBc bu abi anlatiyo nasil csv dosyasini virgullere gore bolecegimizi
    



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
// suan dosyayi acip acmadigini kontrol ediyor ardindan butun satirlari tek tek tarayip virgul sayisina gore hata var mi kontrol ediyor  burdan sonra saatler dogru mu falan diye kontrol edecek
  
// simdi her satiri parcalarina ayiricak ardindan saat kisminda hata var mi kontrol edecek varsa continue yine

// simdilik burayi commente aldim once virgul sayisini kontrol etcem baboli
CommaLocation = line.find(",");
TripID = line.substr(0, CommaLocation);
line = line.substr(CommaLocation + 1,line.length());


if(TripID.empty()){
continue;
}


bool allDigits = true;
for(char c : TripID) {
if(!isdigit(c)) {
allDigits = false;
break;
}
}
if(!allDigits) {
continue;
}





CommaLocation = line.find(",");
PickupZoneID = line.substr(0, CommaLocation);
line = line.substr(CommaLocation + 1,line.length());
 // zaten virgul sayisini kotnrol ediyoz ama eger 123123,,DropoffZoneID... gibi devam ediyosa eliyoruz read.me zoneidler case sensitive demis herhalde adi abc olsa bile aliyoz ondan boyle yaptim
 
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

  std::string JustTheTime = PickupDateTime.substr(PickupDateTime.length() - 5); // pickup datein son 5 satirini aliyor yani sadece 23.42 gibi bir saati(saatler 23:23 gibiymis stoi kullaninca patladik ilk 2 harfi aldi sadece tekrar bolcez)
  std::string JustTheTime = PickupDateTime.substr(PickupDateTime.length() - 5); 

if(JustTheTime[2] != ':'){

continue;
}
if(!isdigit(JustTheTime[0]) || !isdigit(JustTheTime[1]) || !isdigit(JustTheTime[3]) || !isdigit(JustTheTime[4])){

continue;
}
  int int_JustTheHour   = stoi(JustTheTime.substr(0, 2)); // 23 // ideal senaryoda patlamiyo suan ama mesela saati 8:23 gibi 4 haneli verdiyse o zaman hatalar olabilir 
  int int_JustTheMinute = stoi(JustTheTime.substr(3, 2)); // 59
  int int_JustTheHour   = stoi(JustTheTime.substr(0, 2)); 
  int int_JustTheMinute = stoi(JustTheTime.substr(3, 2)); 


  // saaten dolayi patlayan bir sey var mi kontrol ediyok 
  
if(int_JustTheHour < 0 || int_JustTheHour> 23 || int_JustTheMinute < 0 || int_JustTheMinute > 59 ){

continue;
}




CommaLocation = line.find(",");
DistanceKm = line.substr(0, CommaLocation);
line = line.substr(CommaLocation + 1,line.length());

size_t index;
double KMValue;
try {
KMValue = std::stod(DistanceKm, &index);

if (index != DistanceKm.length() || KMValue < 0) {
continue;
}


}
catch (...) {
    continue; // herhangi bi hata verirse input double degil gibi cart curt patlatyiyo ... o ise yariyo
    continue;
}



CommaLocation = line.find(",");
FareAmount = line.substr(0, CommaLocation);
line = line.substr(CommaLocation + 1,line.length());

double FareAmountValue;
try {
FareAmountValue = std::stod(FareAmount, &index);

if (index != FareAmount.length() || KMValue < 0) {
continue;
}


}
catch (...) {
    continue; // herhangi bi hata verirse input double degil gibi cart curt patlatyiyo ... o ise yariyo usttekiylen ayni
    continue; 
}



















    // - extract PickupZoneID and pickup hour
    // bunu zaten yukarda yaptik simdi sadece bunlari birlestircez ve hashmape strin olarak koyucaz su sayma isi icin ondan yeni stringin adina key diyom ne anlatiyon demeyin
std::string key = PickupZoneID + "|" + std::to_string(int_JustTheHour); // hashmapin tanimini while loopun disinda yapiyok o en ustte
std::string key = PickupZoneID + "|" + std::to_string(int_JustTheHour); 
    

    // - aggregate counts bu da tamam


CounterMap[key]++;

}
}


struct ZoneCmp {
bool operator()(const ZoneCount& a, const ZoneCount& b) const {
if (a.count != b.count) {

            return a.count < b.count; // bu priority_queue a ile b yi aliyor b>a mi diye soruyor  biz de a.count < b.count donduruyoruz bu 1 ise a yi asagi itiyor b yukarida queude
            return a.count < b.count;
}


        return a.zone > b.zone;// countlari esitse alfabetik siraya bakiyo ayni mantik yine 
        return a.zone > b.zone;
}
};

std::vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    // TODO:
   

    // en cok secilen top k yeri istiyor ya onu heap sort ile yapicaz zaten veriler suan hashmapin icinde
    // heap sortun built in hali varmis c++da queue diye yukari ekliyom 
    // - sort by count desc, zone asc
    // burada siralarken sadece zone123 gibi olan zone ismi ve counter lazim ssaat onemli degil ama o yuzden  once sadece zone ismi ve counter olan bi hashmap yapcaz
   





std::unordered_map<std::string,int>ZoneBasedMap;

    // bizim ilk yaptigimiz hashmapi nicini geziyor  i.first zone123_23.32 gibi olan yer i.second counter olan kisim oluyo int yani  2 gibi
   // auto tipini int mi string mi oldugunu kendi anlamasina yariyor pairin ne oldugunu degistirmeyelim ana hash mape m
   // ayrica topzones sordugu icin mesela bizim elimizdeki hashmapte zone1_23.32 5 olarak bi veri var ve ayrica zone1_08.23 2 gibi bir sey var 
   // ama top zones için birleştirmek gerekiyomus bu 2 veriyi samed hocam öyle dedi yeni map mi oluşturucaz dedim sana sorulanı bana geri soruyon dedi sanırım yeni map oluşturcaz onu yapıyom
 
for (auto i = CounterMap.begin(); i != CounterMap.end(); ++i) {

const std::string& key = i->first;
int count = i->second;

size_t  OrLocation = key.find('|');
std::string zone = key.substr(0, OrLocation);


ZoneBasedMap[zone] += count;
}// bu yani hashmap
}

//simdi heap sort yapcaz su priority queue ile
    // - return first k
 std::priority_queue<ZoneCount, std::vector<ZoneCount>, ZoneCmp> heap;//ZoneCmp bi struct onu fonksioynun disinda tanimladim
// ZoneBasedMapteki butun elemanlari heap e atiyoruz for loop ile

 std::priority_queue<ZoneCount, std::vector<ZoneCount>, ZoneCmp> heap;

for (auto i = ZoneBasedMap.begin(); i != ZoneBasedMap.end(); ++i) {

heap.push(ZoneCount{ i->first, i->second });

}

    std::vector<ZoneCount> result;//result vektorunun tanimi
    std::vector<ZoneCount> result;

    //k kez tekrarlatiyoz
  
while (k-- > 0 && !heap.empty()) {
        result.push_back(heap.top()); //heap.top(heapin en ustundeki elemani) result vektorune pushluyo
        heap.pop();//heapin 2. elemanina geciriyor
        result.push_back(heap.top()); 
        heap.pop();
}

return result;






}

// pq nun aldigi structi yaziyorum asil neye gore heape atacagini soyleyen yer usttekiyle ayni sadece hour kismi da olacak


struct SlotCmp{ bool operator()(const SlotCount& a, const SlotCount& b) const {


if (a.count != b.count) {
            return a.count < b.count;  // bu priority_queue a ile b yi aliyor b>a mi diye soruyor  biz de a.count < b.count donduruyoruz bu 1 ise a yi asagi itiyor b yukarida queude
            return a.count < b.count;  
}

if (a.zone != b.zone) {
            return a.zone > b.zone;// countlari esitse alfabetik siraya bakiyo ayni mantik yine  
            return a.zone > b.zone;
}
return a.hour > b.hour;

}
};



std::vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {


    // TODO:
    // - sort by count desc, zone asc, hour asc
  std::priority_queue<SlotCount, std::vector<SlotCount>, SlotCmp> heap; // ayni üstteki ama içine koyduğumuz struct farklı hour u da al'yo
// zone ve saat olarak yani ilk yaptigimiz countermap hashmapindeki veri gibi istiyo ondan direkt onu kullaniyoz 
  
  std::priority_queue<SlotCount, std::vector<SlotCount>, SlotCmp> heap; 

for (auto i = CounterMap.begin(); i != CounterMap.end(); ++i) {

const std::string& key = i->first; 
int count = i->second;

size_t  OrLocation = key.find('|');

std::string zone = key.substr(0, OrLocation); // zone neresi saat neresi onu tanimliyok artik hep ayni seyler baboli
std::string zone = key.substr(0, OrLocation); 

int hour = std::stoi(key.substr(OrLocation + 1));// saati inte cevirdik
int hour = std::stoi(key.substr(OrLocation + 1));

heap.push(SlotCount{zone,hour,count});// heapin icine attik
heap.push(SlotCount{zone,hour,count});

}
//topslotsdan direkt aktardim
std::vector<SlotCount> result;//result vektorunun tanimi

std::vector<SlotCount> result;

while (k-- > 0 && !heap.empty()) {
        result.push_back(heap.top()); //heap.top(heapin en ustundeki elemani) result vektorune pushluyo
        heap.pop();//heapin 2. elemanina geciriyor
        result.push_back(heap.top()); 
        heap.pop();
}

return result;

}
