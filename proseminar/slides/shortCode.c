
int child(){
    struct utsname uts;
    sethostname("container",9);
    uname(&uts);
    printf("[C] hostname: %s\n",uts.nodename);
    sleep(4);
    unshare(CLONE_NEWUTS);
    sethostname("container2",10);
    uname(&uts);
    printf("[C] new hostname %s\n",uts.nodename);
    return 0;
}

int main(){
    unshare(CLONE_NEWUSER);
    int pid = clone(child,stack+4096,CLONE_NEWUTS|SIGCHLD,NULL);
    struct utsname uts;
    uname(&uts);
    printf("[P] parent hostname %s\n",uts.nodename);
    sleep(1);
    char path[100];
    snprintf(path,sizeof(path),"/proc/%d/ns/uts",pid);
    int fd = open(path,O_RDONLY);
    setns(fd,CLONE_NEWUTS);
    uname(&uts);
    printf("[P] child hostname %s\n",uts.nodename);
    waitpid(pid,0,0);
    uname(&uts);
    printf("[P] child hostname %s\n",uts.nodename);
    free(stack);
    return 0;
}