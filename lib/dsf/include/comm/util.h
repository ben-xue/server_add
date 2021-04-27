#pragma once

#include "stdinc.h"

NS_BEGIN(dsf)

#ifndef _WIN32

//-----------------lock-------------------------------
template<bool threadsafe>
struct lock_mixin;

template<bool threadsafe>
struct caslock_mixin;


//----------misc------------------------------------------------------------
uint64_t			time_unixtime_us();
inline	uint64_t	time_unixtime_ms(){ return time_unixtime_us() / 1000L; }
inline	time_t		time_unixtime_s() { return time(NULL); }

const char*			strftime(std::string & str, time_t unixtime = 0, const char * format = "%FT%X%z");
std::string			strftime(time_t unixtime = 0, const char * format = "%FT%X%z");
time_t  	        strptime(const std::string & str, const char * format = "%FT%X%z");
const char*			strftime(std::string & str, struct tm & rtm, const char * format = "%FT%X%z");

time_t				stdstrtime(const char * strtime = "1970-01-01T08:08:08+0800");
bool                time_same_hour(time_t t1, time_t t2);
bool                time_same_ten_minutes(time_t t1, time_t t2);
bool                time_same_day(time_t t1, time_t t2);
bool                time_same_month(time_t t1, time_t t2);
bool                time_same_week(time_t t1, time_t t2);
bool                time_same_year(time_t t1, time_t t2);
int	                getminutes(time_t unixtime);


typedef void(*path_walker_t)(const char * dirname, const char * basename, bool is_dir, void * ud);
int                 path_walk(const char * path, path_walker_t walker, void * ud, int depth = 0);
enum  path_entry_filter {
	PATH_IS_FILE = 0x01,
	PATH_IS_DIR = 0x02,
	PATH_IS_BLK = 0x04,
	PATH_IS_FIFO = 0x08,
	PATH_IS_SOCKET = 0x10,
	PATH_IS_LINK = 0X20,
	PATH_IS_CHAR = 0x40,
};
int                 path_list(std::vector<std::string> & file_list, const char * path, int filter = PATH_IS_FILE);
//file://<path>
//tcp://<ip:port>
//udp://<ip:port>
//?
//mode:r:listen/w:connect,others -> file
int                 openfd(const std::string & uri, const char * mode = "w", int timeout_ms = 30000);

int                 closefd(int fd);

int                 nonblockfd(int fd, bool nonblock = true);
bool                isnonblockfd(int fd);
//return 0: readable , other error occurs
int                 waitfd_readable(int fd, int timeout_ms);
int                 waitfd_writable(int fd, int timeout_ms);
int                 ipfromhostname(OUT uint32_t * ip, INOUT int & ipnum, const std::string & hostname);
int                 socknetaddr(struct sockaddr_in & addr, const std::string & saddr);
const char*			strsockaddr(struct sockaddr_in & addr);

//socket util
int                 socket_fd(struct sockaddr_in & addr, const std::string & saddr);
int                 socket_nonblock(int fd, bool nb);
int                 socket_keepalive(int fd, bool ka);
int                 socket_nodelay(int fd, bool nd);
int                 socket_buffer(int fd, int recv_buff_sz, int send_buf_sz);
int                 socket_reuse(int fd);
int                 socket_keeplive(int fd);
int                 socket_linger(int fd, int lingerTime = 10);
int                 socket_bind(int fd, const struct sockaddr_in & addr);
int                 socket_listen(int fd, int max_back_log = 255);
int                 socket_connect(int fd, const struct sockaddr_in & addr);
int                 socket_send(int fd, const char * buff, size_t buff_size);

///////////process////////////////////////////////////////////////////////////////////////
int					daemonlize(int closestdio = 1, int chrootdir = 0, const char * pidfile = NULL);
//he signals SIGKILL and SIGSTOP cannot be caught or ignored
int					signalh_ignore(int sig);
int					signalh_default(int sig);
typedef void(*sah_handler)(int sig, siginfo_t * sig_info, void * ucontex);
int					signalh_push(int sig, sah_handler sah, int sah_flags = 0);
sah_handler			signalh_pop(int sig);
void				signalh_clear(int sig);
int                 signalh_set(int sig, sah_handler sah, int sah_flags = 0);
//-1:open file error , getpid():lock ok , 0:lock error but not known peer, >0: the locker pid.
int					lockpidfile(const std::string & pidfile, int kill_other_sig = SIGQUIT, bool nb = true, int * plockfd = NULL, bool notify = false);
//return fd
int                 lockfile(const std::string & file, bool nb = true);
int                 unlockfile(int fd);
int					killpidfile(const std::string & pidfile, int kill_sig = SIGQUIT);

///////////str////////////////////////////////////////////////////////////////////////////////
enum str_hash_strategy {
	hash_str_dbj2,
	hash_str_sdbm,
};



size_t              strhash(const std::string & buff, str_hash_strategy st = hash_str_sdbm);
size_t				strprintf(std::string & str, const char * format, ...);
size_t				strnprintf(std::string & str, size_t max_sz, const char * format, ...);
size_t				vstrprintf(std::string & str, const char* format, va_list va);
void				strrepeat(std::string & str, const char * rep, int repcount);
bool                strisint(const std::string & str, int base = 10);
const char *		strrandom(std::string & randoms, int length = 8, char charbeg = 0x21, char charend = 0x7E);
const char *		strcharsetrandom(std::string & randoms, int length = 8, const char * charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ_!@#$+-");
string &            strreplace(string & str, const string & sub, const string & repl, bool global = false);
string &            strrereplace(string & str, const string & repattern, const string & repl);
bool                strrefind(string & str, const string & repattern, std::match_results<string::const_iterator>& m);
std::string &       strtrim(std::string & str, const char * charset = " \t");
std::string &       strltrim(std::string & str, const char * charset = " \t");
std::string &       strrtrim(std::string & str, const char * charset = " \t");
void				strupper(std::string & str);
const char         *strjoin(std::string & val, const std::string & sep, const char ** it);
const char		   *strjoin(std::string & val, const std::string & sep, const vector<string>& vs);
const char         *strspack(std::string & str, const std::string & sep, const std::string & ks, ...);
int                 strsunpack(const std::string & str, const std::string & sep, const std::string & k, ...);


//////////////////////////////////////////////////////////////////////////////////////
void    *           dllopen(const char * file, int flag = 0);
void    *           dllsym(void * dll, const char * sym);
int                 dllclose(void * dll);
const char *        dllerror();

////////////////////////////////////////////////////////////////////////////


//implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
struct lock_mixin<false>{
	void lock(){}
	void unlock(){}
};

template<>
struct lock_mixin<true>{
	void lock(){ lock_.lock(); }
	void unlock(){ lock_.unlock(); }
private:
	std::mutex  lock_;
};


template <typename T>
string tostr(const T& val)
{
	std::ostringstream out;
	out << val;
	return out.str();
}

template<>
struct caslock_mixin<false>{
	static void lock(volatile int& lock_){}
	static void unlock(volatile int& lock_){}
};

template<>
struct caslock_mixin<true>{
	static void lock(volatile int& lock_){
		while (!__sync_bool_compare_and_swap(&lock_, 0, 1)){}
	}
	static void unlock(volatile int& lock_){
		lock_ = 0;
	}
};

#ifdef THREAD_SAFE
#define THREAD_SAFE_FLAG true
#else
#define THREAD_SAFE_FLAG false
#endif

#endif

int	strsplit(const std::string & str, const string & sep, std::vector<std::string> & vs, bool ignore_empty = true, int maxsplit = 0, int beg = 0, int end = 0);

NS_END()
