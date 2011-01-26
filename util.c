#include<stdio.h>
#include<time.h>
#include<stdarg.h>

#include "e2.h"

const char *log_level_names[] = {
	"DEBUG",
	"MONITOR"
};

void e2log(enum LOG_LEVEL lvl, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	time_t t;
	t = time(NULL);
	struct tm *tm = localtime(&t);
	printf("[%04i/%02i/%02i %02i:%02i:%02i] [%s] ",
		tm->tm_year+1900,
		tm->tm_mon+1,
		tm->tm_mday,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec,
		log_level_names[lvl]
	);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}
