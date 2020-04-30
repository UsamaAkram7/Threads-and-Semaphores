#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>


sem_t sem1,sem3,sem4,sem2;
char *buff1 ,*buff2;

void *thread1(void * t)
{
    sem_wait(&sem1);
    FILE *fp;
    if ((fp = fopen("file-1.txt","r")) == NULL){
       printf("Error! opening file");
       exit(1);
    }

    fgets(buff1, 11, (FILE*)fp);
    //printf("%s\n", buff1 );
    fclose(fp);
    sem_post(&sem2);
}

void* thread2 (void * t)
{ 
    sem_wait(&sem2);		

    FILE *fp;
    if ((fp = fopen("file-2.txt","r")) == NULL){
       printf("Error! opening file");
       exit(1);
    }
    char buf[10];
    fgets(buf, 11, (FILE*)fp);
    //printf("%s\n", buf );
    for (int i = 10, j = 0; i < 21; i++,j++)
    {
        buff1[i]=buf[j];
    }
    
    

    fclose(fp);
    sem_post(&sem3);
}

void* thread3 (void * t)
{ 
    sem_wait(&sem3);
    int i=0;
    while (i<21)
    {
        buff2[i]=buff1[i];
        i++;
    }
    
    
    sem_post(&sem4);
}

void* thread4 (void * t)
{ 
    sem_wait(&sem4);
            
    printf("\nFirst 10 Characters of First File!\n");
    for (size_t i = 0; i < 21; i++)
    {
        if (i<10)
        {
            printf("%c",buff2[i]);
        }else if (i == 10)
        {
            printf("\n First 10 Characters Of Second File!\n");
            printf("%c",buff2[i]);
        }else
        {
            printf("%c",buff2[i]);
        }
    }
    printf("\n");
    
}

int main(int argc,char* argv[])
{
	buff1 = (char*)malloc(21*sizeof(char));
    buff2 = (char*)malloc(21*sizeof(char));

    pthread_t id1, id2,id3,id4;
	sem_init(&sem1, 0, 1);//a
	sem_init(&sem2, 0, 0);//b
	sem_init(&sem3, 0, 0);//c
    sem_init(&sem4, 0, 0);//d
	pthread_create(&id1, NULL, &thread4, NULL);
	pthread_create(&id2, NULL, &thread3, NULL);
	pthread_create(&id3, NULL, &thread2, NULL);
    pthread_create(&id4, NULL, &thread1, NULL);
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	pthread_join(id3, NULL);
    pthread_join(id4, NULL);
	return 0;
}