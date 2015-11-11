/*
 * =====================================================================================
 *
 *       Filename:  spike_debug.h
 *
 *    Description:  Debug macros for SPIKE solver
 *
 *        Version:  1.0
 *        Created:  10/11/2015 09:40:33
 *       Revision:  none
 *       Compiler:  nvcc - icc
 *
 *         Author:  Samuel Rodriguez Bernabeu
 *   Organization:  Barcelona Supercomputing Center
 *
 * =====================================================================================
 */

#ifndef _SPIKE_DEBUG_H_
	#define _SPIKE_DEBUG_H_

	#include <stdio.h>
	#include <errno.h>
	#include <string.h>

	#ifdef NDEBUG
		#define spike_debug(M, ...)
	#else
		#define spike_debug(M, ...) do{ fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)
	#endif

	#define clean_errno() (errno == 0 ? "None" : strerror(errno))

	#define spike_log_err(M, ...)  do{ fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); } while(0)

	#define spike_log_warn(M, ...) do{ fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__); } while(0)

	#define spike_log_info(M, ...) do{ fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); } while(0)

	#define check(A, M, ...) if(!(A)) { spike_log_err(M, ##__VA_ARGS__); errno=0; goto error; }

	#define sentinel(M, ...)  { spike_log_err(M, ##__VA_ARGS__); errno=0; goto error; }

	#define check_mem(A) check((A), "Out of memory.")

	#define check_debug(A, M, ...) if(!(A)) { spike_debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif

