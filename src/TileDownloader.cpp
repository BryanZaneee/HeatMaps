#include "TileDownloader.h"

bool setupMapResources() {
   TileDownloader downloader;
   
   std::cout << "Downloading map tiles..." << std::endl;
   if (!downloader.downloadTiles()) {
       std::cerr << "Failed to download map tiles" << std::endl;
       return false;
   }
   
   std::cout << "Creating merged map..." << std::endl;
   if (!downloader.createMergedMap()) {
       std::cerr << "Failed to create merged map" << std::endl;
       return false;
   }
   
   return true;
}

// This function should be called before visualization
void initializeMap() {
   if (!setupMapResources()) {
       std::cerr << "Map initialization failed" << std::endl;
       exit(1);
   }
   std::cout << "Map initialization complete" << std::endl;
}