#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
int main(){
   printf("p1 id:%d\t p1 parent id:%d\n",getpid(),getppid());
   int p2;
   p2=fork();
   if(p2==0){
      printf("p2 id:%d\t p2 parent id:%d\n",getpid(),getppid());
      int p4;
      p4=fork();
      if(p4==0){
	 printf("p4 id:%d\t p4 parent id:%d\n",getpid(),getppid());
      }
      wait(NULL);
      exit(0);
   }
   int p3;
   p3=fork();
   if(p3==0){
      printf("p3 id:%d\t p3 parent id:%d\n",getpid(),getppid());
      int p5;
      p5=fork();
      if(p5==0){
	 printf("p5 id:%d\t p5 parent id:%d\n",getpid(),getppid());
      }
      wait(NULL);
      exit(0);
   }
   wait(NULL);
   wait(NULL);
   printf("p1 done");
   return 0;
}
