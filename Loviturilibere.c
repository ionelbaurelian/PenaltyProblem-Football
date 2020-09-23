#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define buff_dim 5 // Numarul executantilor loviturilor de la 11 metri
int a=1;	// Variabila utilizata pentru contorizarea jucatorui ce urmeaza a executa

//Semafoarele utilizate
sem_t player_asteapta;
sem_t player_suteaza;
sem_t goalkeeper_goal_nogoal;

//Taskurile utilizate
void* Task_Goalkeeper();
void* Task_Player();

void* (*Taskuri[])() = {Task_Player,Task_Goalkeeper };


//Tasku-ul ce acopera executarea loviturilor de catre jucatori
void* Task_Player() {

	int i;
	for (i = 0; i < buff_dim; i++)
	{
		sem_wait(&goalkeeper_goal_nogoal);
		printf("Jucatorul %d merge la punctul cu var pentru executarea loviturii de la 11 metrii.\n", a);
		a = a + 1;
		sem_post(&player_asteapta);
		sem_wait(&player_suteaza);
		int x;
		x = rand() % 2;
		if (x == 0)
			printf("Goalkeeperul intervine\n\n");
		else if (x == 1)
			printf("Goalkeeperul incaseaza un gol.\n\n");
		sem_post(&goalkeeper_goal_nogoal);
	}
}

//Task-ul ce acopera interventia sau incasarea unui gol a goalkeeperului in fata jucatorilor
void* Task_Goalkeeper() {

	int i;
	for (i = 0; i < buff_dim; i++)
	{
		sem_wait(&player_asteapta);
		printf("Jucatorul executa lovitura de la 11 metri.\n");
		sem_post(&player_suteaza);
	}
}

int main(void){
	pthread_t Task[2];
	int i;
	
	// Initializarea semafoarelor
	if(!sem_init(&player_asteapta,0,0)){}
		else printf("Eroare la initializarea semaforului player_asteapta \n");
	if(!sem_init(&player_suteaza,0,0)){}
		else printf("Eroare la initializarea semaforului player_suteaza \n");
	if(!sem_init(&goalkeeper_goal_nogoal,0,1)){}
		else printf("Eroare la initializarea semaforului goalkeeper_goal_nogoal \n");
	
	// Crearea firelor de executie
	for(i = 0; i<2; i++)
		if( pthread_create(Task+i, NULL, (void*)(*(Taskuri+i)), NULL) ){
			
			perror("pthread_create");
			return EXIT_FAILURE;

		}

	for(i=0; i<2; i++)
		if(pthread_join(*(Task+i), NULL) != 0){
			
			perror("pthread_join");
			return EXIT_FAILURE;

		}

	printf("\nMain: Task-urile s-au incheiet ... iesire din executie ... \n");

	sem_destroy(&player_asteapta);
	sem_destroy(&player_suteaza);
	sem_destroy(&goalkeeper_goal_nogoal);
	
	exit(0);
}

	
