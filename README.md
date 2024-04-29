# Enigma
Tento projekt vznikl v rámci předmětu BPC-VBA. Naším cílem bylo vytvořit program v jazyce C simulující šifrovací stroj Enigma.
## Struktura kódu
Kód je členěn do několika logických sektorů. Nejprve jsou inicializovány jednotlivé komponenty - plugboard, rotory a reflektor. Plugboard umožňuje manuální změnu mapování jednotlivých písmen. Rotory jsou 3 a pro každý z nich je definována substituce pro průchod šifrovaného znaku v obou směrech. Reflektor přijímá výstup z posledního rotoru a připraví jej pro zpětný průchod rotory. Hodnoty jednotlivých proměnných jsou uloženy v příslušných strukturách, které jsou definovány v souboru [enigma.h](inc/enigma.h). Následují funkce pro samotné šifrování. Jejich princip je obdobný, jedním z parametrů je vždy šifrované písmeno, které je patřičně substituováno. Jednotlivé kroky šifrování propojuje funkce `enigmaEncChar()`. Znak je nejprve odeslán do plugboardu, následuje průchod rotory, reflektorem a opět se vrací přes rotory do plugboardu.

Pro jednotlivé funkce byly vytvořeny odpovídající jednotkové testy ověřující správnou funkčnost kódu.

## Funkcionality
Po spuštění aplikace je uživateli zobrazeno defaultní nastavení Enigmy a prostřednictvím terminálu je vyzván k výběru jedné z nabízených funkcionalit programu, kterými je šifrování, dešifrování a vytvoření nové Enigmy. Svou volbu provede zadáním příslušného vstupu z klávesnice. 

**Šifrování a dešifrování**

V případě šifrování či dešifrování uživatel zadá do terminálu vstup obsahující pouze abecední znaky bez diakritiky. Při zadávání textu nezáleží na použití velkých a malých písmen. Jakékoliv jiné znaky, včetně interpunkčních znamének a mezer, způsobí výpis chybové hlášky. 

**Vytvoření nové Enigmy**

Uživatel má možnost změnit počáteční nastavení plugboardu, vybrat 3 z nabídky 5 dostupných rotorů, startovní pozici rotoru a pozici vnitřního kroužku rotoru. 
Proces je následujicí:

1. Pro změnu zapojení plugboardu je nezbytné zadat 6 dvojic pouze abecedních znaků oddělených mezerami. Pořadí znaků ve dvojici určuje, odkud kam bude písmeno mapováno.
2. Výběr rotorů je proveden zadáním římské číslice z rozsahu I až V. Jednotlivé výběry budou odděleny mezerami.
3. Změna nastavení počáteční pozice pro vybrané rotory proběhne zadáním příslušných abecedních znaků, které budou odděleny mezerami.
4. Změna nastavení počáteční pozice vnitřního kroužku jednotlivých rotorů proběhne analogicky jako v předchozím případě.

Jestliže bude zadán chybný vstup, bude uživatel informován prostřednictvím výpisu v terminálu.


## Odkazy
- [Enigma settings format example](https://www.101computing.net/enigma-daily-settings-generator/)
- [Online enigma example](https://cryptii.com/pipes/enigma-machine)
- [Online enigma exmaple with available internal structure](https://people.physik.hu-berlin.de/~palloks/js/enigma/enigma-u_v262_en.html)
