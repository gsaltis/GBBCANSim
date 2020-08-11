
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdarg.h>


static pthread_mutex_t printf_mutex;

void InitSafePrint()
{
	pthread_mutex_init(&printf_mutex, NULL);
}

// Make a wrapper around printf to make sure that only the thread that got the mutex can call printf(otherwise it will have to block until the mutex is available) :

int printf_safe(const char *format, ...)
{
  int 					n;
  va_list args;
  va_start(args, format);

  pthread_mutex_lock(&printf_mutex);
  n = vprintf(format, args);
  pthread_mutex_unlock(&printf_mutex);

  va_end(args);
  return n;
}
