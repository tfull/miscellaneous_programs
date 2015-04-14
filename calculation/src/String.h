#ifndef __STRING_H__
#define __STRING_H__

typedef struct{
  char *cs;
  int length;
  int capacity;
} String;

String String_Allocate(void);
String String_GetLine(void);
void String_Free(String*);
int String_IsNull(String);
char String_CharAt(String,int);
int String_Length(String);

#endif
