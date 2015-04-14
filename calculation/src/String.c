#include "String.h"
#include <stdio.h>
#include <stdlib.h>

String String_Allocate(void){
  String s;
  int cap = 64;
  
  s.length = 0;
  s.capacity = cap;
  s.cs = calloc(cap, sizeof(char));
  
  return s;
}

String String_GetLine(void){
  String s;
  int cap = 64;
  int i;
  char c;

  s.length = 0;
  s.capacity = cap;
  s.cs = calloc(cap, sizeof(char));

  while(1){
    c = getchar();

    if(c == EOF){
      free(s.cs);
      s.cs = NULL;
      s.length = 0;
      s.capacity = 0;
      return s;
    }else if(c == '\n'){
      break;
    }

    s.cs[s.length] = c;
    s.length += 1;

    if(s.length >= s.capacity){
      int ncap = s.capacity * 2;
      char *ns = calloc(ncap, sizeof(char));

      for(i = 0; i < s.length; i++){
        ns[i] = s.cs[i];
      }

      free(s.cs);
      s.cs = ns;
      s.capacity = ncap;
    }
  }
  return s;
}

void String_Free(String *s){
  free(s->cs);
  s->cs = NULL;
  s->length = 0;
  s->capacity = 0;
}

int String_IsNull(String s){
  return s.cs == NULL;
}

char String_CharAt(String s, int index){
  return s.cs[index];
}

int String_Length(String s){
  return s.length;
}
