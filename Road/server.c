#include "common.h"

#define QUEUE 3

int way_fd,proc_fd,obst_fd,move_fd,rule_fd;
int c_num; //car number
char reader[15];

void parseHello(char *xmlfile, int fd) {
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/TYPE/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"WAY")) {
        way_fd=fd;
        //printf("WAY_FD=%d\n",way_fd);
      }
      if(!strcmp(tmp,"PROC")) {
        proc_fd=fd;
        //printf("PROC_FD=%d\n",proc_fd);
      }
      if(!strcmp(tmp,"OBST")) {
        obst_fd=fd;
        //printf("OBST_FD=%d\n",obst_fd);
      }
      if(!strcmp(tmp,"RULE"))
        rule_fd=fd;
      if(!strcmp(tmp,"MOVE")) {
        move_fd=fd;
        //printf("MOVE_FD=%d\n",move_fd);
      }
    }
}

void askWay(char *xmlfile) {
  if(way_fd!=-1) {
    int xmlsize;
    FILE *xmlfd;
    xmlfd = fopen(xmlfile, "rb");
    fseek(xmlfd, 0L, SEEK_END);
    xmlsize = ftell(xmlfd);
    fseek(xmlfd, 0L, SEEK_SET);
    char *buf = calloc(xmlsize, sizeof(char));
    fread(buf,xmlsize,sizeof(char),xmlfd);
    write(way_fd, buf, xmlsize);
    free(xmlfd); free(buf); 
  }
}

void askObst(char *xmlfile) {
  if(obst_fd!=-1) {
    int xmlsize;
    FILE *xmlfd;
    xmlfd = fopen(xmlfile, "rb");
    fseek(xmlfd, 0L, SEEK_END);
    xmlsize = ftell(xmlfd);
    fseek(xmlfd, 0L, SEEK_SET);
    char *buf = calloc(xmlsize, sizeof(char));
    fread(buf,xmlsize,sizeof(char),xmlfd);
    write(obst_fd, buf, xmlsize);
    free(xmlfd); free(buf); 
  }
}

void askRule(char *xmlfile) {
  if(rule_fd!=-1) {
    int xmlsize;
    FILE *xmlfd;
    xmlfd = fopen(xmlfile, "rb");
    fseek(xmlfd, 0L, SEEK_END);
    xmlsize = ftell(xmlfd);
    fseek(xmlfd, 0L, SEEK_SET);
    char *buf = calloc(xmlsize, sizeof(char));
    fread(buf,xmlsize,sizeof(char),xmlfd);
    write(rule_fd, buf, xmlsize);
    free(xmlfd); free(buf); 
  }
}

void askMove(char *xmlfile) {
  if(move_fd!=-1) {
    int xmlsize;
    FILE *xmlfd;
    xmlfd = fopen(xmlfile, "rb");
    fseek(xmlfd, 0L, SEEK_END);
    xmlsize = ftell(xmlfd);
    fseek(xmlfd, 0L, SEEK_SET);
    char *buf = calloc(xmlsize, sizeof(char));
    fread(buf,xmlsize,sizeof(char),xmlfd);
    write(move_fd, buf, xmlsize);
    free(xmlfd); free(buf); 
  }
}

void sendAns(char *xmlfile) {
  int xmlsize;
  FILE *xmlfd;
  xmlfd = fopen(xmlfile, "rb");
  fseek(xmlfd, 0L, SEEK_END);
  xmlsize = ftell(xmlfd);
  fseek(xmlfd, 0L, SEEK_SET);
  char *buf = calloc(xmlsize, sizeof(char));
  fread(buf,xmlsize,sizeof(char),xmlfd);
  write(proc_fd, buf, xmlsize);
  free(xmlfd); free(buf); 
}

void parseAsk(char *xmlfile) {
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/ASK/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    printf("No result for parsing xml package\n");
  }
  else
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"WAY")) 
	      askWay(xmlfile);
      if(!strcmp(tmp,"OBST"))
	      askObst(xmlfile);
      if(!strcmp(tmp,"RULE"))
        askRule(xmlfile);
      if(!strcmp(tmp,"MOVE"))
        askMove(xmlfile);
    }
}

int addClient(int s_sockfd, fd_set *readfds) {
  int c_len, c_sockfd;
  struct sockaddr_in c_address;
  c_len = sizeof(c_address);
  c_sockfd = accept(s_sockfd, (struct sockaddr *)&c_address, &c_len);
  FD_SET(c_sockfd, readfds);
  //printf("Adding client...\n");
}

int removeClient(int fd, fd_set *readfds) {
  close(fd);
  FD_CLR(fd, readfds);
  //printf("Removing client...\n");
}

int retriveData(int fd, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(fd, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  remove(reader);
  xmlfd = open(reader, O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
  if(way_fd==-1 || proc_fd==-1 || obst_fd==-1 || rule_fd==-1 || move_fd==-1)
    parseHello(reader,fd);
  if((fd==way_fd || fd==obst_fd || fd==move_fd || fd==rule_fd) && proc_fd!=-1)
    sendAns(reader);
  if(fd==proc_fd)
    parseAsk(reader);
  if(!(fd==way_fd || fd==proc_fd || fd==obst_fd || fd==rule_fd || fd==move_fd))
    printf("Unregistered file descryptor\n");
}

int listenSocket(int s_sockfd, struct sockaddr_in s_address, int queue) {
  int result, fd, nread;
  fd_set readfds, testfds;

  listen(s_sockfd, queue);
  FD_ZERO(&readfds);
  FD_SET(s_sockfd, &readfds);

  //printf("Server is running\n");
  while(1){
    testfds = readfds;
    result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
    if (result <1) {
      perror("Server");
      exit(EXIT_FAILURE);
    }
    
    for(fd=0; fd<FD_SETSIZE; fd++) {
      if(!FD_ISSET(fd,&testfds))
	continue;
      if(fd == s_sockfd) {
	addClient(s_sockfd, &readfds);
        continue;
      }
      ioctl(fd, FIONREAD, &nread);
      if(nread) {
	retriveData(fd, nread);
      }
      else{
	removeClient(fd,&readfds);}
    }
  }
}

int main( int argc, char **argv) {
  way_fd=-1;proc_fd=-1;obst_fd=-1;rule_fd=-1;move_fd=-1;
  unsigned port;
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
   char tmp[10];
   sprintf(tmp,"%d",c_num);
   strcpy(reader,"readerS"); 
   strcat(reader,tmp);
   strcat(reader,".xml");

  int s_sockfd;
  struct sockaddr_in s_address;
  s_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = htonl(INADDR_ANY);
  s_address.sin_port = htons(port);

  if(bind(s_sockfd, (struct sockaddr *)&s_address, sizeof(s_address))) {
    printf("Can't bind socket\n");
    return EXIT_FAILURE;
  }
  listenSocket(s_sockfd, s_address, QUEUE);
  exit(EXITOK);
}
