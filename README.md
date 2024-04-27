# Enigma
Tento projekt vznikl v rámci předmětu BPC-VBA. Naším cílem bylo vytvořit program v jazyce C simulující šifrovací stroj Enigma. 
## Struktura kódu
Kód je členěn do několika logických sektorů. Nejprve jsou inicializovány jednotlivé komponenty - plugboard, rotory a reflektor. Plugboard umožňuje manuální změnu mapování jednotlivých písmen. Rotory jsou 3 a pro každý z nich je definována substituce pro průchod šifrovaného znaku v obou směrech. Reflektor přijímá výstup z posledního rotoru a připraví jej pro zpětný průchod rotory. Hodnoty jednotlivých proměnných jsou uloženy v příslušných strukturách, které jsou definovány v souboru [enigma.h](https://ntb-2919-01s.utko.feec.vutbr.cz/bpc-vba/247568/enigma/-/blob/main/inc/enigma.h?ref_type=heads). Následují funkce pro samotné šifrování. Jejich princip je obdobný, jedním z parametrů je vždy šifrované písmeno, které je patřičně substituováno. Jednotlivé kroky šifrování propojuje funkce *enigmaEncChar*. Znak je nejprve odeslán do plugboardu, následuje průchod rotory, reflektorem a opět se vrací přes rotory do plugboardu. Vstup, který má být šifrován je od uživatele přijímán v podobě textového řetězce, proto byla implementována funkce převádějící tento řetězec na jednotlivá písmena.

Pro jednotlivé funkce byly vytvořeny odpovídající jednotkové testy ověřující správnou funkčnost kódu.

## Funkcionality
Uživatel má možnost šifrovat, dešifrovat daný vstup. Další možností je změna počátečního nastavení Enigmy, které je následující:
1. Výběr rotorů – uživatel zvolí 3 rotory z nabídky 5 dostupných
2. Nastavení počáteční pozice rotoru
3. Nastavení počáteční pozice prstence
Uživateli je následně zobrazeno nastavení Enigmy a v případě, že došlo k chybovému stavu, je o něm informován. 


- [Enigma settings format example](https://www.101computing.net/enigma-daily-settings-generator/)
- [Online enigma example](https://cryptii.com/pipes/enigma-machine)
- [Online enigma exmaple with available internal structure](https://people.physik.hu-berlin.de/~palloks/js/enigma/enigma-u_v262_en.html)
