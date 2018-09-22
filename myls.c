/*************************************************************************
	> File Name: myls.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月22日 星期六 18时18分20秒
 ************************************************************************/

#include<myls.h>

void error_print(const char * ptr)/*errno错误处理*/
{
    perror(ptr);
    exit(EXIT_FAILURE);
}

void mylist_dir(const char * pathname) /*读取目录中的文件*/
{   
    DIR * ret_opendir = opendir(pathname);/*打开传递的目录pathname*/
    if(ret_opendir == NULL) /*判断错误并处理*/
        error_print("opendir");

    int ret_chdir = chdir(pathname);/*先change dir到需要list的目录中*/
    if(ret_chdir == -1) /*判断错误并处理*/
        error_print("chdir");

    struct dirent * ret_readdir = NULL;/*定义接收readdir函数返回的结构体变量*/
    while(ret_readdir = readdir(ret_opendir)){/*判断读取是否到目录尾*/
        char * filename = ret_readdir->d_name;
        struct stat get_message = {};
        int ret_stat = stat(filename,&get_message);/*用stat函数读取filenmae文件的信息，并将结果写到get_message结构体中*/
        if(ret_stat == -1)/*stat函数不出错则进行信息输出*/
           error_print("stat error");
        else if(strcmp(filename,".") && strcmp(filename,".."))
            list_message(filename,&get_message);
    }
}

void list_message(const char * filename,const struct stat * get_message)/*处理读取到的文件*/
{
    char type = file_type(get_message);/*判断并打印文件类型*/
    printf("%c",type);
    file_power(get_message);/*判断并打印文件权限*/
    printf("%d ",get_message->st_nlink);/*打印硬链接数*/
    id_to_string(get_message);/*用户id与组id转换成用户名与组名并打印*/
    printf("%d\t",get_message->st_size);/*打印所占空间文件大小*/
    timet_to_tm(get_message);/*将GMT时间的秒数转换成标准时间格式输出*/
    //根据不同type输出不同颜色
    switch (type) {
        case '-' :  printf("\033[31m %s\n\033[m",filename);break;
        case 'd' :  printf("\033[32m %s\n\033[m",filename);break;
        case 'c' :  printf("\033[33m %s\n\033[m",filename);break;
        case 'b' :  printf("\033[34m %s\n\033[m",filename);break;
        case 'p' :  printf("\033[35m %s\n\033[m",filename);break;
        case 'l' :  printf("\033[36m %s\n\033[m",filename);break;
        case 's' :  printf("\033[37m %s\n\033[m",filename);break;

    }
}

char file_type(const struct stat * get_message)
{
    char type;
    mode_t mode = (*get_message).st_mode;
    if(S_ISREG(mode))
        type = '-';/*普通文件*/
    else if(S_ISDIR(mode))
        type = 'd';/*目录文件*/
    else if(S_ISCHR(mode))
        type = 'c';/*字符设备文件*/
    else if(S_ISBLK(mode))
        type = 'b';/*块设备文件*/
    else if(S_ISFIFO(mode))
        type = 'p';/*管道文件*/
    else if(S_ISLNK(mode))
        type = 'l';/*链接文件*/
    else if(S_ISSOCK(mode))
        type = 's';/*socket文件*/
    return type;
}
void file_power(const struct stat * get_message)
{
    mode_t mode = (*get_message).st_mode & 07777;/*取后四位*/
    //user权限判断
    file_power_char(mode,S_IRUSR,'r');
    file_power_char(mode,S_IWUSR,'w');
    file_power_char(mode,S_IXUSR,'x');
    //group权限判断
    file_power_char(mode,S_IRGRP,'r');
    file_power_char(mode,S_IWGRP,'w');
    file_power_char(mode,S_IXGRP,'x');
    //other权限判断
    file_power_char(mode,S_IROTH,'r');
    file_power_char(mode,S_IWOTH,'w');
    file_power_char(mode,S_IXOTH,'x');

    printf(" ");
}
void file_power_char(mode_t mode,mode_t type,const char ch)
{
    if((mode & type) == type)
        printf("%c",ch);
    else
        printf("-");
}

void id_to_string(const struct stat * get_message)
{
    struct passwd * pwd;/*根据用户id获取用户名*/
    pwd = getpwuid(get_message->st_uid);
    printf("%s ",pwd->pw_name);

    struct group * grp;/*根据组id获取组名*/
    grp = getgrgid(get_message->st_gid);
    printf("%s\t",grp->gr_name);

}

//打印时间
void timet_to_tm(const struct stat * get_message)
{
    struct tm * chtm = localtime(&(get_message->st_mtime));
    if(chtm == NULL){
        printf("localtime is error");
        exit(EXIT_FAILURE);
    }
    else
        printf("%d月 %2d  ",chtm->tm_mon+1,chtm->tm_mday);
    if(chtm->tm_hour < 10)
        printf("0");
    printf("%d:",chtm->tm_hour);
    if(chtm->tm_min < 10)
        printf("0");
    printf("%d  ",chtm->tm_min);
}

