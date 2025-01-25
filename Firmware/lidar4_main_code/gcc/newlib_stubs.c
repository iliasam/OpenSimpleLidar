/*
 * newlib_stubs.c
 *
 *  Created on: 2 Nov 2010
 *      Author: nanoage.co.uk
 */
#include "main.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include "stm32f0xx_usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <reent.h>

#define NOSYS 1

#ifndef STDOUT_USART
#define STDOUT_USART 1
#endif

#ifndef STDERR_USART
#define STDERR_USART 1
#endif

#ifndef STDIN_USART
#define STDIN_USART 1
#endif

#undef errno
extern int errno;

int putchar(int c);


/*
 environ
 A pointer to a list of environment variables and their values. 
 For a minimal environment, this empty list is adequate:
 */
char *__env[1] = { 0 };
char **environ = __env;

void _exit(int arg) {
	for(;;) {};
}

#if 0
void _exit(int status) {
    _write(1, "exit", 4);
    while (1) {
        ;
    }
}
#endif

int _close(int file) {
    return -1;
}
/*
 execve
 Transfer control to a new process. Minimal implementation (for a system without processes):
 */
int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}
/*
 fork
 Create a new process. Minimal implementation (for a system without processes):
 */

int _fork() {
    errno = EAGAIN;
    return -1;
}
/*
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */
//int _fstat(int file, struct stat *st) __attribute__((used));
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 getpid
 Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation, for a system without processes:
 */

int _getpid() {
    return 1;
}

/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) __attribute__((used)) ;
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}


/*
 kill
 Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) {
    errno = EINVAL;
    return (-1);
}

/*
 link
 Establish a new name for an existing file. Minimal implementation:
 */
/* 
int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}
*/
/*
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
    return 0;
}

/*
 stat
 Status of a file (by name). Minimal implementation:
 int    _EXFUN(stat,( const char *__path, struct stat *__sbuf ));
 */

int _stat(const char *filepath, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 times
 Timing information for current process. Minimal implementation:
 */

clock_t _times(struct tms *buf) {
    return -1;
}

/*
 unlink
 Remove a file's directory entry. Minimal implementation:
 */
int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

/*
 wait
 Wait for a child process. Minimal implementation:
 */
int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */


int _read(int file, char *ptr, int len) {
    int n;
    int num = 0;
    switch (file) {
    case STDIN_FILENO:
        for (n = 0; n < len; n++) {
#if   STDIN_USART == 1
            char c = getch();
#elif STDIN_USART == 2
            while ((USART2->SR & USART_FLAG_RXNE) == (uint16_t) RESET) {}
            char c = (char) (USART2->DR & (uint16_t) 0x01FF);
#elif STDIN_USART == 3
            while ((USART3->SR & USART_FLAG_RXNE) == (uint16_t)RESET) {}
            char c = (char)(USART3->DR & (uint16_t)0x01FF);
#endif
            *ptr++ = c;
            num++;
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return num;
}


/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
//int _write(int file, char *ptr, int len) __attribute__((used));
int _write(int file, const char *ptr, int len) {
    int n;
    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++) {
#if STDOUT_USART == 1
            putch (*ptr++ & (uint16_t)0x01FF);
#elif  STDOUT_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDOUT_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
#if STDERR_USART == 1
            putch (*ptr++ & (uint16_t)0x01FF);
#elif  STDERR_USART == 2
            while ((USART2->SR & USART_FLAG_TC) == (uint16_t) RESET) {
            }
            USART2->DR = (*ptr++ & (uint16_t) 0x01FF);
#elif  STDERR_USART == 3
            while ((USART3->SR & USART_FLAG_TC) == (uint16_t)RESET) {}
            USART3->DR = (*ptr++ & (uint16_t)0x01FF);
#endif
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}

#if 0
/*
 sbrk
 Increase program data space.
 Malloc and related functions depend on this
 */
caddr_t _sbrk(int incr) {

    extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

char * stack = (char*) __get_MSP();
     if (heap_end + incr >  stack)
     {
         _write (STDERR_FILENO, "Heap and stack collision\n", 25);
         errno = ENOMEM;
         return  (caddr_t) -1;
         //abort ();
     }

    heap_end += incr;
    return (caddr_t) prev_heap_end;

}
#endif

//pure hack!!
//references chain: _printf_float -> _localeconv_r -> (__global_locale, _impure_ptr)
//	only for one single field "decimal_point" which is first in list
struct lconv *_localeconv_r (struct _reent *data) {
	static const char         decimal_point   = '.';
	static const char * const decimal_point_p = &decimal_point;
	return (struct lconv *)&decimal_point_p;
}


// wrappers for the non-reentrant newlib syscalls
#if 0
_ssize_t _read_r(
    struct _reent *r, 
    int file, 
    void *ptr, 
    size_t len)
{
	return _read(file, ptr, len);
}

_ssize_t _write_r (
    struct _reent *r, 
    int file, 
    const void *ptr, 
    size_t len)
{
	return _write(file, ptr, len);
}

int _close_r(
    struct _reent *r, 
    int file)
{
	return _close(file);
}

_off_t _lseek_r(
    struct _reent *r, 
    int file, 
    _off_t ptr, 
    int dir)
{
	return _lseek(file, ptr, dir);
}

int _fstat_r(
    struct _reent *r, 
    int file, 
    struct stat *st)
{
	return _fstat(file, st);
}
/*
int _open_r(struct _reent *r, const char *a, int b, int c)
{
  return _open(a,b,c);
}

void * _sbrk_r(
    struct _reent *_s_r, 
    ptrdiff_t nbytes)
{
	return _sbrk(nbytes);
}
*/
#endif
