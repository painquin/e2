

enum LOG_LEVEL
{
	LOG_DEBUG,
	LOG_MONITOR
};


extern void e2log(enum LOG_LEVEL, const char *, ... );
