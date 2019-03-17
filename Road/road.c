#include "common.h"

#define MY_ENCODING "ISO-8859-1"

pthread_t *pid;
int t_num, c_num;
unsigned port;

void *car(void *argument) {
   port++;
   c_num++;
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./car "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
  while(system(sys)<=0) {}
  for(int i=0; i<t_num; i++) {
    pthread_kill(pid[i],SIGKILL);
	printf("!\n");
}
}

void *draw(void *argument) {
   char tmp[80];
   char sys[90]; 
   strcpy(sys,"./draw ");
   sprintf(tmp,"%d",t_num);
   strcat(sys,tmp);
  while(1)
    system(sys);
}

int main(int argc, char **argv)
{
  port = DEFPORT;
  if(argc>1)
    t_num = atoi(argv[1]);
  else
    t_num = 1;
  if(t_num<1 || t_num>3) {
    printf("Not supported\n");
    exit(EXITOK);
  }
  c_num = 0;

  pthread_t *pid[t_num]; pthread_t draw_t;

  pthread_create(&draw_t, NULL, draw, NULL);
  sleep(1);
  for(int i=0; i<t_num; i++) {
    pthread_create(&pid[i], NULL, car, NULL);
  }
  pthread_join(draw_t,NULL);
  for(int i=0; i<t_num; i++) {
    pthread_join(pid[i], NULL);
  }
  exit(EXITOK);
}
