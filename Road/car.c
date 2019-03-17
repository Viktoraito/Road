#include "common.h"

#define MY_ENCODING "ISO-8859-1"

pthread_t server_t, cl_way_t, cl_rule_t, cl_obst_t, cl_move_t, cl_proc_t, cl_draw_t;
unsigned port;
int c_num; //car number

void *server(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./server "); 
   strcat(sys,tmp);
   //printf("%s\n",sys);
  if(system(sys)==-1)
    printf("server: error\n");
  pthread_kill(cl_way_t,SIGKILL);
  pthread_kill(cl_rule_t,SIGKILL);
  pthread_kill(cl_obst_t,SIGKILL);
  pthread_kill(cl_move_t,SIGKILL);
  pthread_kill(cl_proc_t,SIGKILL);
}

void *cl_way(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./client_way "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
  while(1)
    system(sys);
}

void *cl_rule(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./client_rule "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
  while(1)
    system(sys);
}

void *cl_obst(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./client_obst "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
  while(1)
    system(sys);
}

void *cl_move(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./client_move "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
   //printf("c_num: %d\n",c_num);
  while(1)
    system(sys);
}

void *cl_proc(void *argument) {
   char tmp[80];
   sprintf(tmp,"%u",port);
   char sys[90]; 
   strcpy(sys,"./client_proc "); 
   strcat(sys,tmp);
   sprintf(tmp," %d",c_num);
   strcat(sys,tmp);
   //printf("%s\n",sys);
  while(1)
    system(sys);
}

int main(int argc, char **argv)
{
  if(argc<2)
    port = DEFPORT;
  else
    port = atoi(argv[1]);
  if(argc<3)
    c_num = 1;
  else
    c_num = atoi(argv[2]);
  if(c_num<1 || c_num>3) {
    printf("Not suported\n");
    exit(EXITOK);
  }
  pthread_create(&server_t, NULL, server, cl_way_t, cl_rule_t, cl_obst_t, cl_move_t, cl_proc_t);
  sleep(10);
  pthread_create(&cl_way_t, NULL, cl_way, NULL);
  pthread_create(&cl_rule_t, NULL, cl_rule, NULL);
  pthread_create(&cl_obst_t, NULL, cl_obst, NULL);
  pthread_create(&cl_move_t, NULL, cl_move, NULL);
  sleep(5);
  pthread_create(&cl_proc_t, NULL, cl_proc, NULL);
  pthread_join(server_t, NULL);
  pthread_join(cl_way_t,NULL);
  pthread_join(cl_rule_t,NULL);
  pthread_join(cl_obst_t,NULL);
  pthread_join(cl_move_t,NULL);
  pthread_join(cl_proc_t,NULL);
  exit(EXITOK);
}
