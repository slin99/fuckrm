#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <linux/sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <fcntl.h>
int child(){
    struct utsname uts;
    char * new_hostname = "container";
    char * second_hostname= "container2";
    if (sethostname(new_hostname,strlen(new_hostname)) == -1){
        printf("sethostname %s\n",strerror(errno));
    }
    if (uname(&uts) == -1){
        printf("uname %s\n",strerror(errno));
    }
    printf("[C] hostname: %s\n",uts.nodename);
    sleep(4);
    unshare(CLONE_NEWUTS);
    sethostname(second_hostname,strlen(second_hostname));
    uname(&uts);
    printf("[C] new hostname %s\n",uts.nodename);
    return 0;
}
int main(){
    unshare(CLONE_NEWUSER);
    char * stack = malloc(4096);
    int pid = clone(child,stack+4096,CLONE_NEWUTS|SIGCHLD,NULL);
    printf("clone %s\n",strerror(errno));
    struct utsname uts;
    uname(&uts);
    printf("[P] parent hostname %s\n",uts.nodename);
    sleep(1);
    char path[100];
    snprintf(path,sizeof(path),"/proc/%d/ns/uts",pid);
    int fd = open(path,O_RDONLY);
    if (setns(fd,CLONE_NEWUTS)==-1){
        printf("setns %s\n",strerror(errno));
    }

    uname(&uts);
    //now we are in the child namespace
    printf("[P] child hostname %s\n",uts.nodename);

    waitpid(pid,0,0);
    uname(&uts);

    printf("[P] child hostname %s\n",uts.nodename);
   free(stack);
    return 0;
}