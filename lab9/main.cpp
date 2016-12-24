#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <queue>


void StreamSample(int n, int m, int repeats, char const * path) {
    srand((int)time(NULL));

    int stream[n], result[n], sample[m];
    memset(result, 0, n * sizeof(int));
    
    for (int i = 0; i < n; stream[i] = i + 1, ++i) { }
    
    std::random_shuffle(stream, stream + n);
    
    for (;repeats--;) {
        for (int i = 0; i < n; ++i) {
            if (i < m) {
                sample[i] = stream[i];
                continue;
            }
            if ((double)rand() / RAND_MAX <= (double)m / (i + 1)) {
                sample[rand() % m] = stream[i];
            }
        }
        for (int i = 0; i < m; ++i) {
            ++result[sample[i] - 1];
        }
    }

    std::ofstream fstream(path, std::ios::out | std::ios::trunc);
    std::copy(result, result + n, std::ostream_iterator<int>(fstream, " "));
    fstream.close();
}

struct WeightedEl {
    int el, weight;
};

struct KeyedEl {
    int el;
    double key;
};

struct CompareKeyedEl {
    bool operator()(const KeyedEl &el1, const KeyedEl &el2) {
        return el1.key > el2.key;
    }
};

void WeightedStreamSample(int n, int m, int repeats, char const * path, int (* weight)(int)) {
    srand((int)time(NULL));

    int result[n];
    memset(result, 0, n * sizeof(int));

    struct WeightedEl *weighted_stream = new struct WeightedEl[n];
    for (int i = 0; i < n; ++i) {
        weighted_stream[i] = { i + 1, weight(i + 1) };
    }

    std::random_shuffle(weighted_stream, weighted_stream + n);
    
    for (;repeats--;) {
        std::priority_queue<KeyedEl, std::vector<KeyedEl>, CompareKeyedEl> min_queue;
        
        for (int i = 0; i < n; ++i) {
            double r = pow((double)rand() / RAND_MAX, 1. / weighted_stream[i].weight);
            struct KeyedEl el = { weighted_stream[i].el, r };
            if (min_queue.size() < m) {
                min_queue.push(el);
            } else {
                if (min_queue.top().key < r) {
                    min_queue.pop();
                    min_queue.push(el);
                }
            }
        }
        
        for (int i = 0; i < min_queue.size(); ++i) {
            ++result[min_queue.top().el - 1];
            min_queue.pop();
        }
    }
    
    std::ofstream fstream(path, std::ios::out | std::ios::trunc);
    std::copy(result, result + n, std::ostream_iterator<int>(fstream, " "));
    fstream.close();
}

int LinearWeight(int val) {
    return val;
}

int SquareWeight(int val) {
    return val * val;
}

int main(int argc, char * argv[]) {
    StreamSample(100, 5, 2000, "sample100-5.txt");
    StreamSample(1000, 10, 2000, "sample1000-10.txt");
    StreamSample(1000, 20, 2000, "sample1000-20.txt");
    StreamSample(10000, 100, 2000, "sample10000-100.txt");

    WeightedStreamSample(100, 5, 2000, "weightedSample100-5.txt", LinearWeight);
    WeightedStreamSample(1000, 10, 2000, "weightedSample1000-10.txt", LinearWeight);
    WeightedStreamSample(1000, 20, 2000, "weightedSample1000-20.txt", LinearWeight);
    WeightedStreamSample(10000, 100, 2000, "weightedSample10000-100.txt", LinearWeight);
    
    WeightedStreamSample(100, 5, 2000, "squareWeightedSample100-5.txt", SquareWeight);
    WeightedStreamSample(1000, 10, 2000, "squareWeightedSample1000-10.txt", SquareWeight);
    WeightedStreamSample(1000, 20, 2000, "squareWeightedSample1000-20.txt", SquareWeight);
    WeightedStreamSample(10000, 100, 2000, "squareWeightedSample10000-100.txt", SquareWeight);
    
    return 0;
}
