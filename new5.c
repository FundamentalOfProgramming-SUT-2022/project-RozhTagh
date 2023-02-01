#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <malloc.h>

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

void * dashstr(){
    char dashstr[10];
    char space;
    scanf("%s%c", dashstr, &space);
}

void dashfile(){
    char dashfile[10];
    char space;
    scanf("%s%c", dashfile, &space);
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

void cat(){

    char path[300];
    scanf("%[^\n]s", path);

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

    FILE * filepointer = fopen(path, "r");

    char character = fgetc(filepointer);

    while (character != EOF){
        printf ("%c", character);
        character = fgetc(filepointer);
    }
    printf("\n");
}

void insert(){

    char * path = get_path();

    char dashstr[10], space;
    scanf("%s%c", dashstr, &space);

    char to_insert[1000] = {NULL};
    char x;
    scanf("%c", &x);

    if (x == 34){
        for(int i = 0; ; i++){
            scanf("%c", &x);
            if (x != 34)
                to_insert[i] = x;
            else if(x == 34 && to_insert[i-1] == 92)
                to_insert[i] = x;
            else
                break;
        }
    }

    else{
        to_insert[0] = x;
        for(int i = 1; ;i++){
            scanf("%c", &x);
            if (x != 32)
                to_insert[i] = x;
            else
                break;
        }
    }

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

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

    int n, line = 1;

    for(int i = 0; ; i++){
        fputc(copy[i], filepointer2);
        if(copy[i] == '\n')
            line++;
        if(line == line_num){
            n = i;
            break;
        }
    }

    for(int i = 1; i <= start_pos; i++){
        if(copy[i+n] == EOF)
            break;
        fputc(copy[i+n], filepointer2);
    }

    for(int i = 0; i < strlen(to_insert); i++)
        fputc(to_insert[i], filepointer2);

    for(int i = 1; ; i++){
        if (copy[i+n+start_pos] == EOF)
            break;

        fputc(copy[i+n+start_pos], filepointer2);
    }

    printf("done\n");
    fclose(filepointer2);
}

void removestr(){

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

    int n = 0, line = 1;

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

    printf("success\n");
}

void copystr(){

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

        for(int i = 0; i<start_pos; i++){
            char pos_count = fgetc(filepointer1);
        }

        int position = ftell(filepointer1);
        rewind(filepointer1);

        for(int i = 0; i<position - size; i++){
            char pos_count = fgetc(filepointer1);
        }

        for(int i = 0; i<size; i++)
            fputc(fgetc(filepointer1), filepointer2);
    }

    printf("copied\n");

    fclose(filepointer1);
    fclose(filepointer2);

}

void pastestr(){

    char * path = get_path();

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

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

    int n, line = 1;

    for(int i = 0; ; i++){
        fputc(copy[i], filepointer3);
        if(copy[i] == '\n')
            line++;
        if(line == line_num){
            n = i;
            break;
        }
    }

    for(int i = 1; i <= start_pos; i++){
        if(copy[i+n] == EOF)
            break;
        fputc(copy[i+n], filepointer3);
    }

    for(int i = 0; ; i++){
        if(string[i] == EOF)
            break;
        fputc(string[i], filepointer3);
    }

    for(int i = 1; ; i++){
        if (copy[i+n+start_pos] == EOF)
            break;

        fputc(copy[i+n+start_pos], filepointer3);
    }

    printf("done\n");
    fclose(filepointer3);

}

void cutstr(){

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

    FILE * filepointer = fopen(path, "r+");

    rewind(filepointer);

    if (strcmp(dashdir, "-f") == 0){
        int line = 1;
        while(line != line_num){

            char c = fgetc(filepointer);
            if (c == '\n')
                line++;
        }

        for(int i = 0; i<start_pos; i++){
            char pos_count = fgetc(filepointer);
        }

        /*for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);*/

        for (int i = 0; i<size; i++)
            fprintf("%s", "\b \b");
    }

    else{
        int line = 1;
        while(line != line_num){
            char c = fgetc(filepointer);
            if (c == '\n')
                line++;
        }

        for(int i = start_pos-size; i<start_pos; i++){
            char pos_count = fgetc(filepointer);
        }

        /*for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);*/

        for (int i = 0; i<size; i++)
            fprintf("%s", "\b \b");
    }

    fclose(filepointer);

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

int is_equal(char word[], char pattern[]){

    if(strcmp(word, pattern) == 0)
        return 1;
    else
        return 0;
}

int find2(char string[], char path[], int num){

    if(num == 0){
    FILE * filepointer = fopen(path, "r");
    if (filepointer == NULL)
        printf("can't open");

    char * save = (char *)calloc(1000, sizeof(char));
    fscanf(filepointer, "%s", save);
    int found = 0;

    while(fgetc(filepointer) != EOF){

        if(match(string, save) == 1){
            return (ftell(filepointer) - strlen(save)-1);
            found = 1;
            break;
        }
        fscanf(filepointer, "%s", save);
    }

    if(found == 0)
        return -1;
    fclose(filepointer);
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
    fscanf(filepointer, "%s", save);
    int found = 0;

    while(fgetc(filepointer) != EOF){

        if(strstr(save, given_str)){
            return (ftell(filepointer) - strlen(save)-1);
            found = 1;
            break;
        }
        fscanf(filepointer, "%s", save);
    }

    if(found == 0)
        return -1;

    fclose(filepointer);
    }
}

void replace(){

    dashstr();

    char * str1 = get_string();

    dashstr();

    char * str2 = get_string();

    dashfile();

    char * path = get_path();

    FILE * filepointer = fopen(path, "r");
    char * copy = (char *)calloc(100000, sizeof(char));
    for(int i = 0; ; i++){
        copy[i] = fgetc(filepointer);
        if(copy[i] == EOF)
            break;
    }
    fclose(filepointer);

    int position = find1(str1, path, 0);

    FILE * filepointer2 = fopen(path, "w");

    for(int i = 0; i<position; i++)
        fputc(copy[i], filepointer2);
    fprintf(filepointer2, "%s", str2);

    int n;
    for(int i = position; ;i++){
        if(copy[i] == 32){
            n = i;
            break;
        }
    }

    for(int i = n; ;i++){
        if(copy[i] == EOF)
            break;
        fputc(copy[i], filepointer2);
    }

    fclose(filepointer2);
    printf("done\n");
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

    FILE * filepointer1 = fopen(path1, "r");
    FILE * filepointer2 = fopen(path2, "r");

    char line1[1000] = {NULL};
    char line2[1000] = {NULL};

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

void indent(){

    char * path = get_path();

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

    int round = 0;

    for(int i = 0; i<strlen(copy) ;i++){

        if(copy[i] == 123){
            printf("%c\n", copy[i]);
            round++;
            for(int i = 0; i<round; i++)
                printf("    ");
        }

        else if(copy[i] != 32 && copy[i+1] == 123)
            printf("%c ", copy[i]);

        else if(copy[i] == 32 && copy[i-1] == 123)
            continue;

        else if(copy[i] == 125){
            printf("\n");
            round--;
            for(int i = 0; i<round; i++)
                printf("    ");
            printf("%c", copy[i]);
        }

        else if(copy[i] != 32 && copy[i+1] == 125)
            printf("%c", copy[i]);

        else if(copy[i] == 32 && copy[i+1] == 125)
            continue;

        else if(copy[i] == EOF)
            break;

        else
            printf("%c", copy[i]);
    }

    printf("\n");
}

void undo(){

    char * path = (char *)calloc(100000, sizeof(char));
    scanf("%[^\n]s", path);

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
    printf("done");
}

int main(){

    char operation[30];
    //char dashfile[10];
    char space;

    while(1){

        scanf("%s", operation);

        if (strcmp(operation, "createfile") == 0){
            dashfile();
            char * path = (char *)calloc(100000, sizeof(char));
            scanf("%[^\n]s", path);
            createfile(path); //allright
        }

        else if (strcmp(operation, "cat") == 0){
            dashfile();
            cat(); //allright
        }

        else if (strcmp(operation, "insertstr") == 0){
            scanf("%s%c", dashfile, &space);
            insert(); //done
        }

        else if (strcmp(operation, "removestr") == 0){
            scanf("%s%c", dashfile, &space);
            removestr(); //not done
        }

        else if (strcmp(operation, "copystr") == 0){
            scanf("%s%c", dashfile, &space);
            copystr(); //allright
        }

        else if (strcmp(operation, "pastestr") == 0){
            scanf("%s%c", dashfile, &space);
            pastestr(); //done
        }

        else if (strcmp(operation, "cutstr") == 0){
            scanf("%s%c", dashfile, &space);
            cutstr(); //not done
        }

        else if (strcmp(operation, "tree") == 0){
            int num, depth;
            scanf("%d", &num);
            if (num == -1)
                depth = 30;
            else
                depth = num;
            print_tree("root/", 0, depth); //complete
        }

        else if(strcmp (operation, "find") == 0){
            dashstr();
            char * given_str = get_string();
            dashfile();
            char * path = get_path();
            printf("%d\n", find1(given_str, path, 0));
        }

        else if(strcmp (operation, "replace") == 0){
            replace();
        }

        else if(strcmp (operation, "grep") == 0){
            grep(); //complete
        }

        else if(strcmp (operation, "compare") == 0){
            compare(); //allright
        }

        else if(strcmp (operation, "auto-indent") == 0){
            scanf("%c", &space);
            indent(); //done
        }

        else if(strcmp (operation, "undo") == 0){
            scanf("%s%c", dashfile, &space);
            undo();
        }
    }

}
