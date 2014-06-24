
#include "utils.h"

void *malloc(int sz) 
{

 int next_index = 0;
 unsigned char our_memory[1024 * 1024];  
    void *mem;

if(sizeof our_memory - next_index < sz)
  return '\0';

  mem = &our_memory[next_index];
  next_index += sz;
  return mem; 
}

int caracterValido(char caracter){
    int codigo = (int) caracter;
    return (codigo>=30 && codigo <=39 || codigo>=41 && codigo<=90 || codigo>=97 && codigo<=122);
    
}

char *getCaracteresValidos(char *cadena)
{
    char *temp = malloc(mystrlen(cadena));
    int i=0;
    for(i=0; i<mystrlen(cadena); i++)
    {
        temp[i] = (caracterValido(cadena[i])) ? cadena[i] : '\0';
    }
    temp[i] = '\0';
    return temp;
}

char * getPathName(char * pathName, int type)
{
    int lengthName =  mystrlen(pathName); 
    char *path = malloc(lengthName);
    int tokens = qtytoken(pathName,'/'); 
    int posToken=2; 
    int i=0; 
    char *dirName = malloc(lengthName);
    if(tokens == 1 && type == 1)
    {
        path[0] = pathName[0];
        path[1] = '\0';
    }
    if(tokens == 2)
    {
        path[0] = '/';
        path =getCaracteresValidos(path);
    }
    while (posToken <= tokens-1) 
    { 
        char *token = mystrtok(pathName, '/', posToken); 
        
        if(posToken == 2)
            path = getCaracteresValidos(mistrcat("/",token));
        else
            path = getCaracteresValidos(mistrcat(path,token)); 

        if(posToken < tokens-1) 
        {
            path = getCaracteresValidos(path);
            path = mistrcat(path, "/"); 
            path = getCaracteresValidos(path);
        }
        posToken++; 
    } 
    
    if(type == 1)
    {
        return path;
    }
    else
    {
        dirName = mystrtok(pathName, '/', tokens);
        dirName = getCaracteresValidos(dirName);
        return dirName;
    }

}

int mod(int a, int b)
{
        while(a>=b)
                a=a-b;
        return a;
}     

int mystrlen(char *cadena){
    int i = 0;

    while(cadena[i] !=  '\0'){
        i++;
    }
    return i;
}

char * mistrcat(char * destino,char * fuente)
{
 int i=0, j=0, pos = 0, len = mystrlen(destino) + mystrlen(fuente);

 char * temp = malloc(len);
 
    while(destino[i])
    {
         temp[i] = destino[i];
         i++;
    }
  
 while(fuente[j])
 {
  temp[i]=fuente[j];
  i++;
  j++;
 }
 
 destino = malloc(len);
  for(pos;pos<len;pos++)
  destino[pos] = temp[pos];
 return destino;
}

char * strcpy( char *to,char* from)
{
    int i=0;
    while (to[i]!='\0')
    {
       from[i]=to[i];
       i++;
    }
  return from;
}

int strCmp (char *s1, char *s2)
{
     unsigned char *p1 = (unsigned char *)s1;
     unsigned char *p2 = (unsigned char *)s2;

    while (*p1 != '\0') {
        if (*p2 == '\0') return  0;
        if (*p2 > *p1)   return 0;
        if (*p1 > *p2)   return  0;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return 0;

    return 1;
}

int stlen(char * cadena, int start, char delim)
{
    int count = 0, pos = 0;
    
    while(cadena[pos]!='\0' && cadena[pos]!=delim){
        count++;
        pos++;
    }

    return count;
}

int qtytoken(char * token, char delim)
{
    int totalTokens = 1;
    int pos = 0;

    while(token[pos]!='\0')
    {
        if(token[pos]==delim){
        totalTokens++;
        }
        pos++;
    }
    return totalTokens;
}

char *mystrtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0;
    int len = stlen(str, posStr, delim);
    char * token = malloc(len);
    int posToken = 0;
    if(len == 1)
        return str;

    
    while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            count++;
        
            if(count<pos)token = malloc(len);
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}


char * strtok(char * str, char delim, int pos)
{
    
    int totalTokens = qtytoken(str,delim), count = 1, posStr = 0, posToken = 0;
    int len = stlen(str, posStr, delim);

    char * token;

    if(len==0)
        return '\0';
    
    token = malloc(len);

while(str[posStr]!='\0')
    {
        if(str[posStr]==delim)
        {
            int len = stlen(str, posStr, delim);
            count++;
         
            if(count<pos)token = malloc(len);
        }
        else if(count==pos){
            token[posToken] = str[posStr];
            posToken++;
        }
        posStr++;
    }   
    return token;
}
