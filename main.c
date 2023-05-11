
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dup.h"
#include "types.h"
#include <time.h>
#include <sys/mman.h>


void change_directory(char *text){

    matCommands cm = break_commands(" ", text);

    if(cm.lines == 2){
        cm.commands[1] = aloca_memoria(NULL,sizeof(char)* strlen(getenv("HOME")));
        strcpy(cm.commands[1], getenv("HOME"));

    }

    cm.commands[1] = verify_til(cm.commands[1]);

    if(chdir(cm.commands[1]) == -1){

        printf("Diretorio inexistente: %s\n", cm.commands[1]);

    }
    destructor_mat(&cm);


}

int oneCommandBaseCase (char *commands) {

    pid_t filho = fork();

    if(filho == 0){
        commands = add_str(commands);
        char *commandCopia = aloca_memoria(NULL, sizeof(char)* strlen(commands));
        strcpy(commandCopia, commands);
        matCommands baseCaseMinor = verify_file_in_out(commands, "<", STDIN_FILENO);
        matCommands baseCaseMax = verify_file_in_out(commandCopia, ">>", STDOUT_FILENO);

        if(baseCaseMax.lines == 2){
            baseCaseMax = verify_file_in_out(commandCopia, ">", STDOUT_FILENO);
        }

        if(!strchr(baseCaseMax.commands[0], '>') && !strchr(baseCaseMax.commands[0], '<')){

            baseCaseMax = break_commands(" ", baseCaseMax.commands[0]);
            baseCaseMax.commands[0] = swap_string(baseCaseMax.commands);
            execvp(baseCaseMax.commands[0], baseCaseMax.commands);
        } else {
            baseCaseMinor = break_commands(" ", baseCaseMinor.commands[0]);
            baseCaseMinor.commands[0] = swap_string(baseCaseMinor.commands);

            execvp(baseCaseMinor.commands[0], baseCaseMinor.commands);
        }
        fprintf(stderr, "Erro! Falha ao executar o comando!\n");
        exit(1);
    }
    int status;
    waitpid(filho, &status, 0);

    return status;
}


int main() {
    matCommands commands;
    char command[512] = "seq 1 100 | grep 5 > teste.txt ";
    struct tm horario;
    time_t segundos;
    char string_formated_hour[20];
    char *nomeUsuario;

    char diretorioAt[100];


    while(strcmp(command, "exit")) {


        nomeUsuario = pegaUsuario();
        getcwd(diretorioAt, sizeof(diretorioAt));
        time(&segundos);
        horario = *localtime(&segundos);
        strftime(string_formated_hour, sizeof(string_formated_hour), "%D-%H:%M:%S",&horario);

        printf("%s-%s:%s$ ", nomeUsuario, string_formated_hour, diretorioAt);
        nomeUsuario = clear_memory(nomeUsuario);
        fgets(command, 512, stdin);
        *strchr(command, '\n') = '\0';
        if (*command == '\0'){
            continue;
        }else if(strstr(command, "cd")){
            change_directory(command);
            continue;
        } else if (!strcmp(command, "exit")){
            continue;
        }



        commands = break_commands("|", command);


        if (commands.lines > 3) {
            run_pipe(&commands);
        } else if (commands.lines == 3){
            twoCommandsBaseCase(commands.commands[0], commands.commands[1]);
        } else {
            oneCommandBaseCase(commands.commands[0]);
        }

        destructor_mat(&commands);
    }


}