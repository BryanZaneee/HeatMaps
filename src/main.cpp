#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "bryHashMap.h"
#include "RBTree.cpp"
#include <matplot/matplot.h>
using namespace std;
using namespace matplot;

//struct to hold different attributes of the data
struct crashData{
    int caseNumber;
    double latitude;
    double longitude;
    int totalVehicle;
    string crashDay;
};

//data structure to hold unique coordinates such as longtiudes, and latitudes
struct uniqueCoords {
    int caseNumber;
    double latitude;
    double longitude;
    int totalVehicleSum;
   uniqueCoords(int caseNum, double lat, double lon, int sum) : caseNumber(caseNum), latitude(lat), longitude(lon), totalVehicleSum(sum) {}

};

//split the data
vector<string> split(const string& line, char delim){
    vector<string> tokens;
    istringstream tokenStream(line);
    string token;
    while(getline(tokenStream, token, delim)){
        tokens.push_back(token);
    }
    return tokens;
}

//parse the CSV and hold the data in a vector
void parseCSV(const string& filename, vector<crashData>& crashdata, bHash& crashDataMap, RBTree& RbtreeData) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open " << filename << endl;
        exit(EXIT_FAILURE);  // Exit if the file cannot be opened
    }
    
    string line;
    getline(file, line); // Skip header line

    vector<uniqueCoords> uniqueC; //vector for unique data

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');
        if (tokens.size() >= 24) {  // Ensure enough tokens are present
            crashData data;
            try {
                data.caseNumber = stoi(tokens[0]);
                data.latitude = stod(tokens[23]);
                data.longitude = stod(tokens[22]);
                data.totalVehicle = stoi(tokens[15]);
                data.crashDay = tokens[5];

                crashdata.push_back(data);
                //iterate through uniqueC vector and check if there's any duplicates
                auto it = find_if(uniqueC.begin(), uniqueC.end(), [&](const uniqueCoords& uc){
                    return uc.latitude == data.latitude && uc.longitude == data.longitude;
                });

                if(it != uniqueC.end()){
                    it->totalVehicleSum += data.totalVehicle;
                }
                else{
                    uniqueC.emplace_back(data.caseNumber, data.latitude, data.longitude, data.totalVehicle);
                }
            } catch (const exception& e) {
                continue;  // Skip to the next line if there's an error
            }
        }
    }

    //put the data into hashmaps and red&black tree
    for(auto a: uniqueC){
        crashDataMap.AddItem(a.caseNumber,a.latitude, a.longitude, a.totalVehicleSum);  // Add data to the hashmap
        RbtreeData.insert(a.caseNumber, a.latitude, a.longitude, a.totalVehicleSum);
    }
}

//create scatter plot using HASHMAP data structure
void createGraphHash(bHash& crashDataMap){
   vector<double> latitudeVec;
   vector<double> longitudeVec;
   vector<double> totalVehiclesVec;

    for(int i = 0; i < crashDataMap.tableSize; i++){
        auto* ptr = crashDataMap.hashTable[i];
        while(ptr != nullptr && ptr->caseNumber != 0){
            int caseNumber = ptr->caseNumber;
            double latitude = ptr->latitude;
            double longitude = ptr->longitude;
            int totalVehicle = ptr->totalVehicle;
            latitudeVec.push_back(latitude);
            longitudeVec.push_back(longitude);
            totalVehiclesVec.push_back(totalVehicle);
            ptr = ptr->next;
        }
    }

    vector<double> log_totalVehiclesVec;
    transform(totalVehiclesVec.begin(), totalVehiclesVec.end(), std::back_inserter(log_totalVehiclesVec),
              [](double x) { return log(x + 2); });

    vector<double> scaledLogTotalVehiclesVec;
    transform(log_totalVehiclesVec.begin(), log_totalVehiclesVec.end(), back_inserter(scaledLogTotalVehiclesVec),
              [](double x) { return x * 0.3; });  // Adjust the scaling factor as needed

    auto color = linspace(1, 10, totalVehiclesVec.size());

    xlim({-82.1, -82.5});
    ylim({29.5734, 29.75});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");
    colorbar();

    scatter(longitudeVec, latitudeVec, scaledLogTotalVehiclesVec, color);
    save("Images/MapHash.png", "png");
    show();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//create scatter plot using RED and BLACK tree
void createGraphRB(RBTree& RbtreeData) {
    vector<double> latitudeVec;
    vector<double> longitudeVec;
    vector<double> totalVehiclesVec;

    //retrieve the data through inorder traversal, and add it to the vector,
    RbtreeData.inorder(latitudeVec, longitudeVec, totalVehiclesVec);


    vector<double> log_totalVehiclesVec;
    transform(totalVehiclesVec.begin(), totalVehiclesVec.end(), std::back_inserter(log_totalVehiclesVec),
              [](double x) { return log(x + 2); });

    vector<double> scaledLogTotalVehiclesVec;
    transform(log_totalVehiclesVec.begin(), log_totalVehiclesVec.end(), back_inserter(scaledLogTotalVehiclesVec),
              [](double x) { return x * 0.3; });  // Adjust the scaling factor as needed

    auto color = linspace(1, 10, totalVehiclesVec.size());
    //set parameters
    xlim({-82.1, -82.5});
    ylim({29.5734, 29.75});
    title("Scatter Plot of Number of Vehicles Involved In Traffic Crashes");
    xlabel("Longitude");
    ylabel("Latitude");
    zlabel("Total Vehicles");
    colorbar();
    scatter(longitudeVec, latitudeVec, scaledLogTotalVehiclesVec, color);
    save("Images/MapR&B.png", "png");
    show();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//menu takes input from user for which data structure to use
void menu(bHash& crashDataMap, RBTree& RbtreeData){
    int option;

    while (true) {
        cout << "Choose an option: " << endl;
        cout << "Option 1: Hash Map Data Structure" << endl;
        cout << "Option 2: Red and Black Tree Structure" << endl;
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
        createGraphHash(crashDataMap);
    } else if (option == 2) {
        cout << "Generating Map Using Red and Black Tree..." << endl;
        createGraphRB(RbtreeData);
    }
}

//main function
int main() {
    string filename = "Traffic_Crashes.csv"; //hold filename
    vector<crashData> crashdata; //vector of crashData instance
    bHash crashDataMap; //create hashmap instance
    RBTree RbtreeData; //create R&B tree instance
    parseCSV(filename, crashdata, crashDataMap, RbtreeData); //parse the data
    menu(crashDataMap, RbtreeData);  //menu function

    //for testing
    //createGraphHash(crashDataMap);
    //createGraphRB(RbtreeData);

    return 0;
}
