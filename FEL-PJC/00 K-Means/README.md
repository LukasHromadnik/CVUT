# K-Means algoritmus

## Zadání

K-means je jednoduchý iterativní algoritmus pro shlukování množiny bodů do `k` skupin (kde `k` je známé předem) s jednoduchou paralelizací. Vstupem je množina bodů (vstupní soubor) a počet skupin (parametr na příkazové řádce), na výstupu vrací seznam bodů a jejich příslušnost do skupiny. Výsledek je vhodné vizualizovat výstupem do obrázku SVG. Pro snadnou vizualizaci doporučujeme pracovat ve 2D.

## Popis implementace

Aplikace načte data ze vstupního souboru. Vstupní soubor je jednoduchý textový soubor, který na každém řádku obsahuje dvě souřadnice daného bodu. V současné době je aplikace optimalizovaná pro práci ve 2D. Díky tomu lze výsledek lehce vizualizovat.

Druhým povinným argumentem je počet skupin, do kterých se mají body rozdělit.

Kromě těchto dvou povinných arugmentů aplikace podporuje následující další nastavení, která jsou předána jako přepínače na příkazové řádce.

| Přepínač    | Popis |
| ----------- | ----- |
| `--help`    | Zobrazí nápovědu |
| `-pp`       | Využije k inicializaci počátečních středů jednotlivých skupin algoritmus K-Means++ |
| `-i`        | Spustí aplikaci v interaktivním režimu, kdy se zobrazují průběžné výsledky v jednotlivých iteracích |
| `-s <name>` | Uloží výsledný graf do souboru s názvem `<name>` |
| `-m`        | Zapne podporu více vláken při výpočtu |
| `-p`        | Zobrazí konečný výsledek vizuálně |

V závislosti na nastavení aplikace využije při nastavení úvodních středů chytrou inicializaci K-Means++ (přepínač `-pp`) nebo pouze náhodně vybere `k` bodů ze vstupu.

Následně se v jednotlivých iterací aplikace každý bod přiřadí k nejbližšímu středu. Na konci každé iterace se aktualizuje pozice středů tak, aby jejich pozice odpovídala aritmetickému průměru souřadnic bodů jim přiřazeným.

Iterace se provádějí do nekonečna a výpočet se zastaví ve chvíli, kdy se pozice všech středu nezmění.

## Vzorová vstupní data

K aplikaci jsou dostupná i vzorová vstupní data ve složce `input/`. Jednotlivá vstupní data obsahují název ve formátu `#-k`, kde `#` je pořadové číslo a `k` je počet clusterů v daném souboru.

## Výsledky běhu programu

Výsledky byly měřeny na Macbooku Air 2012 s konfigurací

- 2-jádrový procesor i5
- 4 GB RAM
- 128 GB SSD

Výsledek byl počítán pro testovací vstup `05-50` v Release módu.

| Počet vláken | Naměřený čas |
| ------------ | ------------ |
| 1            | 72 ms        |
| 4            | 44 ms        |

Z důvodu náhodného využití náhodného generátoru byl výsledný čas získán zprůměrováním 100 pokusů na daném vstupu.
