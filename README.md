# Enigma
Tento projekt vznikl v rámci předmětu BPC-VBA. Naším cílem bylo vytvořit program v jazyce C simulující šifrovací stroj Enigma. 
## Struktura kódu
Kód je členěn do několika logických sektorů. Nejprve jsou inicializovány jednotlivé komponenty - plugboard, rotory a reflektor. Plugboard umožňuje manuální změnu mapování jednotlivých písmen. Rotory jsou 3 a pro každý z nich je definována substituce pro průchod šifrovaného znaku v obou směrech. Reflektor přijímá výstup z posledního rotoru a připraví jej pro zpětný průchod rotory. Hodnoty jednotlivých proměnných jsou uloženy v příslušných strukturách, které jsou definovány v souboru [enigma.h](https://ntb-2919-01s.utko.feec.vutbr.cz/bpc-vba/247568/enigma/-/blob/main/inc/enigma.h?ref_type=heads). Následují funkce pro samotné šifrování. Jejich princip je obdobný, jedním z parametrů je vždy šifrované písmeno, které je patřičně substituováno. Jednotlivé kroky šifrování propojuje funkce *enigmaEncChar*. Znak je nejprve odeslán do plugboardu, následuje průchod rotory, reflektorem a opět se vrací přes rotory do plugboardu. Vstup, který má být šifrován či dešifrován je od uživatele přijímán v podobě textového řetězce, který je postupně procházen znak po znaku. Při každé iteraci je pak volána výše uvedená funkce *enigmaEncChar*.

Pro jednotlivé funkce byly vytvořeny odpovídající jednotkové testy ověřující správnou funkčnost kódu.

## Funkcionality
Po spuštění aplikace je uživateli zobrazeno defaultní nastavení Enigmy a prostřednictvím terminálu je vyzván k výběru jedné z nabízených funkcionalit programu, kterými je šifrování, dešifrování a změna nastavení Enigmy. Svou volbu provede zadáním příslušného vstupu z klávesnice. 

**Šifrování a dešifrování**

V případě šifrování zadejte do terminálu vstup obsahující pouze abecední znaky bez diakritiky. Při zadávání textu nezáleží na použití velkých a malých písmen. Jakékoliv jiné znaky, včetně interpunkčních znamének a mezer, způsobí výpis chybové hlášky. 

**Změna nastavení Enigmy**

Uživatel má možnost změnit počáteční nastavení plugboardu, vybrat 3 z nabídky 5 dostupných rotorů, startovní pozici rotoru a pozici vnitřního kroužku rotoru. 
Proces je následujicí:

1. Pro změnu zapojení plugboardu zadejte 6 dvojic pouze abecedních znaků oddělené mezerami. Pořadí znaků ve dvojici určuje, odkud kam bude písmeno mapováno.
2. Výběr rotorů proveďte zadáním římské číslice z rozsahu I až V
3. Nastavení počáteční pozice všech pro vybrané rotory prostřednictvím abecedních znaků
4. Nastavení počáteční pozice vnitřního kroužku jednotlivých rotorů opět prostřednictvím abecedních znaků

V případě, že nastavení jedné z komponent chcete přeskočit, stiskněte klávesu *Enter*. V tomto případě bude použito defaultní nastavení Enigmy. Jestliže bude zadán chybný vstup, budete informováni prostřednictvím výpisu v terminálu.


## Odkazy
- [Enigma settings format example](https://www.101computing.net/enigma-daily-settings-generator/)
- [Online enigma example](https://cryptii.com/pipes/enigma-machine)
- [Online enigma exmaple with available internal structure](https://people.physik.hu-berlin.de/~palloks/js/enigma/enigma-u_v262_en.html)
