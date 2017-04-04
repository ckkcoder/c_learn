# whoami 命令的学习

根据获取到的用户ID获取用户的名字并输出

```
#include <pwd.h>
passwd 结构体

geteuid() //获取当前登录用户的 uid
getpwuid(uid) //根据 uid 获取用户的其他信息(从 /etc/passwd 读取出来)，并返回 passwd 结构体

struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

```