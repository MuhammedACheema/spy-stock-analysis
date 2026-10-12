#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

struct RowBar{
    std::string date;
    double open;
    double high;
    double low; 
    double close; 
    long long volume;
};

std::vector<RowBar> bar;

void loadData(std::ifstream &file){
    // will be used to populate bar
    std::string line;
    if (!std::getline(file, line)) {
        std::cerr << "Error: File is completely empty!" << std::endl;
    }
    while(std::getline(file,line)){
        RowBar row;
        std::stringstream sect(line);
        std::string cell; 
        // organizes all the columns into the struct and grabs them one by one
        if(std::getline(sect, cell, ',')){
            row.date = cell;
        }
        if(std::getline(sect, cell, ',')){
            row.open = std::stod(cell);
        }
        if(std::getline(sect, cell, ',')){
            row.high = std::stod(cell);
        }
        if(std::getline(sect, cell, ',')){
            row.low = std::stod(cell);
        }
        if(std::getline(sect, cell, ',')){
            row.close = std::stod(cell);
        }
        if(std::getline(sect, cell, ',')){
            row.volume = std::stod(cell);
        }
        bar.emplace_back(row);
    }
}
int main(){
    // opens the file
    std::ifstream file("spy_us_d.csv");
    if(!file.is_open()){
        // this is for errors
    std::cerr << "Error: Could not open the file." << std::endl;
        return 1;
    } 
    else{
        std::cout << "could open file" << std::endl;
    }
    loadData(file);
    for(const auto & val:bar){
        std::cout << val.date << std::endl;
    }

    // load data check
    std::cout << bar.size() << "\n";
    std::cout << bar.front().date <<"\n";
    std::cout << bar.front().close <<"\n";
    std::cout << bar.back().close <<"\n";
    std::cout << bar.back().date <<"\n";
}