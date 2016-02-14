#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cs402.h"
#include "my402list.h"

#define TRANSACTION_TYPESIZE 1
#define TRANSACTION_TIMESIZE 10

#define TRANSACTION_AMOUNTSIZE 10
#define TRANSACTION_DESCRIPTIONSIZE 24

FILE *file;

typedef struct transaction{
  char trans_type[2];
  int trans_time;
  int trans_amount;
  char trans_descrpt[25];      

  int trans_typesize;
  int trans_timesize;
  int trans_amountsize;
  int trans_descrptsize;
}transaction;

void PrintTransactionList(My402List *list){
  My402ListElem *elem = NULL;
  fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
  fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
  fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    int balance = 0;
  for(elem = My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)){


    time_t transtime = (time_t)(((transaction *)(elem->obj))->trans_time);
    char *s = ctime(&transtime);
    //char time[25];
    //strftime(time, sizeof(s), "%s %s %d %d", s);
    s[strlen(s)] = '\0';
    s[strlen(s)-1] =' ';
    char *start_ptr = NULL;
    char *space_ptr = NULL;
    int i = 0; 
    int inx = 0; 
    start_ptr = s;
    char time[25];
    memset(time, ' ' ,sizeof(time));
    while (i != 5){
      space_ptr = strchr(start_ptr, ' ');
      while(start_ptr != space_ptr){
    	if(i <= 2 || i >=4){
    	  time[inx] = *start_ptr;
    	  inx++;
    	  start_ptr++;
    	}
    	else{
    	  start_ptr = space_ptr;
    	  inx--;
    	}
      }
      time[inx] = ' ';
      inx++;
      space_ptr++;
      start_ptr = space_ptr;
      if(*start_ptr == ' '){
	start_ptr++;
	time[inx] = ' ';
	inx++;
      }
      i++;
    }
    time[inx-1] = '\0';
    fprintf(stdout, "| %s |", time);
    
    fprintf(stdout, " %s ", ((transaction*)(elem->obj))->trans_descrpt);

    char amount_buf[15];
    memset(amount_buf, ' ', sizeof(amount_buf));
    amount_buf[14] = '\0';

    char temp_amount[20];
    memset(temp_amount, 0 , sizeof(temp_amount));

    int amount = ((transaction*)(elem->obj))->trans_amount;

    int l = snprintf(temp_amount, sizeof(temp_amount), "%d", amount);
    if(l < 0){
      fprintf(stderr, "Amount could not be converted to string - %s\n", strerror(errno));
      exit(0);
    }

    i =0;
    inx = 12;
    //    for(i=strlen(temp_amount)-1; i >= 0 ; i--){
    i = strlen(temp_amount) - 1;
    if(i >= 9){
      strcpy(temp_amount, "?????????");
      i = 8;
    }
    if(i >= 2){
      while(i >= 0){
	if(inx == 10)
	  amount_buf[inx] = '.';
	else if(inx == 6)
	  amount_buf[inx] = ',';
	else if(inx == 2)
	  amount_buf[inx] = ',';
	else{
	  amount_buf[inx] = temp_amount[i];
	  i--;
	}
	inx--;
      }
    }
    else if(i <= 1){
      amount_buf[10] = '.';
      amount_buf[9] = '0';
      amount_buf[11] = '0';
      amount_buf[12] = '0';
      while(i >= 0){
	amount_buf[inx] = temp_amount[i];
	inx--;
	i--;
      }
    }
    if(strcmp((((transaction*)(elem->obj))->trans_type),"+") == 0){
      fprintf(stdout, "| %s |", amount_buf);
      balance += amount;
    }
    else if(strcmp((((transaction*)(elem->obj))->trans_type),"-") == 0){
      amount_buf[13] = ')';
      amount_buf[0] = '(';
      fprintf(stdout, "| %s |", amount_buf);
      balance -= amount;
    }

    
    memset(amount_buf, ' ', sizeof(amount_buf));
    amount_buf[14] = '\0';
    memset(temp_amount, 0 , sizeof(temp_amount));
    int flag =0;
    int temp_balance = 0;
    if(balance < 0){
      flag = 1;
      temp_balance = -balance;
    }
    else
      temp_balance = balance;
    l = snprintf(temp_amount, sizeof(temp_amount), "%d", temp_balance);
    if(l < 0){
      fprintf(stderr, "Amount could not be converted to string - %s\n", strerror(errno));
      exit(0);
    }
    i =0;
    inx = 12;
    i = strlen(temp_amount) - 1;
    if(i >= 9){
      strcpy(temp_amount, "?????????");
      i = 8;
    }
      while(i >= 0){
	if(inx == 10)
	  amount_buf[inx] = '.';
	else if(inx == 6)
	  amount_buf[inx] = ',';
	else if(inx == 2)
	  amount_buf[inx] = ',';
	else{
	  amount_buf[inx] = temp_amount[i];
	  i--;
	}
	inx--;
      }
      if(flag == 0)
	fprintf(stdout, " %s |\n", amount_buf);
      else if(flag == 1){
	amount_buf[13] = ')';
	amount_buf[0] = '(';
	fprintf(stdout, " %s |\n", amount_buf);
      }
   
    //if(((transaction*)(elem->obj))->trans_type
    // fprintf(stdout, "|  %f  |\n", (((transaction*)(elem->obj))->trans_amount)/100.0);
  }
  fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
}

int Transaction_Type(transaction *obj, char *start_ptr, char *tab_ptr){
  //1st Entry - Transaction Type
  obj->trans_typesize = 0;
  if(tab_ptr != NULL){
    while(start_ptr != tab_ptr){
      obj->trans_type[obj->trans_typesize] = *start_ptr;
      obj->trans_typesize++;
      start_ptr++;
      if(obj->trans_typesize > TRANSACTION_TYPESIZE){
	//fprintf(stderr, "Format malformed - more than one charater in transaction type\n");
	break;
      }
    }
    obj->trans_type[1] = '\0';
    if( (strcmp(obj->trans_type, "+") == 0 ) || (strcmp(obj->trans_type, "-")==0 ) ){
      return TRUE;
    }  
    else{
      fprintf(stderr, "Transaction type is neither '+' nor '-'\n");
      return FALSE;
    }
    //fprintf(stdout, "trans_type: %s, trans_typesize: %d\n", obj->trans_type, obj->trans_typesize);
  }
  else{
    fprintf(stderr, "Format malformed\n");
    return FALSE;
  }
}

int Transaction_TimeStamp(transaction *obj, char *start_ptr, char *tab_ptr){
  //2nd Entry - Transaction Time stamp
  char temp_timestamp[15];
  memset(temp_timestamp, 0, sizeof(temp_timestamp));
  time_t curtime;
  if(tab_ptr != NULL){
    while(start_ptr != tab_ptr){
      temp_timestamp[obj->trans_timesize] = *start_ptr;
      obj->trans_timesize++;
      start_ptr++;
      if(obj->trans_timesize > TRANSACTION_TIMESIZE){
	fprintf(stderr, "Format malformed - timestamp is bad more than required charater size, Quiting Program\n");
	return FALSE;
      }
    }
    temp_timestamp[obj->trans_timesize] = '\0';
    //    obj->trans_time = atoi(temp_timestamp);
    curtime = time(0);
    sscanf(temp_timestamp, "%d", &(obj->trans_time));
    if(obj->trans_time == 0){
      fprintf(stderr, "Could not covert timestamp into integer\n");
      return FALSE;
    }
    if(curtime < obj->trans_time){
      fprintf(stderr, "Transaction time more than the current time\n");
      return FALSE;
    }
    return TRUE;
    //else
       //fprintf(stdout, "trans_time: %d, trans_timesize: %d\n", obj->trans_time, obj->trans_timesize);
  }
  else{
    fprintf(stderr, "Format malformed\n");
    return FALSE;
  }
}

int Transaction_Amount(transaction *obj, char *start_ptr, char *tab_ptr){
  //3rd Entry - Transaction Amount
  obj->trans_amountsize = 0;
  char temp_amount[12];
  int digitsafterdecimal = 0;
  memset(temp_amount, 0, sizeof(temp_amount));
  if(tab_ptr != NULL){
    while(start_ptr != tab_ptr){
      if(*start_ptr != '.'){
      temp_amount[obj->trans_amountsize] = *start_ptr;
      obj->trans_amountsize++;
      digitsafterdecimal++;
      }
      else{
	digitsafterdecimal = 0;
      }
      start_ptr++;
      if(obj->trans_amountsize > TRANSACTION_AMOUNTSIZE){
	fprintf(stdout, "Amount is more than 10,000,000.00\n");
	return FALSE;
	//goto finish;
      }
    }
    temp_amount[obj->trans_amountsize] = '\0';
    
    if(digitsafterdecimal > 2){
      fprintf(stderr, "Too many digits after decimal\n");
      return FALSE;
    }
    int amount = 0;
    sscanf(temp_amount, "%d", &amount);
    if(amount == 0){
      fprintf(stderr, "Could not convert amount into integer - %d, %d\n", obj->trans_amount, obj->trans_time);
      return FALSE;
    }
    else{
      obj->trans_amount = (int)(amount);
      return TRUE;
      //fprintf(stdout, "trans_amount: %d, trans_amountsize: %d\n", obj->trans_amount, obj->trans_amountsize);
    }
  }
  else{
    fprintf(stderr, "Format malformed\n");
    return FALSE;
  }
}

void Transaction_Description(transaction *obj, char *start_ptr, char *tab_ptr){
  //4th Entry - Transaction Description
  char temp_descrpt[100];
  memset(temp_descrpt, 0 ,sizeof(temp_descrpt));
  if(tab_ptr != NULL){
    while(start_ptr != tab_ptr){
      temp_descrpt[obj->trans_descrptsize] = *start_ptr;
      obj->trans_descrptsize++;
      start_ptr++;
    }
    char *tmp = temp_descrpt;
    int i;
    while(*tmp == ' '){
      tmp++;
      obj->trans_descrptsize--;
    }
    memset(obj->trans_descrpt, ' ', sizeof(obj->trans_descrpt));
    //obj->trans_descrpt[strlen(obj->trans_descrpt)] = '\0';
    //strcpy(obj->trans_descrpt, tmp);
    // sscanf(tmp, "%s", &(obj->trans_descrpt));
    
    
    i = 0;
    while(i != obj->trans_descrptsize){
      obj->trans_descrpt[i++] = *tmp++;
      if(i >= TRANSACTION_DESCRIPTIONSIZE){
  	//fprintf(stdout, "Description more than 24 characters, truncating the remaining\n");
  	break;
      }
    }
    /* int j = 0; */
    /* for(j = i; j<=23 ; j++){ */
    /*   obj->trans_descrpt[j] = ' '; */
    /* } */
    //  next:
    obj->trans_descrpt[24] = '\0';
    //fprintf(stdout, "trans_descrpt: %s, trans_descrptsize: %d\n", obj->trans_descrpt, obj->trans_descrptsize);
  }
  else
    fprintf(stderr, "Format malformed\n"); 
}

int ReadFile(int argc, char *argv[]){
  struct stat isdir;
  if(argc > 2){
    if(strcmp(argv[1], "sort") == 0){
      if(!(file = fopen(argv[2],"r"))){
	fprintf(stderr, "%s\n ", strerror(errno));  
	return FALSE;
      }
      else{
	if((stat(argv[2], &isdir)==0) && S_ISDIR(isdir.st_mode)){
	  fprintf(stderr, "Is a directory - %s\n", argv[2]);
	  return FALSE;
	}
	return TRUE;
      }
    }
    else{
      fprintf(stderr, "Malformed Command\n"); // donot use errno here - it prints success
      return FALSE;
    }
  }
  else  if(argc == 2){
    if(strcmp(argv[1], "sort") == 0){
      file = stdin;
      //ParseFile(file);
      return TRUE;
    }
    else{
      fprintf(stderr, "Malformed Command\n");
      return FALSE;
    }
  }
  else{
    fprintf(stderr, "Malformed Command\n");
    return FALSE;
  }
}
  
void ParseFile(FILE *file){
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  
  transaction *obj = NULL;
  My402List list;
  memset(&list, 0, sizeof(My402List));
  (void)My402ListInit(&list);

  while(fgets(buffer, sizeof(buffer), file) != NULL){
    obj = (transaction*)malloc(sizeof(transaction));
    if(obj == NULL){
      fprintf(stderr, "%s\n", strerror(errno));
    }
    else{
      char *start_ptr = buffer;
      char *tab_ptr = strchr(start_ptr, '\t');
      
      int noofptr = 0;
      while(tab_ptr != NULL){
	noofptr++;
	tab_ptr++;
	start_ptr = tab_ptr;
	tab_ptr = strchr(start_ptr, '\t');
      }
      
      if(noofptr > 3){
	fprintf(stderr, "Too many tabs, Clearing memory and Quiting Program\n");
	free(obj);
	My402ListUnlinkAll(&list);
	exit(0);
      }

      if(noofptr < 3){
	fprintf(stderr, "Format Malformed, Clearing memory and Quiting Program\n");
	free(obj);
	My402ListUnlinkAll(&list);
	exit(0);
      }

      start_ptr = buffer;
      tab_ptr = strchr(start_ptr, '\t');

      if(Transaction_Type(obj, start_ptr, tab_ptr) == 0){
	fprintf(stderr, "Transaction Type failed, Clearing Memory and Quiting Program\n");
	free(obj);
	My402ListUnlinkAll(&list);
	exit(0);
      }
      tab_ptr++;
      start_ptr = tab_ptr;
      tab_ptr = strchr(start_ptr, '\t');
      
      if(Transaction_TimeStamp(obj, start_ptr, tab_ptr) == 0){
	fprintf(stderr, "Transaction Time failed, Clearing Memory and Quiting Program\n");
	free(obj);
	My402ListUnlinkAll(&list);
	exit(0);
      }

      tab_ptr++;
      start_ptr = tab_ptr;
      tab_ptr = strchr(start_ptr, '\t');
      
      if(Transaction_Amount(obj, start_ptr, tab_ptr) == 0){
	fprintf(stderr, "Transaction Amount failed, Clearing Memory and Quiting Program\n");
	free(obj);
	My402ListUnlinkAll(&list);
	exit(0);
      }

      tab_ptr++;
      start_ptr = tab_ptr;
      tab_ptr = strchr(start_ptr, '\n');

      Transaction_Description(obj, start_ptr, tab_ptr);
      

      //Now the struct is filled with the string fields read from the first line of the file
      if(My402ListLength(&list) == 0)
	(void)My402ListPrepend(&list, obj);
      else{
	My402ListElem *elem = NULL;
	for(elem = My402ListFirst(&list); elem != NULL; elem=My402ListNext(&list, elem)){
	  if((((transaction*)(elem->obj))->trans_time == obj->trans_time)){
	    fprintf(stderr, "Error - Two entry at same time, Quiting program\n");
	    free(obj);
	    My402ListUnlinkAll(&list);
	    exit(0);
	  }
	  if((((transaction*)(elem->obj))->trans_time > obj->trans_time)){
	    (void)My402ListInsertBefore(&list, obj, elem);
	    break;
	  }
	  if(My402ListNext(&list, elem) == NULL){
	    (void)My402ListAppend(&list, obj);
	    break;
	  }
	}
      }
    }
  }
  PrintTransactionList(&list);
  My402ListUnlinkAll(&list);
}

int main(int argc, char *argv[]){
  if(ReadFile(argc, argv) == 0){
    exit(0);
  }
  ParseFile(file);
  fclose(file);
  //  PrintFile();
  return 0;
}
