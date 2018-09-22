/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月22日 星期六 18时20分43秒
 ************************************************************************/
#include "myls.h"

int main(char argc,char ** argv)
{
    if(argc != 3){
        printf("Too few parameter!");
        exit(EXIT_FAILURE);
    }

    if((argc == 3) && !(strcmp(argv[1],"-al"))){/*判断是myls -al argv[2]格式*/
        struct stat get_message = {};
        int ret_stat = stat(argv[2],&get_message);
        if(ret_stat == -1)
            error_print("stat error");

        if(S_ISDIR(get_message.st_mode))/*判断是否是目录，是目录则进一步处理目录中个文件*/
            mylist_dir(argv[2]);
        else/*不是目录则直接处理普通文件*/
            list_message(argv[2],&get_message); /*文件stat信息输出函数*/
    }
    else{/*不符合格式则结束进程*/
        printf("error in main!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}



