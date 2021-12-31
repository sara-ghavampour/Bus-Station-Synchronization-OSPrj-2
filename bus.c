// Fill in empty functions and defintions

#include "pintos_headers.h"

struct station {
    struct condition bus_is_loading;// cond1 // condition is needed for bus_load_passengers and passenger_waitfor_bus
    struct condition passenger_is_taking_bus;//cond2 // condition is needed for passenger_on_board
    struct lock station_lock;
    int station_passengers;

};

void station_setup(struct station *station) {
    // intialize conditions and lock
    cond_init(&station->bus_is_loading);  // intialize bus_is_loading condotion 
    cond_init(&station->passenger_is_taking_bus); // intialize passenger_is_taking_bus condotion 
    lock_init(&station->station_lock);  // intialize the inly lock in station
    station->station_passengers = 0; // first, station is empty
}

void bus_load_passengers(struct station *station, int count) {
    lock_acquire(&station->station_lock); //Acquires lock for the current thread
    while(station->station_passengers>0 && count>0){ // while there is a passenger in station and bus has empty seats this thread must be executed
        cond_signal(&station->bus_is_loading,&station->station_lock); //If any threads are waiting on cond (protected by lock), then this function wakes up one of them
        count--; // by loading each passenger, one seat is taken
        station->station_passengers--;
        cond_wait(&station->passenger_is_taking_bus,&station->station_lock); // one passenger took a bus //Atomically releases lock  and waits for passenger_is_taking_bus to be signaled by passenger_on_board

    }
    lock_release(&station->station_lock);//Releases lock
}

void passenger_waitfor_bus(struct station *station) {
    lock_acquire(&station->station_lock);
    station->station_passengers++;  // passenger entered the station
    cond_wait(&station->bus_is_loading,&station->station_lock); // this thread must not end till bus is not in station // so it should wait for bus_is_loading condition
    lock_release(&station->station_lock);

}

void passenger_on_board(struct station *station) {
    lock_acquire(&station->station_lock);
    cond_signal(&station->passenger_is_taking_bus,&station->station_lock); /// passenger is taking a bus so passenger_is_taking_bus is signalled  
    lock_release(&station->station_lock);
}
