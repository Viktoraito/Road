#include "common.h"

#define MY_ENCODING "ISO-8859-1"

float l_way,f_way,r_way, b_way;
int procIsAsking;
int dx,dy,cx,cy;
float **map;
int c_num;
char reader[15],data[15];

void sendXmlwriterFilename(const char *uri)
{
  FILE *fcoord;
  fcoord = fopen("coord", "r");
  int tdx[3],tdy[3],px[3],py[3];
for(int i=0; i<3; i++) {
  fscanf(fcoord, "%d", &tdy[i]);
  fscanf(fcoord, "%d", &tdx[i]);
  fscanf(fcoord, "%d", &py[i]);
  fscanf(fcoord, "%d", &px[i]);
}

  fclose(fcoord);
  dx=tdx[c_num-1];
  dy=tdy[c_num-1];
  cx=px[c_num-1];
  cy=py[c_num-1];

    //printf("way %d %d %d %d %d\n",c_num,dx,dy,cx,cy);
   wait(5);

  l_way = dx-cx;
  f_way = dy-cy;
  r_way = cx-dx;
  b_way = cy-dy;

	//printf("way %d %f %f %f %f\n",c_num,l_way,f_way,r_way,b_way);

  int rc;
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterFilename(uri, 0);
  if (writer == NULL) {
      printf("Error creating the xml writer\n");
      return;
  }
  rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
  if (rc < 0) {
      printf("Error at xmlTextWriterStartDocument\n");
      return;
  }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "DATA");
    if (rc < 0) {
        printf("Error at xmlTextWriterStartElement\n");
        return;
    } 
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "TYPE", BAD_CAST "WAY");
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "LEFT", "%f", l_way);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "FRONT", "%f", f_way);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "RIGHT", "%f", r_way);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "BACK", "%f", b_way);
      if (rc < 0) {
        printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
      printf("Error at xmlTextWriterEndElement\n");
      return;
    }
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }
    xmlFreeTextWriter(writer);
}

void sendData(void *argument, char *file){
  int serv_sock = *(int *) argument;
  int result, xmlsize;
  FILE *xmlfd;
  LIBXML_TEST_VERSION
  sendXmlwriterFilename(file);
  xmlCleanupParser();
  xmlMemoryDump();
  xmlfd = fopen(file, "rb");
  fseek(xmlfd, 0L, SEEK_END);
  xmlsize = ftell(xmlfd);
  fseek(xmlfd, 0L, SEEK_SET);
  char *buf = calloc(xmlsize, sizeof(char));
  fread(buf,xmlsize,sizeof(char),xmlfd);
  write(serv_sock, buf, xmlsize);
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
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"WAY")) 
        procIsAsking=1;
    }
  }    
}

void readData(int serv_sock, int nread) {
  char *buf = calloc(nread, sizeof(char));
  read(serv_sock, buf, nread);
  int xmlfd;
  mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
  remove(reader);
  xmlfd = open(reader, O_WRONLY | O_CREAT, mode);
  write(xmlfd,buf,nread);
  close(xmlfd); free(buf);
    parseAsk(reader);
}

void *speak(void *argument) {
  while(1) {
    if(procIsAsking){
      sendData(argument, data);
      //printf("Sending client_proc a data\n");
      procIsAsking=0;
    }
  }
}

void *hear(void *argument) {
  int serv_sock = *(int *) argument;
  int nread;
  while(1) {
    ioctl(serv_sock, FIONREAD, &nread);
      if(nread) {
       readData(serv_sock, nread);
    }
  }
}

int conn_serv(int serv_sock, unsigned port) {
  int len,result;
  struct sockaddr_in s_address;
  s_address.sin_family = AF_INET;
  s_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  s_address.sin_port = htons(port);
  //printf("Trying to connect with server\n");
  len = sizeof(s_address);
  result = connect(serv_sock, (struct sockaddr *)&s_address, len);
  if(result == -1) {
    //perror("Connection with server failed");
    return 0;
  }
  sendData(&serv_sock, data);
  return 1;
}

int main(int argc, char **argv)
{
  procIsAsking=0;
  int serv_sock;
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
   strcpy(reader,"readerW"); 
   strcat(reader,tmp);
   strcat(reader,".xml");
   strcpy(data,"wayData");
   strcat(data,tmp);
   strcat(data,".xml");


  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(!conn_serv(serv_sock, port)) {
    printf("Client_move: Can't connect with server\n");
    exit(EXITOK);
  }
  //printf("Successfully connected to server\n");
  pthread_t hear_t, speak_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);
  exit(EXIT_SUCCESS);
}



