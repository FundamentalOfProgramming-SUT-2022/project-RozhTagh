#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

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

    fputs(string, filepointer);

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

void print_tree(char * path, int round, int max){

    if (round == max)
        return;

    DIR * directory;
    struct dirent *dir_entry;
    struct stat state;

    if (directory = opendir(path) == NULL)
        return;

    chdir(path);

    while ((dir_entry = readdir(directory)) != NULL){

        int status = stat(dir_entry->d_name, &state);

        if(status == 0){
            if(state.st_mode & S_IFDIR){

                if(strcmp(".", dir_entry->d_name) == 0 || strcmp("..", dir_entry->d_name) == 0)
                    continue;

                printf("%s\n", dir_entry->d_name);

                strcat(path, dir_entry->d_name);
                strcat(path, "/");

                print_tree(dir_entry->d_name, round+1, max);
            }

            else{
                printf("%s\n", dir_entry->d_name);
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

    int letter_num = strlen(grep_text);
    printf("%d\n", letter_num);

    char character;
    char sample[] = {NULL};
    int line_number = 0;

    while(character != EOF){
        character = fgetc(filepointer);
        if (character == '\n')
            line_number++;

        if (character == grep_text[0]){
            sample[0] = character;

            for (int i = 1; i<strlen(grep_text); i++)
                sample[i] = fgetc(filepointer);

            printf("%s\n", sample);

            if (strcmp(sample, grep_text) == 0){
                printf("%d\n", line_number);
                rewind(filepointer);

                int line = 0;
                while(line != line_number){

                    char c = fgetc(filepointer);
                    if (c == '\n')
                        line++;
                }

                char x;
                while(x != '\n'){
                    printf("%c", fgetc(filepointer));
                } printf("\n");

            }
        }
    }

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
            printf("%s\n", token);
            print_grep(token, grep_text);
            token = strtok(NULL, " - ");
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

        //printf("%s %d\n%s %d\n", line1, ftell(filepointer1), line2, ftell(filepointer2));

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
            printf("2 longer\n");
        }

        if(character1 != EOF && character2 == EOF){
            printf("1 longer\n");
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

int main(){

    char operation[30];
    char dashfile[10];
    char space;

    while(1){

        scanf("%s", operation);

        if (strcmp(operation, "createfile") == 0){
            scanf("%s%c", dashfile, &space);
            createfile();
        }

        else if (strcmp(operation, "cat") == 0){
            scanf("%s%c", dashfile, &space);
            cat();
        }

        else if (strcmp(operation, "removestr") == 0){
            scanf("%s%c", dashfile, &space);
            removestr();
        }

        else if (strcmp(operation, "copystr") == 0){
            scanf("%s%c", dashfile, &space);
            copystr();
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
            int depth;
            scanf("%d", &depth);
            print_tree(".", 0, depth);
        }

        else if(strcmp (operation, "find") == 0){
            find();
        }

        else if(strcmp (operation, "grep") == 0){
            grep();
        }

        else if(strcmp (operation, "compare") == 0){
            compare();
        }
    }

}
