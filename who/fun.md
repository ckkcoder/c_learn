# 调用函数(*)

##	本章基本函数
```
open(filename, how)
creat(filename,mode)
read(fd,buffer,amt)
write(fd,buffer,amt)
lseek(fd,distance,base)
close(fd)

```

###	错误函数
```
#include <errno.h>
extern int errno	//全局变量，会在程序错误时设置为相应的错误代码，并返回-1

perror();

```

### 时间

```
#include <time.h>

```

### 时间
```
/proc/uptime
两个参数，用秒来表示

第一个参数代表系统总的启动时间,
第二个参数表示系统空闲的时间，如果该时间比第一个参数还大，则说明cpu是多核的，cpu0上闲了1秒，cpu1上闲了2秒，则系统一共闲了3秒

```
