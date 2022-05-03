// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

// Mit Verkettung implementierte Streuwerttabelle
// mit Schlüsseltyp K und Werttyp V.
// An der Stelle, an der HashChain für einen bestimmten Schlüsseltyp K
// verwendet wird, muss eine Funktion uint hashval (K) zur Berechnung
// von Streuwerten sowie ein passender Gleichheitsoperator (==) für den
// Typ K bekannt sein.
template <typename K, typename V>
struct HashChain {
    //Hilfsstruktur für die Datenspeicherung und Verkettung
    struct VK {
        K key;
        V value;
        VK* next;
        VK(K k, V v,VK* n) : key(k), value(v), next(n) {}
    };
    uint size;
    //hier pointer, da der Werttyp V unbekannt ist.
    VK **table;

    // Initialisierung mit Größe n.
    HashChain(uint n) {
        // Größe der Tabelle n.
        size = n;
        // Tabelle mit n (= size) Pointern.
        table = new VK *[size]();
    }

    // Eintrag mit Schlüssel k und Wert v (am Anfang der jeweiligen
    // Liste) hinzufügen (wenn es noch keinen solchen Eintrag gibt)
    // bzw. ersetzen (wenn es bereits einen gibt).
    // Der Resultatwert ist immer true.
    bool put(K k, V v) { ///TODO: TESTING
        //hashwert berechnen
        uint hkey = hashval(k);
        //gibt es schon eine Liste an diesem hashwert?
        if (table[hkey] == NULL) { //nein
            //neue Liste anlegen
            table[hkey] = new VK(k, v, NULL);
        } else {  //ja
            //Pointer auf die Liste
            VK *vk = table[hkey];
            VK *mem = table[hkey];
            //Iteriere über die Liste
            while (vk != NULL) {
                //gibt es schon einen Eintrag mit diesem Schlüssel?
                if (vk->key == k) {
                    //wenn ja, dann Wert ändern
                    vk->value = v;
                    return true;
                }
                vk = vk->next;
            }
            //wenn nein, dann neuen Eintrag anfügen
            table[hkey] = new VK(k, v, mem);
        }
        return true;
    }

    // Wert zum Schlüssel k über den Referenzparameter v zurückliefern,
    // falls vorhanden; der Resultatwert ist in diesem Fall true.
    // Andernfalls bleibt v unverändert, und der Resultatwert ist false.
    bool get(K k, V &v) {
        //hashwert berechnen
        uint hkey = hashval(k);
        //checken ob ein Wert an diesem hashwert existiert
        if (table[hkey] != NULL) {
            //pointer auf erstes tupel Speichern
            VK* vk = table[hkey];
            //solange nicht das Ende der Liste erreicht wurde Iterieren wir durch die Liste
            while (vk != NULL) {
                if (vk->key == k) {
                    //wenn key gefunden, dann value zurückgeben
                    v = vk->value;
                    return true;
                }
                vk = vk->next;
            }

        }
        //wenn nicht, dann false zurückgeben
        return false;
    }

    // Eintrag mit Schlüssel k entfernen, falls vorhanden;
    // der Resultatwert ist in diesem Fall true.
    // Andernfalls wirkungslos, und der Resultatwert ist false.
    bool remove(K k) {
        //hashwert berechnen
        uint hkey = hashval(k);
        //checken ob ein Wert an diesem hashwert existiert
        if (table[hkey] != NULL) {
            //pointer auf erstes tupel Speichern
            VK* vk = table[hkey];
            //solange nicht das Ende der Liste erreicht wurde Iterieren wir durch die Liste
            while (vk != NULL) {
                //wenn key gefunden, dann tupel löschen
                if (vk->key == k) {
                    //wenn erster Eintrag, dann nur den Pointer auf den nächsten Eintrag übernehmen
                    if (vk == table[hkey]) {
                        table[hkey] = vk->next;
                    } else {
                        //sonst den Pointer auf den nächsten Eintrag übernehmen
                        VK* vk2 = table[hkey];
                        while (vk2->next != vk) {
                            vk2 = vk2->next;
                        }
                        vk2->next = vk->next;
                    }
                    //löschen
                    delete vk;
                    return true;
                }
                vk = vk->next;
            }
        }
        //wenn nicht, dann false zurückgeben
        return false;
    }

    // Inhalt der Tabelle zu Testzwecken ausgeben:
    // Pro Eintrag eine Zeile bestehend aus der Nummer des Platzes,
    // Schlüssel und Wert, jeweils getrennt durch genau ein Leerzeichen.
    // Dieses Ausgabeformat muss exakt eingehalten werden.
    // Leere Plätze werden nicht ausgegeben.
    // Bei Verwendung von dump muss es passende Ausgabeoperatoren (<<)
    // für die Typen K und V geben.
    void dump() {
        //iterieren über die Tabelle
        for (int i = 0; i < size; i++) {
            //checken, ob ein Wert an diesem hashwert existiert
            if (table[i] != NULL) {
                //wenn ja, dann den Wert ausgeben
                VK *vk = table[i];
                while (vk != NULL) {
                    cout << i << " " << vk->key << " " << vk->value << endl;
                    vk = vk->next;
                }
            }
        }
    };
};
//

// Sondierungssequenz mit Schlüsseltyp K für lineare Sondierung.
// An der Stelle, an der LinProb für einen bestimmten Schlüsseltyp K
// verwendet wird, muss wiederum uint hashval (K) bekannt sein.
template <typename K>
struct LinProb {
    // Initialisierung der Sequenz mit Schlüssel k und Tabellengrößen.
    uint prev;
    uint size;
    K key;
    bool first;
    LinProb (K k, uint n){
        key = k;
        size = n;
        first = true;
    };
    // Den ersten bzw. nächsten Wert der Sequenz liefern.
    // Nach einem Aufruf des Konstruktors darf diese Funktion also
    // bis zu n-mal aufgerufen werden.
    // Achtung: Die direkte Verwendung der Formel
    // s[j](k) = (h(k) + j) mod n
    // kann durch arithmetischen Überlauf zu falschen Ergebnissen
    // führen, wenn h(k) sehr groÃŸ ist.
    // Deshalb sollte nur der erste Wert s[0](k) = h(k) mod n direkt
    // und alle weiteren Werte jeweils aus dem vorigen Wert berechnet
    // werden: s[j](k) = (s[j-1](k) + 1) mod n für j = 1, ..., n-1.
    // Der vorige Wert kann hierfür in einer Elementvariablen
    // gespeichert werden.
    // Dann kann bei realistischen Tabellengrößen n kein Überlauf
    // auftreten.
    uint next (){
        if (first) {
            prev = (hashval(key)%size);
            first = false;
        }
        else {
            prev = (prev + 1)%size;
        }
        return prev;
    };
};

// Sondierungssequenz mit Schlüsseltyp K für quadratische Sondierung,
// analog zu LinProb.
template <typename K>
struct QuadProb {
    uint prev;
    uint size;
    K key;
    int i;
    bool first;
    QuadProb(K k, uint n){
        key = k;
        size = n;
        first = true;
        i = 0;
    };
    uint next (){
        //quadratic probing
        // Formel: s[j](k) = (h(k) + (j + j^2 / 2)) mod n ... aus dem Skript
        if (first) {
            prev = (hashval(key)%size);
            first = false;
        }
        else {
            prev = (prev + i)%size;
        }
        i++;
        return prev;
    };
};

// Sondierungssequenz mit Schlüsseltyp K für doppelte Streuung.
// An der Stelle, an der DblHash für einen bestimmten Schlüsseltyp K
// verwendet wird, muss wiederum uint hashval (K) sowie eine zweite
// Streuwertfunktion uint hashval2 (K, uint n) bekannt sein, die nur
// Werte von 1 bis n - 1 liefert, die teilerfremd zu n sind.
// Ansonsten analog zu LinProb.
template <typename K>
struct DblHash {
    uint prev;
    uint size;
    K key;
    int i;
    bool first;
    DblHash (K k, uint n){
        key = k;
        first = true;
        size = n;
        i = 0;
    };
    uint next (){
        // double hashing
        // Formel: prev = (hashval(key) + i*(hashval2(key, size))%size;
        if (first) {
            prev = (hashval(key)%size);
            first = false;
        }
        else {
            prev = ((prev + i*(hashval2(key, size)))%size);
        }
        i++;
        return prev;
    };
};
enum class marker{gelöscht,null};
// Mit offener Adressierung implementierte Streuwerttabelle mit
// Schlüsseltyp K, Werttyp V und Sondierungssequenz des Typs S.
// Bedeutung von Konstruktor und Elementfunktionen wie bei HashChain
// mit folgenden Änderungen:
// put ist bei einer vollen Tabelle wirkungslos und liefert false,
// wenn ein neuer Eintrag hinzugefügt werden müsste.
// dump gibt von Plätzen mit Löschmarkierung nur ihre Nummer aus.
// Der für S eingesetzte Typ muss einen Konstruktor mit Parametertypen
// K und uint zur Initialisierung der Sequenz sowie eine parameterlose
// Elementfunktion next zur Abfrage des ersten bzw. nächsten Werts der
// Sequenz besitzen, sodass S z. B. wie folgt verwendet werden kann,
// wenn k den Typ K und n den Typ uint besitzt:
// S s = S(k, n);
// for (int j = 0; j < n; j++) { uint i = s.next(); ...... }
template <typename K, typename V, typename S>
struct HashOpen {


    struct VZ{
        K key;
        V value;
        marker m;
        VZ(K k, V v,marker m) : key(k), value(v),m(marker::null) {}
    };
    VZ **table;
    int size;
    // Initialisierung der Tabelle mit Tabellengröße n.
    HashOpen (uint n){
        table= new VZ *[n]();
        size = n;
    }

    int help(K k, int *Mem){
        uint i=0;
        int iMem= -5;
        bool first = true;
        S s = S(k, size);
        //Wenn Tabelle an der Stelle i leer ist
        for (int j = 0; j < size ; j++)
        {
            i = s.next();
            if(table[i]==NULL){//nicht vorhanden
            //Wenn noch kein Index gemerkt wurde
                if(first){//gemerkten Index zurückgeben
                    iMem = i;
                    first = false;
                    *Mem=0;
                    return iMem;
                }
                //Wenn bereits ein Index gemerkt wurde
                else{ //liefere Index zurück
                    *Mem=1;
                    return i ;
                }
            }
            //Tabelle hat an Stelle i eine Löschmarkierung und es wurde noch kein Index gemerkt
            if(table[i]->m==marker::gelöscht && first){
                first= false;
                iMem= i;

            }
            //Key an der Stelle i entspricht übergebenem Key
            if(table[i]->key==k){
                *Mem=2; //vorhanden und i
                return  i;
            }
        }
        if(iMem!=-5) return i; //nicht vorhanden
        *Mem=3;
        return -1;//Tabelle voll
    };

    // Einfügen eines neuen Schlüssels-Wert-Paaren.
    // Liefert true, wenn ein neuer Eintrag hinzugefügt wurde,
    // false, wenn der Schlüssel bereits vorhanden war.

    bool put (K k, V v) {
        int M;
        int i=help(k,&M);
        if(i!=-1){
            table[i]=new VZ(k,v,marker::null);
            return true;
        }
        else return false;
    }


    // Liefert den Wert des Schlüssels k oder NULL, wenn k nicht
    // in der Tabelle enthalten ist.
   bool get (K k, V& v) {
        int M;
        int i=help(k,&M);
        if(M==2){
            v=table[i]->value;
            return true;
        }
        else{return false;}
    }

    // Löschen des Schlüssels k.
    // Liefert true, wenn der Schlüssel k gelöscht wurde,
    // false, wenn k nicht in der Tabelle enthalten war.
    bool remove (K k){
        int M;
        int i=help(k,&M);
        if(M==2){
            table[i]->m=marker::gelöscht;
            return true;
        }
        else{return false;}
    }

    // Ausgabe der Tabelle.
    void dump (){

        for (int i = 0; i < size; i++) {

            if(table[i]!=NULL){
                if(table[i]->m==marker::gelöscht){
                    cout<<i<<endl;
                }
                else{
                    cout<<i<<" "<<table[i]->key<<" "<<table[i]->value<<endl;
                }
            }

        }

    }
};
