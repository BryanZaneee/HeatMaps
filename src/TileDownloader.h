#pragma once
#include <SFML/Graphics.hpp>
#include <curl/curl.h>
#include <cmath>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <sys/stat.h>

class TileDownloader {
private:
   const double MIN_LAT = 29.5734;
   const double MAX_LAT = 29.75;
   const double MIN_LON = -82.5;
   const double MAX_LON = -82.1;
   const int ZOOM = 12;
   const int TILE_SIZE = 256;

   static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
       ((std::string*)userp)->append((char*)contents, size * nmemb);
       return size * nmemb;
   }

   bool directoryExists(const std::string& path) {
       struct stat info;
       return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
   }

   bool fileExists(const std::string& filename) {
       struct stat info;
       return stat(filename.c_str(), &info) == 0;
   }

   bool createDirectory(const std::string& path) {
       #ifdef _WIN32
           return _mkdir(path.c_str()) == 0;
       #else
           return mkdir(path.c_str(), 0777) == 0;
       #endif
   }

   int long2tilex(double lon, int z) {
       return static_cast<int>(floor((lon + 180.0) / 360.0 * std::pow(2.0, z)));
   }
   
   int lat2tiley(double lat, int z) {
       double latrad = lat * M_PI/180.0;
       return static_cast<int>(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * std::pow(2.0, z)));
   }

   bool downloadTile(CURL* curl, int x, int y, const std::string& filename) {
       std::stringstream url;
       url << "https://tile.openstreetmap.org/" << ZOOM << "/" << x << "/" << y << ".png";
       
       std::string readBuffer;
       curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
       curl_easy_setopt(curl, CURLOPT_USERAGENT, "OSM Tile Downloader/1.0");
       
       CURLcode res = curl_easy_perform(curl);
       if (res != CURLE_OK) {
           std::cerr << "Failed to download tile: " << curl_easy_strerror(res) << std::endl;
           return false;
       }

       std::ofstream outFile(filename, std::ios::binary);
       if (!outFile) {
           std::cerr << "Failed to create file: " << filename << std::endl;
           return false;
       }
       outFile.write(readBuffer.c_str(), readBuffer.size());
       return true;
   }

public:
   bool downloadTiles() {
       int minX = long2tilex(MIN_LON, ZOOM);
       int maxX = long2tilex(MAX_LON, ZOOM);
       int minY = lat2tiley(MAX_LAT, ZOOM);
       int maxY = lat2tiley(MIN_LAT, ZOOM);

       CURL* curl = curl_easy_init();
       if (!curl) {
           std::cerr << "Failed to initialize CURL" << std::endl;
           return false;
       }

       try {
           if (!directoryExists("tiles")) {
               if (!createDirectory("tiles")) {
                   std::cerr << "Failed to create tiles directory" << std::endl;
                   curl_easy_cleanup(curl);
                   return false;
               }
           }
           
           for(int x = minX; x <= maxX; x++) {
               for(int y = minY; y <= maxY; y++) {
                   std::string filename = "tiles/" + std::to_string(x) + "_" + std::to_string(y) + ".png";
                   
                   if (!fileExists(filename)) {
                       if (!downloadTile(curl, x, y, filename)) {
                           curl_easy_cleanup(curl);
                           return false;
                       }
                       sf::sleep(sf::milliseconds(100));  // Rate limiting
                   }
               }
           }

           curl_easy_cleanup(curl);
           return true;
       }
       catch (const std::exception& e) {
           std::cerr << "Error downloading tiles: " << e.what() << std::endl;
           curl_easy_cleanup(curl);
           return false;
       }
   }
   
   bool createMergedMap() {
       try {
           int minX = long2tilex(MIN_LON, ZOOM);
           int maxX = long2tilex(MAX_LON, ZOOM);
           int minY = lat2tiley(MAX_LAT, ZOOM);
           int maxY = lat2tiley(MIN_LAT, ZOOM);
           
           int width = (maxX - minX + 1) * TILE_SIZE;
           int height = (maxY - minY + 1) * TILE_SIZE;
           
           sf::RenderTexture mapTexture;
           if (!mapTexture.create(width, height)) {
               std::cerr << "Failed to create render texture" << std::endl;
               return false;
           }
           
           mapTexture.clear(sf::Color::White);
           
           for(int x = minX; x <= maxX; x++) {
               for(int y = minY; y <= maxY; y++) {
                   std::string filename = "tiles/" + std::to_string(x) + "_" + std::to_string(y) + ".png";
                   
                   sf::Texture tileTexture;
                   if(tileTexture.loadFromFile(filename)) {
                       sf::Sprite tileSprite(tileTexture);
                       tileSprite.setPosition(
                           static_cast<float>((x - minX) * TILE_SIZE),
                           static_cast<float>((y - minY) * TILE_SIZE)
                       );
                       mapTexture.draw(tileSprite);
                   }
               }
           }
           
           mapTexture.display();
           return mapTexture.getTexture().copyToImage().saveToFile("gainesville_map.png");
       }
       catch (const std::exception& e) {
           std::cerr << "Error creating merged map: " << e.what() << std::endl;
           return false;
       }
   }
};