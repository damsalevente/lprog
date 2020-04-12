stackserver: stackserver.c player.c statemachine.c
	gcc stackserver.c player.c statemachine.c -o stackserver -lpthread -lm 

client: client.c 
	gcc client.c -o client -lm -lpthread