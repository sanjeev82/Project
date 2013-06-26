//
//  File.cpp
//  FirstCV
//
//  Created by Sanjeev Singh on 6/4/13.
//  Copyright (c) 2013 Sanjeev Singh. All rights reserved.
//

#include "File.h"
#include <iostream>
#include <pthread.h>
using namespace std;

#define NUM_THREADS     5

struct thread_data{
    int  thread_id;
    char *message;
};

void *PrintHello(void *threadarg)
{
    struct thread_data *my_data;
    
    my_data = (struct thread_data *) threadarg;
    
    cout << "\nThread ID : " << my_data->thread_id ;
    cout << "\n Message : " << my_data->message << endl;
    pthread_exit(NULL);
}

int mainth (int argc, char** argv)
{
    /*
     pthread_t t1;
     
     pthread_create(&t1, NULL, &print_message, NULL);
     cout << "Hello";
     */
    pthread_t threads[NUM_THREADS];
    struct thread_data td[NUM_THREADS];
    int rc;
    int i;
    for( i=0; i < NUM_THREADS; i++ ){
        cout << "main() : creating thread, " << i << endl;
        td[i].thread_id=i;
        td[i].message = "Message";
        rc = pthread_create(&threads[i], NULL,
                            PrintHello, (void *)&td[i]);
        if (rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
    return 0;
}