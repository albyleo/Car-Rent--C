Si consideri un’autostrada descritta come una sequenza di stazioni di servizio. Ogni stazione di servizio si trova
ad una distanza dall’inizio dell’autostrada espressa in chilometri da un numero intero positivo o nullo. Non esistono
due stazioni di servizio aventi la stessa distanza: ogni stazione di servizio è quindi univocamente identificata dalla
sua distanza dall’inizio dell’autostrada.
Ogni stazione di servizio è dotata di un parco veicoli elettrici a noleggio. Ogni veicolo è contraddistinto
dall’autonomia data da una carica delle batterie, espressa in chilometri, da un numero intero positivo. Il parco
veicoli di una singola stazione comprende al più 512 veicoli. Presa a noleggio un’auto da una stazione s, è possibile
raggiungere tutte quelle la cui distanza da s è minore o uguale all’autonomia dell’automobile.
Un viaggio è identificato da una sequenza di stazioni di servizio in cui il conducente fa tappa. Ha quindi inizio
in una stazione di servizio e termina in un’altra, passando per zero o più stazioni intermedie. Si assuma che il
conducente non possa tornare indietro durante il viaggio, e noleggi un’auto nuova ogniqualvolta faccia tappa in
una stazione di servizio. Quindi, date due tappe consecutive s e t, t deve essere sempre più lontana dalla partenza
rispetto a s, e t deve essere raggiungibile usando uno dei veicoli disponibili in s.
L’obiettivo del progetto è il seguente: data una coppia di stazioni, pianificare il percorso con il minor numero
di tappe tra di esse. Nel caso siano presenti più percorsi con lo stesso numero minimo di tappe (cioè a pari merito),
deve essere scelto il percorso che predilige le tappe con distanza più breve dall’inizio dell’autostrada. In altre parole,
si consideri l’insieme degli n percorsi a pari merito P = {p1, p2, . . . pn} dove ogni percorso è una tupla di m elementi
pi = ⟨pi,1, pi,2, . . . pi,m⟩ che equivalgono alla distanza da inizio autostrada di ogni tappa in ordine di percorrenza.
Deve essere scelto l’unico percorso pi tale che non esista un altro pj con le stesse k tappe finali precedute da una
tappa con distanza minore, cioè ∄j, k : ⟨pi,m−k+1, . . . pi,m⟩ = ⟨pj,m−k+1, . . . pj,m⟩ ∧ pj,m−k < pi,m−k.
Di seguito un esempio di autostrada. In questo esempio, il percorso corretto tra la stazione a distanza 20 e
quella a distanza 50 è 20 → 30 → 50 (e non 20 → 45 → 50). Si noti che, invece, 50 → 30 → 20 è il percorso
corretto tra la stazione a distanza 50 e quella a distanza 20 (quindi in direzione da destra a sinistra).

Autonomia veicoli
Autostrada
Stazioni: 20 30 45 50
Macchine in ogni stazione: (5, 10, 15, 25) (40) (30) (20, 25)


Formato file in ingresso e stampe attese:

Il file testuale in ingresso contiene una sequenza di comandi, uno per riga, con il seguente formato. Tutti i valori
interi positivi o nulli sono codificabili in 32 bit.

• aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n
Aggiunge una stazione, posta alla distanza indicata, dotata di numero-auto, dall’autonomia indicata.
Ad esempio:
aggiungi-stazione 10 3 100 200 300
aggiunge una stazione a distanza 10 dall’inizio dell’autostrada, con un parco di tre veicoli con autonomia
100, 200 e 300 km rispettivamente. Se esiste già una stazione alla distanza indicata, il comando non fa nulla.
Stampa attesa come risposta: aggiunta oppure non aggiunta.

• demolisci-stazione distanza
Rimuove la stazione posta alla distanza indicata, se essa esiste.
Stampa attesa come risposta: demolita oppure non demolita.

• aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere
Se la stazione esiste, aggiunge un’auto alla stessa. È possibile avere più auto con la stessa autonomia.
Stampa attesa come risposta: aggiunta oppure non aggiunta.

• rottama-auto distanza-stazione autonomia-auto-da-rottamare
Rimuove un’automobile dalla stazione indicata, se la stazione esiste ed è dotata di almeno un’automobile
con l’autonomia indicata.
Stampa attesa come risposta: rottamata oppure non rottamata.

• pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo
Richiede di pianificare il percorso con i vincoli sopra indicati.
Stampa attesa come risposta: le tappe in ordine di percorrenza, rappresentate con la distanza delle stazioni
da inizio autostrada, separate da spazi e alla fine seguite da un a-capo. Devono essere incluse partenza e
arrivo; se coincidono la stazione viene stampata una sola volta. Se il percorso non esiste, stampa nessun
percorso. L’azione di pianificazione non altera le stazioni o il loro parco veicoli. Le stazioni date sono
sicuramente presenti.
