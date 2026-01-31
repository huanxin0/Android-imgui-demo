#define MAX_LINE 1024


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <locale>
#include <string>
#include <codecvt>
#include <dlfcn.h>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<ctime>
#include <random>
#include <vector>
#include <string>

// 文字转码
// 文字转码
#ifndef __UTF_H__
#define __UTF_H__
#define FALSE 0
#define TRUE 1
#define halfShift 10
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
	// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF
typedef unsigned char boolean;
typedef unsigned int CharType;
typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;

static const UTF32 halfMask = 0x3FFUL;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const UTF32 offsetsFromUTF8[6] =
	{ 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };
static const char trailingBytesForUTF8[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

typedef enum
{
	strictConversion = 0,
	lenientConversion
}
ConversionFlags;
typedef enum
{
	conversionOK,				// conversion successful
	sourceExhausted,			// partial character in source,but hit end
	targetExhausted,			// insuff. room in target for conversion
	sourceIllegal,				// source sequence is illegal/malformed
	conversionFailed
}
ConversionResult;
#endif

int Utf16_To_Utf8(const UTF16 * sourceStart, UTF8 * targetStart, size_t outLen,
				  ConversionFlags flags)
{
	int result = 0;
	const UTF16 *source = sourceStart;
	UTF8 *target = targetStart;
	UTF8 *targetEnd = targetStart + outLen;
	if ((NULL == source) || (NULL == targetStart))
	{
		// printf("ERR,Utf16_To_Utf8:source=%p,targetStart=%p\n",source,targetStart);
		return conversionFailed;
	}

	while (*source)
	{
		UTF32 ch;
		unsigned short bytesToWrite = 0;
		const UTF32 byteMask = 0xBF;
		const UTF32 byteMark = 0x80;
		const UTF16 *oldSource = source;	// In case we have to back up
		// because of target overflow.
		ch = *source++;
		// If we have a surrogate pair,convert to UTF32 first.
		if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
		{
			// If the 16 bits following the high surrogate are in the source
			// buffer...
			if (*source)
			{
				UTF32 ch2 = *source;
				// If it's a low surrogate,convert to UTF32.
				if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
				{
					ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) +
						halfBase;
					++source;
				}
				else if (flags == strictConversion)
				{				// it's an unpaired high surrogate
					--source;	// return to the illegal value itself
					result = sourceIllegal;
					break;
				}
			}
			else
			{					// We don't have the 16 bits following the
				// high surrogate.
				--source;		// return to the high surrogate
				result = sourceExhausted;
				break;
			}
		}
		else if (flags == strictConversion)
		{
			// UTF-16 surrogate values are illegal in UTF-32
			if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
			{
				--source;		// return to the illegal value itself
				result = sourceIllegal;
				break;
			}
		}
		// Figure out how many bytes the result will require
		if (ch < (UTF32) 0x80)
		{
			bytesToWrite = 1;
		}
		else if (ch < (UTF32) 0x800)
		{
			bytesToWrite = 2;
		}
		else if (ch < (UTF32) 0x10000)
		{
			bytesToWrite = 3;
		}
		else if (ch < (UTF32) 0x110000)
		{
			bytesToWrite = 4;
		}
		else
		{
			bytesToWrite = 3;
			ch = UNI_REPLACEMENT_CHAR;
		}
		target += bytesToWrite;
		if (target > targetEnd)
		{
			source = oldSource;	// Back up source pointer!
			target -= bytesToWrite;
			result = targetExhausted;
			break;
		}
		switch (bytesToWrite)
		{						// note: everything falls through.
		case 4:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 3:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 2:
			*--target = (UTF8) ((ch | byteMark) & byteMask);
			ch >>= 6;
		case 1:
			*--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
		}
		target += bytesToWrite;
	}
	return result;
}



using namespace std;
//适配rainy版
char Fps[100];
int Pid=-1;
char *Gun;
char extra[32];
// syscall内存读写
#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

//long OBJECt,Oneself,Crosshair,矩阵;//对象数组  自身数组  准星地址
//int OBJECtRS=0;
//long unity, il2cpp, tersafe2;
ssize_t process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
				  const struct iovec *__remote_iov, unsigned long __remote_iov_count,
				  unsigned long __flags, bool iswrite)
{
	return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid,
				   __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}
//float px,px1;
//float py,py1;
int PlayerCount;
typedef char PACKAGENAME;
int ShotStr(char *str1,char *str2)
{
    std::string strr1,strr2;
    strr1 = str1, strr2 = str2;
    if(strr1.length() != strr2.length()) {
        return -2;
    }
    std::sort(strr1.begin(), strr1.end());
    std::sort(strr2.begin(), strr2.end());
    if(strr1 == strr2) {
        return 0;
    }
    return -1;
}

uintptr_t ReadModule(char *name, int index)
    {
        int f = 0, ii = 0, iii = 0;
        long start = 0, end = 0;
        char line[1024] = {0};
        char fname[128];
        char dname[128];
        if (strstr(name, "bss") != NULL) {
            sscanf(name, "%[^:]", dname);
            f++;
        } else {
            sprintf(dname, "%s", name);
        }
        sprintf(fname, "/proc/%d/maps", Pid);
        FILE *p = fopen(fname, "r");
        if (p)
        {
            while (fgets(line, sizeof(line), p))
            {
                if (strstr(line, dname) != NULL)
                {
                    iii++;
                    if (f == 0)
                    {
                        if (iii == index)
                        {
                            if (sizeof(long) == 8) {
                                sscanf(line, "%lx-%lx", &start, &end);
                            } else {
                                sscanf(line, "%x-%x", &start, &end);
                            }
                            break;
                        }
                    } else {
                        ii++;
                    }
                }
                if (ii > 0) {
                    if (strstr(line, "[anon:.bss]") != NULL) {
                        if (sizeof(long) == 8) {
                            sscanf(line, "%lx-%lx", &start, &end);
                        } else {
                            sscanf(line, "%x-%x", &start, &end);
                        }
                        break;
                    }
                }
            }
            fclose(p);
        }
        return start;
    }
long GetModuleBase(const char *module_name)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", Pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

long GetModuleBase2(const char *module_name, const char *modulefield)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", Pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name) && strstr(line, modulefield))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}


long int get_module_base(const char *module_name)
{
	FILE *fp;
	long addr = 0;
	char *pch;
	char filename[64];
	char line[1024];
	snprintf(filename, sizeof(filename), "/proc/%d/maps", Pid);
	fp = fopen(filename, "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			if (strstr(line, module_name))
			{
				pch = strtok(line, "-");
				addr = strtoul(pch, NULL, 16);
				if (addr == 0x8000)
					addr = 0;
				break;
			}
		}
		fclose(fp);
	}
	return addr;
}

int handle;
typedef char PACKAGENAME;

int ReadBuffer(long address, void *buffer, unsigned long size)
{
	struct iovec iov_ReadBuffer, iov_ReadOffset;
	iov_ReadBuffer.iov_base = buffer;
	iov_ReadBuffer.iov_len = size;
	iov_ReadOffset.iov_base = (void *)address;
	iov_ReadOffset.iov_len = size;
	return syscall(SYS_process_vm_readv, Pid, &iov_ReadBuffer, 1, &iov_ReadOffset, 1, 0);
}

char *Unicode64(long Name_pointer)
{
	int len = 0;
	ReadBuffer(Name_pointer + 0x10, &len, 4);
	if (len <= 0 || len > 40)
	{
		return "无";
	}
	char *retres;
	char str[12];
	retres = (char *)malloc(sizeof(short) * 40);
	memset(retres, '\0', sizeof(retres));
	unsigned short Namecode[len];
	ReadBuffer(Name_pointer + 0x14, Namecode, len * 2);
	for (int i = 0; i < len; i++)
	{
		wchar_t wstr[] = {
			Namecode[i], 0
		};
		setlocale(LC_ALL, "");
		wcstombs(str, wstr, sizeof(str) / sizeof(char));
		strcat(retres, str);
	}
	free(retres);

	return retres;
}

int getPID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

bool pvm(void *address, void *buffer, size_t size, bool iswrite)
{
	struct iovec local[1];
	struct iovec remote[1];
	local[0].iov_base = buffer;
	local[0].iov_len = size;
	remote[0].iov_base = address;
	remote[0].iov_len = size;
	if (Pid < 0)
	{
		return false;
	}
	ssize_t bytes = process_v(Pid, local, 1, remote, 1, 0, iswrite);
	return bytes == size;
}
bool vm_readv(unsigned long address, void *buffer, size_t size)
{
	return pvm(reinterpret_cast < void *>(address), buffer, size, false);
}
bool vm_writev(unsigned long address, void *buffer, size_t size)
{
	return pvm(reinterpret_cast < void *>(address), buffer, size, true);
}
// 读取字符信息
void getUTF8(UTF8 * buf, unsigned long namepy)
{
	UTF16 buf16[16] = { 0 };
	vm_readv(namepy, buf16, 28);
	UTF16 *pTempUTF16 = buf16;
	UTF8 *pTempUTF8 = buf;
	UTF8 *pUTF8End = pTempUTF8 + 32;
	while (pTempUTF16 < pTempUTF16 + 28)
	{
		if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End)
		{
			*pTempUTF8++ = (UTF8) * pTempUTF16;
		}
		else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
			*pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
}

unsigned long getZZ(unsigned long addr)
{
	unsigned long var = 0;
	vm_readv(addr, &var, 8);
	return (var);
}

size_t sysread(long address, void *buffer, size_t size)
{

	struct iovec iov_ReadBuffer, iov_ReadOffset;
	iov_ReadBuffer.iov_base = buffer;
	iov_ReadBuffer.iov_len = size;
	iov_ReadOffset.iov_base = (void *)address;
	iov_ReadOffset.iov_len = size;
	return syscall(SYS_process_vm_readv, Pid, &iov_ReadBuffer, 1, &iov_ReadOffset, 1, 0);
}

size_t syswrite(long address, void *buffer, size_t size)
{
	struct iovec iov_WriteBuffer, iov_WriteOffset;
	iov_WriteBuffer.iov_base = buffer;
	iov_WriteBuffer.iov_len = size;
	iov_WriteOffset.iov_base = (void *)address;
	iov_WriteOffset.iov_len = size;
	return syscall(SYS_process_vm_writev, Pid, &iov_WriteBuffer, 1, &iov_WriteOffset, 1, 0);
}

int 偏移D类型(long address)
{
	int local_value = 0;
	sysread(address, &local_value, 4);
	return local_value;
}

	// 根据地址读一个float F类型
float 偏移F类型(long address)
{
	float local_value = 0;
	sysread(address, &local_value, 4);
	return local_value;
}
float getFloat(long int addr)
{
	float var = 0;
	sysread(addr, &var, 4);
	return (var);
}


// 获取D类内存
int getDword(long int addr)
{
	int var = 0;
	sysread(addr, &var, 4);
	return (var);
}

int getByte(long int addr)
{
	int var = 0;
	sysread(addr, &var, 1);
	return (var);
}

long ReadZZ(long address)		// 感谢屿随的写法，QQ:1038716443
{
	if (address < 0xFF51FFAE)
	{
		long int local_value = 0;
		sysread(address, &local_value, 8);
		return local_value;
	}
	else
	{
		long int local_value = 0;
		sysread(address, &local_value, 8);
		return local_value;
	}
}

int readb(int &c, int num)
{
	++c;
	return num;
}


template<typename... s>
    long 跳指针(long addr, s... args) {
        int con   = 0;
        addr=ReadZZ(addr);
        int arr[] = {(readb(con, args))...};
         for (int f = 0; f < con; f++) {
            if (f == con-1) {
                 addr += arr[f];
                 return addr;
            }
            addr=getZZ(addr+arr[f]);
           }
    }
float 修改F类型(long address, float value)
{
	syswrite(address, &value, 4);
	return value;
}

int 修改D类型(long address, int value)
{
	syswrite(address, &value, 4);
	return value;
}


int 修改F类型2(long int addr, float value)
{
	char lj[64];
	sprintf(lj, "/proc/%d/mem", Pid);
	int handle = open(lj, O_RDWR);
	pwrite64(handle, &value, 4, addr);
	return 0;
}

int 修改D类型2(long int addr, int value)
{
	char lj[64];
	sprintf(lj, "/proc/%d/mem", Pid);
	int handle = open(lj, O_RDWR);
	pwrite64(handle, &value, 4, addr);
	return 0;
}

int WriteAddress_DWORD(long int addr, int value)
{
    char lj[64];
    int handle;
    sprintf(lj, "/proc/%d/mem", Pid);
    handle = open(lj, O_RDWR);
    pwrite64(handle, &value, 4, addr);
    return 0;
}

char *Unicode(unsigned long Name_pointer)	// 感谢寂风的写法
{
	int len = 0;
	int offset = (Name_pointer < 0xFFFFFFFF) ? 0x8 : 0x10;

	sysread(Name_pointer + offset, &len, 4);
	if (len <= 0 || len > 128)
		return NULL;

	char *retres;
	char str[12];
	retres = (char *)malloc(sizeof(char) * (len + 1));
	memset(retres, '\0', sizeof(char) * (len + 1));
	unsigned short Namecode[len];
	sysread(Name_pointer + offset + 4, Namecode, len * 2);
	for (int i = 0; i < len; i++)
	{
		wchar_t wstr[] = { Namecode[i], 0 };
		setlocale(LC_ALL, "");
		wcstombs(str, wstr, sizeof(str) / sizeof(char));
		strcat(retres, str);
	}

	return retres;
}/*
char *Unicode(unsigned int Name_pointer)
{
	int len = 0;
	sysread(Name_pointer + 8, &len, 4);
	if (len <= 0 || len > 30 || Name_pointer < 0x10000000)
	{
		return "无";
	}
	char *retres;
	char str[12];
	retres = (char *)malloc(sizeof(short) * 30);
	memset(retres, '\0', sizeof(retres));
	unsigned short Namecode[len];
	sysread(Name_pointer + 12, Namecode, len * 2);
	for (int i = 0; i < len; i++)
	{
		wchar_t wstr[] = { Namecode[i], 0 };
		setlocale(LC_ALL, "");
		wcstombs(str, wstr, sizeof(str) / sizeof(char));
		strcat(retres, str);
	}
	free(retres);

	return retres;
}
*/
char *Unicode1(unsigned long Name_pointer)
{
	int len = 0;
	int offset = (Name_pointer < 0xFFFFFFFF) ? 0x8 : 0x10;
	sysread(Name_pointer + offset, &len, 4);
	if (len <= 0 || len > 30)
	{
		return "";
	}
	char *retres;
	char str[12];
	retres = (char *)malloc(sizeof(char) * (len + 1));
	memset(retres, '\0', sizeof(char) * (len + 1));
	unsigned short Namecode[len];
	sysread(Name_pointer + offset+4, Namecode, len * 2);
	for (int i = 0; i < len; i++)
	{
		wchar_t wstr[] = { Namecode[i], 0 };
		setlocale(LC_ALL, "");
		wcstombs(str, wstr, sizeof(str) / sizeof(char));
		strcat(retres, str);
	}
	return retres;
}





long getbss(const char *module_name)
{
    long addr = 0;
    char path[64];
    char line[1024];
    char *buffer;
    char *bss;
    int flag = 0;
    buffer = strdup(module_name);
    module_name = strtok(buffer, ":");
    bss = strtok(NULL, ":");
    sprintf(path, "/proc/%d/maps", Pid);
    FILE *fp = fopen(path, "r");
    if (fp)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name) && strstr(line, "r-xp"))
            {
                flag = 1;
                if (!bss)
                {
                    sscanf(line, "%lx", &addr);
                    break;
                }
            }
            if (flag == 1 && strstr(line, "[anon:.bss]"))
            {
                sscanf(line, "%lx", &addr);
                break;
            }
        }
        fclose(fp);
    }
    free(buffer);
    return addr;
}



long int getXa(const char *module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[64];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", Pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}




long getCd(const char *module_name)
{
    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[64];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", Pid);
    fp = fopen(filename, "r");
    char str[100];
    sprintf(str, "-%x", getbss(module_name));
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, str ))
            {
                sscanf(line, "%X", &addr);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}


long int lsp64(long int addr)
{
    long int var = 0;
    pread64(handle, &var, 8, addr);
    return var;
}


long int lsp32(long int addr)
{
    long int var = 0;
    pread64(handle, &var, 4, addr);
    return var;
}

//骨骼相关

struct Quat
{
	float X;
	float Y;
	float Z;
	float W;
};

struct Vector2A
{
	float X;
	float Y;

	  Vector2A()
	{
		this->X = 0;
		this->Y = 0;
	}

	Vector2A(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
};

struct Vector3A
{
	float X;
	float Y;
	float Z;

	  Vector3A()
	{
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3A(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

};

struct FTransform
{
	Quat Rotation;
	Vector3A Translation;
	float chunk;
	Vector3A Scale3D;
};


struct FMatrix
{
	float M[4][4];
};

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)
{
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)
{
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}

FTransform getBone(unsigned long addr)
{
	FTransform transform;
	vm_readv(addr, &transform, 4 * 11);
	return transform;
}

Vector3A MarixToVector(FMatrix matrix)
{
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}
