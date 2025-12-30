#include<iostream>
#include<vector>
#include<chrono>



int main(){

    const int n = 5000;
    std::vector<int>arr(n*n,0);

    // Row Major Traversal.
    auto s1 = std::chrono::high_resolution_clock::now();
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++){
            arr[i * n + j]++;
        }
    }

    auto e1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = e1 - s1;


    // Column Major Traversal.
    auto s2 = std::chrono::high_resolution_clock::now();
    for(int j = 0 ; j < n ; j++){
        for(int i = 0 ; i < n ; i++){
            arr[i * n + j]++;
        }
    }


    auto e2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff2 = e2 - s2;


    std::cout << "Flat Row Major - " << diff1.count() << "s\n";
    std::cout << "Flat Column Major - " << diff2.count() << "s\n";


    // to prevent optimization deletion.
    std::cout << "Verify - " << arr[0] << "and" << arr[n*n-1] << "\n";


    return 0;
}
