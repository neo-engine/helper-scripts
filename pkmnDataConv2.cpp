%:include "pokemon.h"
%:include "item.h"
%:include <cstdio>
%:include <vector>
%:include <cstring>

using namespace std;

vector<pokemonData> pD;
char buffer[ 200 ];

%:define OMANYTE        138
%:define KABUTO         140
%:define AERODACTYL     142
%:define LIEEP          345
%:define ANORITH        347
%:define CRAINIDOS      408
%:define SHIELDON       410
%:define TIRTUOGA       564
%:define ARCHEN         566

%:define FOSSIL( f, a ) pD[ f ].m_stage = 3; pD[ f ].m_preEvolution = a;

const char* dex[ MAX_PKMN + 20 ] = {
    "Keine Daten.",
    "Nach der Geburt nimmt es für eine Weile Nährstoffe über den Samen auf seinem Rücken auf.",
    "Sobald die Knospe auf seinem Rücken ein süßes Aroma abgibt, steht die Blüte kurz bevor.",
    "Nach einem Regentag riecht die Blume auf seinem Rücken intensiver. Das Aroma zieht andere Pokémon an.",
    "Lodert die Flamme auf seiner Schweifspitze hell, ist GLUMANDA gesund.",
    "GLUTEXO leben in den Bergen. Die Flammen auf ihren Schweifspitzen leuchten in der Nacht wie Sterne.",
    "Man sagt, GLURAKs Feuer lodert stärker, wenn es harte Kämpfe überstanden hat.",
    "Es zieht sich in seinen Panzer zurück und greift dann mit Wasserstrahlen seine Gegner an.",
    "Man sagt, es werde 10.000 Jahre alt. Sein buschiger Schweif ist ein Symbol für langes Leben.",
    "Der Wasserstrahl, der aus den Wasserdüsen schießt, kann sogar Stahlwände durchdringen.",
    "Seine roten Antennen sondern einen Gestank ab, der Feinde verjagt.",
    "Der stahlharte Panzer schützt seinen zarten Körper. Es wartet geduldig auf seine Entwicklung.",
    "Es liebt Blütenhonig. Es findet selbst Blumen, die sehrwenig Pollen haben.",
    "Es frisst täglich sein Gewichtin Blättern. Die Nadel auf seinem Kopf dient der Verteidigung.",
    "Während es auf seine Entwicklung wartet, versteckt es sich unter Blättern und zwischen Ästen.",
    "Sein bester Angriff: Schnell auf den Gegner zufliegen, mit Giftstacheln zustechen und davonfliegen.",
    "Reizt man dieses an sich gutmütige Pokémon, wehrt es sich wütend.",
    "Auf der Suche nach Nahrung, die es mit scharfen Krallen schlägt, fliegt es durch riesiges Territorium.",
    "Schlägt TAUBOSS mit ganzer Kraft seine Flügel, beugen sich selbst große Bäume durch den Windstoß.",
    "Es strotzt vor Lebenskraft und kann in jeder Umgebung leben. Es ist extrem vorsichtig.",
    "Es wetzt seine ständig wachsenden Zähne an harten Dingen. Es kann Wände aus Beton zernagen.",
    "Im Flug schlägt es eifrig mit den Flügeln. Mit seinem Schnabel sucht es im Gras nach Beute.",
    "Es hat genügend Ausdauer, den ganzen Tag zu fliegen. Sein scharfer Schnabel dient als Waffe.",
    "Es bewegt sich ohne einen Laut durch das Gras und greift seine ahnungslose Beute von hinten an.",
    "Das Muster auf seinem Bauch schüchtert Gegner ein. Sind sie vor Angst erstarrt, umklammert es sie.",
    "Es lebt zusammen mit anderen in Wäldern. In seinen Backentaschen speichert es Elektrizität.",
    "Bei zu viel Elektrizität im Körper wird es aggressiv. Es entlädt sich über seinen Schweif.",
    "Es lebt in trockenen Regionen mit wenig Regen. Es rollt sichein, um sich vor Gegnern zu schützen.",
    "Es rollt sich zusammen und rollt dann auf Gegner zu, die durch seine Stacheln Schaden nehmen.",
    "Es mag keine Kämpfe. Ein einziger Tropfen des Gifts ausseinen Widerhaken hat eine fatale Wirkung.",
    "Bei Gefahr führt es die Widerhaken am Körper aus. Diese wachsen langsamer als die von NIDORINO.",
    "Sein ganzer Körper ist mit harten Schuppen bedeckt. Es beschützt sein Junges mit seinem Leben.",
    "Es untersucht die Umgebung, indem es die Ohren spitzt und lauscht. Sein giftiges Horn schützt es.",
    "Es ist aggressiv und greift seine Gegner mit dem Horn an, welches bei Berührung Gift absondert.",
    "Ein Schlag mit seinem gewaltigen Schweif kann einen Telegrafenmasten knicken wie ein Streichholz.",
    "Es ist bekannt für sein liebenswertes Wesen. Man sagt,es lebt in der Stille der Berge.",
    "Man bekommt es selten zu sehen. Bei Vollmond soll es anden Ufern einsam gelegener Seen spielen.",
    "Es beherrscht Feuerbälle. Während es wächst, teilen sichseine sechs Schweife, um weitere zu bilden.",
    "Seine neun Schweife sollen mystische Kräfte besitzen. Es kann tausend Jahre leben.",
    "Sobald es mit seinen großen, runden Augen rollt, fängt es an, ein Lied zu singen und jeder schläft ein.",
    "Sein feines Fell fühlt sich herrlich an. Es kann sich größer machen, indem es Luft einatmet.",
    "Obwohl es keine Augen hat, kann es Hindernisse mithilfe von Ultraschallwellen wahrnehmen.",
    "Es liebt das Blut von Menschenund Pokémon. Nachts fliegt es umher auf der Suche nach Halsvenen.",
    "Tagsüber verankert es sich mitseinen Wurzelfüßen im Boden. Nachts wandert es und verteilt Samen.",
    "Der Honig, den es abgibt, riecht so entsetzlich, dass sich sogar Nasen in 2 km Entfernung rümpfen.",
    "Es besitzt die größten Blätter der Welt. Es verteilt beim Gehen Pollen, die schreckliche Allergien auslösen.",
    "Auf seinem Rücken wachsen Pilze, die Tochukaso. Sie nehmen an Größe zu, wenn PARAS wächst.",
    "PARASEK wird von einem Pilz, der größer als das Pokémon ist, kontrolliert. Er gibt Giftsporen ab.",
    "Seine großen Augen bestehen aus vielen kleinen Augen. Nachts wird es von Licht angezogen.",
    "Es schlägt mit den Flügeln, um staubartige Schuppen abzusondern. Diese geben bei Kontakt Gift ab.",
    "Dieses Pokémon lebt unterirdisch. Da es an Dunkelheit gewöhnt ist, schreckt helles Licht es ab.",
    "Die drei Köpfe bewegen sich abwechselnd, um sich durch harte Erde in Tiefen von bis zu 100 km zu graben.",
    "Ein nachtaktives Pokémon. Sieht es etwas Schimmerndes, fangen seine Augen an zu glänzen.",
    "Ein sehr stolzes Pokémon. Für Fans ist die Größe des Juwels auf seiner Stirn Anlass für Diskussionen.",
    "Werden seine konstant vorhandenen Kopfschmerzen stärker, entwickelt es seltsame Kräfte.",
    "Dieses Pokémon lebt in Seen. Dank seiner Flossen kann es in atemberaubender Geschwindigkeit schwimmen.",
    "Es lebt mit anderen in Baumkronen. Wird eines von ihnen wütend, werden alle anderen auch wütend.",
    "Siehst du ihm in die Augen, wird es wütend. Rennst du weg, wird es noch wütender. Ärgerst du es...",
    "Ein loyales Pokémon. Es wird erst dann aktiv, wenn sein Trainer ihm Anweisungen gegeben hat.",
    "Mit seiner stolzen und königlichen Erscheinung gewinnt es die Herzen der Menschen.",
    "Seine Haut ist so dünn, dass man die inneren Organe sehen kann. Es hat Schwierigkeiten beim Laufen.",
    "Das Spiralmuster auf seinem Bauch dreht sich langsam. Starrt man darauf, wird einem schwindelig.",
    "Es hat extrem starke Muskeln und kann durch den Ozean schwimmen, ohne sich ausruhen zu müssen.",
    "18 Stunden am Tag schläft es. Und wenn es wach ist, teleportiert es sich, während es sitzen bleibt.",
    "Wenn sich auf deinem Fernsehbildschirm ein Schatten zeigt, ist es in der Nähe. Und das bringt Unglück.",
    "Sein hoch entwickeltes Gehirn ist vergleichbar mit einem Supercomputer.",
    "Es hebt GEOROK hoch, um seinen Körper zu trainieren. Es lebt abseits der Menschen in den Bergen.",
    "MASCHOCKs unbegrenzte Kraft ist gefährlich, daher trägt es einen Gürtel, der die Kraft unterdrückt.",
    "Mit seinen vier Armen schlägt es blitzschnell zu. Für feine Arbeiten sind seine Finger ungeeignet.",
    "Es bevorzugt heiße und feuchte Umgebungen. Seine Beute fängt es blitzschnell mit seinen Ranken.",
    "Ein Pokémon, das wie eine Pflanze aussieht. Es fängt Beute, indem es giftigen Puder darüber verteilt.",
    "In seinem Maul sammelt sich eine Flüssigkeit, die nach Honig riecht, in Wahrheit aber ätzend ist.",
    "Sein Körper besteht aus Wasser. Aus seinen kristallartigen Augen verschießt es eigenartige Strahlen.",
    "Es warnt andere vor Gefahr, indem es seine roten Kugeln auf dem Kopf grell aufleuchten lässt.",
    "Viele von ihnen leben auf Bergpfaden, halb vergraben und Bergsteiger beobachtend.",
    "Sie rollen Abhänge herunter, wobei sie auch Körperteile verlieren. Sie essen eine Tonne Steine am Tag.",
    "Einmal im Jahr wechselt es seine Haut. Seinem Körper kann selbst Dynamit nichts anhaben.",
    "Bereits eine Stunde nach seiner Geburt wachsen seine feurige Mähne und sein feuriger Schweif.",
    "Es braucht aus dem Stand nur 10 Schritte, um seine Höchstgeschwindigkeit zu erreichen.",
    "Es ist langsam, hat aber ein Talent, mit seinem Schweif zu fischen. Beißt etwas an, tut es ihm nicht weh.",
    "Ein begriffsstutziges Pokémon. Beißt das MUSCHAS am Schweif zu, scheint es Intelligenz zu gewinnen.",
    "Die Vorrichtungen an seinem Körper generieren elektromagnetische Wellen, die es schweben lassen.",
    "Eigentlich sind es drei MAGNETILO, die durch Magnetismus verbunden sind.",
    "Ohne seinen Stock kann es nicht leben. Daher beschützt es den Stock mit seinem Leben.",
    "Die Gehirne der beiden Köpfe kommunizieren ihre Gefühle über Telepathie.",
    "Entwickelt sich DODU, teilt sich einer der Köpfe in zwei. Es kann sich mit 60 km/h fortbewegen.",
    "Dieses Pokémon lebt auf Eisbergen. Es schwimmt im Eiswasser und bricht das Eis mithilfe seines Horns.",
    "Sein Körper ist mit einem weißen Fell bedeckt, das es im Schnee fast unsichtbar für Gegner macht.",
    "Es wurde geboren, als Schlamm von den Strahlen des Mondes getroffen wurde. Es erscheint, wo Unrat ist.",
    "Sein Körper sondert eine giftige Substanz ab, die bei Kontakt Pflanzen und Bäume vernichtet.",
    "Es schwimmt rückwärts, indem es seine Schale öffnet und schließt. Es zeigt immer seine breite Zunge.",
    "Im Kampf schließt es seine Schale, um sich zu schützen. Es schießt mit Stacheln auf seine Gegner.",
    "Dieses Pokémon entstand aus giftigen Gasen. Es besiegt auch stärkere Gegner, indem es sie darin einhüllt.",
    "Es kann sich durch jedes Hindernis bewegen. Es versteckt sich in Wänden, um Gegner zu beobachten.",
    "Es lauert im Schatten von Räumen und wartet auf Beute, der es die Lebenskraft rauben kann.",
    "Es gräbt sich mithilfe seines langen und schroffen Körpers durch den Boden. Dabei nimmt es Nahrung auf.",
    "Mit seiner großen Nase kann es die Träume anderer erkennen. Es liebt lustige Träume.",
    "Ein Blick auf das Pendel versetzt einen in 3 Sekunden in Schlaf, selbst wenn man garnicht müde ist.",
    "Es lebt in Höhlen am Strand. Seine Scheren wachsen nach, wenn es sie im Kampf verliert.",
    "Die großen Scheren haben eine Stärke von 10.000 PS. Aber sie sind schwer und kaum zu handhaben.",
    "Sieht aus wie ein Pokéball. Es ist gefährlich, da es bei Berührung explodieren kann.",
    "Es lässt sich vom Wind treiben, wenn es so voller Elektrizität ist, dass es fast explodiert.",
    "Die sechs Eier kommunizieren telepathisch. Werden sie getrennt, finden sie sich schnell wieder.",
    "Man nennt es den „Laufenden Dschungel“. Wird ein Kopf zu groß, fällt er ab und wird zu einem OWEI.",
    "Denkt es an seine verstorbene Mutter, weint es, wobei der Schädel auf seinem Kopf hohl klingt.",
    "Von Geburt an trägt dieses wilde Pokémon Knochen. Es setzt sie talentiert als Waffen ein.",
    "Seine Beine können die doppelte Länge annehmen. Gegner, die das nicht wissen, erschrecken.",
    "Die Schläge, die es austeilt, können Beton pulverisieren. Es muss sich im Kampf alle 3 Minuten ausruhen.",
    "Statt Händen verwendet es seine Zunge, die zweimal so groß wie sein Körper und klebrig ist.",
    "Gase, die leichter als Luft sind, lassen es schweben. Diese Gase stinken und sind explosiv.",
    "Es wächst, indem es Gase aufnimmt, die aus dem Müll aufsteigen. Selten kann man Drillinge von ihnen finden.",
    "Sein Körper ist von einer dicken Haut umgeben und es kann wackelige Gebäude zum Einsturz bringen.",
    "Sein Gehirn entwickelt sich, wenn es sich auf die Hinterbeine stellt. Mit dem Horn bohrt es durch Gestein.",
    "Es legt jeden Tag mehrere Eier, die es aber nicht mit denen teilt, die Schlechtes im Herzen tragen.",
    "Es ist von blauen Ranken umgeben. Niemand hat jemals das Gesicht dieses Pokémon gesehen.",
    "Sein Nachwuchs wächst in seinem Beutel heran. Nur wenn es sicher ist, darf das Junge aus dem Beutel.",
    "Im Schatten von Korallen legt es sein Nest an. Bei Gefahr versprüht es Tinte und flieht.",
    "Seine Stacheln schützen es. Seine Flossen und Knochen werden bei der Herstellung von Medizin verwendet.",
    "Es schwimmt elegant und bewegtseine Hinterflosse wie ein Kleid. Es sieht aus wie eine Königin.",
    "Es baut Nester, indem es Steine im Fluss mit seinem Horn aushöhlt. Es verteidigt seine Eier mit dem Leben.",
    "Wird sein Körper verletzt, kann es sich regenerieren, sofern der rote Kern intakt ist.",
    "In der Körpermitte befindet sich ein roter Kern, der mysteriöse Radiowellen in die Nacht sendet.",
    "Seine Fingerspitzen sondern etwas ab, das die Luft zu einer schützenden Wand werden lässt.",
    "Es ist fast unmöglich, seinen Sensen auszuweichen, da es mit der Schnelligkeit eines Ninja zuschlägt.",
    "Sein Ruf klingt wie menschliche Sprache. Trotzdem ist nicht zu verstehen, was es sagt.",
    "Die Hälfte aller Stromausfälle wird durch dieses Pokémon ausgelöst, das in E-Werken Elektrizität frisst.",
    "Es wurde bei einem Vulkanausbruch geboren. Sein Körper ist von hellen Flammen umgeben.",
    "Es hält seine Beute mit seiner Zange fest und teilt sie dann. Was es nicht teilen kann, wirft es fort.",
    "Sobald es einen Gegner ins Visier genommen hat, rennt es mit dem Kopf voran auf ihn zu.",
    "Es kann nicht einmal gegen schwache Strömungen anschwimmen. Es platscht ohne Unterlass herum.",
    "Sobald es erscheint, nimmt seine Wut nicht eher ab, bis es die Felder und Berge ringsum zerstört hat.",
    "Es liebt es, das Meer mit Pokémon und Menschen auf dem Rücken zu überqueren. Es versteht die Menschen.",
    "Es kann seine Zellstruktur so verändern, dass es sich in alles verwandeln kann, was es sieht.",
    "Ein seltenes Pokémon, das sich seiner Umgebung anpasst, indem es sich in unterschiedlicher Form entwickelt.",
    "Es hat sich so entwickelt, dass es im Wasser leben kann. Im Wasser selbst wird es unsichtbar.",
    "Es kontrolliert 10.000 Volt. Es kann sein Fell so aufstellen, dass es wie scharfe Nadeln wirkt.",
    "In seinem Körper befindet sich eine Flamme. Seine Körpertemperatur liegt vor dem Kampf bei 900 Grad.",
    "Das erste künstlich erzeugte Pokémon der Welt. Es kann durch elektronischen Raum reisen.",
    "Dieses Pokémon wurde aus einem Fossil erweckt. Es lebte im urzeitlichen Meer.",
    "Man geht davon aus, dass das Pokémon ausgestorben ist, weil seine spiralförmige Schale zu groß wurde.",
    "Man geht davon aus, dass dieses Pokémon vor 300 Millionen Jahren die Strände bevölkerte.",
    "Man geht davon aus, dass dieses Pokémon an Land kam, weil seine Beute ebenfalls irgendwann an Land lebte.",
    "Dieses Pokémon flog zu Zeiten der Dinosaurier am Himmel. Seine Zähne sind wie Sägeblätter.",
    "Sein Magen kann jede Art von Nahrung verdauen, selbst wenn sie verschimmelt und verdorben ist.",
    "Ein Legendäres Vogel-Pokémon. Es kann Blizzards verursachen, indem es Feuchtigkeit gefriert.",
    "Ein Legendäres Vogel-Pokémon, das in Gewitterwolken leben soll. Es kontrolliert Blitze. ",
    "Ein Legendäres Vogel-Pokémon. Es wird als Bote des Frühlings angesehen.",
    "Man nennt es „Illusion-Pokémon“,  denn nur wenige haben es gesehen. Nur seine Haut wurde oft gefunden.",
    "Erscheint um seinen Körper eine Aura, gibt es einen Wetterwechsel. Es soll in Seen und Meeren leben.",
    "Man sagt, es lebe in den Meeren. Es bringt Schiffbrüchige sicher an Land.",
    "Die Gene von MEW wurden neu angeordnet, wodurch dieses Pokémon entstand. Es hat ein wildes Herz.",
    "Es beherrscht alle möglichen Attacken, daher sieht man in ihm den Vorfahren aller Pokémon.",

    "Mit dem Blatt auf seinem Kopf bestimmt es die Temperatur und Feuchtigkeit. Es liebt Sonnenbäder.",
    "Die Knospen an seinem Hals geben ein würziges Aroma ab, das andere aufheitert.",
    "Mit seinem Atem kann es tote Pflanzen und Blumen zu neuem Leben erwecken.",
    "Erschrickt sich dieses scheue Pokémon, lodern die Flammen auf seinem Rücken kräftiger.",
    "Es bedroht seine Gegner mit der Hitze seiner Flammen. Ist es kampfbereit, lodern sie stärker.",
    "Es greift seine Gegner mit Feuer an. Dabei entsteht ein Hitzeflimmern, in dem es sich versteckt.",
    "Es hat die Angewohnheit, nach allem zu schnappen, was es sieht. Selbst seine Trainer müssen aufpassen.",
    "Hat es einmal zugebissen, lässt es erst los, wenn es seine Zähne verliert, die schnell nachwachsen.",
    "Eigentlich bewegt es sich langsam, doch seine Beute greift es blitzschnell an.",
    "Ein sehr nervöses Pokémon. Es steht auf seinem Schweif und betrachtet seine Umgebung ganz genau.",
    "Es rollt sich um seine Jungen, wenn diese schlafen sollen. Gegnern begegnet es mit Schnelligkeit.",
    "Es steht ständig auf einem Fuß. Selbst wenn es angreift, stellt es sich nicht auf beide Beine.",
    "Seine Augen sind etwas Besonderes. Sie können selbst bei schwächstem Licht alles erkennen.",
    "Es ist so scheu, dass es sich nur dann bewegt, wenn es sich in einem Schwarm befindet.",
    "Es zieht Energie aus dem Lichtder Sterne. Je mehr Sterne zu sehen sind, desto größer ist sein Muster.",
    "Es baut eine Falle, indem es ein Netz mit dünner, starker Seide spinnt. Es wartet still auf Beute.",
    "Es befestigt Seide an der Beute und lässt sie frei. Es folgt dem Seidenfaden zur Beute und ihren Freunden.",
    "Da seine Beine sich im Laufe der Zeit zu Flügeln entwickelten, besitzt es ausgezeichnete Flugfähigkeiten.",
    "Über seine Antennen entlädt es Elektrizität, mit der es seinen Gegnern einen Schlag versetzt.",
    "LANTURNs Licht kann aus großen Tiefen heraufscheinen. Man nennt es auch „Tiefseestern“.",
    "Sie spielen miteinander, indem sie ihre Schweifspitzen aneinanderhalten und Funken fliegen lassen.",
    "Seine Silhouette sieht aus wie ein Stern. Man sagt, es würde auf Sternschnuppen reisen.",
    "Es hat einen weichen, elastischen Körper. Hat es einmal angefangen zu hüpfen, ist es nicht zu stoppen.",
    "Seine Schale ist mit Glücksgefühlen gefüllt, die es mit gutherzigen Lebewesen teilt.",
    "Man sagt, es erscheint vor denen, die gutherzig sind, und verteilt glänzenden „Glücksstaub“.",
    "Es pickt seine Nahrung aus Kakteen, wobei es Knospen und Dornen verschont.",
    "Dieses Pokémon kann die Vergangenheit und die Zukunft sehen. Es beobachtet ständig die Sonne.",
    "Sein weiches Fell wird doppelt so dick, wenn sich Elektrizität aufbaut.",
    "Hat es sich mit Elektrizität aufgeladen, leuchtet sein Schweif und es feuert Haare ab, die sich entladen.",
    "Seine Schweifspitze leuchtet hell. In alten Zeiten hat man mit seiner Hilfe Lichtsignale gegeben.",
    "Sobald die Regenzeit vorbei ist, wird es von der warmen Sonne nach draußen gezogen, wo es tanzt.",
    "Seinen Schweif verwendet es als eine Art Rettungsring, wenn es taucht. Es liebt Wasserpflanzen.",
    "Seine langen Ohren sind hervorragende Sensoren. Mit ihnen kann es Bewegungen im Fluss wahrnehmen.",
    "Es steht an Pfaden und tut so, als sei es ein Baum. Im Regen scheint es zu verschwinden.",
    "Es versammelt andere Pokémon um sich herum und führt sie an. QUAPSEL gehorchen seinem Ruf. ",
    "Es reitet auf dem Wind. Man sagt, wenn sich HOPPSPROSS versammeln, kommt der Frühling bald.",
    "Sobald es wärmer wird, bläht es auf. Es schwebt am Himmel und nimmt so viel Sonnenlicht auf wie möglich.",
    "Es lässt sich von den Winden um den Globus tragen und verteilt auf seinem Flug Baumwollsamen.",
    "Mit dem Schweif pflückt es hochhängende Früchte. Mit ihm ist es geschickter als mit den Händen.",
    "Morgens fällt es plötzlich aus dem Himmel. War der Sommer kalt, explodiert seine Population.",
    "Warmes Sonnenlicht gibt ihm Energie. Daher wandert es stets dem Sonnenlicht hinterher.",
    "Mit seinen Augen hat es einen Blickwinkel von 360 Grad. Es sieht sogar Beute, die sich hinter ihm befindet.",
    "Es lebt im kalten Wasser. Es vergräbt sich halb im schlammigen Boden, wenn es schlafen will.",
    "Ein träges Pokémon, das auf dem Grund des Flusses liegt und wartet, dass ihm Beute ins Maul schwimmt.",
    "Sein Fell ist wie Samt. Setzt es Psycho-Kräfte ein, leuchtet die Kugel in seiner Stirn.",
    "Mondlicht hat die genetische Struktur von EVOLI verändert. Im Dunkeln wartet es auf Beute.",
    "Es lockt Wanderer tief in den Wald. Mansagt, es bringe Unglück.",
    "Wird es von MUSCHAS gebissen, wird es so intelligent, dass es sich mit Nobelpreisträgern messen kann.",
    "Es liebt es, sich nachts an andere heranzuschleichen und sie mit einem schrillen Schrei zu erschrecken.",
    "Es scheint, als ob diese Pokémon per Telepathie miteinander kommunizieren. Sie sitzen in Wänden.",
    "Verzweifelt versucht es, seinen schwarzen Schweif zu verstecken. Er soll ein Geheimnis enthalten.",
    "Sein Schweif besitzt ebenfalls ein Gehirn. Er beißt jeden Gegner, der sich von hinten heranschleicht.",
    "Es sieht aus wie ein Tannenzapfen. Seine Schale schützt es vor Flug-Pokémon, die an ihm picken wollen.",
    "Dieses Pokémon ist von einer Stahlhülle umgeben. Seine stechenden Augen sind alles, was man von ihm sieht.",
    "Es gräbt sich mit seinem Schweif in die Erde und baut ein labyrinthartiges Nest. Es kann kaum fliegen.",
    "Es segelt durch die Lüfte und breitet seine Gliedmaßen aus, um aus der Luft anzugreifen.",
    "Hoher Druck und hohe Temperaturen haben seinen Körper härter als Stahl werden lassen.",
    "Kleine Pokémon fliehen beim Anblick seines Gesichts. Frauen aber finden es niedlich.",
    "Es ist trotz seines Äußeren schüchtern. Wird es wütend, schnappt es mit seinen Fängen zu.",
    "Es verschießt die giftigen Stacheln seines Körpers in alle Richtungen. Es ist kein guter Schwimmer.",
    "Sein Körper ist stahlhart. Es bedroht seine Gegner, indem es die Augen auf seinen Scheren zeigt.",
    "In seiner Schale bewahrt es Beeren auf. Diese fermentieren zu einem leckeren Saft.",
    "Mit kräftigen Beinen und Krallen kann es seine Gegner über große Entfernungen schleudern.",
    "Es ist sehr grausam und hört nicht eher mit seinem Angriff auf, bis der Gegner bewegungsunfähig ist.",
    "Es lässt Honig in seine Pranken eindringen, so dass es immer welchen dabei hat.",
    "Es durchstreift sein Revier und markiert Bäume, die Früchte oder Beeren tragen, mit Kratzern.",
    "Sein Körper besteht aus Magma. Bleibt es nicht ständig in Bewegung, kühlt es aus und verhärtet sich.",
    "Seine Körpertemperatur beträgt etwa 10.000 Grad. Aus seiner Schale treten immer wieder Flammen aus.",
    "Es liebt Pilze, die unter totem Gras wachsen. Auf der Suche nach Nahrung findet es heiße Quellen.",
    "Sein dichtes Fell verdeckt seine Augen. Es untersucht seine Umgebung mit seiner Nase.",
    "Viele von ihnen leben in den sauberen Seen im Süden. In verschmutzten können sie nicht leben.",
    "Es heftet sich an MANTAX, ernährt sich von dessen Essensresten und weicht so auch Gegnern aus.",
    "Es lebt in Gesteinsritzen und Löchern auf dem Meeresboden. Mit Saugnäpfen hält es Beute fest.",
    "Im eingerollten Schweif transportiert es Futter, das es mit denen teilt, die sich verlaufen haben.",
    "Wenn das Meer ruhig ist, kann man vielleicht einen Schwarm MANTAX sehen, die schnell vorbeischwimmen.",
    "Es wird komplett von einer eisenharten Rüstung geschützt. Wenn es fliegt, erreicht es bis zu 300 km/h.",
    "Durch unterschiedliche Schreie drückt es seine Gefühle aus. Diese Pokémon jagen im Verbund nach Beute.",
    "In alten Zeiten glaubte man, das Heulen dieses Pokémon sei der Ruf des Todes.",
    "Es lebt in Höhlen auf dem Meeresgrund. Jedes Mal, wenn es sich bewegt, entsteht ein Strudel.",
    "Trotz seiner geringen Größe ist es stark. Es kann einen Erwachsenen mühelos auf dem Rücken tragen.",
    "Wenn es angreift, rollt es sich zusammen und auf den Gegner zu. Es kann Häuser zum Einsturz bringen.",
    "Interstellare Software wurde installiert, so dass dieses Pokémon sich auch im All bewegen kann.",
    "Starrt man auf sein Geweih, bekommt man das seltsame Gefühl, in dessen Mitte gezogen zu werden.",
    "Sein Revier markiert es mit seinem pinselartigen Schweif. Es kennt mehr als 5.000 Markierungen.",
    "Es ist bekannt für seine Kampfbegierde. Es erholt sich ständig von Verletzungen.",
    "Es kämpft, während es sich wie ein Kreisel dreht. Die Zentrifugalkraft erhöht die Kampfkraft um 10.",
    "Dieses Pokémon berührt alles mit den Lippen. Diese merken sich, was es mag und nicht mag.",
    "Es bewegt seine Arme und generiert damit Elektrizität. Diese kann es aber nicht speichern.",
    "Seine Körpertemperatur liegt bei etwa 600 Grad. Ist es gesund, atmet es gelbe Flammen aus.",
    "Man sagt, dass Kinder, die MILTANKs Milch trinken, zu gesunden Erwachsenen heranwachsen.",
    "Dieses gutherzige Pokémon pflegt kranke Pokémon gesund. Es spürt die Traurigkeit anderer Lebewesen.",
    "Dieses Pokémon ist der personifizierte Blitz. Es kann aus den Wolken über seinem Rücken Blitze schleudern.",
    "Man sagt, wenn es brüllt, bricht irgendwo in der Welt ein Vulkan aus.",
    "Es wandert beständig in der Welt herum, um verunreinigtes Wasser zu reinigen. Es zieht mit dem Nordwind.",
    "Dieses Pokémon frisst das Erdreich. Hat es einen Berg verspeist, schläft es ein, um zu wachsen.",
    "Sein Körper ist hart wie Fels.Es lässt mit Hochdruck Gas ab, um wie eine Rakete nach oben zu schießen.",
    "Bei einem Tobsuchtsanfall zerstört es ganze Gebirge und legt Flüsse trocken.",
    "Es schläft in einem Tiefseegraben. Schwingt es seine Flügel, entsteht ein Sturm, der 40 Tage dauert.",
    "Seine Federn besitzen 7 Farben. Man sagt, dass demjenigen, der es sieht, ewiges Glück beschieden ist.",
    "Es kann durch die Zeit reisen, aber es erscheint nur zu friedlichen Zeiten.",

};

void readAll( const char* base_path ) {
    pD.assign( MAX_PKMN + 10, { 0 } );
    for( int i = 0; i <= MAX_PKMN; ++i ) {
        sprintf( buffer, "%s/PKMNDATA/%d.data", base_path, i );
        FILE* f = fopen( buffer, "r" );
        if( !f ) {
            fprintf( stderr, "Can't open file \"%s\"", buffer );
            exit( 1 );
        }
        fread( &pD[ i ], sizeof( pokemonData ), 1, f );
        fclose( f );
        pD[ i ].m_stage = 0;
        pD[ i ].m_preEvolution = 0;
    }

    for( int i = 0; i <= MAX_PKMN; ++i ) {
        for( int j = 0; j < 7; ++j ) {
            if( !pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto )
                continue;
            pD[ pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto ].m_stage = pD[ i ].m_stage + 1;
            pD[ pD[ i ].m_evolutions[ j ].m_e.m_evolvesInto ].m_preEvolution = i;
        }
     //   printf( "\"%s\",\n", pD[ i ].m_dexEntry );
        if( i <= 251 )
            strcpy( pD[ i ].m_dexEntry, dex[ i ] );
    }

    // Fossils
    FOSSIL( OMANYTE, I_HELIX_FOSSIL );
    FOSSIL( KABUTO, I_DOME_FOSSIL );
    FOSSIL( AERODACTYL, I_OLD_AMBER );
    FOSSIL( LIEEP, I_ROOT_FOSSIL );
    FOSSIL( ANORITH, I_CLAW_FOSSIL );
    FOSSIL( CRAINIDOS, I_SKULL_FOSSIL );
    FOSSIL( SHIELDON, I_ARMOR_FOSSIL );
    FOSSIL( TIRTUOGA, I_COVER_FOSSIL );
    FOSSIL( ARCHEN, I_PLUME_FOSSIL );
/*
    for( int i = 0; i <= MAX_PKMN; ++i ) {
        printf( "%p, %15s (%03d) is a stage %d Pokémon and evolves from: %15s (%03d) gr: %d\n",
            ( pD[ i ].m_displayName - (char*)&pD[ i ] ),
            pD[ i ].m_displayName,
            i,
            pD[ i ].m_stage,
            pD[ pD[ i ].m_preEvolution ].m_displayName,
            pD[ i ].m_preEvolution,
            pD[ i ].m_expType );
    }
*/
}

void writeAll( const char* base_path ) {
    for( int i = 0; i <= MAX_PKMN; ++i ) {
        sprintf( buffer, "%s/PKMNDATA/%d.data", base_path, i );
        FILE* f = fopen( buffer, "w" );
        if( !f )
            exit( 1 );
        fwrite( &pD[ i ], sizeof( pokemonData ), 1, f );
        fclose( f );
    }
}

int main( int ac, char** av ) {
    if( ac < 2 )
        return 1;
    readAll( av[ 1 ] );
    writeAll( av[ 2 ] );
}
