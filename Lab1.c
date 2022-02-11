#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

int countCharactersInFile(char *filename, char *dir){
	char* slesh = "/";
	char resultPath[1024];

	strcpy(resultPath, dir);
	strcat(resultPath, slesh);
	strcat(resultPath, filename);

	int in;
	int nread;
	char block[1024];
	int count = 0;
	in = open(resultPath, O_RDONLY);

	while((nread = read(in, block, sizeof(block))) > 0)
 		for(int i = 0; i<nread; i++){
 			if(isalpha(block[i]) != 0){
 				count = count + 1;
 			}
 		}

 	close(in);
	printf("%s количество букв: %d\n", filename, count);
	return count;
}



int sumCharactersInDirFiles(char *dir) 
{
DIR *dp;
struct dirent *entry;

if ((dp = opendir(dir)) == NULL) {
 fprintf(stderr, "cannot open directory: %s\n", dir);
 return -1;
 }

 int sumCharaters = 0;
while((entry = readdir(dp)) != NULL) 
 if(strcmp(".",entry->d_name)==0 || strcmp("..",entry->d_name)==0);
 else sumCharaters = sumCharaters + countCharactersInFile(entry->d_name, dir);
closedir(dp);

printf("Всего символов: %d\n", sumCharaters);
return sumCharaters;
}



void writeResultSumCharacters(int result){
	if (result == -1){
		return;	
	}

	int out;
	char bl[5];
	sprintf(bl, "%d", result);
	out = open("output/result.txt", O_WRONLY|O_CREAT);

	write(out, bl, sizeof(bl));
	exit(0);
}

int main() 
{
printf("Directory scan of /home:\n");
writeResultSumCharacters(sumCharactersInDirFiles("/home/daniil/Рабочий стол/daniil6306/lab1/input"));
printf("done.\n");
exit(0);
}