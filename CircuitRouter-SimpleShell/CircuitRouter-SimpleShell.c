/* =============================================================================
 *
 * CircuitRouter-SimpleShell.c
 *
 * =============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "lib/commandlinereader.h"

#define MAX_ARGS 3 /* comando + inputfile + NULL */
#define BUFFERSIZE 100


/*VARIAVEIS GLOBAIS*/

int nr_children=0;
int nr_process=0;


/* =============================================================================
 * WaitFunction
 * =============================================================================
 */

int ** WaitFunction(int** info){ 
    
    int status, child_pid;
    
    /*alocacao de memoria para reserva de informacao de processos*/
    
    info = (int**)realloc(info, (nr_process+1) * sizeof(int *));   
    
    info[nr_process]=(int *)malloc(2 * sizeof(int));
    
    /*espera a terminacao de um processo filho*/
            
    child_pid = wait(&status);
                
    if (child_pid < 0 && errno != EINTR){
                    
        perror("Error: Failed at terminating child.");
            exit(1);
    }
    
    /*verificacao do estado de terminacao do filho*/
                
    if (WIFEXITED(status)){
        
        info[nr_process][0]=child_pid;
        info[nr_process][1]=1;
    }
        

    else{
        
        info[nr_process][0]=child_pid;
        info[nr_process][1]=0;
    }
    
    nr_children--; 
    nr_process++;
           

    return info;
}
            


/* =============================================================================
 * main
 * =============================================================================
 */



int main(int argc, char** argv){
	
    
    int MAXCHILDREN, i;
    int **child_info = NULL;
    
    char *args[MAX_ARGS], buffer[BUFFERSIZE];
    int args_read;            
    
    
    /*verificacao da atribuicao de MAXCHILDREN*/
    
    if (argc>1){
    	if (atoi(argv[1])==0){
    		printf("Error: MAXCHILDREN cannot be 0.");
    		exit(1);
    	}
    	else
    	MAXCHILDREN=atoi(argv[1]);
    }

    else
    	MAXCHILDREN=-1;
    
    puts("\n / CircuitRouter-SimpleShell / \n");
    puts("Insert commands:\n");
    
    
    /*leitura de argumentos stdin*/
    
    while (1) {

        args_read = readLineArguments(args, MAX_ARGS, buffer, BUFFERSIZE);
        
        /*comando EXIT recebido*/

        if (args_read < 0 || (args_read > 0 && strcmp(args[0],"exit")==0)) {
            
            /*espera terminacao de todos os processos que ainda correm*/
            
            while (nr_children > 0)
                child_info=WaitFunction(child_info);
            
            
            /*escrita do output no stdout e libertacao de memoria alocada */
                
            if (child_info != NULL){
                
                for (i=0; i<nr_process; i++)
                    printf("CHILD EXITED (PID=%d; RETURN %s)\n", child_info[i][0],
                           child_info[i][1] ? "OK" : "NOK");
                
                printf("END.\n");
                
                for (int j = 0; j < nr_process; j++)
                    free(child_info[j]);
                
                free(child_info);
            }
            
            else{
                perror("Error: No information for process.");
            }
            
            
            exit(0);
    
        }
        
        else if (args_read==2 && strcmp(args[0],"run")==0){
            
            int pid;
            
            /*espera a terminacao de um processo se MAXCHILDREN for atingido*/
            
            if (nr_children >= MAXCHILDREN && MAXCHILDREN!=-1)
                child_info=WaitFunction(child_info);
    
            
            if ((pid=fork()) == 0){
                
                /*execucao do processo filho*/
                
                if (execl("../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver",
                    "CircuitRouter-SeqSolver", args[1],(char *)0) < 0){
                    perror("\nError: Child process created . Execution failed.");
                    exit(1);
                }
               
            }
            
            else if (pid > 0){
                
                /*codigo processo pai*/
                
                nr_children++;
                continue;
            }
            
            else{
                perror("\nError: Fork() failed. Could not create new process.\n");
                exit(1);
            } 
        }
        
        else{
            puts("\nErro: Command not found, Try:\n-run <inputfile>\n-exit\n");
        }
    }   
}
/* =============================================================================
 *
 * End of CircuitRouter-SimpleShel.c
 *
 * =============================================================================
 */
