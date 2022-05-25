// Quick sort in C++

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// function to print the array
void printArray(vector<int> & data, int size) {
  for (const auto &i : data) {
        cout << i << " ";
  }
  cout << endl;
}

// function to rearrange array (find the partition point)
int partition(vector<int> & data, int low, int high) {
    
  // select the rightmost element as pivot
  int pivot = data[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++) {
    if (data[j] <= pivot) {
        
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&data[i], &data[j]);
    }
  }
  
  // swap pivot with the greater element at i
  swap(&data[i + 1], &data[high]);
  
  // return the partition point
  return (i + 1);
}

void quickSort(vector<int> & data, int low, int high) {
  if (low < high) {
      
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on righ of pivot
    int pi = partition(data, low, high);

    // recursive call on the left of pivot
    quickSort(data, low, pi - 1);

    // recursive call on the right of pivot
    quickSort(data, pi + 1, high);
  }
}

float getMedian (vector<int> & data) {

    float median;

    if(data.size() % 2) {
      
      int half_index = (data.size()) / 2;
      median = data[half_index];

    } else {
      int half_right_index = (data.size()) / 2;
      int half_left_index = half_right_index - 1;

      median = (data[half_left_index] + data[half_right_index]) / 2.0;
    }
    
    return median;
}

bool getFileContent (string fileName, vector<int> & data) {
  // Open the File
    ifstream in(fileName);
  
    // Check if object is valid
    if(!in.is_open()) {
        cerr << "Cannot open the File : "<< fileName << endl;
        return false;
    }
    
    int num;

    // Read the next line from File untill it reaches the end.
    while (in >> num)
    {
        data.push_back(num);
    }
    //Close The File
    in.close();

    return true;
}

void appendCSVFile() {

}

// Driver code
int main() {
  
  ofstream csvFile;
  csvFile.open ("results.csv");

  const int numTests = 4;
  const string path = "./entry/teste";

  // Retorna o PATH de todos os Files para Teste: Para cada entrada, deve-se rodar o programa X vezes
  for (const auto & entry : fs::directory_iterator(path)) {

    csvFile << (entry.path().string()).substr(path.length() + 1) << ", ";

    for (int countState = 0; countState < numTests; countState++) {
      // Vetor que recera o Arquivo para ordenar
      vector<int> data;
      // Carrega arquivo no Vetor
      getFileContent(entry.path().string(), data);
      // define tamanho do vetor
      int dataSize = data.size();
      
      // perform quicksort on data e procura pela media do vetor ordenado
      auto start = std::chrono::high_resolution_clock::now();
      quickSort(data, 0, dataSize - 1);
      cout << getMedian(data) << endl;
      auto end = std::chrono::high_resolution_clock::now();

      //Anota o tempo de execucao, precisamos colocar num CSV
      std::chrono::duration<double, std::milli> float_ms = end - start;

      csvFile << float_ms.count() << ", ";
      std::cout << "quickSort() elapsed time is " << float_ms.count() << " milliseconds" << std::endl;
    }
    
    csvFile << "\n";

  }
}