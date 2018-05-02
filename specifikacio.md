# Szerver - Kliens program

## Motor X állapotgép vezérlése klienssel
> Van valamilyen folyamat, aminek lehet adni kódszavakat, fel lehet konfigurálni stb
## Timer használata: 
> Állapotgépet egy timer eseménnyel léptetek nextstate-be
```c
if(timerEvent){
    timerEvent=0;
    updateStateMachine(); //aktuális állapot futtatása
}
```

# Mining simulation

## Minden kliens indul x kezdőtőkével
- Vásárolhatnak minereket -> BUY ID
- Elindithatnak egy gépet -> START MACHINEID
- Leállíthatnak egy gépet -> STOP MACHINEID
- Eladhatják a BTC-t      -> SELL AMOUNT
- Lekérdezhetnek a gépekről információkat -> GETINFO MACHINEID
- Lekérdezhetik a Game paramétereit
## A szerver 
### Lesz két komponense
- Game
    - ResourcePrice
    - PowerCost
    - TotalMachines (meghatározza a Resource értékét)
- Players
    - Machines
        - type
        - powerconsumption
        - Power (Resource/minute)
    - Money
    - Resource
### Manipulációk (függvények)

A resourcePrice csökken aszerint, hogy mennyien dolgoznak

A powercost egy szinuszfüggvény lesz, az amplitudóját meghatározza az összes player által használt gép

#### Pénz alakulása játékosoknál:
    Levonjuk mindent gépe után a powercostot

#### Update esemény (minden 10 másodpercben mondjuk)
- Ki kell vonni a powerconsumptiont a playerek pénzéből
- Állítani kell az árakat
- Megnézni, hogy valaki csődben van-e 
- Következő eseményt kiszedni a gép statemachine-jéből

#### Update-től független kliens események kezelése
* Bárhányszor tetszőlegesen lekérheti az eseményeket
* bárhány gépet vehet,eladhat (meg kell nézni a pénzét)
* bárhány gépet leállíthat,elindíthat

## Időtartamok definiálása:

* Világ update: minden 10 sec.
* Machine update: azonnal
* Gépek futási ideje : 2-50 sec.

## Esetleges auto üzemmód:
    Megnézi, hogy hány gépet érdemes használni, és aszerint beállítja a gépek mennyiségét.

# Let's do this
## Start time, finish time
- Start time akkor indul, amikor elindul a gép,
- Finish Time-ot beállítjuk gép szerint
- Megnézzük a finish-time-ját mindegyiknek.

# Felépítés 

## Calculate events
    Minden 10 másodpercben fusson le.
``` python
for every player:
    for every machine:
        get state
        if state is Mining:
            Add power*time to resource (time between now and previous iter)
            Add powerconsumption 
```

Így meg lesz egy játékosnak az totális fogyasztása
és a totális anyagmennyisége

## Ciklus után 
- Áram költség levonása
- Elküldeni mindenkinek az állását ( szerv-kliens)
- Pénzügy ellenőrzés ( hátha valaki kiesett)

## Közben (vagyis amikor input van):
- Esemény beregisztrálása

- [] Pénzügyekre (vásárlás,resource eladás) állapotgép vezérlés

- [x] Machine-re állapotgépes vezérlés

## multiple client with select() function 
- File descriptor

## Semaphore or Spinlock for game and players
 - I will reach players and game from two threads.
 - One thread can get every time they want
 - One thread will run an update on them every 10 seconds
 - I need to lock them so no double update will happen, although it might not cause harm, but im not sure so i protecc anyway :)
 - I have sleep() in my thread, but the critical path is not not that, so i lock after the sleep, i will use spinlock because its easier
- I could use timer interrupts, or sleep too.
- TBH we can wait with basic mutex

# File I/O
## Need to do a configuration file, where we can set parameters for the game
 - Start money 
 - inflation rate
 - stb. stb

 ## MultiThread idea thrown away -> timer !
 