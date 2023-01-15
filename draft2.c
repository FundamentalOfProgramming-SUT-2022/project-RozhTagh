#include <stdio.h>
#include <string.h>
#include <unistd.h>

char string[1000];
char path2[1000];

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

    FILE * filepointer = fopen(path, "a");

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
            fprintf("%s", "\b \b");
    }

    fclose(filepointer);

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

    FILE * filepointer = fopen(path, "a");

    rewind(filepointer);

    int line = 1;
    char x;

    while(line != line_num){
        x = fgetc(filepointer);
        if (x == '\n')
            line++;
    }

    for(int i = 0; i<start_pos; i++){
        char pos_count = fgetc(filepointer);
    }

    char sentence[1000];
    strcpy(sentence, string);

    fputs(sentence, filepointer);

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

    FILE * filepointer = fopen(path, "a");

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

int main(){

    char operation[30];
    char dashfile[10];
    char space;

    while(1){

        scanf("%s", operation);
        scanf("%s%c", dashfile, &space);

        if (strcmp(operation, "createfile") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            createfile();

        else if (strcmp(operation, "cat") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            cat();

        else if (strcmp(operation, "removestr") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            removestr();

        else if (strcmp(operation, "copystr") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            copystr();

        else if (strcmp(operation, "pastestr") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            pastestr(); //not printing

        else if (strcmp(operation, "cutstr") == 0 && strcmp(dashfile, "--file") == 0 && space == 32)
            cutstr();
    }

}
