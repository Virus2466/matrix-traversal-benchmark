# Matrix-traversal-benchmark ( Cache Locality Benchmark)

## Environment
- Tested on **Debian via Windows Subsystem for Linux 2 (WSL2)**
- CPU: [Intel i7-11800H]
- Compiled with `g++ -O3  -ffast-math -march=native -g`
- Note: WSL2 introduces minimal virtualization overhead for CPU/cache benchmarks, but the relative difference between row-major and column-major traversal remains clearly visible (and often amplified due to higher memory latency on misses).

Results may vary slightly on native Linux, but the core lesson  align traversal with storage order for cache efficiency holds everywhere.

A practical demonstration of how memory layout and traversal order affect performance in C++. I started with the beginner-friendly std::vector<std::vector<int>>, saw the slowdowns, then optimized to a contiguous flat 1D vector — with benchmarks, perf stats, and explanations.
Key Insight: Cache misses dominate aligning access with storage order can yield 20–50× speedups


## Started With std::vector<std::vector<int>>
Began with the common way to represent 2D Matrix using vectors of vectors. It's easy and intuitive , but each row has seperate allocation. 

### Results with vector<vector<int>> ( n = 5000 x 5000 )
| Traversal              | Time          | L1 Cache Miss Rate (approx) | Notes                          |
|------------------------|---------------|-----------------------------|--------------------------------|
| Row-major (fast)      | ~0.7–0.8s       | ~15–30%                    | Better locality within rows   |
| Column-major (slow)   | ~1–2s           | ~90–99%                    | Huge strides + fragmentation  |

### Row Major 
<img width="1342" height="83" alt="1" src="https://github.com/user-attachments/assets/f132da9f-26a2-47e8-b165-943532e58b93" />

## Column Major 
<img width="1359" height="97" alt="2" src="https://github.com/user-attachments/assets/505fd363-464c-4f8a-8697-d82fde1d661d" />

## Code Example 
```cpp
std::vector<std::vector<int>> arr(n, std::vector<int>(n, 0));

// Row-major (better)
for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
        arr[i][j]++;

// Column-major (terrible here)
for (int j = 0; j < n; ++j)
    for (int i = 0; i < n; ++i)
        arr[i][j]++;
```


## Part 2 : Switching to Flat 1D Vector 
Use a single std::vector<int> with manual indexing -> full line usage -> fast!.

### Results with vector<vector<int>> ( n = 10000 )
| Traversal              | Time          | L1 Cache Miss Rate (approx) | Notes                          |
|------------------------|---------------|-----------------------------|--------------------------------|
| Row-major       | ~0.08s       | ~15–30%                    |20-30x Faster   |
| Column-major    | ~0.7s     | ~90–99%                    | Sill Slow due to Strides. |

<img width="1310" height="109" alt="Screenshot 2025-12-30 152705" src="https://github.com/user-attachments/assets/8c666b2f-9a2b-41f2-b226-be8a4e29bada" />


## Code Example

```cpp

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

```

# Key Lesson Learned
- vector<vector> is convenient but slow for large matrices.
- Use flat 1D arrays for performance-critical code.
- Always traverse in storage order (row-major in C++).
- Cache misses are the real bottleneck — profile with perf!



# Perf Insights [ n = 10000 ] 
- Perf flags : `perf stat -r 5`

### Row-Major Traversal
<img width="1623" height="488" alt="Screenshot 2025-12-30 160642" src="https://github.com/user-attachments/assets/edba29b0-b2a0-4803-a71c-5ab603025f42" />

### Column Major Travesral
<img width="1585" height="488" alt="Screenshot 2025-12-30 160850" src="https://github.com/user-attachments/assets/93a13747-bd1b-4904-98b5-0cc83d2dc0ca" />


### Flat 1D Vector
<img width="1675" height="706" alt="Screenshot 2025-12-30 161052" src="https://github.com/user-attachments/assets/ce5a8dcf-9450-4c34-a484-70df49f160b1" />


