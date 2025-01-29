#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <mutex>

#define BUFFER_SIZE 5

using namespace std;

int buffer[BUFFER_SIZE];  
int in = 0;               
int out = 0;              

mutex cout_mutex;  

void wait_some_time() {
    this_thread::sleep_for(chrono::milliseconds(rand() % 150 + 50));
}

void producer() {
    for (int item = 0; item < 10; item++) {
        buffer[in] = item;
        
        { 
            lock_guard<mutex> lock(cout_mutex);
            cout << "Produced: " << item << " at index " << in << endl;
        }
        
        in = (in + 1) % BUFFER_SIZE;
        wait_some_time();
    }
}

void consumer() {
    for (int i = 0; i < 10; i++) {
        int item = buffer[out]; 
        
        { 
            lock_guard<mutex> lock(cout_mutex);
            cout << "Consumed: " << item << " from index " << out << endl;
        }
        
        out = (out + 1) % BUFFER_SIZE;
        wait_some_time(); 
    }
}

int main() {
    srand(time(0));

    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}
