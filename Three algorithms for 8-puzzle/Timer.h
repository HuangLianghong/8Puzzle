//¼ÆÊ±Æ÷
#pragma once
#include <Windows.h>
#include <time.h>

class Timer {
private: LARGE_INTEGER freq;
	   LARGE_INTEGER begin_time;
	   LARGE_INTEGER end_time;
	   LARGE_INTEGER total_time;
	   int aver;

public:	
	  Timer() { total_time.QuadPart = 0; aver = 0; }
	  
	  void start() {
		  QueryPerformanceFrequency(&freq);
		  QueryPerformanceCounter(&begin_time);
		  ++aver;
	  }
	  void end() {
		  QueryPerformanceCounter(&end_time);
		  total_time.QuadPart += (end_time.QuadPart - begin_time.QuadPart);
	  }
	  double aver_time() {
		  return (double)total_time.QuadPart / (double)freq.QuadPart / (double)aver;
	  }
	  void reset() {
		  total_time.QuadPart = 0; aver = 0;
	  }

};

