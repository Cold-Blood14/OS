#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h> // For sleep()
#include <vector>
#include <mutex>

using namespace std;

#define NUM_PHILOSOPHERS 5
#define itr_cnt 1
sem_t forks[NUM_PHILOSOPHERS]; // Semaphores for forks
mutex cout_mutex;

void think(int i) {
    {
        lock_guard<mutex> lock(cout_mutex);
        cout<<'\n';
        cout << "Philosopher " << i << " is thinking." << endl;
        cout<<'\n';
    }
    sleep(rand() % 5 + 2);
}

void eat(int i) {
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " is eating." << endl;
    }
    sleep(rand() % 5 + 2);
}

void pick_up_forks(int i) {
    if (i % 2 == 0) {
        sem_wait(&forks[i]);
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " picked up fork " << i << "." << endl;
        }
        while (sem_trywait(&forks[(i + 1) % NUM_PHILOSOPHERS]) != 0) {
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Philosopher " << i << " is waiting for fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
            }
            sleep(100);
        }
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " picked up fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
        }
    } else {
        sem_wait(&forks[(i + 1) % NUM_PHILOSOPHERS]);
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " picked up fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
        }
        while (sem_trywait(&forks[i]) != 0) {
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "Philosopher " << i << " is waiting for fork " << i << "." << endl;
            }
            sleep(100);
        }
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Philosopher " << i << " picked up fork " << i << "." << endl;
        }
    }
}

void put_down_forks(int i) {
    sem_post(&forks[i]);
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " put down fork " << i << "." << endl;
    }
    sem_post(&forks[(i + 1) % NUM_PHILOSOPHERS]);
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Philosopher " << i << " put down fork " << (i + 1) % NUM_PHILOSOPHERS << "." << endl;
    }
    think(i);
}

void philosopher(int i) {
    for(int x=0; x<itr_cnt;x++){
    think(i);
    pick_up_forks(i);
    eat(i);
    put_down_forks(i);}
}

int main() {
    vector<thread> philosophers;
    srand(time(NULL));

    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.push_back(thread(philosopher, i));
    }

    // Join threads
    for (auto &t : philosophers) {
        t.join();
    }

    return 0;
}
