using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Perm2Helper
{
    class Program
    {
        struct ability ( public string a, public string b, public int c )
        {
            public void write ( string path )
            {
                using (StreamWriter sw = new StreamWriter( path, false, Encoding.GetEncoding( 1252 ), 1 ))
                    sw.Write( a + "*\n" + b + "*\n" + c );
            }
        }


        static ability[] abilities = {
            new ability( "Energieaufladung", "Schützt vor Lärmattacken.", 1 ),
            new ability( "Duftnote", "Macht Begegnungen mit wilden PKMN unwahrscheinlicher.", 8 ),
            new ability( "Niesel", "Betritt das PKMN den Kampf, beginntes zu regnen.", 18 ),
            new ability( "Temposchub", "Erhöht im Kampf kontinuierlich die Initiative.", 32 ),
            new ability( "Kampfpanzer", "Schützt vor Volltreffern.", 1 ),
            new ability( "Robustheit", "Schützt vor K.O.-Attacken.", 1 ),
            new ability( "Feuchtigkeit", "Verhindert Selbstzerstörung von PKMN im Kampf.", 1 ),
            new ability( "Flexibilität", "Schützt vor Paralyse.", 1 ),
            new ability( "Sandschleier", "Macht Fehlschlag gegn. Attacken im Sandsturm wahrscheinlicher.", 1 ),
            new ability( "Statik", "Paralysiert bei Berührung.", 1 ),
            new ability( "Volt-Absorber", "Wandelt Schaden von [ELEKTRO] Attacken in KP um.", 1 ),
            new ability( "H2O-Absorber", "Wandelt Schaden von [WASSER] Attacken in KP um.", 1 ),
            new ability( "Dösigkeit", "Schützt vor Anziehung.", 1 ),
            new ability( "Wolke Sieben", "Schützt vor Wetter-Effekten.", 1 ),
            new ability( "Facettenauge", "Macht Treffer eigener Attacken wahrscheinlicher.", 1 ),
            new ability( "Insomnia", "Schützt vor Schlaf.", 1 ),
            new ability( "Farbwechsel", "Passt eigenen Typ der letzten gegn. Attacke an.", 32 ),
            new ability( "Immunität", "Schützt vor Vergiftung.", 1 ),
            new ability( "Feuerfänger", "Verstärkt eigene [FEUER] Attacken nach Treffer gegn. [FEUER] Atttacke.", 1 ),
            new ability( "Puderabwehr", "Schützt vor Zusatzeffekten gegn. Attacken.", 1 ),
            new ability( "Tempomacher", "Schützt vor Verwirrung.", 1 ),
            new ability( "Saugnapf", "Schützt vor ungewolltem Austausch.", 1 ),
            new ability( "Bedroher", "Betritt das PKMN den Kampf, wird der gegn. Angriff verringert.", 2 ),
            new ability( "Wegsperre", "Verhindert Austausch oder Fluchtgegn. PKMN.", 16 ),
            new ability( "Rauhaut", "Schadet gegn. PKMN nach direktem Treffer.", 32 ),
            new ability( "Wunderwache", "Nur sehr effektive Attacken können Schaden zufügen.", 1 ),
            new ability( "Schwebe", "Schützt vor [BODEN] Attacken.", 1 ),
            new ability( "Sporenwirt", "Verursacht evtl. Sp. Zustand bei gegn. PKMN nach direktem Treffer.", 32 ),
            new ability( "Synchro", "Gibt Sp. Zustand an gegn. PKMN weiter.", 32 ),
            new ability( "Neutraltorso", "Schützt vor Statusveränd.", 1 ),
            new ability( "Innere Kraft", "Schützt vor Statusveränd.", 1 ),
            new ability( "Blitzfänger", "Schützt vor [ELEKTRO] Attacken.", 1 ),
            new ability( "Edelmut", "Macht Zusatzeffekte von eig. Attacken wahrscheinlicher.", 1 ),
            new ability( "Wassertempo", "Erhöht eigene Initiative bei Regen.", 16 ),
            new ability( "Chlorophyll", "Erhöht eigene Initiative bei starkem Sonnenschein.", 16 ),
            new ability( "Erleuchtung", "Macht Begegnungen mit wilden PKMN wahrscheinlicher.", 8 ),
            new ability( "Fährte", "Kopiert die Fähigkeit eines gegn. PKMN.", 2 ),
            new ability( "Kraftkollos", "Erhöht eigenen Angriff.", 8 ),
            new ability( "Giftdorn", "Vergiftet bei Berührung.", 32 ),
            new ability( "Konzentrator", "Schützt vor Zurückschrecken.", 1 ),
            new ability( "Magmapanzer", "Schützt vor Einfrieren. Beschleunigt Brut-vorgänge.", 9 ),
            new ability( "Aquahülle", "Schützt vor Verbrennungen.", 1 ),
            new ability( "Magnetfalle", "Verhindert Flucht oder Austausch von gegn. [STAHL] PKMN.", 16 ),
            new ability( "Lärmschutz", "Schützt vor Lärmattacken.", 1 ),
            new ability( "Regengenuss", "Regeneriert KP bei Regen.", 16 ),
            new ability( "Sandsturm", "Betritt das PKMN den Kampf, so tritt ein Sandsturm auf.", 16 ),
            new ability( "Erzwinger", "Gegn. PKMN verbrauchen 2AP pro erfolgreicher Attacke.", 32 ),
            new ability( "Speckschicht", "Reduziert Schaden von [EIS] und [FEUER] Attacken.", 1 ),
            new ability( "Frühwecker", "Schnelles Aufwachen.", 16 ),
            new ability( "Flammkörper", "Verbrennt bei Berührung. Beschleunigt Brutvorgänge", 9 ),
            new ability( "Angsthase", "Garantiert das Entkommen von wilden PKMN.", 16 ),
            new ability( "Adlerauge", "Schützt vor Absinken der Genauigkeit.", 1 ),
            new ability( "Scherenmacht", "Schützt vor Absinken des Angriffs.", 1 ),
            new ability( "Mitnahme", "Sammelt Gegenstände.", 8 ),
            new ability( "Schnarchnase", "PKMN kann nicht in zwei aufeinanderfolgenden Zügen angreifen.", 16 ),
            new ability( "Übereifer", "Erhöht Angriff, senkt Genauigkeit.", 8 ),
            new ability( "Charmebolzen", "Betört bei Kontakt.", 32 ),
            new ability( "Plus", "Erhöht Sp. Angriff sofern ein PKMN mit Minus am Kampf teilnimmt.", 1 ),
            new ability( "Minus", "Erhöht Sp. Angriff sofern ein PKMN mit Plus am Kampf teilnimmt.", 1 ),
            new ability( "Prognose", "Passt äußeres Erscheinungsbild dem Wetter an.", 32 ),
            new ability( "Wertehalter", "Schützt vor Item-Diebstahl.", 1 ),
            new ability( "Expidermis", "PKMN heilt evtl. eigene Sp. Zustände.", 16 ),
            new ability( "Adrenalin", "Erhöht Angriff bei Statusveränderungen.", 16 ),
            new ability( "Notschutz", "Erhöht Verteidigung bei Statusveränderungen.", 16 ),
            new ability( "Kloakensoße", "Schadet Anwendern von Saugattacken.", 32 ),
            new ability( "Notdünger", "Erhöht Schaden durch [PFLANZE] Attacken in einer Notlage.", 16 ),
            new ability( "Großbrand", "Erhöht Schaden durch [FEUER] Attacken in einer Notlage.", 16 ),
            new ability( "Sturzbach", "Erhöht Schaden durch [WASSER] Attacken in einer Notlage.", 16 ),
            new ability( "Hexaplaga", "Erhöht Schaden durch [KAEFER] Attacken in einer Notlage.", 16 ),
            new ability( "Steinhaupt", "Schützt vor Rückstoßschaden.", 1 ),
            new ability( "Dürre", "Betritt das PKMN den Kampf, intensiviert sich die Sonneneinstrahlung.", 16 ),
            new ability( "Ausweglos", "Verhindert Austausch oder Flucht gegn. PKMN.", 16 ),
            new ability( "Munterkeit", "Schützt vor Einschlafen.", 1 ),
            new ability( "Pulverrauch", "Schützt vor Absenken von Statuswerten.", 1 ),
            new ability( "Mentalkraft", "Erhöht Schaden durch physische Attacken.", 1 ),
            new ability( "Panzerhaut", "Schützt vor Volltreffern.", 1 ),
            new ability( "Klimaschutz", "Neutralisiert Wettereffekte.", 16 ),
            //GEN 4
            new ability( "Fußangel", "Ist das PKMN verwirrt, sind Treffer gegn. PKMN unwahrscheinlicher.", 1 ),
            new ability( "Starthilfe", "Erhöht eigene Initiative, wenn von [ELEKTRO] Attacke getroffen.", 32 ),
            new ability( "Rivalität", "Eigener Angriff steigt bei gleichem Geschlecht des Gegners.", 16 ),
            new ability( "Felsenfest", "Erhöht eigene Initiative, wenn das PKMN zurückschreckt.", 32 ),
            new ability( "Schneemantel", "Macht Fehlschlag gegn. Attacken bei Hagel wahrscheinlicher.", 1 ),
            new ability( "Völlerei", "Das PKMN benutzt Beeren frühzeitig.", 32 ),
            new ability( "Kurzschluss", "Maximiert Angriff nach erlittenem Volltreffer.", 32 ),
            new ability( "Entlastung", "Erhöht eigene Initiative nach Benutzung eines getragenen Items.", 32 ),
            new ability( "Hitzeschutz", "Schwächt gegn. [FEUER] Attacken.", 1 ),
            new ability( "Wankelmut", "Verdoppelt den Effekt von Statusveränderungen.", 1 ),
            new ability( "Trockenheit", "Heißes Wetter reduziert KP, Wasser erhöht KP.", 1 ),
            new ability( "Download", "Passt Statuswerte entsprechend denen gegn. PKMN an.", 16 ),
            new ability( "Eisenfaust", "Verstärkt schlag-basierte Attacken.", 1 ),
            new ability( "Aufheber", "Regeneriert KP bei Vergiftung.", 1 ),
            new ability( "Anpassung", "Steigert Attacken des selben Typs.", 1 ),
            new ability( "Wertelink", "Mehrfach ausgeführte Attacken treffen über die max. Anz. an Zügen.", 1 ),
            new ability( "Hydration", "Heilt Sp. Zustände bei Regen.", 16 ),
            new ability( "Solarkraft", "Erhöht Sp. Angriff und senkt KP bei Sonne.", 1 ),
            new ability( "Rasanz", "Erhöht Initiative wenn das PKMN von einem Sp. Zustand betroffen ist.", 1 ),
            new ability( "Regulierung", "Alle Attacken des PKMN werden zu [NORMAL] Attacken.", 1 ),
            new ability( "Superschütze", "Steigert Attacken nach Volltreffern.", 1 ),
            new ability( "Magieschild", "Nur physische Attacken schaden dem PKMN.", 1 ),
            new ability( "Schildlos", "Alle Attacken treffen garantiert.", 1 ),
            new ability( "Zeitspiel", "Das PKMN agiert stets zuletzt.", 16 ),
            new ability( "Techniker", "Steigert Effektivität von schwächeren Attacken.", 1 ),
            new ability( "Floraschild", "Heilt Sp. Zustände bei Regen.", 16 ),
            new ability( "Tollpatsch", "Das PKMN kann keine Items verwenden.", 16 ),
            new ability( "Überbrückung", "Greift unabhängig anderer Fähigkeiten an.", 1 ),
            new ability( "Glückspilz", "Macht Volltreffer wahrscheinlicher.", 1 ),
            new ability( "Finalschlag", "Schadet dem PKMN, das finale Attacke landet.", 32 ),
            new ability( "Vorahnung", "Ahnt gefährliche Attacke eines gegn. PKMN.", 2 ),
            new ability( "Vorwarnung", "Ahnt stärkste Attacke eines gegn. PKMN.", 2 ),
            new ability( "Unkenntnis", "Ignoriert gegn. Statusveränderungen.", 1 ),
            new ability( "Aufwertung", "Wertet \"weniger effektive\" Attacken auf.", 1 ),
            new ability( "Filter", "Senkt Stärke sehr effektiver Attacken.", 1 ),
            new ability( "Saumselig", "Halbiert Angriff und Initiative zu Kampfbeginn.", 16 ),
            new ability( "Rauflust", "Ermöglicht gegnerischen [GEIST] PKMN zu schaden.", 1 ),
            new ability( "Sturmsog", "Zieht offensive [WASSER] Attacken auf sich.", 1 ),
            new ability( "Eishaut", "Regeneriert KP bei Hagel.", 16 ),
            new ability( "Felskern", "Senkt Stärke sehr effektiver Attacken.", 1 ),
            new ability( "Hagelalarm", "Betritt das PKMN den Kampf, beginnt es zu hageln.", 16 ),
            new ability( "Honigmaul", "Das PKMN sammelt von Zeit zu Zeit Honig.", 8 ),
            new ability( "Schnüffler", "Erkennt Items gegnerischer PKMN.", 16 ),
            new ability( "Achtlos", "Verstärkt Attacken mit Rückstoß.", 1 ),
            new ability( "Variabilität", "Passt Typ der getragenen Tafel an.", 8 ),
            new ability( "Pflanzengabe", "Steigert Effektivität von Team-PKMN bei Sonne.", 1 ),
            new ability( "Alptraum", "Schadet schlafenden gegnerischen PKMN.", 1 ),
            //Gen 5
            new ability( "Langfinger", "Stiehlt Item bei Kontakt.", 1 ),
            new ability( "Rohe Gewalt", "Wandelt Zusatzeffekte von Attacken in zusätzliche Stärke.", 1 ),
            new ability( "Umkehrung", "Kehrt Statusveränderungen um.", 1 ),
            new ability( "Anspannung", "Verhindert gegn. Einsatz von Beeren.", 1 ),
            new ability( "Siegeswille", "Erhöht Angriff stark, wenn Statuswerte absinken.", 1 ),
            new ability( "Schwächling", "Halbiert Angr. und SpAngr. bei halben KP.", 1 ),
            new ability( "Tastfluch", "Blockiert letzte gegn. Attacke bei Kontakt.", 1 ),
            new ability( "Heilherz", "Heilt Statusprobleme von PKMN im Team.", 1 ),
            new ability( "Freundeshut", "Weniger Schaden bei PKMN im Team.", 1 ),
            new ability( "Bruchrüstung", "Erhöht Initiative, senkt Verteidigung nach Treffer.", 1 ),
            new ability( "Schwermetall", "Verdoppelt Gewicht.", 1 ),
            new ability( "Leichtmetall", "Halbiert Gewicht.", 1 ),
            new ability( "Multischuppe", "Halber Schaden bei vollen KP.", 1 ),
            new ability( "Giftwahn", "Erhöht Angriff, wenn vergiftet.", 1 ),
            new ability( "Hitzewahn", "Erhöht Sp.Angr., wenn verbrannt.", 1 ),
            new ability( "Reiche Ernte", "Regeneriert benutzte Beeren.", 1 ),
            new ability( "Telepathie", "Immun gegen Attaken von PKMN im Team.", 1 ),
            new ability( "Gefühlswippe", "Erhöht und senkt zufällige Statuswerte pro Runde.", 1 ),
            new ability( "Wetterfest", "Immun gegen Wetter.", 1 ),
            new ability( "Giftgriff", "Vergiftet bei Kontakt.", 1 ),
            new ability( "Belebekraft", "Heilt sich bei Austausch.", 1 ),
            new ability( "Brustbieter", "Macht Volltreffer wahrscheinlicher.", 1 ),
            new ability( "Sandscharrer", "Erhöht Initiative bei Sandsturm.", 1 ),
            new ability( "Wunderhaut", "Gegn. Status-Attacken fehlen manchmal.", 1 ),
            new ability( "Analyse", "Erhöht Stä, wenn zuletzt agierend.", 1 ),
            new ability( "Illusion", "Nimmt Gestalt des letzten PKMN im Team an.", 2 ),
            new ability( "Doppelgänger", "Nimmt Gestalt des Gegners an.", 2 ),
            new ability( "Schwebedurch", "Ignoriert Reflektor, Lichtschild und Bodyguard.", 1 ),
            new ability( "Mumie", "Verteilt Mumie bei Kontakt.", 1 ),
            new ability( "Hochmut", "Erhöht Angriff, wenn gegn. PKMN besiegt.", 1 ),
            new ability( "Redlichkeit", "Erhöht Angriff, wenn von gegn. [UNLICHT] Attacke getroffen.", 1 ),
            new ability( "Hasenfuß", "Erhöht Init., wenn von gegn. [GEIST], [KAEFER] oder [UNLICHT] Attacke getroffen.", 1 ),
            new ability( "Magiespiegel", "Kann Statusveränderungen zurückwerfen.", 1 ),
            new ability( "Vegetarier", "Immun gegen [PFLANZE] Attacken.", 1 ),
            new ability( "Strolch", "Erhöht Priorität von Status-Attacken.", 1 ),
            new ability( "Sandgewalt", "Stärkt [BODEN], [KAEFER] und [ELEKTRO] Attacken während eines Sandsturms.", 1 ),
            new ability( "Eisenstachel", "Schadet bei Kontakt.", 1 ),
            new ability( "Zen-Modus", "Wandelt Form bei niedriegen KP.", 1 ),
            new ability( "Triumphstern", "Erhöht Genauigkeit von Attacken von PKMN im Team.", 1 ),
            new ability( "Turbobrand", "Greift unabhängig anderer Fähigkeiten an.", 1 ),
            new ability( "Teravolt", "Greift unabhängig anderer Fähigkeiten an.", 1 ),

            //Gen 6
            new ability( "Dufthülle", "Schützt alle PKMN im Team vor manchen mentalen Attacken.", 1 ),
            new ability( "Blütenhülle", "Schützt [PFLANZE] PKMN vor Statusveränderungen.", 1 ),
            new ability( "Backentaschen", "Heilt nach Beerenkonsum.", 1 ),
            new ability( "Wandlungskunst", "Wandelt Typ zu dem der nächst benutzten Attacke.", 1 ),
            new ability( "Fellkleid", "Halbiert physischen Schaden.", 1 ),
            new ability( "Zauberer", "Stiehlt Items von PKMN, die durch Attacken getroffen wurden.", 1 ),
            new ability( "Kugelsicher", "Schützt vor geworfenen Bomben und Kugeln.", 1 ),
            new ability( "Unbeugsamkeit", "Erhöht Sp.Angr. stark, wenn ein Statuswert gesenkt wurde.", 1 ),
            new ability( "Titankiefer", "Erhöht die Stärke von Bissattacken.", 1 ),
            new ability( "Frostschicht", "Wandelt [NORMAL] Attacken in [EIS] Attacken um und verstärkt diese.", 1 ),
            new ability( "Zuckerhülle", "Verhindert Schlaf.", 1 ),
            new ability( "Taktikwechsel", "Formwandel bei Taktikwechsel.", 1 ),
            new ability( "Orkanschwingen", "Erhöht Priorität von [FLUG] Attacken.", 1 ),
            new ability( "Megawumme", "Erhöht Stärke von Aura-Attacken.", 1 ),
            new ability( "Pflanzenpelz", "Erhöht die Verteidigung, wenn Grasfeld aktiv ist.", 1 ),
            new ability( "Nutznießer", "Das Pokémon kann sein Item an einen Mitstreiter weitergeben.", 1 ),
            new ability( "Krallenwucht", "Erhöht Stärke von phys. Attacken", 1 ),
            new ability( "Feenschicht", "Wandelt [NORMAL] Attacken in [FEE] Attacken um und verstärkt diese.", 1 ),
            new ability( "Viskosität", "Die Initiative von Angreifern sinkt durch bloßes Berühren.", 1 ),
            new ability( "Zenithaut", "Wandelt [NORMAL] Attacken in [KAEFER] Attacken um und verstärkt diese.", 1 ),
            new ability( "Familienbande", "PKMN greift 2 mal nacheinander an.", 1 ),
            new ability( "Dunkelaura", "Verstärkt [UNLICHT] Attacken des Teams.", 1 ),
            new ability( "Feenaura", "Verstärkt [FEE] Attacken des Teams.", 1 ),
            new ability( "Aura-Umkehr", "Kehrt Effekte von Dunkelaura und Feenaura um.", 1 ),

            new ability( "Urmeer", "Solange das PKMN im Kampf ist, herrscht strömender Regen.", 1 ),
            new ability( "Endland", "Solange das PKMN im Kampf ist, herrscht extremes Sonnenlicht.", 1 ),
            new ability( "Delta-Wind", "Solange das PKMN im Kampf ist, herrschen starke Winde.", 1 ),


            //Gen *
            new ability( "Klon", "Das PKMN kann sich weder entwickeln, noch kann es TM oder VM erlernen.", 1 ),
            new ability( "Unantastbar", "Immun gegen alle Attacken, Wetter und Items.", 1 ),
            new ability( "Fischauge", "Erhöht Genauigkeit nach direktem Treffer.", 1 ),
            new ability( "Gleichheit", "Erhöht Effektivität eigener Attacken, bei Gegnern selbem Typs.", 1 ),
            new ability( "Schillerschuppe", "Macht Begegnungen mit schillernden PKMN wahrscheinlicher.", 8 ),
            new ability( "Nebelwand", "Betritt das PKMN den Kampf, zieht starker Nebel auf.", 16 ),
            new ability( "Einschüchterer", "Betritt das PKMN den Kampf, tauscht der Gegner sein PKMN aus.", 1 ),
            new ability( "Effizienz", "Verdoppelt Schwächen und Resistenzen.", 1 ),
            new ability( "Überfluss", "Verbraucht keine AP.", 1 )
        };


        static string[] abilityNames = {
            "Cacophony",
            "Stench",
            "Drizzle",
            "Speed Boost",
            "Battle Armor",
            "Sturdy",
            "Damp",
            "Limber",
            "Sand Veil",
            "Static",
            "Volt Absorb",
            "Water Absorb",
            "Oblivious",
            "Cloud Nine",
            "Compound Eyes",
            "Insomnia",
            "Color Change",
            "Immunity",
            "Flash Fire",
            "Shield Dust",
            "Own Tempo",
            "Suction Cups",
            "Intimidate",
            "Shadow Tag",
            "Rough Skin",
            "Wonder Guard",
            "Levitate",
            "Effect Spore",
            "Synchronize",
            "Clear Body",
            "Natural Cure",
            "Lightning Rod",
            "Serene Grace",
            "Swift Swim",
            "Chlorophyll",
            "Illuminate",
            "Trace",
            "Huge Power",
            "Poison Point",
            "Inner Focus",
            "Magma Armor",
            "Water Veil",
            "Magnet Pull",
            "Soundproof",
            "Rain Dish",
            "Sand Stream",
            "Pressure",
            "Thick Fat",
            "Early Bird",
            "Flame Body",
            "Run Away",
            "Keen Eye",
            "Hyper Cutter",
            "Pickup",
            "Truant",
            "Hustle",
            "Cute Charm",
            "Plus",
            "Minus",
            "Forecast",
            "Sticky",
            "Shed Skin",
            "Guts",
            "Marvel Scale",
            "Liquid Ooze",
            "Overgrow",
            "Blaze",
            "Torrent",
            "Swarm",
            "Rock Head",
            "Drought",
            "Arena Trap",
            "Vital Spirit",
            "White Smoke",
            "Pure Power",
            "Shell Armor",
            "Air Lock",
            "Tangled Feet",
            "Motor Drive",
            "Rivalry",
            "Steadfast",
            "Snow Cloak",
            "Gluttony",
            "Anger Point",
            "Uburden",
            "Heatproof",
            "Simple",
            "Dry Skin",
            "Download",
            "Iron Fist",
            "Poison",
            "Adaptability",
            "Skill Link",
            "Hydration",
            "Solar Power",
            "Quick Feet",
            "Normalize",
            "Sniper Powers",
            "Magic Guard",
            "No Guard",
            "Stall",
            "Technician",
            "Leaf Guard",
            "Klutz",
            "Mold Breaker",
            "Super Luck",
            "Aftermath",
            "Anticipation",
            "Forewarn",
            "Unaware",
            "Tinted Lens",
            "Filter",
            "Slow Start",
            "Scrappy",
            "Storm Drain",
            "Ice Body",
            "Solid Rock",
            "Snow Warning",
            "Honey Gather",
            "Frisk",
            "Reckless",
            "Multitype",
            "Flower Gift",
            "Bad Dreams",
            "Pickpocket",
            "Sheer Force",
            "Contrary",
            "Unnerve",
            "Defiant",
            "Defeatist",
            "Cursed Body",
            "Healer",
            "Friend Guard",
            "Weak Armor",
            "Heavy Metal",
            "Light Metal",
            "Multiscale",
            "Toxic Boost",
            "Flare Boost",
            "Harvest",
            "Telepathy",
            "Moody",
            "Overcoat",
            "Poison Touch",
            "Regenerator",
            "Big Pecks",
            "Sand Rush",
            "Wonder Skin",
            "Analytic",
            "Illusion",
            "Imposter",
            "Infiltrator",
            "Mummy",
            "Moxie",
            "Justified",
            "Rattled",
            "Magic Bounce",
            "Sap Sipper",
            "Prankster",
            "Sand Force",
            "Iron Barbs",
            "Zen Mode",
            "Victory Star",
            "Turboblaze",
            "Teravolt",
            "Aroma Veil",
            "Flower Veil",
            "Cheek Pouch",
            "Protean",
            "Fur Coat",
            "Magician",
            "Bulletproof",
            "Competitiv",
            "Strong Jaw",
            "Refrigerate",
            "Sweet Veil",
            "Stance Change",
            "Gale Wings",
            "Mega Launcher",
            "Grass Pelt",
            "Symbiosis",
            "Tough Claws",
            "Pixilate",
            "Gooey",
            "Aerilate",
            "Parental Bond",
            "Dark Aura",
            "Fairy Aura",
            "Aura Break",

            "Primordial Sea",
            "Desolate Land",
            "Delta Stream",


            "Cloned",
            "Total Guard",
            "Fish Eye",
            "Equality",
            "Shining Fin",
            "Fog Wall",
            "Fear Generator",
            "Efficiency",
            "Overflow"
        };

        static void Main ( string[ ] args )
        {
            for ( int i = 0; i < abilities.Length; i++ )
                abilities[ i ].write( @"C:\Users\Philip\Projects\pErm2\P-Emerald_2\FSROOT\PKMNDATA\ABILITIES\" + i + ".data" );
            using (StreamWriter sw = new StreamWriter( @"C:\Users\Philip\Desktop\a.h", false, Encoding.GetEncoding( 1252 ), 1 ))
                for ( int i = 0; i < abilityNames.Length; ++i )
                    sw.WriteLine( "#define A_" + abilityNames[ i ].Replace( ' ', '_' ).Replace( '-', '_' ).ToUpper( ) + " " + i );

        }
    }
}