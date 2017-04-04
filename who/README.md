#<center>第二章 编写who命令</center>

##	概念与技巧
- #####	联机帮助的作用与使用方法
- #####	Unix 的文件操作函数: open,read,write,lseek,close
- #####	文件的建立与读写
- #####	文件描述符
- #####	缓冲:	用户级的缓冲与内核级的缓冲
- #####	内核模式，用户模式和系统调用的代价
- #####	Unix 表示时间的方法与时间格式间的转换
- #####	借助 utmp 文件来列出已登录的用户
- #####	系统调用中的错误检测与处理

##	相关的系统调用
- #####	open,read,write,creat,lseek,close
- #####	perror

##	相关命令
- #####	man
- #####	who
- #####	cp
- #####	login


>	problems:
>>	1.	who 命令能做什么？
>>	2.	who 命令如何工作的？
>>	3.	如何编写 who？

```
系统的用户实时登录用户信息存放在 /var/run/utmp
历史记录存放在	/var/log/wtmp

```

```
<utmp.h>
<bits/utmp.h>

struct utmp
{
  short int ut_type;		/* Type of login.  */
  pid_t ut_pid;			/* Process ID of login process.  */
  char ut_line[UT_LINESIZE];	/* Devicename.  */
  char ut_id[4];		/* Inittab ID.  */
  char ut_user[UT_NAMESIZE];	/* Username.  */
  char ut_host[UT_HOSTSIZE];	/* Hostname for remote login.  */
  struct exit_status ut_exit;	/* Exit status of a process marked
				   as DEAD_PROCESS.  */
/* The ut_session and ut_tv fields must be the same size when compiled
   32- and 64-bit.  This allows data files and shared memory to be
   shared between 32- and 64-bit applications.  */
  int32_t ut_session;		/* Session ID, used for windowing.  */
  struct
  {
    int32_t tv_sec;		/* Seconds.  */
    int32_t tv_usec;		/* Microseconds.  */
  } ut_tv;			/* Time entry was made.  */

  int32_t ut_addr_v6[4];	/* Internet address of remote host.  */
  char __glibc_reserved[20];		/* Reserved for future use.  */
};

#define ut_name		ut_user

#define ut_time	ut_tv.tv_sec


```


## 调用函数
调用函数会在 fun.md 中进行说明


















