/**
 * @file main.cc
 * @author Jamishon
 * @brief 
 * @version 0.1
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "executor.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

void fun(void* p) {
  int* a = static_cast<int *>(p);
  std::ostringstream oss;
  oss << std::this_thread::get_id();
  printf("%s output : %d\n", oss.str().c_str(), *a);
  delete a;
  //std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
   Executor executor(5);
   executor.Execute();
   
   long long index = 0;
   while(1 && index < 1000) {
     long long *num = new long long(index++);
     executor.PostTask(fun, num);
     printf("PostTask num:%d\n",*num);
    
   }

   //std::this_thread::sleep_for(std::chrono::seconds(5));
  
   executor.Stop();
}
