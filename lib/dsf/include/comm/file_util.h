#pragma once

#include "stdinc.h"

#ifndef _WIN32

NS_BEGIN(dsf)

//file releataed
//if sz = 0 , test file exist
int					readfile(const std::string & file, std::string & content);
int					readfile(const std::string & file, char * buffer, size_t sz);
int                 touch_file(const std::string & file_path);
int					writefile(const std::string & file, const char * buffer, size_t sz = 0);
bool				file_exists(const std::string & file);
size_t              file_size(const std::string & file);
const char *		path_base(const char * path);
std::string         path_dir(const char * path);

NS_END()

#endif