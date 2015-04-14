#include "String.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN 256
#define MAX_NUM 64

typedef struct{
  enum{ NUM, PLUS, MINUS, TIMES, DIV, MOD, LPAR, RPAR, NIL, ERROR } token;
  int value;
} Token;

typedef struct{
  enum{ SOME, NONE } option;
  int value;
} Value;

void Analyze(String,Token*);
Value Evaluate(Token*);

int main(void){
  String s;
  Value v;
  Token ts[MAX_TOKEN];
  int i;
  int error;

  while(1){
    printf("> ");
    s = String_GetLine();
    
    if(String_IsNull(s)){
      printf("\n");
      break;
    }
    
    Analyze(s, ts);

    error = 0;

    for(i = 0; ts[i].token != NIL; i++){
      if(ts[i].token == ERROR){
        error = 1;
      }
    }

    if(error){
      printf("ERROR\n");
    }else{
      v = Evaluate(ts);

      if(v.option == SOME){
        printf("%d\n", v.value);
      }else{
        printf("NONE\n");
      }
    }

    String_Free(&s);
  }
  
  return 0;
}

void Analyze(String s, Token *ts){
  int i, j;
  int len = String_Length(s);
  int index = 0; // reading token
  int num_index = 0; // reading number
  char cs[MAX_NUM];
  char c;

  for(i = 0; i < MAX_NUM; i++){
    cs[i] = '\0';
  }

  for(i = 0; i < MAX_TOKEN; i++){
    ts[i].token = NIL;
  }

  for(i = 0; i < len; i++){
    c = String_CharAt(s, i);
    
    if(c >= '0' && c <= '9'){
      cs[num_index] = c;
      num_index++;
    }else{
      if(num_index > 0){
        ts[index].token = NUM;
        ts[index].value = atoi(cs);
        for(j = 0; j < num_index; j++){
          cs[j] = '\0';
        }
        num_index = 0;
        index++;
      }
      
      switch(c){
      case '+':
        ts[index].token = PLUS;
        index++;
        break;
      case '-':
        ts[index].token = MINUS;
        index++;
        break;
      case '*':
        ts[index].token = TIMES;
        index++;
        break;
      case '/':
        ts[index].token = DIV;
        index++;
        break;
      case '%':
        ts[index].token = MOD;
        index++;
        break;
      case '(':
        ts[index].token = LPAR;
        index++;
        break;
      case ')':
        ts[index].token = RPAR;
        index++;
        break;
      default:
        if(c != ' ' && c != '\t'){
          ts[index].token = ERROR;
          index++;
        }
        break;
      }
    }
  }

  if(num_index > 0){
    ts[index].token = NUM;
    ts[index].value = atoi(cs);
    index++;
  }
  
  ts[index].token = NIL;
}

void debug(Token *ts, int end){
  int i;
  
  for(i = 0; i < end; i++){
    if(ts[i].token == PLUS){
      fprintf(stderr, "+,");
    }else if(ts[i].token == MINUS){
      fprintf(stderr, "-,");
    }else if(ts[i].token == TIMES){
      fprintf(stderr, "*,");
    }else if(ts[i].token == DIV){
      fprintf(stderr, "/,");
    }else if(ts[i].token == MOD){
      fprintf(stderr, "%%,");
    }else if(ts[i].token == LPAR){
      fprintf(stderr, "(");
    }else if(ts[i].token == RPAR){
      fprintf(stderr, ")");
    }else{
      fprintf(stderr, "%d,", ts[i].value);
    }
  }
  fprintf(stderr, "\n");
}

Value Eval(Token *ts, int begin, int end){
  int h, i, j;
  Token new_ts[MAX_TOKEN];
  Value val;
  int index = 0;
  Value none = { NONE, 0 };

  // evaluate braket
  for(i = begin; i < end; i++){
    if(ts[i].token == LPAR){
      int count = 1;
      int left = i + 1;
      for(i = left; i < end; i++){
        if(ts[i].token == LPAR){
          count++;
        }else if(ts[i].token == RPAR){
          count--;
          if(count == 0){
            Value v = Eval(ts, left ,i);
            if(v.option == NONE){
              return v;
            }else{
              Token tk = { SOME, v.value };
              new_ts[index] = tk;
            }
            break;
          }
        }
      }
      if(count > 0){
        return none;
      }
    }else{
      new_ts[index] = ts[i];
    }
    index++;
  }

  if(index == 0){
    return none;
  }

  // *, / and %
  for(i = 0; i < index; i++){
    if(new_ts[i].token == TIMES || new_ts[i].token == DIV || new_ts[i].token == MOD){
      int n;

      if(! (i - 1 >= 0 && i + 1 < index && new_ts[i - 1].token == NUM && new_ts[i + 1].token == NUM)){
        return none;
      }

      switch(new_ts[i].token){
      case TIMES:
        n = new_ts[i - 1].value * new_ts[i + 1].value;
        new_ts[i - 1].token = NUM;
        new_ts[i - 1].value = n;
        break;
      case DIV:
        if(new_ts[i + 1].value != 0){
          n = new_ts[i - 1].value / new_ts[i + 1].value;
          new_ts[i - 1].token = NUM;
          new_ts[i - 1].value = n;
        }else{
          return none;
        }
        break;
      case MOD:
        if(new_ts[i + 1].value != 0){
          n = new_ts[i - 1].value % new_ts[i + 1].value;
          new_ts[i - 1].token = NUM;
          new_ts[i - 1].value = n;
        }else{
          return none;
        }
        break;
      default:
        break;
      }

      for(h = 0; h < 2; h++){
        for(j = i; j < index - 1; j++){
          new_ts[j] = new_ts[j + 1];
        }
        index--;
      }
      i--;
    }
  }

  // new_ts[0] == '-'
  if(new_ts[0].token == MINUS){
    if(new_ts[1].token == NUM){
      new_ts[0].token = NUM;
      new_ts[0].value = - new_ts[1].value;
      
      for(i = 1; i < index - 1; i++){
        new_ts[i] = new_ts[i + 1];
      }
      index--;
    }else{
      return none;
    }
  }

  // + and -
  for(i = 0; i < index; i++){
    if(new_ts[i].token == PLUS || new_ts[i].token == MINUS){
      int n;

      if(! (i - 1 >= 0 && i + 1 < index && new_ts[i - 1].token == NUM && new_ts[i + 1].token == NUM)){
        return none;
      }

      switch(new_ts[i].token){
      case PLUS:
        n = new_ts[i - 1].value + new_ts[i + 1].value;
        new_ts[i - 1].token = NUM;
        new_ts[i - 1].value = n;
        break;
      case MINUS:
        n = new_ts[i - 1].value - new_ts[i + 1].value;
        new_ts[i - 1].token = NUM;
        new_ts[i - 1].value = n;
        break;
      default:
        break;
      }

      for(h = 0; h < 2; h++){
        for(j = i; j < index - 1; j++){
          new_ts[j] = new_ts[j + 1];
        }
        index--;
      }
      i--;
    }
  }

  val.option = SOME;
  val.value = new_ts[0].value;
  
  return val;
}

Value Evaluate(Token *ts){
  int i;
  for(i = 0; ts[i].token != NIL; i++);
  return Eval(ts, 0, i);
}
