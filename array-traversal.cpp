#include<iostream>
#include<vector>
#include<chrono>

int main(){

    int n = 10000;
    std::vector<std::vector<int>> arr(n , std::vector<int>(n,0));



    int rows = arr.size();
    int col = arr[0].size();

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0 ; i < rows ; i++){
    
        for(int j = 0 ; j < col ; j++){
            arr[i][j]++;
        
        }

    
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << " Elapsed Time : " << diff.count() << "s\n";

    // std::cout << "Verification: " << arr[4999][4999] << "\n";

 
}



