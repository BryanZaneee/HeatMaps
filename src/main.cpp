#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "bryHashMap.h"
#include "RBTree.cpp"
#include "TileDownloader.h"
using namespace std;

struct crashData {
   int caseNumber;
   double latitude;
   double longitude; 
   int totalVehicle;
   string crashDay;
};

struct uniqueCoords {
   int caseNumber;
   double latitude;
   double longitude;
   int totalVehicleSum;
   uniqueCoords(int caseNum, double lat, double lon, int sum) : 
       caseNumber(caseNum), latitude(lat), longitude(lon), totalVehicleSum(sum) {}
};

vector<string> split(const string& line, char delim) {
   vector<string> tokens;
   istringstream tokenStream(line);
   string token;
   while(getline(tokenStream, token, delim)) {
       tokens.push_back(token);
   }
   return tokens;
}

void parseCSV(const string& filename, vector<crashData>& crashdata, bHash& crashDataMap, RBTree& RbtreeData) {
   ifstream file(filename);
   if (!file.is_open()) {
       cerr << "Failed to open " << filename << endl;
       exit(EXIT_FAILURE);
   }
   
   string line;
   getline(file, line);
   vector<uniqueCoords> uniqueC;

   while (getline(file, line)) {
       vector<string> tokens = split(line, ',');
       if (tokens.size() >= 24) {
           crashData data;
           try {
               data.caseNumber = stoi(tokens[0]);
               data.latitude = stod(tokens[23]);
               data.longitude = stod(tokens[22]);
               data.totalVehicle = stoi(tokens[15]);
               data.crashDay = tokens[5];

               crashdata.push_back(data);
               auto it = find_if(uniqueC.begin(), uniqueC.end(), [&](const uniqueCoords& uc){
                   return uc.latitude == data.latitude && uc.longitude == data.longitude;
               });

               if(it != uniqueC.end()) {
                   it->totalVehicleSum += data.totalVehicle;
               } else {
                   uniqueC.emplace_back(data.caseNumber, data.latitude, data.longitude, data.totalVehicle);
               }
           } catch (const exception& e) {
               continue;
           }
       }
   }

   for(auto a: uniqueC) {
       crashDataMap.AddItem(a.caseNumber, a.latitude, a.longitude, a.totalVehicleSum);
       RbtreeData.insert(a.caseNumber, a.latitude, a.longitude, a.totalVehicleSum);
   }
}

void createMapVisualization(bHash& crashDataMap) {
   sf::RenderWindow window(sf::VideoMode(1200, 800), "Gainesville Traffic Crashes");
   
   // Load the map texture
   sf::Texture mapTexture;
   if (!mapTexture.loadFromFile("gainesville_map.png")) {
       std::cerr << "Failed to load map background" << std::endl;
       return;
   }
   
   // Create and scale map sprite
   sf::Sprite mapSprite(mapTexture);
   mapSprite.setScale(
       window.getSize().x / (float)mapTexture.getSize().x,
       window.getSize().y / (float)mapTexture.getSize().y
   );
   
   // Constants for coordinate transformation
   const double MIN_LAT = 29.5734;
   const double MAX_LAT = 29.75;
   const double MIN_LON = -82.5;
   const double MAX_LON = -82.1;

   // For crash intensity colors
   const int MAX_INTENSITY = 255;
   
   while (window.isOpen()) {
       sf::Event event;
       while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed)
               window.close();
       }

       window.clear();
       window.draw(mapSprite);  // Draw map first

       for(int i = 0; i < crashDataMap.tableSize; i++) {
           auto* ptr = crashDataMap.hashTable[i];
           while(ptr != nullptr && ptr->caseNumber != 0) {
               float x = (ptr->longitude - MIN_LON) / (MAX_LON - MIN_LON) * window.getSize().x;
               float y = (MAX_LAT - ptr->latitude) / (MAX_LAT - MIN_LAT) * window.getSize().y;
               
               // Scale point size based on vehicle count
               float radius = log(ptr->totalVehicle + 2) * 3;
               
               sf::CircleShape point(radius);
               point.setPosition(x - radius, y - radius);
               
               // Color based on vehicle count
               int intensity = std::min(MAX_INTENSITY, 
                   (int)(ptr->totalVehicle * 25));
               point.setFillColor(sf::Color(intensity, 0, MAX_INTENSITY - intensity, 180));
               
               window.draw(point);
               ptr = ptr->next;
           }
       }

       window.display();
   }
}

void createRBMapVisualization(RBTree& RbtreeData) {
   sf::RenderWindow window(sf::VideoMode(1200, 800), "Gainesville Traffic Crashes (RB Tree)");
   
   // Load the map texture
   sf::Texture mapTexture;
   if (!mapTexture.loadFromFile("gainesville_map.png")) {
       std::cerr << "Failed to load map background" << std::endl;
       return;
   }
   
   // Create and scale map sprite
   sf::Sprite mapSprite(mapTexture);
   mapSprite.setScale(
       window.getSize().x / (float)mapTexture.getSize().x,
       window.getSize().y / (float)mapTexture.getSize().y
   );
   
   // Constants for coordinate transformation
   const double MIN_LAT = 29.5734;
   const double MAX_LAT = 29.75;
   const double MIN_LON = -82.5;
   const double MAX_LON = -82.1;

   // Get data from RB Tree
   vector<double> latitudeVec;
   vector<double> longitudeVec;
   vector<double> totalVehiclesVec;
   RbtreeData.inorder(latitudeVec, longitudeVec, totalVehiclesVec);
   
   while (window.isOpen()) {
       sf::Event event;
       while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed)
               window.close();
       }

       window.clear();
       window.draw(mapSprite);  // Draw map first

       // Draw points for each crash location
       for(size_t i = 0; i < latitudeVec.size(); i++) {
           float x = (longitudeVec[i] - MIN_LON) / (MAX_LON - MIN_LON) * window.getSize().x;
           float y = (MAX_LAT - latitudeVec[i]) / (MAX_LAT - MIN_LAT) * window.getSize().y;
           
           // Scale point size based on vehicle count
           float radius = log(totalVehiclesVec[i] + 2) * 3;
           
           sf::CircleShape point(radius);
           point.setPosition(x - radius, y - radius);
           
           // Color based on vehicle count
           int intensity = std::min(255, (int)(totalVehiclesVec[i] * 25));
           point.setFillColor(sf::Color(intensity, 0, 255 - intensity, 180));
           
           window.draw(point);
       }

       window.display();
   }
}

void menu(bHash& crashDataMap, RBTree& RbtreeData) {
   int option;

   while (true) {
       cout << "Choose an option: " << endl;
       cout << "Option 1: Hash Map Visualization" << endl;
       cout << "Option 2: Red and Black Tree Visualization" << endl;
       cout << "Enter option (1 or 2): " << endl;

       if (!(cin >> option) || (option != 1 && option != 2)) {
           cout << "Invalid input. Please enter 1 or 2." << endl;
           cin.clear();
           cin.ignore(numeric_limits<streamsize>::max(), '\n');
       } else {
           break;
       }
   }

   if (option == 1) {
       cout << "Generating Map Using Hash Map..." << endl;
       createMapVisualization(crashDataMap);
   } else if (option == 2) {
       cout << "Generating Map Using Red and Black Tree..." << endl;
       createRBMapVisualization(RbtreeData);
   }
}

int main() {
   TileDownloader downloader;
   if (!downloader.downloadTiles() || !downloader.createMergedMap()) {
       std::cerr << "Failed to setup map" << std::endl;
   }
   
   string filename = "Traffic_Crashes.csv";
   vector<crashData> crashdata;
   bHash crashDataMap;
   RBTree RbtreeData;
   parseCSV(filename, crashdata, crashDataMap, RbtreeData);
   menu(crashDataMap, RbtreeData);
   
   return 0;
}