#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include "myconio.h"
#include <math.h>

int is_inserting = 0;

int does_exist(const char * path){

    if (access(path, F_OK) == -1)
        return 0;

    else
        return 1;
}

char * get_path(){

    char * path = (char *)calloc(100000, sizeof(char));
    char c;
    scanf("%c", &c);

    if (c == 34){
        for(int i = 0; ; i++){
            scanf("%c", &c);
            if (c != 34)
                path[i] = c;
            else
                break;
        }
    }

    else{
        path[0] = c;
        for(int i = 1; ;i++){
            scanf("%c", &c);
            if (c != 32 && c != '\n')
                path[i] = c;
            else
                break;
        }
    }

    return path;
}

char * get_string(){

    char * string = (char *)calloc(100000, sizeof(char));
    char c;
    scanf("%c", &c);

    if (c == 34){
        for(int i = 0; ; i++){
            scanf("%c", &c);
            if (c != 34)
                string[i] = c;
            else if(c == 34 && string[i-1] == 92)
                string[i] = c;
            else
                break;
        }
    }

    else{
        string[0] = c;
        for(int i = 1; ;i++){
            scanf("%c", &c);
            if (c != 32)
                string[i] = c;
            else
                break;
        }
    }
    return string;
}

char * dashstr(){
    char dashstr[10];
    char space;
    scanf("%s%c", dashstr, &space);
    return dashstr;
}

char * dashfile(){
    char dashfile[10];
    char space;
    scanf("%s%c", dashfile, &space);
    return dashfile;
}

int isDir(const char* fileName){

    struct stat path;
    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

int match(char *first, char *second) {
    if (*first == '\0' && *second == '\0')
        return 1;

    if (*first == '*') {
        while (*(first + 1) == '*') {
            first++;
        }
    }

    if (*first == '*' && *(first + 1) != '\0' && *second == '\0')
        return 0;

    if (*first == *second)
        return match(first + 1, second + 1);

    if (*first == '*')
        return match(first + 1, second) || match(first, second + 1);

    return 0;
}

void createfile(char * path){

    char * path2 = (char *)calloc(100000, sizeof(char));

    if (path[0] == 34){
        for (int i = 0; i<300; i++){
            if (path[i+1] != 34)
                path[i] = path[i+1];
            else{
                path[i] = NULL;
                path[i+1] = NULL;
                break;
            }
        }
    }

    if (does_exist(path) == 1){
        printf("file already exists\n");
        return;
    }

    char * token = {NULL};
    token = strtok(path, "/");

    while(token != NULL){
        strcat(path2, token);

        token = strtok(NULL, "/");
        if (token != NULL){

            if (does_exist(path2) == 0)
                mkdir(path2);

            strcat(path2, "/");
        }

        else{
            FILE * filepointer = fopen(path2, "w");
            fclose(filepointer);
        }
    }
}

void undo_saver(char * path){

    char * path2 = (char *)calloc(100000, sizeof(char));

    strcpy(path2, "undo/");
    strcat(path2, path);

    if(does_exist(path2) == 0)
        createfile(path2);

    FILE * open_undo = fopen(path2, "w");
    if(open_undo == 0)
        printf("not found");

    FILE * open_source = fopen(path, "r");

    char copier = fgetc(open_source);

    while(copier != EOF){
        fputc(copier, open_undo);
        copier = fgetc(open_source);
    }

    fclose(open_source);
    fclose(open_undo);

}

void insert(char * path, char * to_insert, int line_num, int start_pos){

    undo_saver(path);

    char * copy = (char *)malloc(100000 * sizeof(char));

    FILE * filepointer1 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer1);
        copy[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer1);

    FILE * filepointer2 = fopen(path, "w");

    int n =0, line = 1;

    for(int i = 0; line != line_num; i++){
        n = i;
        if (copy[i] == 10)
            line++;
    }

    for(int i = 0; i <= n+start_pos; i++)
        fputc(copy[i], filepointer2);

    for(int i = 0; i < strlen(to_insert); i++){

        if(to_insert[i] == 'n' && to_insert[i-1] == 92){
            if(to_insert[i-2] != 92)
                continue;
        }
        else if(to_insert[i] == 92 && to_insert[i-1] == 92)
            continue;

        else if (to_insert[i] == 34 && to_insert[i-1] == 92)
            continue;

        if(to_insert[i] == 92){
            if(to_insert[i+1] == 92)
                fprintf(filepointer2, "\\");

            else if(to_insert[i+1] == 'n')
                fprintf(filepointer2, "\n");

            else if(to_insert[i+1] == 34)
                fprintf(filepointer2, "\"");

            else
                fputc(to_insert[i], filepointer2);
        }

        else
            fputc(to_insert[i], filepointer2);
    }

    for(int i = 1; ; i++){
        if (copy[i+n+start_pos] == EOF)
            break;
        fputc(copy[i+n+start_pos], filepointer2);
    }

    fclose(filepointer2);
}

void removestr(char * path, int line_num, int start_pos, int size, char dashdir[]){

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    undo_saver(path);

    char * copy = (char *)calloc(100000, sizeof(char));

    FILE * filepointer1 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer1);
        copy[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer1);

    FILE * filepointer2 = fopen(path, "w");

    int line = 1, n;

    for(int i = 0; line != line_num; i++){
        n = i;
        if (copy[i] == 10)
            line++;
    }

    if(strcmp(dashdir, "-f") == 0){

        for(int i = 0; i <= n+start_pos; i++)
            fputc(copy[i], filepointer2);

        for(int i = 1; ; i++){
            if (copy[i+n+start_pos+size] == EOF)
                break;
            fputc(copy[i+n+start_pos+size], filepointer2);
        }
    }

    else{
        for(int i = 0; i <= n+start_pos-size; i++)
            fputc(copy[i], filepointer2);

        for(int i = 1; ; i++){
            if(copy[i+n+start_pos] == EOF)
                break;
            fputc(copy[i+n+start_pos], filepointer2);
        }
    }

    fclose(filepointer2);
}

void copystr(char * path, int line_num, int start_pos, int size, char dashdir[]){

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    FILE * filepointer1 = fopen(path, "r");
    FILE * filepointer2 = fopen("copy/copy.txt", "w");

    int line = 1;
    while(line != line_num){
        char c = fgetc(filepointer1);
        if (c == '\n')
            line++;
        }

    if (strcmp(dashdir, "-f") == 0){

        for(int i = 0; i<start_pos; i++){
            char pos_count = fgetc(filepointer1);
        }

        for (int i = 0; i<size; i++)
            fputc(fgetc(filepointer1), filepointer2);
    }

    else{

        for(int i = 0; i<start_pos - size; i++){
            char pos_count = fgetc(filepointer1);
        }

        for(int i = 0; i<size; i++)
            fputc(fgetc(filepointer1), filepointer2);
    }

    printf("copied\n");

    fclose(filepointer1);
    fclose(filepointer2);

}

void pastestr(char * path, int line_num, int start_pos){

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    undo_saver(path);

    char * copy = (char *)calloc(100000, sizeof(char));

    FILE * filepointer1 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer1);
        copy[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer1);

    FILE * filepointer2 = fopen("copy/copy.txt", "r");

    char * string = (char *)calloc(1000000, sizeof(char));
    char get;

    for(int i = 0; ; i++){
        get = fgetc(filepointer2);
        string[i] = get;
        if(get == EOF)
            break;
    }

    fclose(filepointer2);

    FILE * filepointer3 = fopen(path, "w");

    int n = 0, line = 1;

    for(int i = 0; ; i++){
        if(line == line_num){
            n = i;
            break;
        }
        fputc(copy[i], filepointer3);
        if(copy[i] == '\n')
            line++;
    }

    for(int i = 0; i < start_pos; i++){
        if(copy[i+n] == EOF)
            break;
        fputc(copy[i+n], filepointer3);
    }

    for(int i = 0; ; i++){
        if(string[i] == EOF)
            break;
        fputc(string[i], filepointer3);
    }

    for(int i = 0; ; i++){
        if (copy[i+n+start_pos] == EOF)
            break;

        fputc(copy[i+n+start_pos], filepointer3);
    }

    printf("done\n");
    fclose(filepointer3);

}

void cutstr(char * path, int line_num, int start_pos, int size, char dashdir[]){

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    undo_saver(path);

    FILE * filepointer1 = fopen(path, "r");
    FILE * filepointer2 = fopen("copy/copy.txt", "w");

    int line = 1;
    while(line != line_num){
        char c = fgetc(filepointer1);
        if (c == '\n')
            line++;
        }

    if (strcmp(dashdir, "-f") == 0){

        for(int i = 0; i<start_pos; i++){
            char pos_count = fgetc(filepointer1);
        }

        for (int i = 0; i<size; i++){
            char c = fgetc(filepointer1);
            fputc(c, filepointer2);
        }
    }

    else{

        for(int i = 0; i<start_pos - size; i++){
            char pos_count = fgetc(filepointer1);
        }

        for(int i = 0; i<size; i++){
            char c = fgetc(filepointer1);
            fputc(c, filepointer2);
        }
    }

    fclose(filepointer1);
    fclose(filepointer2);

    char * copy2 = (char *)calloc(100000, sizeof(char));

    FILE * filepointer3 = fopen(path, "r");

    char character;

    for(int i = 0; ; i++){
        character = fgetc(filepointer3);
        copy2[i] = character;
        if(character == EOF)
            break;
    }

    fclose(filepointer3);

    FILE * filepointer4 = fopen(path, "w");

    int line2 = 1, n;

    for(int i = 0; line2 != line_num; i++){
        n = i;
        if (copy2[i] == 10)
            line2++;
    }

    if(strcmp(dashdir, "-f") == 0){

        for(int i = 0; i <= n+start_pos; i++)
            fputc(copy2[i], filepointer4);

        for(int i = 1; ; i++){
            if (copy2[i+n+start_pos+size] == EOF)
                break;
            fputc(copy2[i+n+start_pos+size], filepointer4);
        }
    }

    else{
        for(int i = 0; i <= n+start_pos-size; i++)
            fputc(copy2[i], filepointer4);

        for(int i = 1; ; i++){
            if(copy2[i+n+start_pos] == EOF)
                break;
            fputc(copy2[i+n+start_pos], filepointer4);
        }
    }

    fclose(filepointer4);

}

void print_tree(char * basepath, int round, int max){

    if (round == max)
        return;

    char path[1000];

    DIR * directory;
    struct dirent *dir_entry;
    struct stat statbuf;

    directory = opendir(basepath);

    while ((dir_entry = readdir(directory)) != NULL){
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") ==  0)
            continue;
        else{
            if (round > 0){
                for (int i = 0; i<(round-1)*6+2; i++)
                    printf(" ");
                printf("|");
                for (int i = 0; i< 4; i++)
                    printf("-");
            }
            printf("%s\n", dir_entry->d_name);
            int mark = isDir(dir_entry->d_name);
            if (!mark){
                strcpy(path, basepath);
                strcat(path, "/");
                strcat(path, dir_entry->d_name);
                print_tree(path, round+1, max);
            }
        }
    }

    closedir(directory);
}

int find2(char string[], char path[], int num){

    if(num == 0){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string, save) == 1){
                return (ftell(filepointer) - strlen(save));
                found = 1;
                break;
            }
        }

        if(found == 0){
            return -1;
        }
        fclose(filepointer);
    }

    else if(num == 1){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(100000, sizeof(char));
        int * word_num = (int *)calloc(10000, sizeof(int));
        int word_count = 0;
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(match(string, save) == 1){
                found = 1;
                printf("%d\n", word_count);
                break;
            }
        }
        if(found == 0)
            printf("-1\n");

        fclose(filepointer);
    }

    else if(num == 2){

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int * pointer = (int *)calloc(1000, sizeof(int));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string, save) == 1){
                pointer[found] = ftell(filepointer) - strlen(save);
                found++;
            }
        }

        if(found == 0)
            printf("-1\n");

        else{
            for(int i = 0; i<found; i++){
                if (i == found-1)
                    printf("%d\n", pointer[i]);
                else
                    printf("%d, ", pointer[i]);
            }
        }

        fclose(filepointer);
    }

    else if(num == 3){

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string, save) == 1)
                found++;
        }
        printf("%d\n", found);
        fclose(filepointer);
    }

    else if(num == 4){
        int at_num;
        scanf("%d", &at_num);

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string, save) == 1)
                found++;

            if(found == at_num){
                return ftell(filepointer) - strlen(save);
                break;
            }
        }
        if(found < at_num){
            return -1;
        }
    }

    else if(num == 5){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(100000, sizeof(char));
        int * word_num = (int *)calloc(10000, sizeof(int));
        int word_count = 0;
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(match(string, save) == 1){
                word_num[found] = word_count;
                found++;
            }
        }

        fclose(filepointer);

        for(int i = 0; i<found; i++){
            if(i == found-1){
                printf("%d\n", word_num[i]);
                break;
            }
            printf("%d, ", word_num[i]);
        }
    }

    else if(num == 6){
        int at_num;
        scanf("%d", &at_num);

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;
        int word_count = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(match(string, save) == 1)
                found++;

            if(found == at_num){
                printf("%d\n", word_count);
                break;
            }
        }
        if(found < at_num){
            printf("-1\n");
            return -1;
        }
    }
}

int find1(char given_str[], char * path, int num){

    for(int i = 1; i<strlen(given_str); i++){
        if((given_str[i] == 42 && given_str[i-1] != 92) || given_str[0] == 42){
            find2(given_str, path, num);
            return;
        }
    }

    if(strstr(given_str, "\*")){

        int length = strlen(given_str);

        for(int i = 1; i<length; i++){
            if (given_str[i-1] == 92 && given_str[i] == 42){
                given_str[i-1] = '*';
                for(int j = i; j<length-1; j++){
                    given_str[j] = given_str[j+1];
                }
                given_str[length-1] = NULL;
            }
        }
    }

    if (num == 0){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(strstr(save, given_str)){
                return (ftell(filepointer) - strlen(save));
                found = 1;
                break;
            }
        }

        if(found == 0){
            return -1;
        }

        fclose(filepointer);
    }

    else if(num == 1){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(100000, sizeof(char));
        int * word_num = (int *)calloc(10000, sizeof(int));
        int word_count = 0;
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(strstr(save, given_str)){
                found = 1;
                printf("%d\n", word_count);
                break;
            }
        }
        if (found == 0)
            printf("-1\n");

        fclose(filepointer);
    }

    else if (num == 2){
        FILE * filepointer = fopen(path, "r");

        char * save = (char *)calloc(1000, sizeof(char));
        int * pointer = (int *)calloc(1000, sizeof(int));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(strstr(save, given_str)){
                pointer[found] = ftell(filepointer) - strlen(save);
                found++;
            }
        }

        if(found == 0){
            printf("-1\n");
        }

        else{
            for(int i = 0; i<found; i++){
                if (i == found-1)
                    printf("%d\n", pointer[i]);
                else
                    printf("%d, ", pointer[i]);
            }
        }

        fclose(filepointer);
    }

    else if (num == 3){
        FILE * filepointer = fopen(path, "r");

        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(strstr(save, given_str))
                found++;
        }
        fclose(filepointer);
        printf("%d\n", found);
    }

    else if (num == 4){
        int at_num;
        scanf("%d", &at_num);

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(strstr(save, given_str)){
                found++;
                if(found == at_num){
                    return ftell(filepointer) - strlen(save);
                    break;
                }
            }
        }

        if(found < at_num){
            return -1;
        }
        fclose(filepointer);
    }

    else if(num == 5){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(100000, sizeof(char));
        int * word_num = (int *)calloc(10000, sizeof(int));
        int word_count = 0;
        int found = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(strstr(save, given_str)){
                word_num[found] = word_count;
                found++;
            }
        }

        fclose(filepointer);

        for(int i = 0; i<found; i++){
            if(i == found-1){
                printf("%d\n", word_num[i]);
                break;
            }
            printf("%d, ", word_num[i]);
        }
    }

    else if (num == 6){
        int at_num;
        scanf("%d", &at_num);

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;
        int word_count = 0;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);
            word_count++;

            if(strstr(save, given_str)){
                found++;
                if(found == at_num){
                    printf("%d\n", word_count);
                    break;
                }
            }
        }

        if(found < at_num)
            printf("-1\n");
        fclose(filepointer);
    }
}

void replace2(char string1[], char string2[], char path[], int num){

    FILE * filepointer = fopen(path, "r");
    char * copy = (char *)calloc(100000, sizeof(char));
    for(int i = 0; ; i++){
        copy[i] = fgetc(filepointer);
        if(copy[i] == EOF)
            break;
    }
    fclose(filepointer);

    if(num == 0){
        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;
        int position, size;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string1, save) == 1){
                position = (ftell(filepointer) - strlen(save));
                size = strlen(save);
                found = 1;
                break;
            }
        }

        if(found == 0){
            printf("str1 not found\n");
            return;
        }
        fclose(filepointer);

        FILE * filepointer2 = fopen(path, "w");

        for(int i = 0; i <= position; i++)
            fputc(copy[i], filepointer2);
        fprintf(filepointer2, "%s", string2);

        for(int i = position + size +1; ;i++){
            if(copy[i] == EOF)
                break;
            fputc(copy[i], filepointer2);
        }
        fclose(filepointer2);
        printf("done\n");
    }

    else if(num == 1){
        int at_num;
        scanf("%d", &at_num);

        FILE * filepointer = fopen(path, "r");
        char * save = (char *)calloc(1000, sizeof(char));
        int found = 0;
        int position, size;

        while(fgetc(filepointer) != EOF){
            fscanf(filepointer, "%s", save);

            if(match(string1, save) == 1)
                found++;

            if(found == at_num){
                position = ftell(filepointer) - strlen(save);
                size = strlen(save);
                break;
            }
        }
        if(found < at_num){
            printf("str1 not found\n");
            return;
        }
        fclose(filepointer);

        FILE * filepointer2 = fopen(path, "w");

        for(int i = 0; i <= position; i++)
            fputc(copy[i], filepointer2);
        fprintf(filepointer2, "%s", string2);

        for(int i = position + size +1; ;i++){
            if(copy[i] == EOF)
                break;
            fputc(copy[i], filepointer2);
        }
        fclose(filepointer2);
        printf("done\n");
    }
}

void replace1(char string1[], char string2[], char * path, int num){

    for(int i = 1; i<strlen(string1); i++){
        if((string1[i] == 42 && string1[i-1] != 92) || string1[0] == 42){
            replace2(string1, string2, path, num);
            return;
        }
    }

    FILE * filepointer = fopen(path, "r");
    char * copy = (char *)calloc(100000, sizeof(char));
    for(int i = 0; ; i++){
        copy[i] = fgetc(filepointer);
        if(copy[i] == EOF)
            break;
    }
    fclose(filepointer);

    if(num == 0){

        int position = find1(string1, path, 0);

        if(position == -1){
            printf("str1 not found\n");
            return;
        }

        FILE * filepointer2 = fopen(path, "w");

        for(int i = 0; i <= position; i++)
            fputc(copy[i], filepointer2);
        fprintf(filepointer2, "%s", string2);

        for(int i = position + strlen(string1) +1; ;i++){
            if(copy[i] == EOF)
                break;
            fputc(copy[i], filepointer2);
        }
        fclose(filepointer2);
        printf("done\n");
    }

    else if(num == 1){

        int position = find1(string1, path, 4);

        if(position == -1){
            printf("str1 not found\n");
            return;
        }

        FILE * filepointer2 = fopen(path, "w");

        for(int i = 0; i <= position; i++)
            fputc(copy[i], filepointer2);
        fprintf(filepointer2, "%s", string2);

        for(int i = position + strlen(string1) +1; ;i++){
            if(copy[i] == EOF)
                break;
            fputc(copy[i], filepointer2);
        }
        fclose(filepointer2);
        printf("done\n");
    }

    else if(num == 2){
        FILE * filepointer2 = fopen(path, "r");

        char * save = (char *)calloc(1000, sizeof(char));
        int * pointer = (int *)calloc(1000, sizeof(int));
        int found = 0;

        while(fgetc(filepointer2) != EOF){
            fscanf(filepointer2, "%s", save);

            if(strstr(save, string1)){
                pointer[found] = ftell(filepointer2) - strlen(save);
                found++;
            }
        }

        if(found == 0){
            printf("str1 not found\n");
            return;
        }
        fclose(filepointer2);

        FILE * filepointer3 = fopen(path, "w");
        int n = 0;

        while(n < found){
            for(int i = 0; i <= pointer[n]; i++)
                fputc(copy[i], filepointer2);
            fprintf(filepointer2, "%s", string2);

            for(int i = pointer[n] + strlen(string1) +1; i<pointer[n+1] ;i++){
                if(copy[i] == EOF)
                    break;
                fputc(copy[i], filepointer3);
            }
            n++;
        }
        fclose(filepointer3);
        printf("done\n");
    }
}

void print_grep(char * path, char grep_text[]){

    FILE * filepointer = fopen(path, "r");
    char line[1000] = {NULL};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0){
            printf("%s: %s\n", path, line);
        }

        if (character == EOF)
            break;
    }

    fclose(filepointer);

}

void print_grep_c(char * path, char grep_text[], int * is_found){

    FILE * filepointer = fopen(path, "r");
    char line[1000] = {NULL};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0)
            (*is_found)++;

        if (character == EOF)
            break;
    }

    fclose(filepointer);
}

void print_grep_l(char * path, char grep_text[]){

    FILE * filepointer = fopen(path, "r");
    char line[1000] = {NULL};

    while(1){
        fscanf(filepointer, "%[^\n]s", line);
        char character = fgetc(filepointer);

        if (strstr(line, grep_text) != 0){
            printf("%s\n", path);
            break;
        }

        if (character == EOF)
            break;
    }

    fclose(filepointer);

}

void grep(){

    char unknown[7];
    scanf("%s", unknown);

    if (strcmp(unknown, "-str") == 0){

        char grep_text[1000];
        scanf("%s", grep_text);

        for (int i = 0; i<1000; i++){
            if (grep_text[i+1] != 34)
                grep_text[i] = grep_text[i+1];
            else{
                grep_text[i] = NULL;
                grep_text[i+1] = NULL;
                break;
            }
        }

        char dashfile[10];
        scanf("%s", dashfile);

        char address[1000];
        scanf("%[^\n]s", address);

        char* token = strtok(address, " ");

        while (token != NULL) {
            print_grep(token, grep_text);
            token = strtok(NULL, " ");
        }
    }

    else if(strcmp(unknown, "-c") == 0){
        char dashstr[10];
        scanf("%s", dashstr);

        char grep_text[1000];
        scanf("%s", grep_text);

        for (int i = 0; i<1000; i++){
            if (grep_text[i+1] != 34)
                grep_text[i] = grep_text[i+1];
            else{
                grep_text[i] = NULL;
                grep_text[i+1] = NULL;
                break;
            }
        }

        char dashfile[10];
        scanf("%s", dashfile);

        char address[1000];
        scanf("%[^\n]s", address);

        int is_found = 0;

        char* token = strtok(address, " ");

        while (token != NULL) {
            print_grep_c(token, grep_text, &is_found);
            token = strtok(NULL, " ");
        }

        printf("%d\n", is_found);
    }

    else if(strcmp(unknown, "-l") == 0){
        char dashstr[10];
        scanf("%s", dashstr);

        char grep_text[1000];
        scanf("%s", grep_text);

        for (int i = 0; i<1000; i++){
            if (grep_text[i+1] != 34)
                grep_text[i] = grep_text[i+1];
            else{
                grep_text[i] = NULL;
                grep_text[i+1] = NULL;
                break;
            }
        }

        char dashfile[10];
        scanf("%s", dashfile);

        char address[1000];
        scanf("%[^\n]s", address);

        char* token = strtok(address, " ");

        while (token != NULL) {
            print_grep_l(token, grep_text);
            token = strtok(NULL, " ");
        }
    }
}

void compare(){

    char * path1 = get_path();
    char * path2 = get_path();

    if(does_exist(path1) == 0){
        printf("file1 does not exist\n");
        return;
    }

    if(does_exist(path2) == 0){
        printf("file2 does not exist\n");
        return;
    }

    FILE * filepointer1 = fopen(path1, "r");
    FILE * filepointer2 = fopen(path2, "r");

    char * line1 = (char *)calloc(100000, sizeof(char));
    char * line2 = (char *)calloc(100000, sizeof(char));

    int line = 1;

    while(1){

        fscanf(filepointer1, "%[^\n]s", line1);
        fscanf(filepointer2, "%[^\n]s", line2);

        char character1 = fgetc(filepointer1);
        char character2 = fgetc(filepointer2);

        if(character1 == EOF && character2 == EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
        }
            break;
        }

        if(character1 == EOF && character2 != EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
            }

            int start_line = line+1;
            FILE * filepointer3 = fopen(path2, "r");
            char c;
            int end_line;
            while(c != EOF){
                c = fgetc(filepointer3);
                if (c == '\n')
                    end_line++;
            }
            fclose(filepointer3);

            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n", start_line, end_line+1);

            while (character2 != EOF){
                character2 = fgetc(filepointer2);
                printf ("%c", character2);
            }

            printf("\n");
            break;
        }

        if(character1 != EOF && character2 == EOF){

            if (strcmp(line1, line2) != 0){
                printf("============ #%d ============\n", line);
                printf("%s\n", line1);
                printf("%s\n", line2);
            }

            int start_line = line+1;
            FILE * filepointer3 = fopen(path2, "r");
            char c;
            int end_line;
            while(c != EOF){
                c = fgetc(filepointer3);
                if (c == '\n')
                    end_line++;
            }
            fclose(filepointer3);

            printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n", start_line, end_line+1);

            while (character1 != EOF){
                character1 = fgetc(filepointer1);
                printf ("%c", character1);
            }

            printf("\n");
            break;
        }

        if (strcmp(line1, line2) == 0){
            line++;
            continue;
        }

        else if (strcmp(line1, line2) != 0){
            printf("============ #%d ============\n", line);
            printf("%s\n", line1);
            printf("%s\n", line2);
        }

        line++;
    }

    fclose(filepointer1);
    fclose(filepointer2);
}

void indent(char * path){

    char * copy = (char *)calloc(100000, sizeof(char));
    FILE * filepointer = fopen(path, "r");
    copy[0] = fgetc(filepointer);
    while(copy[0] == 32){
        copy[0] = fgetc(filepointer);
    }

    int i = 1;

    while(1){
        copy[i] = fgetc(filepointer);
        while(copy[i] == 32 && copy[i-1] == 32){
            copy[i] = fgetc(filepointer);
        }
        if(copy[i] == EOF)
            break;
        else
            i++;
    }

    fclose(filepointer);

    FILE * filepointer2 = fopen(path, "w");

    int round = 0;

    for(int i = 0; i<strlen(copy) ;i++){

        if(copy[i] == 123){
            fprintf(filepointer2, "%c\n", copy[i]);
            round++;
            for(int i = 0; i<round; i++)
                fprintf(filepointer2, "    ");
        }

        else if(copy[i] != 32 && copy[i+1] == 123)
            fprintf(filepointer2, "%c ", copy[i]);

        else if(copy[i] == 32 && copy[i-1] == 123)
            continue;

        else if(copy[i] == 125){
            fprintf(filepointer2, "\n");
            round--;
            for(int i = 0; i<round; i++)
                fprintf(filepointer2, "    ");

            if(i != strlen(copy)-1)
                fprintf(filepointer2, "%c\n", copy[i]);
            else
                fprintf(filepointer2, "%c", copy[i]);
        }

        else if(copy[i] != 32 && copy[i+1] == 125)
            fprintf(filepointer2, "%c", copy[i]);

        else if(copy[i] == 32 && copy[i+1] == 125)
            continue;

        else if(copy[i] == EOF)
            break;

        else
            fprintf(filepointer2, "%c", copy[i]);
    }

    fclose(filepointer2);
}

void undo(char * path){

    char * path2 = (char *)calloc(100000, sizeof(char));

    strcpy(path2, "undo/");
    strcat(path2, path);

    FILE * open_undo = fopen(path2, "r");
    if (open_undo == NULL)
        printf("null 1");

    FILE * open_source = fopen(path, "w");

    char copier = fgetc(open_undo);

    while(copier != EOF){
        fputc(copier, open_source);
        copier = fgetc(open_undo);
    }

    fclose(open_source);
    fclose(open_undo);
}

void cat(char * path, int mode, int x1, int y1, int is_save){

    //normal = 0; visual = 1; insert = 2

    if(does_exist(path) == 0){
        printf("file does not exist\n");
        return;
    }

    FILE * filepointer_read = fopen(path, "r");
    int * each_line = (int *)calloc(1000, sizeof(int));

    int line = 1;
    int i = 0;

    while(1){
        char c = fgetc(filepointer_read);
        if(c == 10){
            line++;
            i = 0;
        }

        else if(c == EOF)
            break;

        else{
            i++;
            each_line[line-1] = i;
        }
    }

    fclose(filepointer_read);

    if(mode == 0) {

        textbackground(BLACK);
        clrscr();
        textbackground(BLACK);

        char * copy = (char *)calloc(100000, sizeof(char));
        FILE * filepointer_copy = fopen("copy/copy.txt", "r");

        for(int i = 0; ; i++){
            char c = fgetc(filepointer_copy);
            if(c == EOF)
                break;
            else
                copy[i] = c;
        }
        fclose(filepointer_copy);

        FILE *filepointer = fopen(path, "r");

        char character = fgetc(filepointer);
        int line = 1;
        int x_cursor = 5, y_cursor;
        printf("   1  ");

        while (character != EOF) {
            printf("%c", character);

            if (character == 10) {
                line++;

                if (line != 14) {
                    printf("  %2d  ", line);
                    x_cursor = 5;
                }
            }

            if (line == 14)
                break;

            else {
                x_cursor++;
            }

            character = fgetc(filepointer);
        }

        if (line < 14) {
            for (int y = 1; y < 14 - line; y++)
                printf("\n   ~  ");
            printf("\n");
        }

        textbackground(12);
        printf(" normal ");

        textbackground(8);
        printf("  %s  ", path);

        if(is_save == 0){
            printf("+");

            for (int y = 0; y < 49 - strlen(path); y++) {
                printf(" ");
            }
        }

        else{
            for (int y = 0; y < 50 - strlen(path); y++) {
                printf(" ");
            }
        }

        printf("\n");

        textbackground(5);
        printf("command args: ");

        for(int y = 0; y < 48; y++)
            printf(" ");

        if(strlen(copy) != 0){
            printf("\n");

            textbackground(2);
            printf("clipboard: %s", copy);

            for (int y = 0; y < 51 - strlen(copy); y++)
                printf(" ");
        }

        if (line == 14) {
            y_cursor = line - 1;
            gotoxy(x_cursor, y_cursor);
        } else {
            y_cursor = line;
            gotoxy(x_cursor, y_cursor);
        }

        char action = getch();

        int first_x, first_y;
        int second_x, second_y;

        while (1) {

            if (action == ':' || action == '/') {
                gotoxy(15, 15);

                textbackground(5);

                char operation[30];
                scanf("%s", operation);

                textbackground(5);

                if(strcmp(operation, "=") == 0){
                    indent(path);
                    cat(path, 0, 0, 0, 0);
                }

                if(strcmp(operation, "u") == 0){
                    undo(path);
                    cat(path, 0, 0, 0, 0);
                }

                if(strcmp(operation, ":save") == 0)
                    cat(path, 0, 0, 0, 1);

                if(strcmp(operation, ":open") == 0){
                    char space;
                    scanf("%c", &space);
                    char * path = get_path();
                    cat(path, 0, 0, 0, 1);
                }

                else
                    run(operation);
            }
            else if (action == 'k') {
                if(y_cursor != 1){
                    y_cursor = y_cursor - 1;

                if(each_line[y_cursor-1] < x_cursor -6)
                    x_cursor = each_line[y_cursor-1] + 6;

                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'j') {
                if(y_cursor != 13 && y_cursor != line){
                    y_cursor = y_cursor + 1;

                if(each_line[y_cursor-1] < x_cursor -6)
                    x_cursor = each_line[y_cursor-1] + 6;

                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'h') {
                if(x_cursor != 6){
                    x_cursor = x_cursor - 1;
                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'l') {
                if(x_cursor != each_line[y_cursor-1]+7){
                    x_cursor = x_cursor + 1;
                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'v') {
                first_x = x_cursor;
                first_y = y_cursor;
                cat(path, 1, first_x, first_y, 1);
            }

            else if(action == 'i'){
                first_x = x_cursor;
                first_y = y_cursor;
                cat(path, 2, first_x, first_y, 1);
            }

            action = getch();
        }
    }

    else if(mode == 1){

        textbackground(BLACK);
        clrscr();
        textbackground(BLACK);

        FILE *filepointer = fopen(path, "r");

        char character = fgetc(filepointer);
        int line = 1;
        int x_cursor, y_cursor;
        printf("   1  ");

        while (character != EOF) {
            printf("%c", character);

            if (character == 10) {
                line++;

                if (line != 14) {
                    printf("  %2d  ", line);
                    x_cursor = 5;
                }
            }

            if (line == 14)
                break;

            else {
                x_cursor++;
            }

            character = fgetc(filepointer);
        }

        if (line < 14) {
            for (int y = 1; y < 14 - line; y++)
                printf("\n   ~  ");
            printf("\n");
        }

        textbackground(12);
        printf(" visual ");

        textbackground(8);
        printf("  %s  ", path);

        if(is_save == 0){
            printf("+");

            for (int y = 0; y < 49 - strlen(path); y++) {
                printf(" ");
            }
        }

        else{
            for (int y = 0; y < 50 - strlen(path); y++) {
                printf(" ");
            }
        }

        printf("\n");

        textbackground(5);
        printf("command args: ");

        for (int y = 0; y < 48; y++)
            printf(" ");

        gotoxy(x1, y1);
        char action = getch();

        int first_x = x1, first_y = y1;
        x_cursor = x1, y_cursor = y1;
        int second_x, second_y;

        while (1) {

            if (action == ':' || action == '/') {
                gotoxy(15, 15);
                char c = getch();
                textbackground(5);
                while(c != 10){
                    printf("%c", c);
                    c = getch();
                }
            }

            else if (action == 'k') {

                if(y_cursor != 1){
                    y_cursor = y_cursor - 1;

                    if(each_line[y_cursor-1] < x_cursor -6)
                        x_cursor = each_line[y_cursor-1] + 6;

                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'j') {
                if(y_cursor != 13 && y_cursor != line){
                    y_cursor = y_cursor + 1;

                    if(each_line[y_cursor-1] < x_cursor -6)
                        x_cursor = each_line[y_cursor-1] + 6;

                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'h') {
                if(x_cursor != 6){
                    x_cursor = x_cursor - 1;
                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if (action == 'l') {

                if(x_cursor != each_line[y_cursor-1] +7){
                    x_cursor = x_cursor + 1;
                    gotoxy(x_cursor, y_cursor);
                }
            }

            else if(action == 'n'){
                cat(path, 0, 0, 0, is_save);
            }

            else if (action == 'd') {
                second_x = x_cursor;
                second_y = y_cursor;

                char dashdir[5];
                int size;

                if(second_x - first_x > 0){
                    strcpy(dashdir, "-f");
                    size = second_x - first_x +1;
                }

                else{
                    strcpy(dashdir, "-b");
                    size = first_x - second_x +1;
                }

                gotoxy(second_x, second_y);

                removestr(path, first_y, first_x - 7, size, dashdir);
                cat(path, 0, first_x, first_y, 0);
            }

            else if (action == 'x') {
                second_x = x_cursor;
                second_y = y_cursor;

                char dashdir[5];
                int size;

                if(second_x - first_x > 0){
                    strcpy(dashdir, "-f");
                    size = second_x - first_x +1;
                }

                else{
                    strcpy(dashdir, "-b");
                    size = first_x - second_x +1;
                }

                gotoxy(second_x, second_y);

                cutstr(path, first_y, first_x-7, size, dashdir);
                cat(path, 0, first_x, first_y, 0);
            }

            else if (action == 'c') {
                second_x = x_cursor;
                second_y = y_cursor;

                char dashdir[5];
                int size;

                if(second_x - first_x > 0){
                    strcpy(dashdir, "-f");
                    size = second_x - first_x +1;
                }

                else{
                    strcpy(dashdir, "-b");
                    size = first_x - second_x +1;
                }

                gotoxy(second_x, second_y);

                copystr(path, first_y, first_x-7, size, dashdir);
                cat(path, 0, second_x, second_y, 0);
            }

            action = getch();
        }

        if (action == ':' || action == '/')
            return;
    }

        if(mode == 2) {

        textbackground(BLACK);
        clrscr();
        textbackground(BLACK);

        char * copy = (char *)calloc(100000, sizeof(char));
        FILE * filepointer_copy = fopen("copy/copy.txt", "r");

        for(int i = 0; ; i++){
            char c = fgetc(filepointer_copy);
            if(c == EOF)
                break;
            else
                copy[i] = c;
        }
        fclose(filepointer_copy);

        FILE *filepointer = fopen(path, "r");

        char character = fgetc(filepointer);
        int line = 1;
        int x_cursor = 5, y_cursor;
        printf("   1  ");

        while (character != EOF) {
            printf("%c", character);

            if (character == 10) {
                line++;

                if (line != 14) {
                    printf("  %2d  ", line);
                    x_cursor = 5;
                }
            }

            if (line == 14)
                break;

            else {
                x_cursor++;
            }

            character = fgetc(filepointer);
        }

        if (line < 14) {
            for (int y = 1; y < 14 - line; y++)
                printf("\n   ~  ");
            printf("\n");
        }

        textbackground(12);
        printf(" insert ");

        textbackground(8);
        printf("  %s  ", path);

        if(is_save == 0){
            printf("+");

            for (int y = 0; y < 49 - strlen(path); y++) {
                printf(" ");
            }
        }

        else{
            for (int y = 0; y < 50 - strlen(path); y++) {
                printf(" ");
            }
        }

        printf("\n");

        textbackground(5);
        printf("command args: ");

        for(int y = 0; y < 48; y++)
            printf(" ");

        if(strlen(copy) != 0){
            printf("\n");

            textbackground(2);
            printf("clipboard: %s", copy);

            for (int y = 0; y < 51 - strlen(copy); y++)
                printf(" ");
        }

        if (line == 14) {
            y_cursor = line - 1;
            gotoxy(x_cursor, y_cursor);
        } else {
            y_cursor = line;
            gotoxy(x_cursor, y_cursor);
        }

        gotoxy(x1, y1);

        char action;

        if(is_inserting == 0)
            action = getch();
        else
            action = 'a';

        int first_x, first_y;
        int second_x, second_y;

        while (1) {

            if (action == ':' || action == '/') {
                gotoxy(15, 15);
                char c = getch();
                textbackground(5);
                while(c != 10){
                    printf("%c", c);
                    c = getch();
                }
            }

            else if (action == 'k') {

                if(y1 != 1){
                    y1 = y1 - 1;

                    if(each_line[y1-1] < x1-6)
                        x1 = each_line[y1-1] +6;

                    gotoxy(x1, y1);
                }
            }

            else if (action == 'j') {
                if(y1 != 13 && y1 != line){
                    y1 = y1 + 1;

                if(each_line[y1-1] < x1-6)
                    x1 = each_line[y1-1] +6;

                    gotoxy(x1, y1);
                }
            }

            else if (action == 'h') {

                if(x1 != 6){
                    x1 = x1 - 1;
                    gotoxy(x1, y1);
                }
            }

            else if (action == 'l') {

                if(x1 != each_line[y1-1] +7){
                    x1 = x1 + 1;
                    gotoxy(x1, y1);
                }
            }

            else if(action == 'n'){
                cat(path, 0, 0, 0, is_save);
            }

            else if (action == 'v') {
                cat(path, 1, x1-6, y1, 1);
            }

            else if(action == 'p'){
                pastestr(path, y1, x1-6);
                cat(path, 0, y1, x1-6, 0);
            }

            else if(action == 'a'){
                char * c = (char *)calloc(10000, sizeof(char));

                for(int i = 0; ; i++){
                    char x = getch();
                    if(x != '!')
                        c[i] = x;
                    else
                        break;
                }

                insert(path, c, y1, x1-6);
                cat(path, 0, y1, x1-6, 0);

                /*while(1){
                    c[0] = getch();
                    c[1] = NULL;
                    if(c[0] == '!'){
                        is_inserting = 0;
                        break;
                    }
                    else{
                        insert(path, c, y1, x1-6);
                        is_inserting = 1;
                        cat(path, 2, x1+1, y1, 0);
                        x1++;
                    }
                }*/
            }

            action = getch();
        }
    }
}

void run(char operation[]){

    char space;
    int round = 0;

        textbackground(BLACK);
        clrscr();
        textbackground(BLACK);

    while(1){

        if (strcmp(operation, "createfile") == 0){
            char * dash_file = dashfile();
            char * path = get_path();
            createfile(path); //allright
        }

        else if (strcmp(operation, "cat") == 0){
            char * dash_file = dashfile();
            char * path = get_path();
            cat(path, 0, 0, 0, 1); //allright
        }

        else if (strcmp(operation, "insertstr") == 0){

            char * dash_file = dashfile();
            char * path = get_path();

            dashstr();
            char * to_insert = get_string();

            char dashpos[10];
            scanf("%s", dashpos);

            int line_num, start_pos;
            scanf("%d:%d", &line_num, &start_pos);

            insert(path, to_insert, line_num, start_pos); //done
        }

        else if (strcmp(operation, "removestr") == 0){
            char * dash_file = dashfile();
            char * path = get_path();

            char dashpos[10];
            scanf("%s", dashpos);

            int line_num, start_pos;
            scanf("%d:%d", &line_num, &start_pos);

            char dashsize[10];
            scanf("%s", dashsize);

            int size;
            scanf("%d", &size);

            char dashdir[5];
            scanf("%s", dashdir);

            removestr(path, line_num, start_pos, size, dashdir); //done
        }

        else if (strcmp(operation, "copystr") == 0){
            dashfile();
            char * path = get_path();

            char dashpos[10];
            scanf("%s", dashpos);

            int line_num, start_pos;
            scanf("%d:%d", &line_num, &start_pos);

            char dashsize[10];
            scanf("%s", dashsize);

            int size;
            scanf("%d", &size);

            char dashdir[5];
            scanf("%s", dashdir);

            copystr(path, line_num, start_pos, size, dashdir); //allright
        }

        else if (strcmp(operation, "pastestr") == 0){
            char * dash_file = dashfile();
            char * path = get_path();

            char dashpos[10];
            scanf("%s", dashpos);

            int line_num, start_pos;
            scanf("%d:%d", &line_num, &start_pos);

            pastestr(path, line_num, start_pos); //done
        }

        else if (strcmp(operation, "cutstr") == 0){
            char * dash_file = dashfile();
                char * path = get_path();

            char dashpos[10];
            scanf("%s", dashpos);

            int line_num, start_pos;
            scanf("%d:%d", &line_num, &start_pos);

            char dashsize[10];
            scanf("%s", dashsize);

            int size;
            scanf("%d", &size);

            char dashdir[5];
            scanf("%s", dashdir);

            cutstr(path, line_num, start_pos, size, dashdir); //done
        }

        else if (strcmp(operation, "tree") == 0){
            int num, depth;
            scanf("%d", &num);
            if (num == -1)
                depth = 30;
            else if(num < -1){
                printf("invalid depth\n");
                continue;
            }
            else
                depth = num;
            print_tree("root/", 0, depth); //complete
        }

        else if(strcmp (operation, "find") == 0){
            char * dash_str = dashstr();
            char * given_str = get_string();
            char * dash_file = dashfile();
            char * path = get_path();
            if(does_exist(path) == 0){
                printf("file does not exist\n");
                continue;
            }
            char function[10];
            scanf("%s", function);
            int num;
            if(strcmp(function, "-f") == 0){
                num = 0;
                printf("%d\n", find1(given_str, path, num));
            }
            else if(strcmp(function, "-byword") == 0){
                num = 1;
                find1(given_str, path, num);
            }
            else if(strcmp(function, "-all") == 0){
                num = 2;
                find1(given_str, path, num);
            }
            else if(strcmp(function, "-count") == 0){
                num = 3;
                find1(given_str, path, num);
            }
            else if(strcmp(function, "-at") == 0){
                num = 4;
                printf("%d\n", find1(given_str, path, num));
            }
            else if(strcmp(function, "-all-byword") == 0 || strcmp(function, "-byword-all") == 0){
                num = 5;
                find1(given_str, path, num);
            }
            else if(strcmp(function, "-byword-at") == 0){
                num = 6;
                find1(given_str, path, num);
            }
        }

        else if(strcmp (operation, "replace") == 0){
            char * dash_str1 = dashstr();
            char * str1 = get_string();
            char * dash_str2 = dashstr();
            char * str2 = get_string();
            char * dash_file = dashfile();
            char * path = get_path();

            if(does_exist(path) == 0){
                printf("file does not exist\n");
                continue;
            }
            char function[10];
            scanf("%s", function);
            int num;
            if(strcmp(function, "-r") == 0)
                num = 0;
            else if(strcmp(function, "-at") == 0)
                num = 1;
            else if(strcmp(function, "-all") == 0)
                num = 2;
            replace1(str1, str2, path, num);
        }

        else if(strcmp (operation, "grep") == 0){
            grep(); //complete
        }

        else if(strcmp (operation, "compare") == 0){
            scanf("%c", &space);
            compare(); //allright
        }

        else if(strcmp (operation, "auto-indent") == 0){
            scanf("%c", &space);
            char * path = get_path();
            if(does_exist(path) == 0){
                printf("file does not exist\n");
                continue;
            }
            indent(path); //done
            printf("done\n");
        }

        else if(strcmp (operation, "undo") == 0){
            char * dash_file = dashfile();
            char * path = get_path();
            if(does_exist(path) == 0){
                printf("file does not exist\n");
                continue;
            }
            undo(path);
        }

        else
            printf("invalid command\n");
    }
}

int main(){

    char operation[30];
    scanf("%s", operation);
    run(operation);

}
