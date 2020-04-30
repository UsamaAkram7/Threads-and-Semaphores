#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>



sem_t s1;
sem_t s2;
sem_t s3;
sem_t s4;
char *temp1;
char temp2[10];
char *temp3;
FILE *fp;
FILE *fp1;

void * proc1(void * var)
{
    sem_wait(&s1);
    fp = fopen("file-1.txt","r");
    if (fp == NULL)
	{
       printf("Could not open file.");
       exit(1);
    }
	
    fgets(temp1,11,(FILE*)fp);
    fclose(fp);
	
    sem_post(&s2);
}

void * proc2(void * var)
{ 
    sem_wait(&s2);		
	fp1 = fopen("file-2.txt","r");
    if (fp1== NULL)
	{
       printf("Could not open file.");
       exit(1);
    }
	
    fgets(temp2,11,(FILE*)fp1);
    
	//since we still have to write to temp1 but we can't override hence:
	int i=10;
	int j=0;
	while(j<11)
	{
		temp1[i]=temp2[j];
		i++;
		j++;
	}
    fclose(fp1);
    sem_post(&s3);
}

void * proc3(void * var)
{ 
    sem_wait(&s3);
    int i=0;
    while (i<21)
    {
        temp3[i]=temp1[i];
        i++;
    }
    
    
    sem_post(&s4);
}

void * proc4(void * var)
{ 
    sem_wait(&s4);
            
    printf("\nReading From Buffer and printing to Screen\n");
    int i=0;
	while(i<21)
	{
		if (i==10)
        {
            printf("%c",temp3[i]);
        }
		else if (i<10)
        {
            printf("%c",temp3[i]);
        }
		else
        {
            printf("%c",temp3[i]);
        }
		i++;
	}
}

int main(int argc,char* argv[])
{
	temp1=(char*)malloc(21*sizeof(char));
	temp3=(char*)malloc(21*sizeof(char));
    pthread_t id1, id2,id3,id4;
	sem_init(&s1, 0, 1);
	sem_init(&s2, 0, 0);
	sem_init(&s3, 0, 0);
    sem_init(&s4, 0, 0);
	pthread_create(&id1, NULL, &proc4, NULL);
	pthread_create(&id2, NULL, &proc3, NULL);
	pthread_create(&id3, NULL, &proc2, NULL);
    pthread_create(&id4, NULL, &proc1, NULL);
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	pthread_join(id3, NULL);
    pthread_join(id4, NULL);
	
	return 0;
}
