/*
 * BY_log.c
 *
 *  Created on: 2018��5��21��
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdarg.h>
#include "BY_log.h"
#define MAX_SIZE 100

static const char *level_names[] =
{
	"[TRACE]",
	"[DEBUG]",
	"[INFO]",
	"[WARN]",
	"[ERROR]",
	"[FATAL]"
};

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
	char buf[MAX_SIZE] = {0};
	va_list args;

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printf("\r\n %-7s %s:%d:%s\r\n", level_names[level], file, line, buf);
}
