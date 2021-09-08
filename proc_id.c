#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LEN 128

int get_pid(char *pid_name) {

	char ch;
	char *line = malloc(LEN*sizeof(char));
	int iter = 0;
	
	char *src = malloc(LEN*sizeof(char));
	int retval;
	
	strcpy(src, "pidof ");	
	
	if((strlen(src)+strlen(pid_name))>LEN)
		return(0);
	else
		strcat(src, pid_name);

	FILE *cmd = popen(src, "r");
  	
  	if(cmd==NULL){
		printf("Unable to open process");
		return(1);
	}

	while(((ch=fgetc(cmd)) != '\n')&&(iter<LEN))
		line[iter++] = ch;
	
	retval = atoi(line);
	
	pclose(cmd);
	free(line);
	free(src);

	return(retval);
}

int get_tid(char *tid_name) {

	char ch;
	char *line = malloc(LEN*sizeof(char));
	char *src = malloc(LEN*sizeof(char));
	char *token = malloc(LEN*sizeof(char));
	const char delim[2] = " ";
	int iter = 0;
	int retval;
	
	
	
	strcpy(src, "ps -AT | grep ");	
	
	if((strlen(src)+strlen(tid_name))>LEN)
		return(0);
	else
		strcat(src, tid_name);

	FILE *cmd = popen(src, "r");
  	
  	if(cmd==NULL){
		printf("Unable to open process");
		return(1);
	}

	while(((ch=fgetc(cmd)) != '?')&&(iter<LEN))
		line[iter++] = ch;
		
	token = strtok(line, delim);
	token = strtok(NULL, delim);

	retval = atoi(token);

	pclose(cmd);
	free(line);
	free(src);
	//free(token);

	return(retval);
}




int main(){
	
	int plc_pid = get_tid("PLC_Task");
	int codesys_pid = get_pid("codesys3");
	
	printf("%d\n", plc_pid);
	printf("%d\n", codesys_pid);
}
