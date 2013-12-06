#include <sys/stat.h>
#include "stm32f4xx_conf.h"

#ifdef __cplusplus
 extern "C" {
#endif

     extern int  __HEAP_START;

     void* __dso_handle;
	int __errno;

	int _close(int file) {
		return 0;
	}

	int _fstat(int file, struct stat *st) {
		return 0;
	}

	int _isatty(int file) {
		return 1;
	}

	int _lseek(int file, int ptr, int dir) {
		return 0;
	}

	int _open(const char *name, int flags, int mode) {
		return -1;
	}

	int _read(int file, char *ptr, int len) {
		return 0;
	}

	caddr_t _sbrk ( int incr )
	{
	  static unsigned char *heap = NULL;
	  unsigned char *prev_heap;

	  if (heap == NULL) {
	    heap = (unsigned char *)&__HEAP_START;
	  }
	  prev_heap = heap;
	  /* check removed to show basic approach */

	  heap += incr;

	  return (caddr_t) prev_heap;
	}

	int _write(int file, char *ptr, int len) {
		int i;

		for(i = 0;i < len;i++) {
			//USART_SendData(USART2, (uint8_t)ptr[i]);
			//while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET){};
		}

		return len;
	}

	int _getpid() {
	    return 1;
	}

	int _kill(int pid, int sig) {
	    return (-1);
	}
#ifdef __cplusplus
 }
#endif
