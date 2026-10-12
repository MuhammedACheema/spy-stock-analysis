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


// will read through the file, organize the data, and populate our vector 
std::vector<RowBar>loadData(const std::string &filename){
    std::vector<RowBar> bar;
    std::ifstream file(filename);
    if(!file.is_open()){
        // this is for errors
        std::cerr << "Error: Could not open the file." << std::endl;
        return bar;
    } 
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
            row.volume = std::stoll(cell);
        }
        bar.emplace_back(row);
    }
    return bar;
}

// percent made or lost based on x days held:
double profit(const std::string &startDate, const std::string &endDate,const std::vector<RowBar> &data){
    int startDateIndex = -1;
    int endDateIndex = -1;
    for(unsigned int i = 0; i < data.size(); i++){
        if (data[i].date == startDate){
            startDateIndex = i;
        }
        else if (data[i].date == endDate){
            endDateIndex = i;
            break;
        }
    }

    if (startDateIndex == -1 || endDateIndex == -1){
       std::cerr << "values are invalid" << "\n";
        return 0;
    }
    // create formula
    double numerator = data[endDateIndex].close - data[startDateIndex].close;
    double value = numerator/data[startDateIndex].close;
    return value;
}

int main(){
    // opens the file and populates the data
    std::string filename = "spy_us_d.csv";
    std::vector<RowBar> bar = loadData(filename);

    // load data check
    std::cout << bar.size() << "\n";
    std::cout << bar.front().date <<"\n";
    std::cout << bar.front().close <<"\n";
    std::cout << bar.back().close <<"\n";
    std::cout << bar.back().date <<"\n";


    // fail data this is a weekend
    std::cout << profit("2005-02-26","2005-02-28",bar) << "\n";

    // success data 
    std::cout << profit("2005-02-25","2005-02-28",bar) << "\n";

}
