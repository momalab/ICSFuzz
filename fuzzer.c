#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define LEN 128
#define MAX_LINE_LENGTH 80

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

int get_tid(int pid) {

	char ch;
	char *line = NULL;
	char *token = NULL;
	char *src = malloc(LEN*sizeof(char));
	char *pid_str = malloc((pid/10+1)*sizeof(char));
	const char delim[2] = " ";
	int iter = 0;
	int retval = 0;
	size_t len =0;
	ssize_t read;
	
	
	
	strcpy(src, "ps -AT | grep ");	
	sprintf(pid_str, "%d", pid);
	strcat(src, pid_str);
	
	FILE *cmd = popen(src, "r");

  	
  	if(cmd==NULL){
		printf("Unable to open process");
		return(1);
	}
	
	while((read = getline(&line, &len, cmd)) != -1){
		iter = 0;
		while((line[iter] != '_')&&(iter<read))
			iter++;
		
		if((line[iter-1]=='C') && (line[iter+1]=='T')){
			token = strtok(line, delim);
			token = strtok(NULL, delim);
			retval = atoi(token);
			break;
		}
		else
			continue;
	}
	
		
	
	free(pid_str);
	free(src);
	pclose(cmd);
	return(retval);
}


int addr_calc(char *proc_maps, int tid){
	
	FILE *fd_proc_maps = fopen(proc_maps, "r");
  	if (!fd_proc_maps) {
    		printf("Could not open %s\n", proc_maps);
		perror(proc_maps);
   		return EXIT_FAILURE;
  	}
	
	char lines[15][MAX_LINE_LENGTH] = {0};
	unsigned int line_count = 0;
	int retval;
	int tempid;
	
	while((fgets(lines[line_count], MAX_LINE_LENGTH, fd_proc_maps))&&(line_count<14))
		line_count++;		

	char *token = NULL;	
	char delim[4][2] = {"]",
	 		    "[",
			    ":",
			    "-"};
	int i=1;

	
	token = strtok(lines[6], delim[0]);
	
	while((token != NULL)&&(i<3)){
		token = strtok(token, delim[i]);
		token = strtok(NULL, delim[i]);
		i++;
	}
	
	if(token!=NULL){
		tempid = atoi(token);
		token = NULL;
	}
	else{
		printf("Tokenization process failed\n");
		return 1;
	}

	if(tempid!=tid){
		printf("Thread stack is out of alignment. Please reboot your PLC\n");
		return 1;
		
	}
	else{
		token = strtok(lines[4], delim[3]);
		retval = atoi(token);	
	}
		
	if(fclose(fd_proc_maps)){
		return EXIT_FAILURE;
		perror(proc_maps);
	}
	printf("retval is %d", retval);
	return retval;
}
		
	

int main(int argc, char* argv[]) {

 	if (argc != 3) {
    		printf("proc-2  pid  addr  length\n");
   		exit(1);
  	}

  	int pid = get_pid("codesys3");
  	if(pid==0){
		printf("Could not open process. Please check if the codesys3 runtime is open.\n");
		exit(1);
  	}
  	int taskid = get_tid(pid);   
 	 if(taskid==0){
		printf("PLC Task is not initialized/running. Please restart it through the Codesys or Wago HMI.\n");
		exit(1);
	}
	
  	unsigned long addr = strtoul(argv[1], NULL, 16);
  	int           len  = strtol (argv[2], NULL, 10);

  	char* proc_mem = malloc(50);
  	sprintf(proc_mem, "/proc/%d/mem", pid);
	
  	char* proc_maps = malloc(50);
  	sprintf(proc_maps, "/proc/%d/maps", pid);
  
  	int tempy = addr_calc(proc_maps, taskid);
  	int in_addr = 0x0;
  	if((tempy==EXIT_FAILURE)||(tempy==1))
		exit(1);
  	else
		in_addr = tempy+0x12;

  	printf("PLC input fuzzing address is %d\n", in_addr);
	
  
  	printf("opening %s, address is %lx\n", proc_mem, addr);
  	int fd_proc_mem = open(proc_mem, O_RDWR);
  	if (fd_proc_mem == -1) {
    	printf("Could not open %s\n", proc_mem);
    	exit(1);
	  }

  	int* buf = malloc(len);
  	int *beef = malloc(len);
  	*beef = 0x777;
                    

  	lseek(fd_proc_mem, addr, SEEK_SET);
  	//read (fd_proc_mem, buf , len);

  	//printf("String at 0x%lx in process %d is:\n", addr, pid);
  	//printf("  %s\n", buf);
  
  	//printf("\nNow, this string is modified\n");
  	memcpy(buf, beef, len);
	
	for(int j=0; j<1000000; j++){
  		lseek(fd_proc_mem, addr, SEEK_SET);
  		if (write (fd_proc_mem, buf , len) == -1) {
  	   		printf("Error while writing\n");
  	  		exit(1);
  		}
  	}
  	free(buf);
  	free(proc_mem);
  	free(proc_maps);

  	return 0;

}
