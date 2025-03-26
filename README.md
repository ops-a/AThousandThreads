
## Statement

> Design a thread management library that supports efficient creation, synchronization, and termination of threads. The library should be scalable to handle thousands of threads concurrently, suitable for high-performance computing applications.


## Tests

### Compile tests:

`g++ -std=c++17 -pthread -o thread_manager_test thread_manager_test.cpp ../src/thread_manager.cpp`

`g++ -std=c++17 -pthread -o synchronization_test synchronization_test.cpp ../synchronization/mutex.cpp ../synchronization/condition_variable.cpp`


### Run tests:

`./thread_manager <num_threads>`

`./synchronization_test`

