#include "common.h"

#define MY_ENCODING "ISO-8859-1"

/*Global parameters. If equal to 1, answer was received.*/
int AnsWay=0, AnsObst=0, AnsMove=0, AnsRule=0;

/*dx, dy - global parameters, delta of robot's coordinates. transfered from coordinates from client xml-message*/
float dx=0, dy=0;

/*global parameters for values*/
float way_l, way_f, way_r, way_b, obst_l, obst_f, obst_r, obst_b, rule_l, rule_f, rule_r, rule_b;

int c_num; //car number
char reader[15],data[15],askObst[15],askRule[15],askWay[15],askMove[15];

void askXmlwriterFilename(const char *uri, char *ask, int code)
{
  int rc;
  float test=1.0;
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterFilename(uri, 0);
  if (writer == NULL) {
      //printf("Error creating the xml writer\n");
      return;
  }
  rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
  if (rc < 0) {
      //printf("Error at xmlTextWriterStartDocument\n");
      return;
  }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "DATA");
    if (rc < 0) {
        //printf("Error at xmlTextWriterStartElement\n");
        return;
    } 
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "TYPE", BAD_CAST "PROC");
      if (rc < 0) {
        //printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      rc = xmlTextWriterWriteElement(writer, BAD_CAST "ASK", BAD_CAST ask);
      if (rc < 0) {
        //printf("Error at xmlTextWriterWriteAttribute\n");
        return;
      }
      if(code!=NIHIL) {
        rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "DEC", "%d", code);
        if (rc < 0) {
          //printf("Error at xmlTextWriterWriteAttribute\n");
          return;
        }
      }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
      //printf("Error at xmlTextWriterEndElement\n");
      return;
    }
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        //printf            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }
    xmlFreeTextWriter(writer);
}

void ask(void *argument, char *ask, char *file, int code) {
  int serv_sock = *(int *) argument;
  int xmlsize;
  FILE *xmlfd;
  LIBXML_TEST_VERSION
  askXmlwriterFilename(file, ask, code);
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

void ansWay(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/LEFT/text()";
  xmlChar *xpath2 = "/DATA/FRONT/text()";
  xmlChar *xpath3 = "/DATA/RIGHT/text()";
  xmlChar *xpath4 = "/DATA/BACK/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      way_l = strtod(tmp, NULL);
    }
  }
  result = xmlXPathEvalExpression(xpath2, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      way_f = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath3, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      way_r = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath4, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      way_b = strtod(tmp, NULL);
    }
  }
  AnsWay=1;
}

void ansObst(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/LEFT/text()";
  xmlChar *xpath2 = "/DATA/FRONT/text()";
  xmlChar *xpath3 = "/DATA/RIGHT/text()";
  xmlChar *xpath4 = "/DATA/BACK/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      obst_l = strtod(tmp, NULL);
    }
  }
  result = xmlXPathEvalExpression(xpath2, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      obst_f = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath3, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      obst_r = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath4, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      obst_b = strtod(tmp, NULL);
    }
  }
  AnsObst=1;
}

void ansRule(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/LEFT/text()";
  xmlChar *xpath2 = "/DATA/FRONT/text()";
  xmlChar *xpath3 = "/DATA/RIGHT/text()";
  xmlChar *xpath4 = "/DATA/BACK/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rule_l = strtod(tmp, NULL);
    }
  }
  result = xmlXPathEvalExpression(xpath2, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rule_f = strtod(tmp, NULL);
    }
  }
result = xmlXPathEvalExpression(xpath3, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rule_r = strtod(tmp, NULL);
    }
  }
  result = xmlXPathEvalExpression(xpath4, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      rule_b = strtod(tmp, NULL);
    }
  }
  AnsRule=1;
}

void ansMove(char *xmlfile){
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlChar *xpath1 = "/DATA/ANS/text()";
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath1, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"OK"))
        AnsMove=1;
    }
  }
}

void parseAns(char *xmlfile) {
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
    //printf("No result for parsing xml package\n");
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),
			 sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      if(!strcmp(tmp,"WAY")) 
        ansWay(xmlfile);
      if(!strcmp(tmp,"OBST"))
        ansObst(xmlfile);
      if(!strcmp(tmp,"RULE"))
        ansRule(xmlfile);
      if(!strcmp(tmp,"MOVE"))
        ansMove(xmlfile);
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
    parseAns(reader);
}

double calculate(char *xmlfile, xmlChar *xpath) {
  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;
  xmlInitParser ();
  LIBXML_TEST_VERSION
  xmlDoc *doc = xmlParseFile (xmlfile);
  context = xmlXPathNewContext(doc);
  result = xmlXPathEvalExpression(xpath, context);
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
    xmlXPathFreeObject(result);
    //printf("No result for parsing xml package\n");
    return 0.0;
  }
  else {
    if(result->nodesetval[0].nodeTab[0]->content != NULL) {
      char *tmp = calloc(sizeof(result->nodesetval[0].nodeTab[0]->content),sizeof(char));
      tmp = (char *) result->nodesetval[0].nodeTab[0]->content;
      return strtod(tmp, NULL);
    }
  }
}

//Greater or equal?
bool goe(double a, double b) {
  if(a>b || fabs(a-b)<=0.000001)
    return true;
  return false;
}

//Which way to go?
bool wwg(float way) {
  if(goe(way,way_f) && goe(way,way_l) && goe(way,way_r) && goe(way,way_b))
    return true;
  return false;
}

int Decision(char *xmlfile) {
  if((fabs(way_f)==1.0 && fabs(way_l)==1.0) || (fabs(way_f)==1.0 && fabs(way_l)==0.0) || (fabs(way_f)==0.0 && fabs(way_l)==1.0))
    return REACH;
  double F=0.0, L=0.0, R=0.0, B=0.0, S=0.0;
  const xmlChar 
    *STEP_F="/DATA/F_STEP", 
    *STEP_L="/DATA/L_STEP",
    *STEP_R="/DATA/R_STEP",
    *STEP_B="/DATA/B_STEP",
    *STEP_S="/DATA/STOP",
    *WAY_F="/WAY_F/text()",
    *WAY_L="/WAY_L/text()",
    *WAY_R="/WAY_R/text()",
    *WAY_B="/WAY_B/text()",
    *FREE_F="/FREE_F/text()",
    *FREE_L="/FREE_L/text()",
    *FREE_R="/FREE_R/text()",
    *FREE_B="/FREE_B/text()",
    *OBST_F="/OBST_F/text()",
    *OBST_L="/OBST_L/text()",
    *OBST_R="/OBST_R/text()",
    *OBST_B="/OBST_B/text()",
    *RULE_F="/RULE_F/text()",
    *RULE_L="/RULE_L/text()",
    *RULE_R="/RULE_R/text()",
    *RULE_B="/RULE_B/text()";
 
  if(wwg(way_f)) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,WAY_F,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,WAY_F,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,WAY_F,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,WAY_F,-1));
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,WAY_F,-1));
  }

  if(wwg(way_l)) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,WAY_L,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,WAY_L,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,WAY_L,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,WAY_L,-1));
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,WAY_L,-1));	
  }

  if(wwg(way_r)) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,WAY_R,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,WAY_R,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,WAY_R,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,WAY_R,-1));
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,WAY_R,-1));
  }

  if(wwg(way_b)) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,WAY_B,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,WAY_B,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,WAY_B,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,WAY_B,-1));
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,WAY_B,-1));	
 }

  if(obst_f==0.0 && rule_f==0.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,FREE_F,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,FREE_F,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,FREE_F,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,FREE_F,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,FREE_F,-1));
  }
  if(obst_f==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,OBST_F,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,OBST_F,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,OBST_F,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,OBST_F,-1));
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,OBST_F,-1));  
  }

  if(obst_l==0.0 && rule_l==0.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,FREE_L,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,FREE_L,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,FREE_L,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,FREE_L,-1)); 
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,FREE_L,-1));
  }
  if(obst_l==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,OBST_L,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,OBST_L,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,OBST_L,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,OBST_L,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,OBST_L,-1));
  }

  if(obst_r==0.0 && rule_r==0.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,FREE_R,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,FREE_R,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,FREE_R,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,FREE_R,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,FREE_R,-1));
  }
  if(obst_r==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,OBST_R,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,OBST_R,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,OBST_R,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,OBST_R,-1));    
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,OBST_R,-1));
  }

  if(obst_b==0.0 && rule_b==0.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,FREE_B,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,FREE_B,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,FREE_B,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,FREE_B,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,FREE_B,-1));
  }
  if(obst_b==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,OBST_B,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,OBST_B,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,OBST_B,-1));
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,OBST_B,-1));    
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,OBST_B,-1));
  }

if(rule_f==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,RULE_F,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,RULE_F,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,RULE_F,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,RULE_F,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,RULE_F,-1));
  }

if(rule_l==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,RULE_L,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,RULE_L,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,RULE_L,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,RULE_L,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,RULE_L,-1));
  }

if(rule_r==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,RULE_R,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,RULE_R,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,RULE_R,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,RULE_R,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,RULE_R,-1));
  }

if(rule_b==1.0) {
    F+=calculate(xmlfile,xmlStrncatNew(STEP_F,RULE_B,-1));
    L+=calculate(xmlfile,xmlStrncatNew(STEP_L,RULE_B,-1));
    R+=calculate(xmlfile,xmlStrncatNew(STEP_R,RULE_B,-1));  
    B+=calculate(xmlfile,xmlStrncatNew(STEP_B,RULE_B,-1));  
    S+=calculate(xmlfile,xmlStrncatNew(STEP_S,RULE_B,-1));
  }

  //printf("%d F=%f L=%f R=%f B=%f S=%f\n",c_num,F,L,R,B,S);

  if((rule_f==1.0 && wwg(way_f)) || (rule_l==1.0 && wwg(way_l)) || (rule_r==1.0 && wwg(way_r)) || (rule_b==1.0 && wwg(way_b)))
    return STOP;
  if(goe(F,L) && goe(F,R) && goe(F,B) && goe(F,S))
    return F_STEP;
  if(goe(L,F) && goe(L,R) && goe(L,B) && goe(L,S))
    return L_STEP;
  if(goe(R,F) && goe(R,L) && goe(R,B) && goe(R,S))
    return R_STEP;
  if(goe(B,F) && goe(B,L) && goe(B,R) && goe(B,S))
    return B_STEP;
  if(goe(S,F) && goe(S,L) && goe(S,R) && goe(S,S))
    return STOP;
  return REACH;
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

void *speak(void *argument) {
  while(1) {
    ask(argument, "WAY", askWay, NIHIL);
    //printf("Asking client_way for data\n");
    while(!AnsWay) {}
    AnsWay=0;
    //printf("way_l=%f\tway_f=%f\tway_r=%f\tway_b=%f\n",way_l,way_f,way_r,way_b);
    ask(argument, "OBST", askObst, NIHIL);
    //printf("Asking client_obst for data\n");
    while(!AnsObst) {}
    AnsObst=0;
    //printf("obst_l=%f\tobst_f=%f\tobst_r=%f\tobst_b=%f\n",obst_l,obst_f,obst_r,obst_b);
    ask(argument, "RULE", askRule, NIHIL);
    //printf("Asking client_rule for data\n");
    while(!AnsRule) {}
    AnsRule=0;
    //printf("rule_l=%f\trule_f=%f\trule_r=%f\trule_b=%f\n",rule_l,rule_f,rule_r,rule_b);
    int dec;
    dec=Decision(data);
    //printf("c_num=%d dec=%d\n",c_num,dec);
    //getchar();
    if(dec==REACH) {
      ask(argument, "MOVE", askMove, dec);
      break;
    }
    ask(argument, "MOVE", askMove, dec);
    //printf("Asking client_move to move\n");
    while(!AnsMove) {}
    AnsMove=0;
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
  return 1;
}

int main(int argc, char **argv)
{
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
  //printf("!!!\t%d\t\n",c_num);
   char tmp[10];
   sprintf(tmp,"%d",c_num);
   strcpy(reader,"readerP"); 
   strcat(reader,tmp);
   strcat(reader,".xml");
   strcpy(data,"DATA");
   strcat(data,tmp);
   strcat(data,".xml");
   strcpy(askObst,"askObst");
   strcat(askObst,tmp);
   strcat(askObst,".xml");
   strcpy(askRule,"askRule");
   strcat(askRule,tmp);
   strcat(askRule,".xml");
   strcpy(askWay,"askWay");
   strcat(askWay,tmp);
   strcat(askWay,".xml");
   strcpy(askMove,"askMove");
   strcat(askMove,tmp);
   strcat(askMove,".xml");

  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(!conn_serv(serv_sock,port)) {
    printf("Client_proc: Can't connect with server\n");
    exit(EXIT_FAILURE);
  }
  //printf("Successfully connected to server\n");
  pthread_t hear_t, speak_t;
  pthread_create(&hear_t, NULL, hear, &serv_sock);
  pthread_create(&speak_t, NULL, speak, &serv_sock);
  pthread_join(hear_t, NULL);
  pthread_join(speak_t,NULL);
  exit(EXITOK);
}



