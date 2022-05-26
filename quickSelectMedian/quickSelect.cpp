#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

int partition(vector<int> & data, int left_ordered_index, int last_array_index) {

    int x = data[last_array_index];
    int i = left_ordered_index;
    int auxSwap = 0;

    for(int j = left_ordered_index; j < last_array_index; j++) {
        if(data[j] <= x) {
            auxSwap = data[i];
            data[i] = data[j];
            data[j] = auxSwap;
            i++;
        }
    }
    
    auxSwap = data[i];
    data[i] = data[last_array_index];
    data[last_array_index] = auxSwap;

    return i;
}

int get_k_smallest(vector<int> & data, int left_ordered_index, int last_array_index, int k) {

    if(k > 0 and k <= (last_array_index - left_ordered_index + 1)) {

        int position = partition(data, left_ordered_index, last_array_index);

        if((position - left_ordered_index) == (k - 1)) {
            return data[position];
        }

        if((position - left_ordered_index) > (k - 1)) {
            return get_k_smallest(data, left_ordered_index, (position - 1), k);
        }

        return get_k_smallest(data, (position + 1), last_array_index, (k - position + left_ordered_index - 1));
    }

    return numeric_limits<int>::max();
}

float getMedian (vector<int> & data) {

    float median;

    if(data.size() % 2) {

        int half_index = (data.size() + 1) / 2;
        median = get_k_smallest(data, 0, data.size() - 1, half_index);

    } else {
        int half_left_index = (data.size()) / 2;
        int half_right_index = half_left_index + 1;

        int median_left = get_k_smallest(data, 0, data.size() - 1, half_left_index);
        int median_right = get_k_smallest(data, 0, data.size() - 1, half_right_index);

        median = (median_left + median_right) / 2.0;
    }

    return median;
}

bool getFileContent (string fileName, vector<int> & data) {

    ifstream in(fileName);

    if(!in.is_open()) {
        cerr << "Cannot open the File : "<< fileName << endl;
        return false;
    }

    int num;

    while (in >> num)
    {
        data.push_back(num);
    }

    in.close();

    return true;
}

int main() {
  
  ofstream csvFile;
  csvFile.open ("resultsQuickSelect.csv");

  const int numTests = 5;
  const string path = "./entry";

    for (const auto & entry : fs::directory_iterator(path)) {

    csvFile << (entry.path().string()).substr(path.length() + 1) << ", ";
    float median = 0;

    for (int countState = 0; countState < numTests; countState++) {

        vector<int> data;
        getFileContent(entry.path().string(), data);
        int dataSize = data.size();
        
        auto start = std::chrono::high_resolution_clock::now();
        median = getMedian(data);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> float_ms = end - start;

        csvFile << float_ms.count() << ", ";
    }

    csvFile << median << ", ";
    csvFile << "\n";

    }

    cout << "Execution Completed Successfully!!" << endl;

}