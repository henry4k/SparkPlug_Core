#include <SparkPlug/Platform.h>
#include <SparkPlug/Time.h>

namespace SparkPlug
{

#if defined(__WIN32__)
	#define WINDOWS_MEAN_AND_LEAN
	#include <windows.h>
	
	union
	{
		__int64 qpc;
		DWORD tgt;
	} t0;
	bool HasPerformanceCounter;
	double Conversion;
	
	bool InitTime_()
	{
		__int64 freq;
		HasPerformanceCounter = QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

		if(HasPerformanceCounter)
			Conversion = 1.0 / double(freq);
		else
			Conversion = 1.0 / 0.001;
		
		return true;
	}
	
	double RuntimeInSeconds()
	{
		if(HasPerformanceCounter)
		{
			static __int64 t1;
			QueryPerformanceCounter((LARGE_INTEGER*)&t1);
			return ( t1 - t0.qpc ) * Conversion;
		}
		else
		{
			return ( timeRuntimeInSeconds() - t0.tgt ) * Conversion;
		}
	}
#elif defined(__MACOSX__)
	#include <mach/mach.h>
	#include <mach/mach_time.h>
	
	uint64_t t0;
	double Conversion;
	
	bool InitTime_()
	{
		t0 = mach_absolute_time();
		
		mach_timebase_info_data_t info;
		mach_timebase_info(&info);
		Conversion = 1e-9 * (double) info.numer / (double) info.denom;
		return true;
	}
	
	double RuntimeInSeconds()
	{
		return double( mach_absolute_time() - t0 ) * Conversion;
	}
#else // *NIX
	#include <time.h>
	#include <unistd.h>
	#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0) && defined(_POSIX_MONOTONIC_CLOCK)
		struct timespec t0;
		
		bool InitTime_()
		{
			clock_gettime(CLOCK_MONOTONIC, &t0);
			return true;
		}
		
		double RuntimeInSeconds()
		{
			static struct timespec t1;
			clock_gettime(CLOCK_MONOTONIC, &t1);
			
			return double(t1.tv_sec - t0.tv_sec) + double(t1.tv_nsec - t0.tv_nsec)*1e-9;
		}
	#else
		#include <sys/time.h>
		
		long long t0;
		
		bool InitTime_()
		{
			struct timeval tv;
			gettimeofday(&tv, NULL);
			t0 = (long long)tv.tv_sec * (long long)1000000 + (long long)tv.tv_usec;
			return true;
		}
		
		double RuntimeInSeconds()
		{
			long long t;
			static struct timeval tv;
			
			gettimeofday( &tv, NULL );
			t = ( (long long)tv.tv_sec * (long long)1000000 + (long long) tv.tv_usec );
			
			return double(( (long long)tv.tv_sec * (long long)1000000 + (long long) tv.tv_usec ) - t0) * 1e-6;
		}
	#endif
#endif


bool TimeInitialized_ = InitTime_();

}
