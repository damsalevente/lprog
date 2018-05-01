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
