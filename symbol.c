#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"


enum symbols sy = beginsy;              /* curent symbol, default beginsy */
char ch = ' ';                          /* curent ch, default ' ' */
char id[IDLMX] = {0};                   /* curent id buffer, default 0 */
int inum = 0;                           /* curent inum, default 0 */
char strbuf[BUFLNX] = {0};              /* curent string buffer, default 0 */
char linebuf[BUFLNX] = {0};             /* curent line buffer, default 0 */
int line = 0;                           /* curent line number, default 0 */
int np = 0;                             /* curent char pointer, default 0 */
int len = -1;                           /* curent line length, default -1 */
int errpos = 0;                         /* curent symbol begin position, default 0*/
int cc = 0;                             /* curent line char counter, default 0 */

int binarySearch(){
    int low = 0, high = nkw;
    int mid;
    while(low <= high){
        mid = (low + high) / 2;
        if(strcmp(id, keys[mid]) < 0) high = mid - 1;
        if(strcmp(id, keys[mid]) > 0) low = mid + 1;
        if(strcmp(id, keys[mid]) == 0) return ksys[mid];
    }
    return ident;
}

void nextline(){
    if(fgets(linebuf,1000,srcfp) == NULL){
        len = -1;
        ch = 0;
        np = 0;
        cc = 0;
    }else{
        //printf("%s\n", linebuf);
        len = strlen(linebuf);
        if(linebuf[len-1] != '\n'){
            linebuf[len] = '\n';
            len += 1;
        }
        np = 0;
        cc = 0;
        ch = *(linebuf + np);
        np++;
        if(ch == '\t') cc += TABSTEP;
        else cc += 1;
        line += 1;
        /*
        int i;
        for(i = 0; i < np; i++) linebuf[i] = 0;
        np = 0;
        line += 1;
        nextch();
        */
    }
}

void nextch(){
    if(np < len){
        ch = *(linebuf + np);
        np++;
        if(ch == '\t') cc += TABSTEP;
        else cc += 1;
    }else{
        nextline();
    }
}

int isLetter(){
    if(ch == '_' || (ch >= 'a' && ch <= 'z' )|| (ch >= 'A' && ch <= 'Z'))
        return 1;
    return 0;
}

int isDigt(){
    if(ch >= '0' && ch <= '9') return 1;
    return 0;
}

int isTwoSps(){
    if(ch == '/' || ch == '<' || ch == '>' || ch == '=' || ch == '!')
        return 1;
    return 0;
}

int isOneSps(){
    if(ch == '+' || ch == '-' || ch == '*' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}'
        || ch == ',' || ch == ':' || ch == ';')
        return 1;
    return 0;
}

int isStrChar(){
    if(ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) return 1;
    return 0;
}

void parseIdent(){
    errpos = cc;
    int k = 0;
    while(k < IDLMX){
        id[k] = 0;
        k++;
    }
    k = 0;
    while(isLetter() || isDigt()){
        if(k < IDLMX){
            if(ch >= 'A' && ch <= 'Z') id[k++] = ch + 32;
            else id[k++] = ch;
        }
        nextch();
    }
    if(k >= IDLMX) error(id_len);
}

void parseIntcon(){
    errpos = cc;
    int k = 0;
    int long long num = 0;
    while(isDigt()){
        if(k < NUMLMX)
            num = num*10 + ch - '0';
        k++;
        nextch();
    }
    if(k >= NUMLMX){
        //ERROR("Num length limt\n");
        error(intcon_size);
        num = 0;
    }else if(num < INT_MIN || num > INT_MAX){
        //ERROR("Num limt\n");
        error(intcon_size);
        num = 0;
    }
    inum = num;
    sy = intcon;
}

void parseTwoSps(){
    errpos = cc;
    if(ch == '/'){
        //comments?
        char tmp;
        nextch();
        if(ch == '/'){
            //getchar
            /*
            nextch();
            while(ch != '\n') nextch();
            nextch();
            nextsy();
            */
            //gets
            nextline();
            //nextch();
            nextsy();
            return;
        }
        if(ch == '*'){ 
            nextch();
            tmp = ch;
            nextch();
            while(tmp != '*' || ch != '/'){
                tmp = ch;
                nextch();
                if(tmp == 0 || ch == 0){
                    sy = endsy;
                }
            }
            nextch();
            nextsy();
            return;
        }
        sy = idiv;
        return;
    }
    if(ch == '<'){
        nextch();
        if(ch == '='){
            sy = leq;
            nextch();
        }
        else sy = lss;
    }else if(ch == '>'){
        nextch();
        if(ch == '='){
            sy = geq;
            nextch();
        }
        else sy = gtr;
    }else if(ch == '='){
        nextch();
        if(ch == '='){
            sy = eql;
            nextch();
        }
        else sy = becomes;
    }else if(ch == '!'){
        nextch();
        if(ch == '='){
            sy = neq;
            nextch();
        }
        else{
            //if(ch != '\n') printf("Next char error!\n");
            //printf("%d %d\n", line, np);
            //ERROR("Invalid character !\n");
            error(char_ill);
            nextsy();
        }
    }
}

void parseOneSps(){
    int index;
    index = ch;
    errpos = cc;
    sy = sps[index];
    nextch();
}

void parseCharcon(){
    int k = 0;

    errpos = cc;
    nextch();
    inum = ch;
    k++;
    nextch();
    while(isDigt() || isLetter()){
        //ERROR("Parse charcon error\n");
        nextch();
        k++;
    }
    if(ch != '\''){
        error(miss_sq);
    }else if(k > 1){
        error(charcon_len);
    }else{
        sy = charcon;
        nextch();
    }
}

void parseStrcon(){
    errpos = cc;
    nextch();
    int k = 0;
    while(k < STRLMX){
        strbuf[k] = 0;
        k++;
    }
    k = 0;
    while(isStrChar()){
        if(k < STRLMX){
            strbuf[k++] = ch;
        }
        nextch();
    }
    //if(ch == '\n') printf("The next char is enter\n");
    if(k >= STRLMX){
        error(strcon_len);
    }
    if(ch != '"'){
        error(miss_dq);
    }else{
        nextch();
    }
    sy = strcon;
}

char *getName(){
    int len;
    char *name;
    int i;
    
    len = strlen(id);
    name = (char *)malloc(len + 1);
    if(name == NULL){
        fatal(3);
    }
    for(i = 0; i < len; i++){
        name[i] = id[i];
    }
    name[i] = '\0';
    return name;
}

char *getStrcon(){
    int len;
    char *name;
    int i;
    
    len = strlen(strbuf);
    name = (char *)malloc(len + 1);
    if(name == NULL){
        fatal(3);
    }
    for(i = 0; i < len; i++){
        name[i] = strbuf[i];
    }
    name[i] = '\0';
    return name;
}

void nextsy(){
    while(ch == ' ' || ch == '\t' || ch == '\n'){
        /*
        if(ch == '\n') nextline();
        else nextch();
        */
        nextch();
        errpos = cc;
    }
    //printf("%c", ch);
    if(ch == 0){
        sy = endsy;
        return;
    }
    if(isLetter()){
        parseIdent();
        sy = binarySearch();
        return;
    }
    if(isDigt()){
        parseIntcon();
        return;
    }
    if(isTwoSps()){
        parseTwoSps();
        return;
    }
    if(isOneSps()){
        parseOneSps();
        return;
    }
    if(ch == '\''){
        parseCharcon();
        return;
    }
    if(ch == '"'){
        parseStrcon();
        return;
    }
    error(char_ill);
    nextch();
    nextsy();
}