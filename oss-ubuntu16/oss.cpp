/*
*  g++ oss.cpp -o oss -std=c++11
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <errno.h>
using namespace std;

__attribute__((constructor))

void set_timeout(void) {
 alarm(10);//Release -> 10
 setbuf(stdout, NULL);
}

unsigned long getNum(){
  string buf;
  unsigned long i;
  buf = string(10,'\0');
  read(0,(char *)buf.c_str(),0x100);//BOF
  return stol(buf.c_str());
}

int main(void) {
  unsigned long l=0;
  string s;
  while(l!=0xdeadbeef){
    try{
      l = getNum();
    }catch(exception e){
      cout << e.what() << endl;
    }
    printf("input : %p\n",(void *)l);
  }
}
