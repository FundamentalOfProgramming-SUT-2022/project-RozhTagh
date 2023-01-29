#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

char string[1000];
char path2[1000];

int isDir(const char* fileName){

    struct stat path;
    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

int does_exist(const char * path){

    if (access(path, F_OK) == -1)
        return 0;

    else
        return 1;
}

void createfile(){

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

    if (does_exist(path) == 1){
        printf("file already exists\n");
        return;
    }

    char * token = strtok(path, "/");

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

    char dashfile[10];
    scanf("%s", dashfile);

    char path[1000];
    scanf("%s", path);

    char space;
    char dashstr[10];
    scanf("%s%c", dashstr, &space);

    char to_insert[1000] = {NULL};
    char c;
    scanf("%c", &c);

    if (c == 34){
        for(int i = 0; ; i++){
            scanf("%c", &c);
            if (c != 34)
                to_insert[i] = c;
            else if(c == 34 && to_insert[i-1] == 92)
                to_insert[i] = c;
            else
                break;
        }
    }

    else{
        to_insert[0] = c;
        for(int i = 1; ;i++){
            scanf("%c", &c);
            if (c != 32)
                to_insert[i] = c;
            else
                break;
        }
    }

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

    FILE * filepointer = fopen(path, "a");

    rewind(filepointer);

    int line = 1;

    while(line != line_num){
        char x = fgetc(filepointer);
        if (x == '\n')
            line++;
    }

    for(int i = 0; i<start_pos; i++){
        char pos_count = fgetc(filepointer);
    }

    for(int i = 0; i<strlen(to_insert); i++){

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
                fprintf(filepointer, "\\");
            else if(to_insert[i+1] == 'n')
                fprintf(filepointer, "\n");
            else if(to_insert[i+1] == 34)
                fprintf(filepointer, "\"");
            else
                fputc(to_insert[i], filepointer);
        }

        else
            fputc(to_insert[i], filepointer);
    }

    fclose(filepointer);
}

void removestr(){

    char path[300];

    char c;
    for (int i = 0; i<1000; i++){
        c = getchar();
        if (c != '\n')
            path[i] = c;
        else
            break;
    }

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

    FILE * filepointer = fopen(path, "a+");

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

        for (int i = 0; i<size; i++)
            fprintf(filepointer, "\b \b"); //there's a problem here

        fclose(filepointer);
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

        for (int i = 0; i<size; i++)
            fprintf(filepointer, "%s", "\b \b"); //there's a problem here
            fclose(filepointer);
    }

    printf("success");

}

void copystr(){

    char path[300];

    char c;
    for (int i = 0; i<1000; i++){
        c = getchar();
        if (c != '\n')
            path[i] = c;
        else
            break;
    }

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

    int line = 1;
    while(line != line_num){
        char c = fgetc(filepointer);
        if (c == '\n')
            line++;
        }

    if (strcmp(dashdir, "-f") == 0){

        for(int i = 0; i<start_pos; i++){
            char pos_count = fgetc(filepointer);
        }

        for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);
    }

    else{

        for(int i = start_pos-size; i<start_pos; i++){
            char pos_count = fgetc(filepointer);
        }

        for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);
    }

    printf("copied\n");

    fclose(filepointer);

}

void pastestr(){

    char path[300];
    char c;

    for (int i = 0; i<1000; i++){
        c = getchar();
        if (c != '\n')
            path[i] = c;
        else
            break;
    }

    char dashpos[10];
    scanf("%s", dashpos);

    int line_num, start_pos;
    scanf("%d:%d", &line_num, &start_pos);

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

    FILE * filepointer = fopen(path, "a+");

    rewind(filepointer);

    int line = 1;

    while(line != line_num){
        char x = fgetc(filepointer);
        if (x == '\n')
            line++;
    }

    for(int i = 0; i<start_pos; i++){
        char pos_count = fgetc(filepointer);
    }

    fprintf(filepointer, string);

    fclose(filepointer);

}

void cutstr(){

    char path[300];

    char c;
    for (int i = 0; i<1000; i++){
        c = getchar();
        if (c != '\n')
            path[i] = c;
        else
            break;
    }

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

        for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);

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

        for (int i = 0; i<size; i++)
            string[i] = fgetc(filepointer);

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

void find(){

    char dashstr[10];
    scanf("%s", dashstr);

    char given_str[300];

    char c;
    for (int i = 0; i<1000; i++){
        c = getchar();
        if (c != '\n')
            given_str[i] = c;
        else
            break;
    }

    if (given_str[0] == 34){
        for (int i = 0; i<300; i++){
            if (given_str[i+1] != 34)
                given_str[i] = given_str[i+1];
            else{
                given_str[i] = NULL;
                given_str[i+1] = NULL;
                break;
            }
        }
    }

    printf("%s", given_str);

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

    char * path1 = (char *) malloc (1000 * sizeof(char));
    char * path2 = (char *) malloc (1000 * sizeof(char));

    scanf("%s", path1);
    scanf("%s", path2);

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

    char character1, character2;
    character1 = fgetc(filepointer);
    character2 = fgetc(filepointer);

    int indent_round = 0;

    while(character2 != EOF){

        if(character2 == '{' && character1 == 32){
            printf("%c%c\n", character1, character2);
            indent_round++;
            for (int i = 0; i<indent_round; i++)
                printf("\t");
        }

        else if(character2 == '{' && character1 != 32){
            printf("%c %c\n", character1, character2);
            indent_round++;
            for (int i = 0; i<indent_round; i++)
                printf("\t");
        }

        else if(character2 == '}' && character1 == 32){
            printf("\b \b");
            printf("\n%c\n", character2);
            indent_round--;
        }

        else if(character2 == '}' && character1 != 32){
            printf("%c\n", character1);
            indent_round--;

            for (int i = 0; i<indent_round; i++)
                printf("\t");
            printf("%c\n", character2);
        }

        else if(character1 == 32 && character2 == 32){}

        else if(character1 != '{'){
            printf("%c", character1);
        }

        character1 = character2;
        character2 = fgetc(filepointer);
    }

    fclose(filepointer);

}

int main(){

    char operation[30];
    char dashfile[10];
    char space;

    while(1){

        scanf("%s", operation);

        if (strcmp(operation, "createfile") == 0){
            scanf("%s%c", dashfile, &space);
            createfile(); //allright
        }

        else if (strcmp(operation, "cat") == 0){
            scanf("%s%c", dashfile, &space);
            cat(); //allright
        }

        else if (strcmp(operation, "insertstr") == 0){
            insert();
        }

        else if (strcmp(operation, "removestr") == 0){
            scanf("%s%c", dashfile, &space);
            removestr();
        }

        else if (strcmp(operation, "copystr") == 0){
            scanf("%s%c", dashfile, &space);
            copystr(); //allright
        }

        else if (strcmp(operation, "pastestr") == 0){
            scanf("%s%c", dashfile, &space);
            pastestr(); //not printing
        }

        else if (strcmp(operation, "cutstr") == 0){
            scanf("%s%c", dashfile, &space);
            cutstr();
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
            find();
        }

        else if(strcmp (operation, "grep") == 0){
            grep(); //complete
        }

        else if(strcmp (operation, "compare") == 0){
            compare(); //allright
        }

        else if(strcmp (operation, "auto-indent") == 0){
            scanf("%c", &space);
            indent(); //has bugs
        }
    }

}
