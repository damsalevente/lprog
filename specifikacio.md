# Szerver - Kliens program
## Alap funkciók:
* `Valamire lehessen regisztrálni`
* `legyen benne valami számítás`
* `az aktuális résztvevők között legyen valami interakció`

---
## Témák
>Légitársaságot téma van megadva ami nem olyan rossz, sorban állás, hány gép van (erőforrás), utasok (userek (kliensek)), jegy vásárlás, felszállás, repülőgép figyelés, kell csinálni egy blokkdiagramot és ide betenni és akkor jó lesz
---
## Valamilyen hatalmas rendszert akarok csinálni
### Lesz benne:
- fájlkezelés
- socketkezelés
- közös memóriaterület
- konfigurációs fájlok formájában (szimuláció szerű valami
    - num of features
## Ötlet 
### Trade money and conquer resources
- There will be a constant map with every information about it like
- - resource type (wood, stone, gold)
- Global events
- - new law 
- - some resource collapsed
- - values changed
- Everyone whom involved in this get resources and the client knows him money 
## So it works like this 
- Corporation (kliens)
     - Workers
        - HR value cwHRValue
        - numOf Workers -> working power cwPower
        - Technology -> how smart are we cwTechnology
        - Finance -> smart money decisions cwFinanceValue
        - PR -> people dont throw shit at you cwPRValue
    - Money
        - A simple amount of money
        - Inflation and shit
    - Market Share
        - How much common resource do we own (This is what we get from the server)
- Market (Server)
    - Resources
        - Different types
        - Values
        - It makes changes over time
    - Corporations
        - Connect to resources
        - Give answer on how much is there market share
        - Give them answers about a resource
        - Transfer communication between corporations
            - I want to buy from you 
            - How much
            - No
            - Okay...
        - buy/sell basically
    - If someone buys a lot of resources and they become worthless and   they didnt sell them, then they are fucked
> Corporation Value = owned Resources() + Money 

> Resource unlocks new technologies

# VAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGY
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