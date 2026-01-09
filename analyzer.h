#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>

struct ZoneCount {
    std::string zone;
    long long count;
};

struct SlotCount {
    std::string zone;
    int hour;              // 0–23
    long long count;
};

class TripAnalyzer {
private:
  std::unordered_map<std::string, int>  CounterMap;

public:
    // Parse Trips.csv, skip dirty rows, never crash
    void ingestFile(const std::string& csvPath);

    // Top K zones: count desc, zone asc
    std::vector<ZoneCount> topZones(int k = 10) const;

    // Top K slots: count desc, zone asc, hour asc
    std::vector<SlotCount> topBusySlots(int k = 10) const;
};
