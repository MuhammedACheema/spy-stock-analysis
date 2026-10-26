#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>

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

std::vector<double> dailyReturns(const std::vector<RowBar>& data){
    std::vector<double> daily;
    for(unsigned int i = 1; i < data.size(); i++){
        double numerator = data[i].close - data[i-1].close;
        double value = numerator/data[i-1].close;
        daily.emplace_back(value);
    }
    return daily;
}

double avgReturns(const std::vector<double> &data){
    if(data.size() == 0){
        std::cerr << "data provided not valid" << "\n";
        return 0;
    }
    double val = 0; 
    for(unsigned int i = 0; i < data.size(); i++){
        val += data[i];
    }
    double ans = val / data.size();
    return ans;
}

double avgWindow(const std::vector<RowBar> &data, const std::string &date,int days){
    double total = 0;
    int startIndex = -1;
    for (int i = data.size()-1; i >= 0; i--){
        if(data[i].date == date){
            startIndex = i;
            break;
        }
    }    
    if (startIndex == -1){
       std::cerr << "values are invalid" << "\n";
        return 0;
    }
    int batch = startIndex - days;
    for(int i = startIndex; i > batch; i--){
        if(i < 0){
            std::cerr << "values out of range" << "\n";
            return 0;
        }
        else{
            total += data[i].close;
        }
    }
    return total / days;
}

double compoundCheck(const std::vector<double> &data){
    double curr = 1;
    for(unsigned int i = 0; i < data.size(); i++){
        double val = 1 + data[i];
        curr = curr * val;
    }
    return curr;
}

double volatility(const std::vector<double> &data){
    if(data.size() <= 1){
        std::cerr << "data provided not valid" << "\n";
        return 0;
    }
    // this will give the mean
    double mean = avgReturns(data);
    double total = 0; 

    for(unsigned int i = 0; i < data.size(); i++){
        double spread = mean - data[i];
        spread = spread * spread;
        total += spread;
    }
    // this is a sample std because we are only taking a couple pieces of it
    double inside = total / (data.size() - 1);

    return std::sqrt(inside);
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

    // // success data 
    // std::cout << profit("2005-02-25","2005-02-28",bar) << "\n";

    std::vector<double> ans = dailyReturns(bar);

    std::cout<<ans.size() << "\n";
    std::cout << avgReturns(ans) << "\n";
    std::cout << compoundCheck(ans) << "\n";
    std::cout<< "the avg close is for 20 days from 2005-06-29: " << avgWindow(bar,"2005-06-29", 20) <<"\n"; 
    std::cout<< "this case should fail is the 18th index: " << avgWindow(bar,"2005-03-23", 20) <<"\n"; 
    std::cout<< "this case should pass it is the 19th index: " << avgWindow(bar,"2005-03-24", 20) <<"\n"; 

    std::cout << volatility(ans) << "\n";
    std::cout << "Annualized volatility: " << std::sqrt(252) * volatility(ans) << "\n";

}
