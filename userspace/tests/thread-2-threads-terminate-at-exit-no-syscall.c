#include "assert.h"
#include "pthread.h"

void* foo(void* arg){
  while (1);
  return (void*)5;
}

int main(){
  printf("Thread termination test!\n");
  pthread_create(0, 0, &foo, (void*)5);
  printf("press a key to terminate\n");
  getchar();
  return 0;
}