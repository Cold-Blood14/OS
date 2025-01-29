#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <unistd.h> // For sleep()

using namespace std;

#define NUM_PHILOSOPHERS 5

enum State { THINKING, HUNGRY, EATING };
mutex cout_mtx;

class Monitor {
private:
    mutex mtx;
    condition_variable cv[NUM_PHILOSOPHERS];
    State state[NUM_PHILOSOPHERS];
    bool forks[NUM_PHILOSOPHERS];
    mutex cout_mtx;

public:
    Monitor() {
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            state[i] = THINKING;
            forks[i] = true;
        }
    }

    void test(int i) {
        if (state[i] == HUNGRY && forks[i] && forks[(i + 1) % NUM_PHILOSOPHERS]) {
            state[i] = EATING;
            forks[i] = forks[(i + 1) % NUM_PHILOSOPHERS] = false;
            cv[i].notify_one();
        }
    }

    void pickup_forks(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = HUNGRY;
        {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Philosopher " << i << " is HUNGRY" << endl;
        }
        test(i);
        while (state[i] != EATING) {
            cv[i].wait(lock);
        }
        {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Philosopher " << i << " picked up forks " << i << " and " << (i + 1) % NUM_PHILOSOPHERS << endl;
        }
    }

    void putdown_forks(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = THINKING;
        forks[i] = forks[(i + 1) % NUM_PHILOSOPHERS] = true;
        {
            lock_guard<mutex> cout_lock(cout_mtx);
            cout << "Philosopher " << i << " put down forks " << i << " and " << (i + 1) % NUM_PHILOSOPHERS << endl;
        }
        test((i + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
        test((i + 1) % NUM_PHILOSOPHERS);
    }
};

Monitor monitor;

void think(int i) {
    {
        lock_guard<mutex> cout_lock(cout_mtx);
        cout << "Philosopher " << i << " is THINKING" << endl;
    }
    sleep(rand() % 3 + 1);
}

void eat(int i) {
    {
        lock_guard<mutex> cout_lock(cout_mtx);
        cout << "Philosopher " << i << " is EATING" << endl;
    }
    sleep(rand() % 3 + 1);
}

void philosopher(int i) {
    think(i);
    monitor.pickup_forks(i);
    eat(i);
    monitor.putdown_forks(i);
}

int main() {
    vector<thread> philosophers;
    srand(time(NULL));

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers.push_back(thread(philosopher, i));
    }

    for (auto &t : philosophers) {
        t.join();
    }

    return 0;
}
