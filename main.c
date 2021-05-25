#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

int tokenCount = 0;
int commandCount = 0;
int isAssign = 0;
int isTektirnak = 0;
int isPipeline = 0;
char **tokens;
char **commands;
char *ident;
char *temp;

void strtrim(const char *input, char *result)
{
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (!isspace((unsigned char) input[i])) {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
}

int assignCount=0;
char line[1024] = "c = $a + 2";

char cmd[5][10] = { "echo", "cp", "wc", "cat", "expr" };


struct SymbolTable {
    char symbol[10];
    int value;
};
struct SymbolTable ST[20];

struct ParseTable
{
    int exists;
    char cmd[10];
    char par1[10];
    char par2[10];
    char par3[10];
};
struct ParseTable *PT;


void print()
{
    
    for (size_t i = 0; i < 3; i++)
    {
        printf("%d\t;\t%s\t;\t%s\t;\t%s\t;\t%s\n", PT[i].exists, PT[i].cmd, PT[i].par1, PT[i].par2, PT[i].par3);
    }
}

char **str_split(char *a_str, const char a_delim, int *_count)
{
    
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    
    result =(char **) malloc(sizeof(char *) * count);
    
    if (result)
    {
        size_t idx = 0;
        char *token = strtok(a_str, delim);
        
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        
        //assert(idx == count - 1);
        *(result + idx) = 0;
    }
    *_count = count - 1;
    
    return result;
}

int searchST(char *src){
    int j;
    for(j=0;j<assignCount;j++){
        if(src == ST[j].symbol){
            return ST[j].value;
        }
    }
    return 0;
}

//PART 2
void assign(char * ident,int value){
    int i;
    strtrim(ident,ident);
    for(i=0;i<assignCount;i++){
        
        if(!strcmp(ident, ST[i].symbol)){
            ST[i].value = value;
            return;
        }
    }
    strcpy(ST[assignCount].symbol, ident);
    ST[assignCount].value = value;
    assignCount++;
}

void echo_p(char * str){
    if(strstr(str,"$")){
        
        tokens = str_split(str, '$', &tokenCount);
        printf("\ntoken is :%s",tokens[0]);
        int j;
        for(j=0;j<assignCount;j++){
            printf("\ndeneme !%s!",tokens[j]);
            if(!strcmp(tokens[0], ST[j].symbol)){
                printf ("\n%d",ST[j].value);
                return;
            }
            //else{
              //  printf ("Variable not found");
            //}
            
        }
    }
    else{
       
        printf("%s\n", str);
    }
    
}


/*void cp(char *fs, char *fd){
    FILE *filepointer1, *filepointer2;
    char index;
    filepointer1=fopen(fs,"r");
    filepointer2=fopen(fd,"w");
    index = fgetc(filepointer1);
    while (index != EOF){
        fputc(index,filepointer2);
        index=fgetc(filepointer1);
    }
           fclose(filepointer1);
           fclose(filepointer2);
}*/

void cp( char * filename, char * filename2 ){
    FILE *fptr1, *fptr2;
    char c;
    // Open one file for reading
    fptr1 = fopen(filename, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", filename);
        exit(0);
    }


    // Open another file for writing
    fptr2 = fopen(filename2, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", filename2);
        exit(0);
    }
    
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    
    printf("\nContents copied to %s", filename2);
    
    fclose(fptr1);
    fclose(fptr2);
    return ;
}



int wc(char opt, char *filename){
    FILE *fp = fopen (filename,"r");
    char index;
    int l=0,w=0,b=0;
    index=fgetc(fp);
 while (index != EOF ){
     index=fgetc(fp);
     if (index == '\n'){
         l++;
     }
     if (index == ' ' || index == '\n'){
         w++;
     }
     if (index != ' ' && index != '\n'){
         b++;
     }
    }
    fclose(fp);
    if (opt == "l"){
        printf("%d\n",l);
        return l;
    }
    else if(opt == "w"){
        printf ("%d",w);
        return w;
    }
    else if (opt == "b"){
        printf("%d\n",b);
        return b;
    }
    else{
        printf("%d %d %d\n",l,w,b);
    }
return 0;
}


void cat(char *filename){
    FILE *fptr = fopen(filename,"r");
    char index;
    index=fgetc(fptr);
    while (index != EOF ){
        printf ("%c",index);
        index = fgetc(fptr);
    }
}
           
        
int expr(char * opnd1, char opr, char * opnd2){
    int j;
    int val1 = 0;
    int val2 = 0;
    int result = 0;
    bool flag=false;
    bool flag2=false;
    for(j=0;j<assignCount;j++){
        printf("\ndeneme !%s!",tokens[j]);
        if(!strcmp(opnd1, ST[j].symbol)){
            printf ("\n%d",ST[j].value);
            val1 = ST[j].value;
            flag=true;
        }
        //else{
        //  printf ("Variable not found");
        //}
        
    }
    if (!flag){
        val1 = atoi(opnd1);
    }
    
    for(j=0;j<assignCount;j++){
        
        printf("\ndeneme !%s!",tokens[j]);
        if(!strcmp(opnd2, ST[j].symbol)){
            printf ("\n%d",ST[j].value);
            val2 = ST[j].value;
            flag2=true;
        }
        //else{
        //  printf ("Variable not found");
        //}
        
    }
    if (!flag2){
        val2 = atoi(opnd2);
    }
    
    printf ("\nval1: %d",val1);
    printf ("\nval2: %d",val2);
    
    if (opr=='+'){
        result=val1+val2;
    }
    else if (opr=='-'){
        result=val1-val2;
    }
    else if (opr=='*'){
        result=val1*val2;
    }
    else if (opr=='/'){
        result=val1/val2;
    }
    else if (opr=='%'){
        result=val1%val2;
    }
    
    
    printf ("\nresult: %d\n",result);
    return 0;
}



//PART 2 END

int main(int argc, char const *argv[])
{

    PT =(struct ParseTable*) calloc(sizeof(struct ParseTable), 3); //iÁi bo˛ olsun diye bˆyle yapt˝k
    //fgets(line, 1024, stdin);
    
    commands = str_split(line, '|', &commandCount);
    
    if (commandCount > 1)
    {
        
        for (size_t i = 0; i < strlen(line); i++)
        {
            if (line[i] == '|')
                line[i] = '\0';
        }
    }
    
    char *data = commands[0];
    char *data1 = commands[1];
    if (commandCount > 1) {
        if (data1[-1] == ' ')
            data1[-1] = '\0';
        if (data1[0] == ' ')
            data1++;
    }
    
    
    printf("data:\t%s\ndata1:\t%s\n", data, data1);
    
    int i, j;
    for (i = 0; i < strlen(data); i++)
    {
        if (data[i] == '=')
            isAssign = 1;
        if (data[i] == '`')
        {
            data[i] = ' ';
            isTektirnak = 1;
        }
    }
    
    
    char copy[1024];
    strcpy(copy, data);
    tokens = str_split(copy, ' ', &tokenCount);
    
    
    if (isAssign) {
        int x;
        char copy[1024];
        strcpy(copy, data);
        ident = str_split(copy, '=', &x)[0];
        
        strcpy(copy, data);
        temp = str_split(copy, '=', &x)[1];
        temp++;
        
        assign(ident,atoi(temp));
        
        PT[2].exists = 1;
        strcpy(PT[2].par1, ident);
        
        tokens = str_split(temp, ' ', &tokenCount);
        
    }
    
    if (!strcmp(cmd[0], tokens[0]))
    {
        //printf("echo"); //TODO: d¸zelt
        
        
        
        
        PT[0].exists = 1;
        strcpy(PT[0].cmd, cmd[0]);
        strcpy(PT[0].par1, tokens[1]);
        
        
        
        echo_p(PT[0].par1);
    }
    else if (!strcmp(cmd[1], tokens[0]))
    {
        //printf("cp");
        PT[0].exists = 1;
        strcpy(PT[0].cmd, cmd[1]);
        strcpy(PT[0].par1, tokens[1]);
        if (tokenCount == 3)
            strcpy(PT[0].par2, tokens[2]);
    }
    else if (!strcmp(cmd[2], tokens[0]))
    {
        //printf("wc");
        PT[0].exists = 1;
        strcpy(PT[0].cmd, cmd[2]);
        strcpy(PT[0].par1, tokens[1]);
        if (tokenCount == 3)
            
            strcpy(PT[0].par2, tokens[2]);
    }
    else if (!strcmp(cmd[3], tokens[0]))
    {
        //printf("cat");
        PT[0].exists = 1;
        strcpy(PT[0].cmd, cmd[3]);
        strcpy(PT[0].par1, tokens[1]);
    }
    else if (!strcmp(cmd[4], tokens[0]))
    {
        //printf("expr");
        PT[0].exists = 1;
        strcpy(PT[0].cmd, cmd[4]);
        strcpy(PT[0].par1, tokens[1]);
        if (tokenCount == 3)
            strcpy(PT[0].par2, tokens[2]);
        else if (tokenCount >3) {
            strcpy(PT[0].par2, tokens[2]);
            strcpy(PT[0].par3, tokens[3]);
        }
        
    }
    
    
    
    if (commandCount == 2) {
        isAssign = 0;
        isTektirnak = 0;
        for (i = 0; i < strlen(data1); i++)
        {
            if (data1[i] == '=')
                isAssign = 1;
            if (data1[i] == '`')
            {
                data1[i] = ' ';
                isTektirnak = 1;
            }
        }
        
        strcpy(copy, data1);
        tokens = str_split(copy, ' ', &tokenCount);
        
        if (isAssign) {
            int x;
            char copy[1024];
            strcpy(copy, data1);
            ident = str_split(copy, '=', &x)[0];
            strcpy(copy, data1);
            temp = str_split(copy, '=', &x)[1];
            temp++;
             assign(ident,atoi(temp));
            
            PT[2].exists = 1;
            strcpy(PT[2].par1, ident);
            
            tokens = str_split(temp, ' ', &tokenCount);
            
        }
        
        if (!strcmp(cmd[0], tokens[0]))
        {
            //printf("echo"); //TODO: d¸zelt
            
            PT[1].exists = 1;
            strcpy(PT[1].cmd, cmd[0]);
            strcpy(PT[1].par1, tokens[1]);
        }
        else if (!strcmp(cmd[1], tokens[0]))
        {
            //printf("cp");
            PT[1].exists = 1;
            strcpy(PT[1].cmd, cmd[1]);
            strcpy(PT[1].par1, tokens[1]);
            if (tokenCount == 3)
                strcpy(PT[1].par2, tokens[2]);
        }
        else if (!strcmp(cmd[2], tokens[0]))
        {
            //printf("wc");
            PT[1].exists = 1;
            
            strcpy(PT[1].cmd, cmd[2]);
            strcpy(PT[1].par1, tokens[1]);
            if (tokenCount == 3)
                
                strcpy(PT[1].par2, tokens[2]);
        }
        else if (!strcmp(cmd[3], tokens[0]))
        {
            //printf("cat");
            PT[1].exists = 1;
            strcpy(PT[1].cmd, cmd[3]);
            strcpy(PT[1].par1, tokens[1]);
        }
        else if (!strcmp(cmd[4], tokens[0]))
        {
            //printf("expr");
            PT[1].exists = 1;
            strcpy(PT[1].cmd, cmd[4]);
            strcpy(PT[1].par1, tokens[1]);
            if (tokenCount == 3)
                strcpy(PT[1].par2, tokens[2]);
            else if (tokenCount > 3) {
                strcpy(PT[1].par2, tokens[2]);
                strcpy(PT[1].par3, tokens[3]);
            }
        }
        
    }
    getchar();
    print();
    expr("50",'-',"3");
    return 0;
}

