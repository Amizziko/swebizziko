#include "assert.h"
#include "pthread.h"

void* foo(void* arg){
  printf("[Foo] arg is %p\n", arg);
  return (void*)5;
}

int main(){
  printf("Pthread Create test!\n");
  pthread_create(0, 0, &foo, (void*)5);
  return 0;
}