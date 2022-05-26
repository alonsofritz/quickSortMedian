#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

void swap(int * a, int * b) {
    int t = * a;
    * a = * b;
    * b = t;
}

void printArray(vector < int > & data, int size) {
    for (const auto & i: data) {
        cout << i << " ";
    }
    cout << endl;
}

int partition(vector < int > & data, int low, int high) {

    int pivot = data[high];

    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if (data[j] <= pivot) {

            i++;
            swap( & data[i], & data[j]);

        }
    }

    swap( & data[i + 1], & data[high]);

    return (i + 1);
}

void quickSort(vector < int > & data, int low, int high) {
    if (low < high) {

        int pi = partition(data, low, high);
        
        quickSort(data, low, pi - 1);
        quickSort(data, pi + 1, high);
    }
}

float getMedian(vector < int > & data) {

    float median;

    if (data.size() % 2) {

        int half_index = (data.size()) / 2;
        median = data[half_index];

    } else {
        int half_right_index = (data.size()) / 2;
        int half_left_index = half_right_index - 1;

        median = (data[half_left_index] + data[half_right_index]) / 2.0;
    }

    return median;
}

bool getFileContent(string fileName, vector < int > & data) {

    ifstream in (fileName);

    if (! in .is_open()) {
        cerr << "Cannot open the File : " << fileName << endl;
        return false;
    }

    int num;

    while ( in >> num) {
        data.push_back(num);
    }

    in .close();

    return true;
}

// Driver code
int main() {

    ofstream csvFile;
    csvFile.open("resultsQuickSort.csv");

    const int numTests = 5;
    const string path = "./entry";

    for (const auto & entry: fs::directory_iterator(path)) {

        csvFile << (entry.path().string()).substr(path.length() + 1) << ", ";
        int median = 0;

        for (int countState = 0; countState < numTests; countState++) {

            vector < int > data;
            getFileContent(entry.path().string(), data);
            int dataSize = data.size();

            auto start = std::chrono::high_resolution_clock::now();
            quickSort(data, 0, dataSize - 1);
            median = getMedian(data);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration < double, std::milli > float_ms = end - start;

            csvFile << float_ms.count() << ", ";
        }

        csvFile << median << ", ";
        csvFile << "\n";

    }

    cout << "Execution Completed Successfully!!" << endl;

}