        #include "player.h"
        #include <stdlib.h>
        #include <unistd.h>
        #include <stdio.h>
        #include <signal.h>
        #include <time.h>
        #include <pthread.h>
        #include <string.h>
        #define MAX_PLAYERS 5


        //Global variables for all threads
        Player players[MAX_PLAYERS];
        int playernum=0;
        Game game;
        static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

        void *updateThread(void *arg)
        {
            sleep(10);  //sleep 10 seconds
            pthread_mutex_trylock(&mtx);
            for(int i = 0 ;i < playernum;i++)
            {
                updateForPlayer(&game,&players[i]);
                updateGame(&game);
                printf("Game updated");
            }
            pthread_mutex_unlock(&mtx);
            return 0;
        }
        int get_command(char massage [])
        {
            if(strncmp(massage,"get",3) == 0){
                printf("the message is get\n");
                return 1;
            }
            else if(strncmp(massage,"sell",4) == 0){
                            printf("the message is sell\n");

                return 2;
            }
            else if(strncmp(massage,"add",3) == 0){
                            printf("the message is add\n");

                return 3;
            }
            else if(strncmp(massage,"trade",5) == 0){
                            printf("the message is trade\n");

                return 4;
            }
             else if(strncmp(massage,"stop",4) == 0){
                            printf("the message is stop\n");

                return 5;
            }
             else if(strncmp(massage,"start",5) == 0){
                            printf("the message is start\n");

                return 6;
            }
            else return 0;

        }
        int main()
        {
            pthread_t thr;


            elem e;
            Game g;
            initStateMachine();
            createGame(&g);
            Player p;

            if(pthread_create(&thr,NULL,updateThread,NULL))
            {
                printf("There was a problem during creating the thread\n");
                exit(1);
            }

            printf("Conversion rate before:%lf\n",g.conversionRate);
            printf("P money before init : %lf\n",p.money);
            addPlayer(&p,"Yolo janos");
            printf("%lf\n",p.powerConsumption);
            addMachine(&p,Weak);
            printf("ONe don\n");
            addMachine(&p,Weak);
            addMachine(&p,Heavy);
            players[0] = p;
            playernum++;
            for(int i=0;i<p.numMachines;i++)
            {
                printf("%lf\n",p.machines[i].power);
            }
            eventName eName = Start;
            while(1)
            {
                char  buffer[26];
                struct tm* tm_info;
                fgets(buffer,26,stdin);
                int cmd =get_command(buffer);
                if (cmd == 1){
                    pthread_mutex_lock(&mtx);
                    
                    for(int i=0;i<playernum;i++)
                    {
                        printf("Player number %d", i);
                        getInfo(&players[i],&g);
                    }
                    pthread_mutex_unlock(&mtx);
                }
                if(cmd == 2){
                    sellMachine(&players[0]);
                }
                if(cmd == 3){
                    getInfo(&players[0],&g);
                }
                if(cmd == 5){
                    handleMessage(&players[0],0,Start);
                }
                if(cmd == 6){
                    handleMessage(&players[0],0,Stop);
                }
        
            }

            
           
            return 0;
        }