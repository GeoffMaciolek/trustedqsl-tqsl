
/*
    TrustedQSL Digital Signature Library
    Copyright (C) 2001  Darryl Wagoner WA1GON wa1gon@arrl.net

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <openssl/dsa.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <string.h>

#include "amcert.h"
#include "sign.h"

static char cvsID[] = "$Id$";

extern int errno;

void *readQpub(char *fname,char *typ)
{

  char	buf[500];
  char	*retbuf;
  int	rc;
  int	fd;

  fd = open(fname,O_RDONLY);
  if (fd < 0)
    return(NULL);

  rc = read(fd,buf,500);
  close(fd);  // we are done with it.
  if (rc <= 0)
    {
      return(NULL);
    }
  
  switch (buf[0])
    {
    case '1':
      retbuf = (char *)malloc(sizeof(PublicKey));
      memcpy(retbuf,buf,sizeof(PublicKey));

      return((void*) retbuf);
    }
}
  
int writeSign(char *fname,unsigned char *sig,int len)
{
  char	newName[500];
  int	fd;

  sprintf(newName,"%s.sig",fname);
  fd = open(newName,O_WRONLY|O_CREAT|O_TRUNC,0644);
  if (fd < 0)
    return(-1);

  int rc;
  rc = write(fd,sig,len);
  close(fd);

  if (rc != len)
    return(-2);
  return(rc);
}
int writeSignAsc(char *fname, unsigned char *sig,int len)
{
  char	newName[500];
  
  int	fd;

  sprintf(newName,"%s.asig",fname);
  fd = open(newName,O_WRONLY|O_CREAT|O_TRUNC,0644);
  if (fd < 0)
    return(-1);

  int rc;
  char *t;
  t = bin2hex(sig,len);
  write(fd,t,len*2);
  close(fd);
  free(t);
  
}
int readSignAsc(char *fname,unsigned char *sig,int len)
{
  char	newName[200];
  char  txtSign[200];
  int	fd;

  sprintf(newName,"%s.asig",fname);
  fd = open(newName,O_RDONLY);
  if (fd < 0)
    return(-1);

  int rc;
  rc = read(fd,txtSign,len);
  close(fd);
  hex2bin(txtSign,sig,rc);
  return(rc/2);
}
int readSign(char *fname,unsigned char *sig,int len)
{
  char	newName[500];
  int	fd;

  sprintf(newName,"%s.sig",fname);
  fd = open(newName,O_RDONLY);
  if (fd < 0)
    return(-1);

  int rc;
  rc = read(fd,sig,len);
  close(fd);
  return(rc);
}

int getFileSize(char *fname)
{
  struct stat sbuf;
  int rc;

  rc = stat(fname,&sbuf);
  if (rc < 0) 
    return(0);
  return(sbuf.st_size);
}

int readCert(char *fname,AmCertExtern *cert)
{
  int fileSize;
  int rc;

  fileSize = getFileSize(fname);
  memset(cert,' ',sizeof(cert));
 
  int fd;
  fd = open(fname,O_RDONLY);
  if (fd < 0)
    return(-1);

  rc = read(fd,cert,sizeof(AmCertExtern));
  close(fd);
  return(rc);

}
int sha1File(char *fname,unsigned char *hash)
{

  struct stat sbuf;
  int rc;
  unsigned char *hbuf;

  rc = stat(fname,&sbuf);
  if (rc < 0)
    return(0);

  
  int fd;

  fd = open(fname,O_RDONLY);
  if (fd < 0)
    return(0);


  hbuf = new unsigned char [sbuf.st_size+1];
  rc = read(fd,hbuf,sbuf.st_size);
  if (rc != sbuf.st_size)
    {
      delete hbuf;
      return(0);
    }

  hbuf[sbuf.st_size] = '\0';
  SHA1(hbuf,sbuf.st_size,hash);
  
  return(1);
}
int storeDSA(DSA *dsa)
{

	unsigned char		buf[1024];
	unsigned char		*ptr;
	unsigned int		DER_size;
	unsigned int		DER_to_file;
	FILE			*f;
	int			CounterRet;
	unsigned long		h_ret;

	dsa = DSA_generate_parameters(1024, NULL, 0, &CounterRet, &h_ret,
				      NULL, NULL);
	if (dsa == NULL)
		return ERROR;

	if (DSA_generate_key(dsa) != 1)
		return ERROR;

	// Store the private key (in DER encoding binary) in the specified file.
	ptr = buf;
	DER_size = i2d_DSAPrivateKey(dsa, &ptr);
	if (DER_size == 0)
	{
		DSA_free(dsa);
		return ERROR;
	}
	f = fopen("privatekey","wb");
	if (f == NULL)
	{
		return ERROR;
	}
	DER_to_file = htonl(DER_size);
	fwrite(&DER_to_file,sizeof(int),1,f);
	fwrite(buf,sizeof(char),DER_size,f);
	fclose(f);
	return(0);
}
//******************* LOAD **********************
int loadDSA(DSA *dsa)
{
	unsigned char		buf[1024];
	unsigned char		*ptr;
	unsigned int		DER_size;
	FILE			*f;

	ptr = buf;
	f = fopen("privatekey","rb");
	if (f == NULL)
		return ERROR;
	fread(&DER_size,sizeof(int),1,f);
	DER_size = ntohl(DER_size);
	fread(buf,sizeof(char),DER_size,f);
	fclose(f);
	if (d2i_DSAPrivateKey(&dsa, &ptr, DER_size) == NULL)
		return ERROR;
	return(0);
}
int readBig(const char *fname,BIGNUM **bn)
{
  FILE *fkey;
  char p[5000];
  char *t;
  int rc;

  fkey = fopen(fname,"r");
  rc = -1;
  if (fkey)
    {
      t = fgets(p,4999,fkey);
      if (t != NULL)
	{
	  //strip out eol
	  t = strchr(p,'\n');
	  if (t != NULL)
	    *t = '\0';
	  t = strchr(p,'\r');
	  if (t != NULL)
	    *t = '\0';	
	  

	  rc = BN_hex2bn(bn,p);
	  if (rc == 0)
	    {
	      printf("BN_hex2bn: %d\n",rc);
	      fclose(fkey);
	      return(-1);
	    }
	  
	}

      fclose(fkey);
      return(rc);
    }
  return(errno);
}
