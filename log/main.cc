/**
 * @file main.cc
 * @author Jamishon
 * @brief Log function
 * @version 0.1
 * @date 2022-09-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "log.h"
#include <pthread.h>
#include <unistd.h>

int main() {
  Log log;
  int i = 10;
  log.Start();
  while (i--) {
  std::string* pstr = new std::string("Hello world !");
  log.Print(INFO, pstr);
  }
  sleep(2);
  log.Stop();
  pthread_join(log.LogThread(), NULL);

}