/* 
    TrustedQSL/LoTW client and PKI Library
    Copyright (C) 2001  Darryl Wagoner WA1GON wa1gon@arrl.net and
                     ARRL

    This library is free software; you can redistribute it and/or
    modify it under the terms of the Open License packaged with
    the source.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

    You should have received a copy of the Open License see 
    http://www.trustedQSL.org/ or contact wa1gon@arrl.net.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "tqsl.h"

extern int debugLevel;
char *readPrivKey(const char *fname);


static char cvsID[] = "$Id$";

int getFileSz(char *fname)
{
  
  struct stat sbuf;
  int rc;

  cvsID = cvsID;
  rc = stat(fname,&sbuf);
  if (rc < 0) 
    return(-1);
  return(sbuf.st_size);
}

int readFile(char *fName, unsigned char *buf,int len)
{
  int fd;
  fd = open(fName,O_RDONLY);
  if (fd < 0)
    {
      return(0);
    }

  int rc;
  rc = read(fd,buf,len);
  close(fd);
#if 0
  if (rc == len)
    {
      return(0);
    }
#endif     
  return(0);
     
}

int main(int argc, char *argv[])
{  

  const int 		maxFileNameSize = 100;
  int 			optCnt=0;
  int 			c,errFlg=0;
  char			msgFile[maxFileNameSize];
  char			signFile[maxFileNameSize];
  TqslSignature 	signature;
  unsigned char		*msg;


  while ((c = getopt(argc, argv, "f:s:d")) != EOF)
    switch (c) 
      {
	case 'd':
	  debugLevel =5;
	  break;
      case 'f':
	strcpy(msgFile,optarg);
	optCnt++;
	break;
      case 's':
	strcpy(signFile,optarg);
	optCnt++;
	break;
      default:
	printf("invalid option -%c\n",c);
	errFlg++;
	break;
      }

  if (errFlg || optCnt != 2)
    {
      fprintf(stderr,"%s usage %s -s signature file -f message file\n",
	      argv[0],argv[0]);
      return(1);
    }


  int fs;

  fs = getFileSz(msgFile);
  if (fs <= 0)
    {
      fprintf(stderr,"Can't get file size of %s\n",msgFile);
      return(3);
    }

  msg = (unsigned char *)malloc(fs);
  if (msg == NULL)
    {
      fprintf(stderr,"Problem allocating memory \n");
      return(4);
    }

  int rc;
  rc = readFile(msgFile,msg,fs);
  if (rc != 0)
    {
      fprintf(stderr,"Problem reading message file %s\n",msgFile);  
      free(msg);
      return(6);
    }     

  char sigStr[1000];
  rc = readFile(signFile,(unsigned char *)sigStr,sizeof(sigStr));
  if (rc != 0)
    {
      fprintf(stderr,"Problem reading signature file %s\n",signFile);  
      free(msg);
      return(7);
    }       

  // we could and should also validate the cert this was signed with

  rc = tqslStrToSig(&signature,sigStr);
  if (rc == 0)
    return(3);
  rc = tqslVerifyData(msg,&signature,fs);

  if (rc >0)
    printf("File %s has been validated as signed by %12.12s\n",msgFile,
	   signature.cert.data.publicKey.callSign);
  else
    printf("File %s could not be validated\n",msgFile);

  free(msg);
  return(0);
  
}