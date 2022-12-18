#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <stdlib.h>
#include <vector>

static constexpr int kDimension = 6000000;
static constexpr int kNumIterations = 1000;
static constexpr int kRandRange = 100;

/// Generate a random double. 
double computeRandDouble() {
    return static_cast<double>(std::rand() % kRandRange);
}

/// Do busywork math. Compute a sum of many square roots. 
double computeSquareRootSum(double initSum) {
    for (int i = 0; i < kNumIterations; i++) { 
        initSum += std::sqrt(static_cast<double>(i));
    }
    return initSum;
} 

/// Call computeSquareRootSum() once for each cell in a vector sequentially. 
void processSingleThread(const std::vector<double>& myVector) {
    long long runningSum = 0;
    std::cout << "Starting single threaded ops." << std::endl;
    const auto start = std::chrono::steady_clock::now();
    for (int vecIndex = 0; vecIndex < kDimension; vecIndex++) {
        runningSum += computeSquareRootSum(myVector[vecIndex]);
    }
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsedSeconds = end-start;
    std::cout << "Completed single threaded ops in " << elapsedSeconds.count() << "s." << std::endl;
    std::cout << "Sum = " << runningSum << std::endl;
}

/// Call computeSquareRootSum() once for each cell in a vector sequentially with specifiable start and end.
long long workerFunction(const std::vector<double>& myVector, int start, int end) {
    long long runningSum = 0;
    for (int vecIndex = start; vecIndex < end; vecIndex++) {
        runningSum += computeSquareRootSum(myVector[vecIndex]);
    }
    return runningSum;
}

/// Call computeSquareRootSum() once for each cell in a vector using async calls.
void processMultiThread(const std::vector<double>& myVector) {
    std::cout << "Starting multi threaded ops." << std::endl;
    const auto start = std::chrono::steady_clock::now();
    // f0 ... f3 are std::futures
    auto f0 = std::async(workerFunction, myVector, 0, myVector.size() / 4);
    auto f1 = std::async(workerFunction, myVector, myVector.size() / 4, 2*myVector.size() / 4);
    auto f2 = std::async(workerFunction, myVector, 2*myVector.size() / 4, 3*myVector.size() / 4);
    auto f3 = std::async(workerFunction, myVector, 3*myVector.size() / 4, myVector.size());
    const long long runningSum = f0.get() + f1.get() + f2.get() + f3.get();
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsedSeconds = end-start;
    std::cout << "Completed multi threaded ops in " << elapsedSeconds.count() << "s. " << std::endl;
    std::cout << "Sum = " << runningSum << std::endl;
}

int main() {
    std::vector<double> myVector(kDimension,  0.0f);
    for (int i = 0; i < kDimension; i++) {
        myVector[i] = computeRandDouble();
    }
    processSingleThread(myVector);
    processMultiThread(myVector);
    return 0;
}