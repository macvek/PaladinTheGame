/*
MIT License

Copyright (c) 2023 Maciej Aleksandrowicz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// SECTION:DEFINES

#define COLOR_RESET     0
#define COLOR_BLACK     30
#define COLOR_RED       31
#define COLOR_GREEN     32
#define COLOR_YELLOW    33
#define COLOR_BLUE      34
#define COLOR_MAGENTA   35
#define COLOR_CYAN      36
#define COLOR_WHITE     37

#define COLOR_BRIGHT_BLACK      90
#define COLOR_BRIGHT_RED        91
#define COLOR_BRIGHT_GREEN      92
#define COLOR_BRIGHT_YELLOW     93
#define COLOR_BRIGHT_BLUE       94
#define COLOR_BRIGHT_MAGENTA    95
#define COLOR_BRIGHT_CYAN       96
#define COLOR_BRIGHT_WHITE      97

#define C_RST       0
#define C_BLACK     30
#define C_RED       31
#define C_GREEN     32
#define C_YELLOW    33
#define C_BLUE      34
#define C_MAGENTA   35
#define C_CYAN      36
#define C_WHITE     37

#define C_BBLACK      90
#define C_BRED        91
#define C_BGREEN      92
#define C_BYELLOW     93
#define C_BBLUE       94
#define C_BMAGENTA    95
#define C_BCYAN       96
#define C_BWHITE      97

#define C_HEAD        C_WHITE
#define C_STORY       C_BBLACK
#define C_MSG         C_GREEN
#define C_MSGHIGH     C_YELLOW
#define C_HINT        C_MAGENTA
#define C_FIGHTHINT   C_CYAN
#define C_HP_FULL     C_WHITE
#define C_HP_OK       C_BGREEN
#define C_HP_AVG      C_YELLOW
#define C_HP_LOW      C_RED
#define C_HP_DEAD     C_BBLACK

#define C_FIGHT_YOU     C_BWHITE
#define C_FIGHT_MSG     C_WHITE
#define C_FIGHT_MSGHIGH C_BRED

#define C_TALK_WELCOME      C_RST
#define C_TALK_OPTION       C_BBLUE
#define C_TALK_EXIT         C_RED
#define C_TALK_BUY          C_BCYAN
#define C_TALK_SELL         C_BBLUE
#define C_TALK_NOBUYSELL    C_BBLACK

#define C_ITEM_ARMOR     C_YELLOW
#define C_ITEM_HEALING   C_GREEN
#define C_ITEM_ITEM       C_BBLACK
#define C_ITEM_KEY       C_BWHITE
#define C_ITEM_WEAPON    C_BRED
#define C_ITEM_QUEST     C_MAGENTA

#define C_MAP_BRDR      C_YELLOW
#define C_MAP_ROOM      C_BBLACK
#define C_MAP_CURSOR    C_BGREEN

#define C_TRACK_HINT        C_CYAN
#define C_TRACK_HIGHHINT    C_YELLOW

#define C_LOC_PRM       C_WHITE
#define C_LOC_SND       C_BBLACK
#define C_LOC_ITEMS     C_BBLUE
#define C_LOC_MONSTERS  C_RED
#define C_LOC_NPCS_TLT  C_BGREEN
#define C_LOC_NPCS      C_GREEN
#define C_LOC_NPCS_DESC C_WHITE
#define C_LOC_BOXES     C_BMAGENTA

#define C_EQUIP         C_BBLUE
#define C_QUEST_TITLE   C_BCYAN
#define C_QUEST_DONE    C_BBLACK

#define C_MONEY C_YELLOW
#define C_DISABLED C_BBLACK

#define END_OF_LIST_VALUE -100

#define CC(N) PASSON(N)
#define ASCOLOR(N)  PASSON(N)
#define PASSON(N) "\x1B["#N"m"

#define SYMBOL_WORLD_PATH '^'
#define SYMBOL_CHURCH_INTERIOR '='
#define SYMBOL_DARKFOREST_PATH '~'
#define SYMBOL_VILLAGE_BUILDING '*'
#define SYMBOL_VILLAGE_PATH '_'
#define SYMBOL_STAIRSUP '<'
#define SYMBOL_STAIRSDOWN '>'
#define SYMBOL_TREASURE '$'
#define SYMBOL_DUNGEONPATH '-'
#define SYMBOL_VILLAGE_DUNGEONPATH '~'
#define SYMBOL_DOOROUT '['
#define SYMBOL_DOORIN ']'
#define SYMBOL_TOWN_BUILDING '%'
#define SYMBOL_TOWN_CHURCH '!'
#define SYMBOL_TOWN_PATH '.'
#define SYMBOL_V 'V'

#define SYMBOL_FARMS_ABANDONED_COTTAGE '"'
#define SYMBOL_FARMS_FIELD '\''

#define SYMBOL_FARMS_COTTAGE ','
#define SYMBOL_FARMS_STABLES ';'
#define SYMBOL_CATACOMBS_PATH '+'
#define SYMBOL_CATACOMBS_TRAP '!'

#define MOVE_NORTH    1 << NORTH
#define MOVE_EAST     1 << EAST
#define MOVE_SOUTH    1 << SOUTH
#define MOVE_WEST     1 << WEST
#define MOVE_TRAVEL   1 << TRAVEL

#define MOVE_NORTH_CLOSED     MOVE_NORTH << 7
#define MOVE_EAST_CLOSED      MOVE_EAST << 7
#define MOVE_SOUTH_CLOSED     MOVE_SOUTH << 7
#define MOVE_WEST_CLOSED      MOVE_WEST << 7
#define MOVE_TRAVEL_CLOSED    MOVE_TRAVEL << 7

#define DrawMapHeight 10
// 1+ // for string terminating character
#define DrawMapWidth  (1+32)
#define DrawMapSize (DrawMapHeight * DrawMapWidth)
#define DRAWMAPOFFSET(x, y) ( y * DrawMapWidth + x )

void printFightMsg(char* msg) {
    printf(CC(C_FIGHT_MSG)"!! %s !!\n"CC(C_RST), msg);
}

void printMsg(char* msg) {
    printf(CC(C_MSG)"~~ %s ~~\n"CC(C_RST), msg);
}

void printMsgHigh(char* msg, char* highArg) {
    printf(CC(C_MSG)"~~ %s "CC(C_MSGHIGH)"%s"CC(C_MSG)" ~~\n"CC(C_RST), msg, highArg);
}

void printMsgDisabled(char* msg) {
    printf(CC(C_DISABLED)"%s\n"CC(C_RST), msg);
}

void printSeparator() {
    printf(CC(C_BBLACK)"-~~~~~-\n"CC(C_RST));
}

void printLine() {
    printf("\n");
}

// SECTION:CONSTS

char* HelpPrompt = "type '?' for help";

const char* DIRECTIONNAMES[] = {
    "NORTH",
    "EAST",
    "SOUTH",
    "WEST",
    "TRAVEL",
};

char* ClosedDoorText = "closed door";

int CAN_BUY = 1 << 0;
int CAN_SELL = 1 << 1;

char* youCanBuy = "You can buy:";
char* youCanSell = "You can sell:";
char* buyDisabledMessage = "There is nothing you can buy.";
char* sellDisabledMessage = "There is nothing you can sell.";

#ifdef WITHDEBUG
char* ratName = "rat";
char* strangerName = "stranger";
char* strangerDescription = "A stranger ... he seems he is willing to talk";
char* talkerName = "talker";
char* talkerDescription = "Talker looks like he has a quest for you";
char* shopperName = "shoper";
char* shopperDescription = "Shoper would like to sell you something";

char* BOTSequence_ShowHelp[] = {
    "echo BOT - ShowHelp",
    "help",
    "echo BOT - End",
    NULL
};

char* BOTSequence_WalkDemo[] = {
    "echo WalkDemo sequence",
    "jmpto DEMO",
    "n","e","e","s","s","n","w","w","w","w","s",
    "echo WalkDemo done",
    NULL
};

char* BOTSequence_WalkMain[] = {
    "echo WalkMain sequence",
    "jmpto W",
    "e","e","e","e","e",
    "s","w","w",
    "s","e","e",                        // at village dungeon
    "t","w","s","w","e","e","n","t",    // back to old well
    "n","e","e","e","s","e","e","n","w",
        "e","e","e","e","s","s",        // at entrance to abandoned temple
    "t","e","e","e","e","e",            // at chest with amulet
    "w","w","w","w","w","t",            // back at entrance to abandoned temple
    "n","n","w","w","w","w",
        "w","w","w","w","w","w","s","w",// at road to farms
    "s","w","s","w","s","n","e","e",    // crossing to town and secret path
    "s","s","s","e","e","e",            // at travel to secret path; but we move back to town
    "w","w","n","n","n",                // back at crossing
    "e","e","e",                        // at town gate
    "e","e","e","s","s","s","w","w","w",
    "n","n","e","e","s","w","w","s",    // town visited; go to church
    "t","e","e",                        // at golem
    "t","s","s","s","s","n","e","e",    // catacombs 1
    "n","n","n","s","e","e","e","e",
    "e","e","e","e","e","e","e","e",
    "e","e","s","s","w","w","w","w",
    "w","w","w","e","s","n",            // getting to stairs to catacombs 2
    "e","e","e","e","e","e","n","n",
    "w","w","w","w","w","w","w","w","w",
    "w","s","s","s",                    // at stairs to catacombs 2
    "t","w","e",    // trap #1
    "n","w","e",    // trap #2
    "n","s",        // trap #3
    "e","n","s",    // trap #4
    "e","n","s",    // trap #5
    "e","w",        // trap #6
    "s","e","w",    // trap #7
    "w","e",        // trap #8
    "s","e","w",    // trap #9
    "s","n",        // trap #10
    "w","w","e",    // trap #11
    "s","w","e",    // trap #12, at stairs to catacombs 3
    "t","e","e","e","e",                // final stage
    "echo getting back to worlds start via secret path",
    "w","w","w","w","t",
    "n","e","n","n","w","w","s","t",
    "n","n","n","w","w","s","s","w","w","n","n","t",
    "w","w","n","n","n","n","n","e","e","n","n","n",
    "w","w","w","w",
    "echo WalkMain done",
    NULL
};

char* BOTSequence_FinalFight[] = {
    "echo FinalFight sequence",
    "jmpto C3",
    "e","e","e",
    "p 1","p 1","p 1","equip 1", "e",
    "echo FinalFight done",
    NULL
};

char* DEMO_MAP[] = {
    "  Nrr",
    "rWxEr",
    "r S r",
    NULL,
};
#endif

/* MAPS */
char* MAP_CHURCH[] = {
    "QqG",
    NULL
};

char* MAP_CATACOMBS_1[] = {
    "A 5            ",
    "a Z$$$$$Z$$$$$$",
    "$ $ $         $",
    "z$$ z  $$$z$$$$",
    "4   B   6      ",
    NULL
};

char* MAP_CATACOMBS_2[] = {
    " YYY ",
    "Y,,,Y",
    "Y:Y,Y",
    " Y,,Y",
    " Y;Y ",
    NULL
};

char* MAP_CATACOMBS_3[] = {
    "[()^&",
    NULL
};

char* MAP_ABANDONED_TEMPLE[] = {
    "Tt123g",
    NULL
};

char* MAP_VILLAGE_DUNGEON[] = {
    " vV",
    "Dvv",
    NULL
};

char* MAP_WORLD[] = {
    "s--e!u        ",
    "   !!h--ddd---",
    "  -b!w  RdC  -",
    " jJ          l",
    "Kkk--O.o.     ",
    "K -  .pXU     ",
    "  -  ..P.     ",
    "  --mMy..     ",
    NULL
};

// SECTION:ENUMS
enum MOVES {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    TRAVEL,

    MOVES_COUNT
};

typedef enum {
    SHOW_INV,
    SHOW_PRICE,
    SHOW_USE
} SHOW_MODE;

enum {
    ITYPE_WEAPON,
    ITYPE_BOMB,
    ITYPE_ARMOR,
    ITYPE_HEALING,
    ITYPE_ITEM,
    ITYPE_KEY,
    ITYPE_QUEST
};

// SECTION:STRUCTS
struct InventoryItemDefinition {
    char* name;
    char* description;
    int value;
    int type;
    int healing;
    int minAttack;
    int maxAttack;
    int armorClass;
    char* saveName;
};

struct InventoryItem {
    struct InventoryItemDefinition* def;
    int count;
};

struct LocationTransfer {
    struct Location* destination;
    int closed;
    int cursorOffset;
    void (*onClosedCallback)(int direction);
};

struct Location {
    char* name;
    char* description;
    char mapMark;
    /* Order is clockwise, starting from north*/
    struct LocationTransfer* moves[MOVES_COUNT];

    int identity;
    int id;

    //loadable
    int visited;
    struct ListItem* inventory;
    struct ListItem* monsters;
    struct ListItem* npcs;
    struct ListItem* boxes;
};

struct ListItem {
    struct ListItem* next;
    struct ListItem* prev;

    void* payload;
};

struct MonsterDefinition {
    char* saveName;
    char* name;
    int maxHp;

    int minAttack;
    int maxAttack;
    int armorClass;
    int hostile;
    int joinsFight;
};

struct Monster {
    struct MonsterDefinition* def;
    int hp;
    int stun;
};

struct NPC {
    int identity;
    char* name;
    char* description;
    void (*onTalk)(struct NPC* this);
    struct ListItem* inventory;
};

enum {
    IDENTITY_BOXCHEST,
    IDENTITY_BOXPALADINSBAG,
    IDENTITY_BOXCATACOMBS,
    IDENTITY_BOXFINAL,
};

struct Box {
    int identity;
    const char* name;
    struct ListItem* inventory;
    void (*onOpen)(struct Box* this);
};


struct TalkOption {
    char* message;
    char* response;
    int identity;
    struct TalkTree* next;
    int (*vis)(struct TalkOption* this);
    struct TalkTree* (*action)(struct TalkOption* this);
};

struct TalkTree {
    char* welcome;
    struct TalkOption* options;
};

struct TalkTree TalkSelf; // empty structure only used as a reference

struct JournalEntry {
    char* title;
    char* description;
    int identity;
    int completed;
};

struct CommandHandler {
    char* testString[2];
    char* desc;
    char exactMatch;
    void (*callback)(char*);
};

// SECTION:GLOBALS
struct ListItem* BOTInput = NULL;
int parseInputRetCode = 1;
char* input = NULL;
size_t inputSize = 80;
int health = 100;
int money = 0;
struct ListItem* inventory = NULL;
struct ListItem* journal = NULL;
struct Location* here;
struct Location* prevHere;

struct NPC* talker;

struct ListItem* enemies = NULL;
struct Monster* enemy;
int fleeDirection;
int itemId = 0;
int DrawMapCursor = 0;
char DrawMap[DrawMapSize];

#ifdef WITHDEBUG
int globalHostile = 1;
#endif

struct {
    // SINGLE ITEM INVENTORIES
    struct ListItem* weapon;
    struct ListItem* armor;

} Equipped;

enum {
    CATQUEST_ENDING_GIRL = 1,
    CATQUEST_ENDING_MOTHER = 2
};

struct {
    int catCorpsePicked;
    int catQuestEnding;
    int secretPathOpened;
    int secretPathOpenedFromCatacomb;
    int priestGaveKey;
    int churchDoorOpened;
    int golemAttacks;
    int evilEnding;
    int goodEnding;
} GameState;


// SECTION:ENGINE interface
struct Location* symbolToLocationIndexed(char symbol);
void defaultOnClosedCallback(int direction);
void globalOnEnterLocation();
void saveGame();
void loadGame();

// SECTION:ENGINE forward for fight mode
void hostilesAttack();
void fightMode(struct Monster* attackIt, int userAttacks);
void attack(int idx);

// SECTION:ENGINE stateless
char* findArg(char* from, int idx) {
    char* iter = from;
    while (idx > 0) {
        if (*iter == 0) {
            return NULL;
        }

        if (*iter == ' ') {
            while (' ' == *(iter + 1)) {
                ++iter;
            }
            --idx;
        }

        iter++;
    }

    if (*iter == 0) {
        return NULL;
    }
    return iter;
}

char* findIntEnd(char* str) {
    char* end;

    for (end = str; *end >= '0' && *end <= '9'; ++end);

    if (end == str) {
        return NULL;
    }

    /* case of '123abc' */
    if (*end != ' ' && *end != 0) {
        return NULL;
    }

    return end;
}

int parseInt(char* str, char* end) {
    int val = 0;
    int mult = 1;

    for (; end-- > str; ) {
        val += mult * ((*end) - '0');
        mult *= 10;
    }

    return val;
}

int readInt(char* str) {
    char* end = findIntEnd(str);
    if (end == NULL) {
        return -1;
    }

    return parseInt(str, end);
}

int argToNumOrFail(char* cmd, int id, char* argNotFound) {
    char* argIdx = findArg(cmd, id);
    if (argIdx) {
        int num = readInt(argIdx);
        if (num < 0) {
            printf("expected non negative value\n");
        }
        return num;
    }
    else {
        printf("%s\n", argNotFound);
        return -1;
    }
}

int startsWithAndSpace(char* prefix) {
    int inputLen = strlen(input);
    int prefixLen = strlen(prefix);

    return 0 == strncmp(prefix, input, prefixLen) && (prefixLen == inputLen || ' ' == input[prefixLen]);
}

// ref: https://en.wikipedia.org/wiki/ANSI_escape_code
void changeColor(int color) {
    //CSI: ESC ]
    //CSI COLORCODE m
    printf("\x1B[%im", color);
}

void cursorMoveTo(int x, int y) {
    //CSI x ; y H
    printf("\x1B[%i;%iH", x, y);
}

void cursorMoveOffset(int x, int y) {
    if (x != 0) {
        printf("\x1B[%i%c", x > 0 ? x : -x, x > 0 ? 'C' : 'D');
    }
    if (y != 0) {
        printf("\x1B[%i%c", y > 0 ? y : -y, y > 0 ? 'B' : 'A');
    }
}

int myStrlen(char* str) {
    // strlen which does not take into account color codes
    char* head = str;
    int l = 0;
    for (;;) {
        if (*head == 0) {
            return l;
        }

        if (*head == '\x1B') {
            for (; *head != 'm' && *head != 0; ++head);
            if (*head == 'm') {
                ++head;
                continue;
            }
            else {
                printf("--> IT SHOULD NEVER HIT 0 on string: %s\n <--", str);
                return 0;
            }
        }
        else {
            ++l;
            ++head;
        }
    }
}

void myStrcpy(char* input, char* cmd) {
#ifdef BUILDONWINDOWS
    strncpy_s(input, inputSize, cmd, inputSize);
#else
    strcpy(input, cmd);
#endif
}

int xyToOffset(int width, int y, int x) {
    return width * y + x;
}

int randOf(int border) {
    return rand() % border;
}

// ENGINE FUNCTIONS - containers
struct ListItem* newListItem(void* payload) {
    struct ListItem* item = (struct ListItem*)malloc(sizeof(struct ListItem));
    memset(item, 0, sizeof(struct ListItem));
    item->payload = payload;
    return item;
}

void freeListItem(struct ListItem* toRemove) {
    free(toRemove);
}

struct ListItem* findPayload(struct ListItem* list, void* payload) {
    for (struct ListItem* head = list; head != NULL; head = head->next) {
        if (head->payload == payload) {
            return head;
        }
    }

    return NULL;
}

void removeFromSequence(struct ListItem* it) {
    if (it->prev) {
        it->prev->next = it->next;
    }

    if (it->next) {
        it->next->prev = it->prev;
    }

    it->prev = NULL;
    it->next = NULL;
}

void linkItemToNext(struct ListItem* prev, struct ListItem* next) {
    prev->next = next;
    next->prev = prev;
}

void placeItemNextToHost(struct ListItem* host, struct ListItem* nextInSequence) {
    removeFromSequence(nextInSequence);
    struct ListItem* oldNext = host->next;
    linkItemToNext(host, nextInSequence);
    if (oldNext) {
        linkItemToNext(nextInSequence, oldNext);
    }
}

struct ListItem* findNthNextItem(struct ListItem* item, int nth, int lastIfNotFound) {
    if (item == NULL) {
        return NULL;
    }

    struct ListItem* each = item;
    int i = 0;
    for (; i < nth && each->next; i++, each = each->next);

    if (i == nth) {
        return each;
    }

    return lastIfNotFound ? each : NULL;
}

struct ListItem* findTail(struct ListItem* list) {
    return findNthNextItem(list, 99999999, 1);
}

void insertToList(struct ListItem** list, void* payload, int idx) {
    --idx; // append item to index-1 item

    struct ListItem* newItem = newListItem(payload);
    if (*list == NULL) {
        *list = newItem;
        return;
    }

    if (idx < 0) { // case of appending 'before' list
        linkItemToNext(newItem, *list);
        *list = newItem;
    }
    else {
        struct ListItem* elementToAppend = findNthNextItem(*list, idx, 1);
        struct ListItem* nextItem = elementToAppend->next;
        linkItemToNext(elementToAppend, newItem);
        if (nextItem != NULL) {
            linkItemToNext(newItem, nextItem);
        }
    }
}

void appendToList(struct ListItem** list, void* payload) {
    insertToList(list, payload, 99999999);
}

void removeAllFromList(struct ListItem** list) {
    struct ListItem* next = *list;
    *list = 0;

    for (;;) {
        struct ListItem* this = next;
        if (!this) {
            break;
        }
        next = this->next;
        freeListItem(this);
    }
}

void removeFromList(struct ListItem** list, void* payload) {
    if (*list == NULL) {
        printf("!! ERR: trying to remove from empty list !!\n");
        exit(1);
        return;
    }

    struct ListItem* toRemove = findPayload(*list, payload);
    if (toRemove == NULL) {
        printf("!! ERR: trying to remove content from list which is not found !!\n");
        return;
    }

    if (*list == toRemove) {
        struct ListItem* listCandidate = toRemove->next;
        removeFromSequence(toRemove);
        *list = listCandidate;
    }
    else {
        removeFromSequence(toRemove);
    }

    freeListItem(toRemove);
}

// SECTION:ENGINE constructors/destructors
struct Monster* newMonster(struct MonsterDefinition* def) {
    struct Monster* monster = malloc(sizeof(struct Monster));
    memset(monster, 0, sizeof(struct Monster));
    monster->def = def;
    monster->hp = def->maxHp;
    return monster;
}

void freeMonster(struct Monster* toRemove) {
    free(toRemove);
}

struct LocationTransfer* newLocationTransfer(struct Location* dest, int cursorOffset) {
    struct LocationTransfer* ret = malloc(sizeof(struct LocationTransfer));
    ret->destination = dest;
    ret->closed = 0;
    ret->cursorOffset = cursorOffset;
    ret->onClosedCallback = &defaultOnClosedCallback;
    return ret;
}

void freeLocationTransfer(struct LocationTransfer* a) {
    free(a);
}

void locationInitWithName(struct Location* what, char* name, char* description, char mapMark) {
    memset(what, 0, sizeof(struct Location));
    what->name = name;
    what->description = description;
    what->mapMark = mapMark;
    what->id = -1;
}

struct Location* newLocation(char* name, char* description, char mapMark) {
    struct Location* ret = malloc(sizeof(struct Location));
    memset(ret, 0, sizeof(struct Location));
    ret->name = name;
    ret->description = description;
    ret->mapMark = mapMark;
    ret->id = -1;

    return ret;
}

struct LocationTransfer* asLocationTransfer(struct Location* destination, int cursorOffset) {
    if (destination) {
        return newLocationTransfer(destination, cursorOffset);
    }
    else {
        return NULL;
    }
}

struct InventoryItem* newInventoryItemCount(struct InventoryItemDefinition* def, int count) {
    struct InventoryItem* item = malloc(sizeof(struct InventoryItem));
    item->def = def;
    item->count = count;
    return item;
}

struct InventoryItem* newInventoryItem(struct InventoryItemDefinition* def) {
    return newInventoryItemCount(def, 1);
}

void freeInventoryItem(struct InventoryItem* toRemove) {
    free(toRemove);
}

void initNPC(int identity, struct NPC* npc, char* name, char* description, void (*onTalkCallback)(struct NPC* this), struct Location* where) {
    npc->identity = identity;
    npc->name = name;
    npc->description = description;
    npc->onTalk = onTalkCallback;
    appendToList(&where->npcs, npc);
}

struct Box* newBox(int identity, char* name, void (*onOpenCallback)(struct Box*)) {
    struct Box* box = malloc(sizeof(struct Box));
    memset(box, 0, sizeof(struct Box));
    box->identity = identity;
    box->name = name;
    box->onOpen = onOpenCallback;

    return box;
}

// ENGINE FUNCTIONS - input handling
int inputIs(char* it) {
    return 0 == strcmp(it, input);
}

int matchCommand(struct CommandHandler* cmd) {
    for (int i = 0; i < 2; i++) {
        if (cmd->testString[i] != NULL && (
            (1 == cmd->exactMatch && inputIs(cmd->testString[i])) ||
            (0 == cmd->exactMatch && startsWithAndSpace(cmd->testString[i]))
            )) {
            return 1;
        }
    }

    return 0;
}

int parseInputAsCommands(struct CommandHandler* commandList) {
    for (int i = 0; commandList[i].exactMatch != END_OF_LIST_VALUE; i++) {
        struct CommandHandler* cmd = &commandList[i];
        if (matchCommand(cmd)) {
            cmd->callback(input);
            return 1;
        }
    }

    return 0;
}

void fillInput() {
    if (BOTInput) {
        char* cmd = BOTInput->payload;
        removeFromList(&BOTInput, cmd);
        int len = strlen(cmd);
        if (len + 1 > inputSize) {
            printf("ERROR!!, found too long cmd in bot payload: %s, aborting", cmd);
            exit(1);
        }

        printf("BOT: %s\n", cmd);
        myStrcpy(input, cmd);
        input[inputSize - 1] = 0;
        return;
    }

    for (;;) {
        char* ret = fgets(input, inputSize, stdin);
        if (!ret) {
            printf("\n!!!\nFound empty line; aborting with error\n");
            exit(1);
        }
        // mark end of line as 0
        for (int i = 0; i < inputSize; ++i, ++ret) {
            if (*ret == '\n') {
                *ret = 0;
            }
        }
        return;
    }
}

// SECTION:ENGINE moving
int moveTest() {
    int options = 0;
    if (NULL != here->moves[NORTH]) {
        options |= here->moves[NORTH]->closed ? MOVE_NORTH_CLOSED : MOVE_NORTH;
    }
    if (NULL != here->moves[EAST]) {
        options |= here->moves[EAST]->closed ? MOVE_EAST_CLOSED : MOVE_EAST;
    }
    if (NULL != here->moves[SOUTH]) {
        options |= here->moves[SOUTH]->closed ? MOVE_SOUTH_CLOSED : MOVE_SOUTH;
    }
    if (NULL != here->moves[WEST]) {
        options |= here->moves[WEST]->closed ? MOVE_WEST_CLOSED : MOVE_WEST;
    }
    if (NULL != here->moves[TRAVEL]) {
        options |= here->moves[TRAVEL]->closed ? MOVE_TRAVEL_CLOSED : MOVE_TRAVEL;
    }

    return options;
}

int decodeDirection(char* dir) {
    if (0 == strcmp("n", dir) || 0 == strcmp("north", dir)) return NORTH;
    if (0 == strcmp("e", dir) || 0 == strcmp("east", dir)) return EAST;
    if (0 == strcmp("s", dir) || 0 == strcmp("south", dir)) return SOUTH;
    if (0 == strcmp("w", dir) || 0 == strcmp("west", dir)) return WEST;
    if (0 == strcmp("t", dir) || 0 == strcmp("travel", dir)) return TRAVEL;
    return -1;
}

int reverseDirection(int dir) {
    if (NORTH == dir) return SOUTH;
    if (EAST == dir) return WEST;
    if (SOUTH == dir) return NORTH;
    if (WEST == dir) return EAST;
    if (TRAVEL == dir) return TRAVEL;

    printf("PANIC!!: got location %i to decode, aborting\n", dir);
    exit(1);
}

// SECTION:ENGINE map
void cursorMap(int offset, char mapMark) {
    DrawMapCursor += offset;
    if (DrawMapCursor < 0 || DrawMapCursor >= DrawMapSize) {
        // Happens in case of debugging using jumps
        DrawMapCursor = 0;
    }
    DrawMap[DrawMapCursor] = mapMark;
}

void initDrawMap() {
    memset(DrawMap, ' ', DrawMapSize);
    for (int i = 1; i <= DrawMapHeight; i++) {
        int lineEndOffset = i * DrawMapWidth - 1;
        DrawMap[lineEndOffset] = 0;     // inject string terminators after every line
    }
}

void showMap() {
    cursorMoveOffset(100 - DrawMapWidth, -DrawMapHeight - 1);
    printf(CC(C_MAP_BRDR)"/--------------------------------\\");
    int moveBack = -1 - DrawMapWidth;
    cursorMoveOffset(moveBack, 1);

    int lineWithCursor = DrawMapCursor / DrawMapWidth;

    for (int i = 0; i < DrawMapHeight; i++) {
        int lineStartOffset = i * DrawMapWidth;
        char* linePtr = &DrawMap[lineStartOffset];
        changeColor(COLOR_BRIGHT_BLACK);
        if (i == lineWithCursor) {
            char bg = DrawMap[DrawMapCursor];
            DrawMap[DrawMapCursor] = 0; // terminate at cursor position to change color
            printf(CC(C_MAP_BRDR)"|"CC(C_MAP_ROOM)"%s", linePtr);
            printf(CC(C_MAP_CURSOR)"@"CC(C_MAP_ROOM));

            DrawMap[DrawMapCursor] = bg;
            linePtr = &DrawMap[DrawMapCursor + 1];
            printf("%s"CC(C_MAP_BRDR)"|", linePtr);
        }
        else {
            printf(CC(C_MAP_BRDR)"|"CC(C_MAP_ROOM)"%s"CC(C_MAP_BRDR)"|", linePtr);
        }
        cursorMoveOffset(moveBack, 1);
    }
    printf(CC(C_MAP_BRDR)"\\--------------------------------/\n"CC(C_RST));
}

void walkMap(char* sourceMap[], int width, int height) {
    struct Location** cache = malloc(sizeof(struct Location*) * width * height);

    memset(cache, 0, sizeof(struct Location*) * width * height);

    // Initialize every item and fill cache with pointers
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            cache[xyToOffset(width, y, x)] = symbolToLocationIndexed(sourceMap[y][x]);
        }

    // Add joins between locations
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            struct Location* each = cache[xyToOffset(width, y, x)];
            if (each == NULL) {
                continue;
            }

            if (y - 1 >= 0)       each->moves[NORTH] = asLocationTransfer(cache[xyToOffset(width, y - 1, x)], -DrawMapWidth);
            if (x + 1 < width)    each->moves[EAST] = asLocationTransfer(cache[xyToOffset(width, y, x + 1)], +1);
            if (y + 1 < height)   each->moves[SOUTH] = asLocationTransfer(cache[xyToOffset(width, y + 1, x)], +DrawMapWidth);
            if (x - 1 >= 0)       each->moves[WEST] = asLocationTransfer(cache[xyToOffset(width, y, x - 1)], -1);
        }

    free(cache);
}

void initMap(char* sourceMap[]) {
    int height = 0;
    int width = strlen(sourceMap[height]);
    for (;;) {
        if (!sourceMap[height]) {
            break;
        }

        int w = strlen(sourceMap[height]);

        if (w != width) {
            printf("FATAL ERROR; mapToInit is not aligned with width:%i on line: %i\n", width, height);
            exit(1);
            return;
        }

        ++height;
    }

    walkMap(sourceMap, width, height);
}

void removeTravelFromLocation(struct Location* a) {
    if (a->moves[TRAVEL]) {
        freeLocationTransfer(a->moves[TRAVEL]);
        a->moves[TRAVEL] = NULL;
    }
}

void joinLocationOneWay(struct Location* a, struct Location* b, int cursorOffset) {
    a->moves[TRAVEL] = asLocationTransfer(b, cursorOffset);
}

void joinLocations(struct Location* a, struct Location* b, int cursorOffset) {
    joinLocationOneWay(a, b, cursorOffset);
    joinLocationOneWay(b, a, -cursorOffset);
}

void unjoinLocations(struct Location* a, int from, int to) {
    if (!a->moves[from]) {
        printf("FATAL, cannot break such join from %s\n", a->name);
        exit(1);
    }

    struct Location* b = a->moves[from]->destination;
    free(a->moves[from]);
    free(b->moves[to]);
    a->moves[from] = NULL;
    b->moves[to] = NULL;
}

// SECTION:ENGINE inventory
struct ListItem* numItemFromInventory(int num, struct ListItem* givenInventory, char* notFoundMessage) {
    if (num == 0) {
        printMsg("0 won't pass here...");
        return NULL;
    }
    struct ListItem* found = findNthNextItem(givenInventory, num - 1, 0);
    if (!found) {
        printMsg(notFoundMessage);
        return NULL;
    }
    return found;
}

struct InventoryItem* findInventoryItemLike(struct ListItem* list, struct InventoryItemDefinition* itemLike) {
    for (struct ListItem* head = list; head != NULL; head = head->next) {
        struct InventoryItem* item = head->payload;
        if (item->def == itemLike) {
            return item;
        }
    }

    return NULL;
}

struct InventoryItem* findInventoryIndexedItem(struct ListItem* inventory, int nth) {
    struct ListItem* item = findNthNextItem(inventory, nth - 1, 0);
    if (item == NULL) {
        printMsg("Cannot find such an item");
        return NULL;
    }
    else {
        return (struct InventoryItem*)item->payload;
    }
}

void removeItemFromInventory(int idx) {
    if (inventory == NULL) {
        printf("Cannot remove element from an empty inventory\n");
        return;
    }
    struct ListItem* item = findNthNextItem(inventory, idx, 1);

    struct InventoryItem* payload = item->payload;
    if (payload->count > 1) {
        --payload->count;
    }
    else {
        removeFromList(&inventory, payload);
        free(payload);
    }
}

void transferItemOrCountAll(struct ListItem** from, struct ListItem** to, struct InventoryItem* payload, int countAll) {
    int releaseItem = 0;
    int transferCount = countAll ? payload->count : 1;

    if (payload->count == 1 || countAll) {
        removeFromList(from, payload);
        releaseItem = 1;
    }
    else {
        --payload->count;
    }

    struct InventoryItem* foundItem = findInventoryItemLike(*to, payload->def);
    if (foundItem) {
        foundItem->count += transferCount;
        if (releaseItem) {
            freeInventoryItem(payload);
            payload = NULL;
        }
    }
    else {
        if (releaseItem) {
            appendToList(to, payload);
        }
        else {
            appendToList(to, newInventoryItem(payload->def));
        }
    }
}

void transferItem(struct ListItem** from, struct ListItem** to, struct InventoryItem* payload) {
    transferItemOrCountAll(from, to, payload, 0);
}

struct InventoryItem* FindInventoryEntry(struct InventoryItemDefinition* identity) {
    for (struct ListItem* head = inventory; head != NULL; head = head->next) {
        struct InventoryItem* payload = head->payload;
        if (payload->def == identity) {
            return payload;
        }
    }

    return NULL;
}

// SECTION:ENGINE journal
struct JournalEntry* findJournalEntry(int identity) {
    for (struct ListItem* head = journal; head != NULL; head = head->next) {
        struct JournalEntry* payload = head->payload;
        if (payload->identity == identity) {
            return payload;
        }
    }

    return NULL;
}

void addQuest(struct JournalEntry* entry) {
    printMsgHigh("Journal entry added:", entry->title);
    appendToList(&journal, entry);
}

void completeQuest(struct JournalEntry* entry) {
    entry->completed = 1;
    printMsgHigh("Quest completed:", entry->title);
}

// SECTION:ENGINE prints
void printMoves() {
    int options = moveTest();
    printf(CC(C_HINT)"( ");
    if (options & MOVE_NORTH) printf("n ");
    if (options & MOVE_NORTH_CLOSED) printf("(n) ");
    if (options & MOVE_EAST) printf("e ");
    if (options & MOVE_EAST_CLOSED) printf("(e) ");
    if (options & MOVE_SOUTH) printf("s ");
    if (options & MOVE_SOUTH_CLOSED) printf("(s) ");
    if (options & MOVE_WEST) printf("w ");
    if (options & MOVE_WEST_CLOSED) printf("(w) ");
    if (options & MOVE_TRAVEL) printf("t ");
    if (options & MOVE_TRAVEL_CLOSED) printf("(t) ");

    printf(")");
    changeColor(C_RST);
}

void printHealth() {
    if (health == 100) {
        changeColor(C_HP_FULL);
    }
    else if (health > 50) {
        changeColor(C_HP_OK);
    }
    else if (health > 30) {
        changeColor(C_HP_AVG);
    }
    else if (health > 0) {
        changeColor(C_HP_LOW);
    }
    else {
        changeColor(C_HP_DEAD);
    }
    printf("HP:%i "CC(C_RST), health);
}

void printFightMoves() {
    if (enemies->next) {
        printf(CC(C_FIGHTHINT)"( a u "CC(C_HINT)"t"CC(C_FIGHTHINT)" f )"CC(C_RST));
    }
    else {
        printf(CC(C_FIGHTHINT)"( a u f )"CC(C_RST));
    }
}

// SECTION: game logic //
void talkMode(struct TalkTree* talkTree) {
    struct TalkTree* currentTree = talkTree;

    while (currentTree) {
        printf(CC(C_TALK_WELCOME)"%s\n"CC(C_RST), currentTree->welcome);
        int count = 0;
        for (struct TalkOption* options = currentTree->options; options->message != NULL; options++) {
            if (options->vis == NULL || options->vis(options)) {
                printf(CC(C_TALK_OPTION)"#%i: %s\n"CC(C_RST), ++count, options->message);
            }
        }

        printf(CC(C_RST));

        int num;
        for (;;) {
            printf("> ");
            fillInput();

            num = readInt(input);
            if (num < 0) {
                printf("please provide a valid number\n");
                continue;
            }

            if (num > count) {
                printf("There are only %i options\n", count);
                continue;
            }

            if (num == 0) {
                printf("There is no option 0\n");
                continue;
            }

            break;
        }

        struct TalkOption* picked = NULL;
        int i = 1;
        for (struct TalkOption* options = currentTree->options; options->message != NULL; options++) {
            if (options->vis == NULL || options->vis(options)) {
                if (num == i) {
                    picked = options;
                    break;
                }

                ++i;
            }
        }

        if (picked == NULL) {
            printf("!! FATAL ERROR; no option picked\n");
            exit(1);
            return;
        }

        if (picked->response != NULL) {
            printf("%s\n", picked->response);
        }

        struct TalkTree* nextTree;
        if (picked->action != NULL) {
            nextTree = picked->action(picked);
        }
        else {
            nextTree = picked->next;
        }

        if (nextTree == &TalkSelf) {
            nextTree = currentTree;
        }

        currentTree = nextTree;
        if (currentTree)

            printSeparator();
    }
}

void shopPrompt(int flags) {
    printf(CC(C_TALK_EXIT) "#0 Exit\n");

    if (flags & CAN_BUY) {
        changeColor(C_TALK_BUY);
        printf("#1 ");
    }
    else {
        changeColor(C_TALK_NOBUYSELL);
        printf("#- ");
    }
    printf("Buy\n");

    if (flags & CAN_SELL) {
        changeColor(C_TALK_SELL);
        printf("#2 ");
    }
    else {
        changeColor(C_TALK_NOBUYSELL);
        printf("#- ");
    }

    printf("Sell\n");

    changeColor(C_RST);
    printf("#?> ");
}

void showMoneyStatus() {
    printf(CC(C_MONEY)"~~~~~~  Money: %ig  ~~~~~~\n\n"CC(C_RST), money);
}

void indexedListInventoryWithMode(struct ListItem* invList, SHOW_MODE showMode) {
    int maxLength = 0;
    for (struct ListItem* head = invList; head != NULL; head = head->next) {
        struct InventoryItem* item = head->payload;
        int len = myStrlen(item->def->name);
        maxLength = maxLength < len ? len : maxLength;
    }

    int tabSize = maxLength + 4;
    int idx = 1;
    for (struct ListItem* head = invList; head != NULL; head = head->next, ++idx) {
        struct InventoryItem* item = head->payload;

        int leadColor = C_WHITE;
        int commonColor = C_RST;
        if (showMode == SHOW_USE) {
            if (item->def->type == ITYPE_WEAPON || item->def->type == ITYPE_ARMOR) {
                leadColor = COLOR_BRIGHT_BLUE;
            }
            else if (item->def->type == ITYPE_BOMB) {
                leadColor = COLOR_YELLOW;
            }
            else if (item->def->type == ITYPE_HEALING) {
                leadColor = COLOR_BRIGHT_GREEN;
            }

            commonColor = leadColor;
        }
        else {
            if (item->def->type == ITYPE_ARMOR) {
                leadColor = C_ITEM_ARMOR;
            }
            else if (item->def->type == ITYPE_HEALING) {
                leadColor = C_ITEM_HEALING;
            }
            else if (item->def->type == ITYPE_ITEM) {
                leadColor = C_ITEM_ITEM;
            }
            else if (item->def->type == ITYPE_KEY) {
                leadColor = C_ITEM_KEY;
            }
            else if (item->def->type == ITYPE_WEAPON || item->def->type == ITYPE_BOMB) {
                leadColor = C_ITEM_WEAPON;
            }
            else if (item->def->type == ITYPE_QUEST) {
                leadColor = C_ITEM_QUEST;
            }
        }

        int len = tabSize - myStrlen(item->def->name);
        changeColor(commonColor);
        printf("#%i ", idx);

        changeColor(leadColor);
        if (item->def->type == ITYPE_ARMOR) {
            printf("[ ARMOR] ");
        }
        else if (item->def->type == ITYPE_HEALING) {
            printf("[POTION] ");
        }
        else if (item->def->type == ITYPE_ITEM) {
            printf("[ ITEM ] ");
        }
        else if (item->def->type == ITYPE_KEY) {
            printf("[ KEY  ] ");
        }
        else if (item->def->type == ITYPE_WEAPON) {
            printf("[WEAPON] ");
        }
        else if (item->def->type == ITYPE_BOMB) {
            printf("[ BOMB ] ");
        }
        else if (item->def->type == ITYPE_QUEST) {
            printf("[ QUEST] ");
        }
        else {
            printf("!! ERROR !!, unknown item type %i, for item %s, aborting\n", item->def->type, item->def->name);
            exit(1);
        }
        changeColor(commonColor);

        printf("%s", item->def->name);

        for (int i = 0; i < len; i++) {
            printf(" ");
        }

        if (showMode == SHOW_PRICE) {
            if (item->def->value == 0) {
                printf("%i "CC(C_ITEM_QUEST)"( quest item ) "CC(C_RST)"\n", item->count);
            }
            else {
                printf("%i "CC(C_MONEY)"( %ig ) "CC(C_RST) "\n", item->count, item->def->value);
            }
        }
        else {
            printf("%i\n", item->count);
        }

        printf(CC(C_DISABLED)"  %s\n"CC(C_RST), item->def->description);
    }
    changeColor(C_RST);
}

void indexedListInventory(struct ListItem* invList) {
    indexedListInventoryWithMode(invList, SHOW_INV);
}

void shopBuyMode(struct NPC* npc) {
    for (;;) {
        printLine();
        showMoneyStatus();
        printMsg(youCanBuy);
        printf(CC(C_TALK_EXIT)"#0 - back\n"CC(C_RST));
        indexedListInventoryWithMode(npc->inventory, SHOW_PRICE);
        printf("buy #?> ");
        fillInput();
        int num = readInt(input);
        if (num <= 0) {
            return;
        }

        struct InventoryItem* itemPayload = findInventoryIndexedItem(npc->inventory, num);
        if (itemPayload == NULL) {
            continue;
        }

        if (itemPayload->def->value == 0) {
            printMsg("This is a quest item, you cannot buy it. There must be some other way to obtain it.");
        }
        else if (itemPayload->def->value > money) {
            printf(CC(C_MSG)"~~ You cannot afford to buy "CC(C_MSGHIGH)"%s"CC(C_MSG)" for "CC(C_MSGHIGH)"%ig "CC(C_MSG)"~~\n"CC(C_RST),
                itemPayload->def->name,
                itemPayload->def->value);
        }
        else {
            printf(CC(C_MSG)"~~ You bought "CC(C_MSGHIGH)"%s"CC(C_MSG)" for "CC(C_MSGHIGH)"%ig "CC(C_MSG)"~~\n"CC(C_RST),
                itemPayload->def->name,
                itemPayload->def->value);
            money -= itemPayload->def->value;
            transferItem(&npc->inventory, &inventory, itemPayload);
        }
    }
}

void shopSellMode(struct NPC* npc) {
    for (;;) {
        printLine();
        showMoneyStatus();
        printMsg(youCanSell);
        printf(CC(C_TALK_EXIT)"#0 - back\n"CC(C_RST));
        indexedListInventoryWithMode(inventory, SHOW_PRICE);
        printf("sell #?> ");
        fillInput();
        int num = readInt(input);
        if (num <= 0) {
            return;
        }

        struct InventoryItem* itemPayload = findInventoryIndexedItem(inventory, num);
        if (itemPayload == NULL) {
            continue;
        }

        if (itemPayload->def->value == 0) {
            printMsg("This is a quest item, you cannot sell it.");
        }
        else {
            printf(CC(C_MSG)"~~ You sold "CC(C_MSGHIGH)"%s"CC(C_MSG)" for "CC(C_MSGHIGH)"%ig "CC(C_MSG)"~~\n"CC(C_RST),
                itemPayload->def->name,
                itemPayload->def->value);

            money += itemPayload->def->value;
            transferItem(&inventory, &npc->inventory, itemPayload);
        }
    }
}

void shopMode(struct NPC* this) {
    for (;;) {
        printMsgHigh("You trade with", this->name);
        showMoneyStatus();

        int flags = 0;
        if (this->inventory) {
            printMsg(youCanBuy);
            indexedListInventoryWithMode(this->inventory, SHOW_PRICE);
            flags |= CAN_BUY;
        }
        else {
            printMsgDisabled(buyDisabledMessage);
        }
        printLine();

        if (inventory) {
            printMsg(youCanSell);
            indexedListInventoryWithMode(inventory, SHOW_PRICE);
            flags |= CAN_SELL;
        }
        else {
            printMsgDisabled(sellDisabledMessage);
        }
        printLine();

        for (;;) {
            shopPrompt(flags);
            fillInput();

            int num = readInt(input);
            if (num == 0) {
                return;
            }
            else if (num == 1) {
                if (flags & CAN_BUY) {
                    shopBuyMode(this);
                    break;
                }
                else {
                    printMsg(buyDisabledMessage);
                }
            }
            else if (num == 2) {
                if (flags & CAN_SELL) {
                    shopSellMode(this);
                    break;
                }
                else {
                    printMsg(sellDisabledMessage);
                }
            }
            else {
                printf("Please pick valid number to proceed\n");
            }
        }
    }
}

void unequipItem(int itemType) {
    struct ListItem** equipSlot;

    if (itemType == ITYPE_ARMOR) {
        equipSlot = &Equipped.armor;
    }
    else if (itemType == ITYPE_WEAPON) {
        equipSlot = &Equipped.weapon;
    }
    else {
        printf("FATAL ERROR, itemtype not supported, aborting\n");
        exit(1);
    }

    if (*equipSlot != NULL) {
        transferItem(equipSlot, &inventory, (*equipSlot)->payload);
        if (itemType == ITYPE_ARMOR) {
            printMsg("You no longer wear any armor");
        }
        else {
            printMsg("You no longer wield any weapon");
        }
    }
    else {
        if (itemType == ITYPE_ARMOR) {
            printMsg("You don't wear any armor");
        }
        else {
            printMsg("You don't wield any weapon");
        }
    }
}

void drinkItem(int idx) {
    struct InventoryItem* item = findInventoryIndexedItem(inventory, idx);
    if (!item) {
        return;
    }

    if (item->def->type != ITYPE_HEALING) {
        printMsgHigh("You cannot drink", item->def->name);
        return;
    }

    printf(CC(C_MSG)"~~ You drink "CC(C_MSGHIGH)"%s"CC(C_MSG)" and regain "CC(C_MSGHIGH)"%i"CC(C_MSG)" hp ~~\n", item->def->name, item->def->healing);

    health += item->def->healing;
    if (health > 100) {
        health = 100;
    }

    struct ListItem* removeMe = NULL;
    transferItem(&inventory, &removeMe, item);

    if (removeMe) {
        freeListItem(removeMe);
    }
}

void equipItem(int idx) {
    struct InventoryItem* item = findInventoryIndexedItem(inventory, idx);
    if (!item) {
        return;
    }

    struct ListItem** equipSlot;

    if (item->def->type == ITYPE_ARMOR) {
        printMsgHigh("You wear", item->def->name);
        equipSlot = &Equipped.armor;
    }
    else if (item->def->type == ITYPE_WEAPON) {
        printMsgHigh("You wield", item->def->name);
        equipSlot = &Equipped.weapon;
    }
    else {
        printMsg("You cannot equip this item");
        return;
    }

    if (*equipSlot != NULL) {
        transferItem(equipSlot, &inventory, (*equipSlot)->payload);
    }
    transferItem(&inventory, equipSlot, item);
}

enum {
    QuestMain = 0,
    QuestMissingCat,
    QuestMissingPaladinInDarkForest,
    QuestReleaseGhost,
    QuestFarmerRatsProblem,
};

struct InventoryItemDefinition ITEMD_WEAPON_BareFists = {
    .name = "bare fists",
    .description = "",
    .value = 0,
    .type = ITYPE_WEAPON,
    .minAttack = 0,
    .maxAttack = 1
};

struct InventoryItemDefinition ITEMD_WEAPON_SwordLvl1 = {
    .saveName = "ITEMD_WEAPON_SwordLVL1",
    .name = "Paladin's sword",
    .description = "Hit[2-7] Basic Sword of the Order.",
    .value = 20,
    .type = ITYPE_WEAPON,
    .minAttack = 2,
    .maxAttack = 7
};

struct InventoryItemDefinition ITEMD_WEAPON_SwordLvl2 = {
    .saveName = "ITEMD_WEAPON_SwordLVL2",
    .name = "Long sword",
    .description = "Hit[4-12] Made of hard steel. Very sharp.",
    .value = 150,
    .type = ITYPE_WEAPON,
    .minAttack = 4,
    .maxAttack = 12
};

struct InventoryItemDefinition ITEMD_WEAPON_PaladinsDarkSword = {
    .saveName = "ITEMD_WEAPON_PaladinsDarkSword",
    .name = "Paladin's Dark Sword",
    .description = "Hit[5-13] Dark sword which you found in a dark forest.",
    .value = 100,
    .type = ITYPE_WEAPON,
    .minAttack = 5,
    .maxAttack = 13
};

struct InventoryItemDefinition ITEMD_WEAPON_SwordLvl3 = {
    .saveName = "ITEMD_WEAPON_SwordLVL3",
    .name = "Well balanced Long Sword",
    .description = "Hit[7-20] This blade is so well balanced that it can cause serious damage.",
    .value = 900,
    .type = ITYPE_WEAPON,
    .minAttack = 7,
    .maxAttack = 20
};

struct InventoryItemDefinition ITEMD_WEAPON_SwordLvl4 = {
    .saveName = "ITEMD_WEAPON_SwordLVL4",
    .name = "Master's sword",
    .description = "Hit[10-22] Blade for a master swordsman. It is fast and powerful.",
    .value = 2500,
    .type = ITYPE_WEAPON,
    .minAttack = 10,
    .maxAttack = 22
};


struct InventoryItemDefinition ITEMD_ARMOR_ArmorLvl1 = {
    .saveName = "ITEMD_ARMOR_ArmorLVL1",
    .name = "Leather armor",
    .description = "AC[1]",
    .value = 25,
    .type = ITYPE_ARMOR,
    .armorClass = 1,
};

struct InventoryItemDefinition ITEMD_ARMOR_ArmorLvl2 = {
    .saveName = "ITEMD_ARMOR_ArmorLVL2",
    .name = "Chainmail",
    .description = "AC[3]",
    .value = 200,
    .type = ITYPE_ARMOR,
    .armorClass = 3,
};


struct InventoryItemDefinition ITEMD_ARMOR_ArmorLvl3 = {
    .saveName = "ITEMD_ARMOR_ArmorLVL3",
    .name = "Plate armor",
    .description = "AC[6]",
    .value = 800,
    .type = ITYPE_ARMOR,
    .armorClass = 6,
};

struct InventoryItemDefinition ITEMD_ARMOR_ArmorLvl4 = {
    .saveName = "ITEMD_ARMOR_ArmorLVL4",
    .name = "Fullplate armor",
    .description = "AC[8]",
    .value = 2100,
    .type = ITYPE_ARMOR,
    .armorClass = 8,
};


struct InventoryItemDefinition ITEMD_WEAPON_SilverSword = {
    .saveName = "ITEMD_WEAPON_SilverSword",
    .name = "Silver sword",
    .description = "Does not look sharp but has some special abilities.",
    .value = 1000,
    .type = ITYPE_WEAPON,
    .minAttack = 2,
    .maxAttack = 4
};

struct InventoryItemDefinition ITEMD_WEAPON_FakeSilverSword = {
    .saveName = "ITEMD_WEAPON_FakeSilverSword",
    .name = "Silver sword",
    .description = "Shines like silver",
    .value = 199,
    .type = ITYPE_WEAPON,
    .minAttack = 1,
    .maxAttack = 1
};

struct InventoryItemDefinition ITEMD_WEAPON_WoodenPeg = {
    .saveName = "ITEMD_WEAPON_WoodenPeg",
    .name = "Wooden peg",
    .description = "Just a wooden peg. Hit with the pointy end",
    .value = 5,
    .type = ITYPE_WEAPON,
    .minAttack = 1,
    .maxAttack = 1
};

struct InventoryItemDefinition ITEMD_KEY_KeyToChurch = {
    .saveName = "ITEMD_KEY_KeyToChurch",
    .name = "Key to church",
    .description = "Church is locked. But you have a key.",
    .value = 0,
    .type = ITYPE_KEY,
};

struct InventoryItemDefinition ITEMD_QUEST_CatCorpse = {
    .saveName = "ITEMD_QUEST_CatCorpse",
    .name = "Cat corpse",
    .description = "This one hasn't managed to land on four legs",
    .value = 0,
    .type = ITYPE_QUEST,
};

struct InventoryItemDefinition ITEMD_QUEST_WoodenRing = {
    .saveName = "ITEMD_QUEST_WoodenRing",
    .name = "Wooden ring",
    .description = "A gift of gratitude you received for finding kid's missing cat",
    .value = 10,
    .type = ITYPE_ITEM,
};

struct InventoryItemDefinition ITEMD_HEALING_Ale = {
    .saveName = "ITEMD_HEALING_Ale",
    .name = "Mug of ale",
    .description = "HP[+1] Mug of best ale.",
    .value = 1,
    .type = ITYPE_HEALING,
    .healing = 1
};

struct InventoryItemDefinition ITEMD_HEALING_Wine = {
    .saveName = "ITEMD_HEALING_Wine",
    .name = "Bottle of wine",
    .description = "HP[+3] Bottle of wine.",
    .value = 3,
    .type = ITYPE_HEALING,
    .healing = 3
};

struct InventoryItemDefinition ITEMD_HEALING_Lagger = {
    .saveName = "ITEMD_HEALING_Lagger",
    .name = "Mug of lagger",
    .description = "HP[+1] Mug of lagger.",
    .value = 1,
    .type = ITYPE_HEALING,
    .healing = 1
};

struct InventoryItemDefinition ITEMD_HEALING_HealingPotion = {
    .saveName = "ITEMD_HEALING_HealingPotion",
    .name = "Healing potion",
    .description = "HP[+25] Looks like wine, tastes like wine, it is definitely healing potion.",
    .value = 10,
    .type = ITYPE_HEALING,
    .healing = 25
};

struct InventoryItemDefinition ITEMD_BOMB_SparklingBooze = {
    .saveName = "ITEMD_BOMB_SparklingBooze",
    .name = "Sparkling booze",
    .description = "Hits every enemy[10-20]. Strong alcohol. It is flammable.",
    .value = 45,
    .type = ITYPE_BOMB,
    .minAttack = 10,
    .maxAttack = 20
};

struct InventoryItemDefinition ITEMD_BOMB_FlameLizard = {
    .saveName = "ITEMD_BOMB_FlameLizard",
    .name = "Flame lizard",
    .description = "Hits every enemy[20-40]. You feel heat coming from a bottle. It is very explosive.",
    .value = 100,
    .type = ITYPE_BOMB,
    .minAttack = 20,
    .maxAttack = 40
};

struct InventoryItemDefinition ITEMD_BOMB_RatShot = {
    .saveName = "ITEMD_BOMB_RatShot",
    .name = "Rat-Shot",
    .description = "Stuns rats[2-5 rounds]. Rats cannot stand power of this shot. ",
    .value = 30,
    .type = ITYPE_BOMB,
    .minAttack = 2,
    .maxAttack = 5
};

struct InventoryItemDefinition ITEMD_QUEST_GhostAmulet = {
    .saveName = "ITEMD_QUEST_GhostAmulet",
    .name = "Mistic amulet",
    .description = "You feel a strange aura coming from it",
    .value = 1000,
    .type = ITYPE_ITEM,
};

struct InventoryItemDefinition ITEMD_ITEM_GoldenRing = {
    .saveName = "ITEMD_ITEM_GoldenRing",
    .name = "Golden ring",
    .description = "Looks more like a wedding ring than a ring of power ",
    .value = 250,
    .type = ITYPE_ITEM
};

struct InventoryItemDefinition ITEMD_ITEM_Ruby = {
    .saveName = "ITEMD_ITEM_Ruby",
    .name = "Ruby",
    .description = "Pretty pink stone",
    .value = 400,
    .type = ITYPE_ITEM
};

struct InventoryItemDefinition ITEMD_ITEM_SilverNeckless = {
    .saveName = "ITEMD_ITEM_SilverNeckless",
    .name = "Silver neckless",
    .description = "Made of silver but has no sharp edges.",
    .value = 150,
    .type = ITYPE_ITEM
};

struct InventoryItemDefinition ITEMD_ITEM_VeryOldBook = {
    .saveName = "ITEMD_ITEM_VeryOldBook",
    .name = "Ancient Book",
    .description = "Old tome, written in ancient language.",
    .value = 350,
    .type = ITYPE_ITEM
};

struct InventoryItemDefinition ITEMD_ITEM_GoldenGoblet = {
    .saveName = "ITEMD_ITEM_GoldenGoblet",
    .name = "Golden goblet",
    .description = "Great for high quality wine",
    .value = 300,
    .type = ITYPE_ITEM
};

struct InventoryItemDefinition ITEMD_ITEM_Wine = {
    .saveName = "ITEMD_ITEM_Wine",
    .name = "Bottle of fine wine",
    .description = "The older the better",
    .value = 100,
    .type = ITYPE_ITEM
};

// NPCs //
struct NPC NPCVillageElder;
struct NPC NPCVillageShopkeeper;
struct NPC NPCVillageEldersDaughter;
struct NPC NPCVillageBlacksmith;
struct NPC NPCVillageHerbalist;
struct NPC NPCGhost;
struct NPC NPCFarmer;
struct NPC NPCTownSmith;
struct NPC NPCTownArmorer;
struct NPC NPCTownHealer;
struct NPC NPCTownPriest;
struct NPC NPCTownTavernBartender;
struct NPC NPCVampireSlayer;
struct NPC NPCVampire;

enum {
    Identity_NPCVillageElder = 0,
    Identity_NPCVillageShopkeeper,
    Identity_NPCVillageEldersDaughter,
    Identity_NPCVillageBlacksmith,
    Identity_NPCVillageHerbalist,
    Identity_NPCGhost,
    Identity_NPCFarmer,
    Identity_NPCTownSmith,
    Identity_NPCTownArmorer,
    Identity_NPCTownHealer,
    Identity_NPCTownPriest,
    Identity_NPCTownTavernBartender,
    Identity_NPCVampireSlayer,
    Identity_NPCVampire,
};
// BOSSES - MONSTERS //
struct MonsterDefinition MONSTERDEF_Vampire = {
    .saveName = "MONSTERDEF_Vampire",
    .name = "Vampire",
    .maxHp = 250,
    .armorClass = 3,
    .maxAttack = 20,
    .minAttack = 5,
    .hostile = 1,
    .joinsFight = 0
};

struct MonsterDefinition MONSTERDEF_Ghost = {
    .saveName = "MONSTERDEF_Ghost",
    .name = "Ghost",
    .maxHp = 50,
    .armorClass = 1,
    .maxAttack = 12,
    .minAttack = 3,
    .hostile = 0,
    .joinsFight = 0
};

struct MonsterDefinition MONSTERDEF_Golem = {
    .saveName = "MONSTERDEF_Golem",
    .name = "Golem",
    .maxHp = 300,
    .armorClass = 1,
    .maxAttack = 15,
    .minAttack = 1,
    .hostile = 0,
};

// MONSTERS //
struct MonsterDefinition MONSTERDEF_SmallRat = {
    .saveName = "MONSTERDEF_SmallRat",
    .name = "Small rat",
    .maxHp = 14,
    .armorClass = 0,
    .maxAttack = 5,
    .minAttack = 2,
    .hostile = 1,
};

struct MonsterDefinition MONSTERDEF_Rat = {
    .saveName = "MONSTERDEF_Rat",
    .name = "Rat",
    .maxHp = 20,
    .armorClass = 0,
    .maxAttack = 7,
    .minAttack = 2,
    .hostile = 0,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_Wolf = {
    .saveName = "MONSTERDEF_Wolf",
    .name = "Wolf",
    .maxHp = 30,
    .armorClass = 1,
    .maxAttack = 6,
    .minAttack = 3,
    .hostile = 0,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_AlphaWolf = {
    .saveName = "MONSTERDEF_AlphaWolf",
    .name = "Alpha Wolf",
    .maxHp = 45,
    .armorClass = 1,
    .maxAttack = 8,
    .minAttack = 3,
    .hostile = 1,
    .joinsFight = 0
};

struct MonsterDefinition MONSTERDEF_RatKing = {
    .saveName = "MONSTERDEF_RatKing",
    .name = "Rat King",
    .maxHp = 50,
    .armorClass = 2,
    .maxAttack = 10,
    .minAttack = 5,
    .hostile = 0,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_LesserSceleton = {
    .saveName = "MONSTERDEF_LesserSkeleton",
    .name = "Lesser Skeleton",
    .maxHp = 30,
    .armorClass = 0,
    .maxAttack = 6,
    .minAttack = 3,
    .hostile = 1,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_Sceleton = {
    .saveName = "MONSTERDEF_Sceleton",
    .name = "Skeleton",
    .maxHp = 30,
    .armorClass = 2,
    .maxAttack = 15,
    .minAttack = 5,
    .hostile = 1,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_Ghoul = {
    .saveName = "MONSTERDEF_Ghoul",
    .name = "Ghoul",
    .maxHp = 35,
    .armorClass = 3,
    .maxAttack = 25,
    .minAttack = 1,
    .hostile = 1,
    .joinsFight = 1
};

struct MonsterDefinition MONSTERDEF_Zombie = {
    .saveName = "MONSTERDEF_Zombie",
    .name = "Zombie",
    .maxHp = 50,
    .armorClass = 3,
    .maxAttack = 30,
    .minAttack = 1,
    .hostile = 1,
    .joinsFight = 1
};

// MAIN LOCATIONS
struct Location loc_CHURCH_GOLEM;
void l_CHURCH_GOLEM() {
    locationInitWithName(&loc_CHURCH_GOLEM,
        "Church altar",
        "You step towards altar.\n"
        "Dust is everywhere. Behind, on a side, there are doors.\n"
        "You know they lead to catacombs.\n"
        "This is where this hellish evil must be coming from.\n"
        , SYMBOL_CHURCH_INTERIOR
    );

    appendToList(&loc_CHURCH_GOLEM.monsters, newMonster(&MONSTERDEF_Golem));
}

struct Location loc_CHURCH_ENTRY;
void l_CHURCH_ENTRY() {
    locationInitWithName(&loc_CHURCH_ENTRY,
        "Church entrance",
        "It is dark inside. You hear your breath echoing in large hall.\n"
        "Church was closed for a long time, there is nobody here.\n",
        SYMBOL_DOOROUT
    );
}

struct Location loc_CATACOMBS_2_ENTRY;
void l_CATACOMBS_2_ENTRY() {
    locationInitWithName(&loc_CATACOMBS_2_ENTRY,
        "Room with the sign",
        "You enter to a room full of torches. It is bright here.\n"
        "On the wall there is a message:\n"
        "> All the sides are interesting, but some are less than others.\n"
        "> North is cold, just a step from barbarian east.\n"
        "> To the east trail is longer\n"
        "> Once there, southern rims welcomes you.. also far away\n"
        "> Wild west is dangerous, make a step there.\n"
        "> In the end, follow down stairs.\n"
        , SYMBOL_STAIRSUP
    );
}

struct Location loc_CATACOMBS_2_STAIRSDOWN;
void l_CATACOMBS_2_STAIRSDOWN() {
    locationInitWithName(&loc_CATACOMBS_2_STAIRSDOWN,
        "Stone stairs",
        "Stairs lead down, deeper underground.\n"
        "You hear footsteps below.\n\n\n"
        , SYMBOL_STAIRSDOWN
    );
}

struct Location loc_CATACOMBS_3_ENTRY;
void l_CATACOMBS_3_ENTRY() {
    locationInitWithName(&loc_CATACOMBS_3_ENTRY,
        "Stone stairs",
        "You see a large hall covered with gold.\n"
        "Yellow torch light reflections make you almost blind.\n"
        "You see a person, wearing black coat.\n"
        "He looks a bit pale, with friendly smile on the face.\n",
        SYMBOL_STAIRSUP
    );
}

char* goldenCorridorName = "Golden corridor";
char* goldenCorridorDesc =
"Deeper you walk down the path.\n"
"The more excited you are how much rich can these catacombs fit.\n\n\n"
;

void TakeAllFromBox(struct Box* this) {
    while (this->inventory) {
        struct InventoryItem* item = this->inventory->payload;
        printf(CC(C_MSG)"~~ You found "CC(C_MSGHIGH)"%s x%i"CC(C_MSG)" ~~\n"CC(C_RST), item->def->name, item->count);
        transferItemOrCountAll(&this->inventory, &inventory, item, 1);
    }
}

void onOpenFinalPathChest(struct Box* this) {
    if (this->inventory) {
        printMsg("You open the golden chest.");
        TakeAllFromBox(this);
    }
    else {
        printMsg("The chest is empty");
    }
}

struct Box* newFinalPathBox() {
    return newBox(IDENTITY_BOXFINAL, "Golden chest", &onOpenFinalPathChest);
}


struct Location loc_CATACOMBS_3_PART1;
void l_CATACOMBS_3_PART1() {
    locationInitWithName(&loc_CATACOMBS_3_PART1,
        goldenCorridorName,
        goldenCorridorDesc,
        SYMBOL_TREASURE
    );

    struct Box* box = newFinalPathBox();
    appendToList(&box->inventory, newInventoryItem(&ITEMD_ITEM_GoldenRing));
    appendToList(&loc_CATACOMBS_3_PART1.boxes, box);

}

struct Location loc_CATACOMBS_3_PART2;
void l_CATACOMBS_3_PART2() {
    locationInitWithName(&loc_CATACOMBS_3_PART2,
        goldenCorridorName,
        goldenCorridorDesc,
        SYMBOL_TREASURE
    );

    struct Box* box = newFinalPathBox();
    appendToList(&box->inventory, newInventoryItem(&ITEMD_ITEM_GoldenGoblet));
    appendToList(&loc_CATACOMBS_3_PART2.boxes, box);
}

struct Location loc_CATACOMBS_3_PART3;
void l_CATACOMBS_3_PART3() {
    locationInitWithName(&loc_CATACOMBS_3_PART3,
        goldenCorridorName,
        goldenCorridorDesc,
        SYMBOL_TREASURE
    );

    struct Box* box = newFinalPathBox();
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_ITEM_Wine, 3));
    appendToList(&loc_CATACOMBS_3_PART3.boxes, box);

#if WITHDEBUG
    appendToList(&loc_CATACOMBS_3_PART3.inventory, newInventoryItem(&ITEMD_WEAPON_SilverSword));
    appendToList(&loc_CATACOMBS_3_PART3.inventory, newInventoryItem(&ITEMD_WEAPON_WoodenPeg));
    appendToList(&loc_CATACOMBS_3_PART3.inventory, newInventoryItem(&ITEMD_WEAPON_SwordLvl4));
    appendToList(&loc_CATACOMBS_3_PART3.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl4));
#endif
}

struct Location loc_CATACOMBS_3_FINAL;
void l_CATACOMBS_3_FINAL() {
    locationInitWithName(&loc_CATACOMBS_3_FINAL,
        "End of the line",
        "You finally meet it. The source of the evil.\n"
        "The end of your quest is near. Defeat it and set world free\n"
        "from this blasphemy\n"
        , SYMBOL_V
    );

    appendToList(&loc_CATACOMBS_3_FINAL.monsters, newMonster(&MONSTERDEF_Vampire));
    appendToList(&loc_CATACOMBS_3_FINAL.monsters, newMonster(&MONSTERDEF_Zombie));
    appendToList(&loc_CATACOMBS_3_FINAL.monsters, newMonster(&MONSTERDEF_Zombie));
    appendToList(&loc_CATACOMBS_3_FINAL.monsters, newMonster(&MONSTERDEF_Zombie));
}

struct Location loc_ABANDONED_TEMPLE_ENTRY;
void l_ABANDONED_TEMPLE_ENTRY() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_ENTRY,
        "Temple entrance",
        "It is dark here. You hear echo of water droplets in the abyss.\n"
        "God must have forsaken this place.\n"
        "You sense it is full of lost souls.\n"
        "Temple must be huge, you cannot see the end of a hall.\n"
        , SYMBOL_STAIRSUP
    );


}

struct Location loc_ABANDONED_TEMPLE_PATH;
void l_ABANDONED_TEMPLE_PATH() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_PATH,
        "Temple",
        "Floor is made of stone. You barely see anything.\n"
        "Light comes through holes in the roof\n"
        "There are no windows here.\n"
        "You feel more like in a tomb, than as in a house of God.\n"

        , SYMBOL_DUNGEONPATH
    );

}

struct Location loc_ABANDONED_TEMPLE_SCELETON_1;
void l_ABANDONED_TEMPLE_SCELETON_1() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_SCELETON_1,
        "Temple",
        "You see a couple of benches by the wall.\n"
        "It looks like something is sitting here.\n"
        "Or maybe it is your imagination.\n"
        "Further you go, colder it becomes.\n"
        , SYMBOL_DUNGEONPATH
    );

    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_1.monsters, newMonster(&MONSTERDEF_LesserSceleton));
}

struct Location loc_ABANDONED_TEMPLE_SCELETON_2;
void l_ABANDONED_TEMPLE_SCELETON_2() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_SCELETON_2,
        "Temple",
        "Another set of benches.\n"
        "You see spiders webs lighted by sun trails\n"
        "Far to the east there is something shining,\n"
        "but it is too far to see what it is.\n"
        , SYMBOL_DUNGEONPATH
    );

    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_2.monsters, newMonster(&MONSTERDEF_LesserSceleton));
    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_2.monsters, newMonster(&MONSTERDEF_LesserSceleton));
}

struct Location loc_ABANDONED_TEMPLE_SCELETON_3;
void l_ABANDONED_TEMPLE_SCELETON_3() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_SCELETON_3,
        "Altar",
        "You see an altar. It is very bright, made of white stone.\n"
        "Trails of light shine on it, making it glow\n"
        "in surrounding darkness.\n"
        "There is a room behind the alter to the east.\n"
        , SYMBOL_DUNGEONPATH
    );

    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_3.monsters, newMonster(&MONSTERDEF_LesserSceleton));
    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_3.monsters, newMonster(&MONSTERDEF_LesserSceleton));
    appendToList(&loc_ABANDONED_TEMPLE_SCELETON_3.monsters, newMonster(&MONSTERDEF_Sceleton));
}

void boxOnChestInTemple(struct Box* this) {
    if (this->inventory) {
        struct InventoryItem* amulet = this->inventory->payload;
        printMsgHigh("You search the chest and find", amulet->def->name);
        transferItem(&this->inventory, &inventory, amulet);
    }
    else {
        printMsg("Chest is empty");
    }
}

struct Box* newChestBox() {
    return newBox(IDENTITY_BOXCHEST, "Chest", &boxOnChestInTemple);
}

struct Location loc_ABANDONED_TEMPLE_GOAL;
void l_ABANDONED_TEMPLE_GOAL() {
    locationInitWithName(&loc_ABANDONED_TEMPLE_GOAL,
        "Room behind an altar",
        "You are in a small room behind an alter.\n"
        "There is not much left of a roof.\n"
        "It is bright here, almost comfy.\n"
        "This place looks nothing like a temple you passed by.\n"
        "A small chest catches your attention.\n"
        "It has no dust over it,\n"
        "as if someone left it a moment ago."
        , SYMBOL_TREASURE
    );

    struct Box* box = newChestBox();
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_QUEST_GhostAmulet, 1));
    appendToList(&loc_ABANDONED_TEMPLE_GOAL.boxes, box);
}

struct Location loc_VILLAGE_DUNGEON_ENTRY;
void l_VILLAGE_DUNGEON_ENTRY() {
    locationInitWithName(&loc_VILLAGE_DUNGEON_ENTRY,
        "Bottom of well",
        "It is wet here.\n"
        "You hear mouses or rats crumbling in the dark.\n"
        "Or is it something else? Hard to tell."
        ,
        SYMBOL_STAIRSUP
    );
}

struct Location loc_VILLAGE_DUNGEON_CAT;
void l_VILLAGE_DUNGEON_CAT() {
    locationInitWithName(&loc_VILLAGE_DUNGEON_CAT,
        "Cave",
        "There is a bit more light here.\n"
        "It looks like a rats' nest.\n"
        "Looks like they were feasting here.\n",
        SYMBOL_VILLAGE_DUNGEONPATH
    );

    appendToList(&loc_VILLAGE_DUNGEON_CAT.inventory, newInventoryItem(&ITEMD_QUEST_CatCorpse));
    appendToList(&loc_VILLAGE_DUNGEON_CAT.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl2));
    appendToList(&loc_VILLAGE_DUNGEON_CAT.monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&loc_VILLAGE_DUNGEON_CAT.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_VILLAGE_DUNGEON_CAT.monsters, newMonster(&MONSTERDEF_Rat));
}


char* sharedCatacombsName = "Catacombs";

char* sharedCatacombsDesc =
"Path leads straight. On the sides you see decorations.\n"
"It looks like made of bones, or maybe it is wood.\n"
"It is dark here.\n";

struct Location* createCatacombs1Path() {
    return newLocation(sharedCatacombsName,
        sharedCatacombsDesc
        , SYMBOL_CATACOMBS_PATH);
}

struct Location* createCatacombs1Sceleton() {
    struct Location* l = newLocation(sharedCatacombsName,
        sharedCatacombsDesc,
        SYMBOL_CATACOMBS_PATH);

    appendToList(&l->monsters, newMonster(&MONSTERDEF_Sceleton));

    return l;
}

struct Location* createCatacombs1Rat() {
    struct Location* l = newLocation(sharedCatacombsName,
        sharedCatacombsDesc,
        SYMBOL_CATACOMBS_PATH);

    appendToList(&l->monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&l->monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&l->monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&l->monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&l->monsters, newMonster(&MONSTERDEF_SmallRat));


    return l;
}

struct Location loc_CATACOMBS_1_ENTRY_MAIN;
void l_CATACOMBS_1_ENTRY_MAIN() {
    locationInitWithName(&loc_CATACOMBS_1_ENTRY_MAIN,
        sharedCatacombsName,
        "You are in tunnels under the church. An alley of coffins.\n"
        "Some are opened and ... empty."
        , SYMBOL_STAIRSUP
    );
}

struct Location loc_CATACOMBS_1_ENTRY_SECRET;
void l_CATACOMBS_1_ENTRY_SECRET() {
    locationInitWithName(&loc_CATACOMBS_1_ENTRY_SECRET,
        sharedCatacombsName, sharedCatacombsDesc,
        SYMBOL_DOOROUT
    );
}

void onOpenCatacombsChest(struct Box* this) {
    if (this->inventory) {
        printMsg("You search the chest. You feel like a thief, stealing from the dead.");
        TakeAllFromBox(this);
    }
    else {
        printMsg("Chest is empty");
    }
}

struct Box* newCatacombsBox() {
    return newBox(IDENTITY_BOXCATACOMBS, "Wooden chest covered with dust", &onOpenCatacombsChest);
}


struct Location loc_CATACOMBS_1_CHEST_1;
void l_CATACOMBS_1_CHEST_1() {
    locationInitWithName(&loc_CATACOMBS_1_CHEST_1,
        sharedCatacombsName, sharedCatacombsDesc,
        SYMBOL_TREASURE
    );

    struct Box* box = newCatacombsBox();
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_ITEM_SilverNeckless, 1));
    appendToList(&loc_CATACOMBS_1_CHEST_1.boxes, box);

    appendToList(&loc_CATACOMBS_1_CHEST_1.monsters, newMonster(&MONSTERDEF_Ghoul));

}

struct Location loc_CATACOMBS_1_CHEST_2;
void l_CATACOMBS_1_CHEST_2() {
    locationInitWithName(&loc_CATACOMBS_1_CHEST_2,
        sharedCatacombsName, sharedCatacombsDesc,
        SYMBOL_TREASURE
    );

    struct Box* box = newCatacombsBox();
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_ITEM_GoldenRing, 2));
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_ITEM_GoldenGoblet, 2));
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_ITEM_VeryOldBook, 1));
    appendToList(&loc_CATACOMBS_1_CHEST_2.boxes, box);

    appendToList(&loc_CATACOMBS_1_CHEST_2.monsters, newMonster(&MONSTERDEF_Ghoul));
    appendToList(&loc_CATACOMBS_1_CHEST_2.monsters, newMonster(&MONSTERDEF_Ghoul));
}

struct Location loc_CATACOMBS_1_CHEST_3;
void l_CATACOMBS_1_CHEST_3() {
    locationInitWithName(&loc_CATACOMBS_1_CHEST_3,
        sharedCatacombsName, sharedCatacombsDesc,
        SYMBOL_TREASURE
    );

    struct Box* box1 = newCatacombsBox();
    appendToList(&box1->inventory, newInventoryItemCount(&ITEMD_ITEM_Ruby, 3));
    appendToList(&box1->inventory, newInventoryItemCount(&ITEMD_ITEM_VeryOldBook, 2));
    appendToList(&loc_CATACOMBS_1_CHEST_3.boxes, box1);

    struct Box* box2 = newCatacombsBox();
    appendToList(&box2->inventory, newInventoryItemCount(&ITEMD_ITEM_Wine, 5));
    appendToList(&loc_CATACOMBS_1_CHEST_3.boxes, box2);

    appendToList(&loc_CATACOMBS_1_CHEST_3.monsters, newMonster(&MONSTERDEF_Ghoul));
    appendToList(&loc_CATACOMBS_1_CHEST_3.monsters, newMonster(&MONSTERDEF_Ghoul));
    appendToList(&loc_CATACOMBS_1_CHEST_3.monsters, newMonster(&MONSTERDEF_LesserSceleton));
    appendToList(&loc_CATACOMBS_1_CHEST_3.monsters, newMonster(&MONSTERDEF_LesserSceleton));
    appendToList(&loc_CATACOMBS_1_CHEST_3.monsters, newMonster(&MONSTERDEF_LesserSceleton));

}

struct Location loc_CATACOMBS_1_STAIRSDOWN;
void l_CATACOMBS_1_CHEST_STAIRSDOWN() {
    locationInitWithName(&loc_CATACOMBS_1_STAIRSDOWN,
        "Catacombs stairs",
        "You see rounded stair leading to lower levels.\n"
        "A dim torch light is coming from below\n"
        "\n\n"
        , SYMBOL_STAIRSDOWN
    );

    appendToList(&loc_CATACOMBS_1_STAIRSDOWN.inventory, newInventoryItemCount(&ITEMD_BOMB_FlameLizard, 3));
}

// MAXLINE: "Should be extra lines in the beginning just to move cursor to map->\n" 67 chars

struct Location loc_WORLD_START;
void l_WORLD_START() {
    locationInitWithName(&loc_WORLD_START,
        "Road to the village",
        "It is a muddy road. Your quest begins here.\n\n"
        "Driver told you to go east.\n"
        "You will find a village there.\n"
        "Good people should help you understand what sort of evil lives here\n"
        "and how to destroy it.",
        SYMBOL_WORLD_PATH
    );
    loc_WORLD_START.visited = 1;
}

struct Location loc_VILLAGE_ELDER;
void l_VILLAGE_ELDER() {
    locationInitWithName(&loc_VILLAGE_ELDER,
        "Village elder's house",
        "You see a wooden cottage.\n"
        "In front there is a big old tree.\n"
        "By the entrance a small garden holds variety of herbs.\n"
        "There is small wooden fence with a broken gate and a sign 'Elder'.\n"
        "An old man sits by the entrance and smokes pipe.\n"
        "You feel his sight on you, yet his eyes look calm.\n",
        SYMBOL_VILLAGE_BUILDING
    );

}

struct Location loc_VILLAGE_BLACKSMITH;
void l_VILLAGE_BLACKSMITH() {
    locationInitWithName(&loc_VILLAGE_BLACKSMITH,
        "Blacksmith",
        "You hear him before seeing him.\n"
        "A tall, well built, young man, hits an anvil with a hammer.\n"
        "Smith is busy with fixing farmers tools.\n"
        "Once you come closer, you notice knifes and swords hanging\n"
        "on an inner wall of his workshop.\n"
        "He does not seem to notice your presence.\n"
        , SYMBOL_VILLAGE_BUILDING
    );
}

struct Location loc_VILLAGE_HEALER;
void l_VILLAGE_HEALER() {
    locationInitWithName(&loc_VILLAGE_HEALER,
        "House of the Herbalist",
        "You approach a cottage which is surrounded by herbs.\n"
        "And old lady is preparing medicine in front.\n"
        "She is focused on her task.\n"
        "Once you come closer, she stops and looks at you.\n"
        "She has a gentle smile, showing you few remaining teeth.\n"
        , SYMBOL_VILLAGE_BUILDING
    );
}

struct Location loc_VILLAGE_SHOP;
void l_VILLAGE_SHOP() {
    locationInitWithName(&loc_VILLAGE_SHOP,
        "Small market",
        "There are few stages here and there. People come by and watch products.\n"
        "Not many people buy anything.\n"
        "A lady stands between with a cart full of various products.\n"
        "It is a mix of handmade decorations, potions and clothes.\n"
        "\n"
        "A little girl sits next to a lady. Kid looks very sad.\n"
        , SYMBOL_VILLAGE_BUILDING
    );
}

struct Location loc_VILLAGE_OLDWELL;
void l_VILLAGE_OLDWELL() {
    locationInitWithName(&loc_VILLAGE_OLDWELL,
        "Old well",
        "Between fields, there is a well. It looks very old.\n"
        "It is covered with dust and short grass rises on top of its root.\n"
        "The well is covered by wooden planks,\n"
        "yet there is a small hole between them.\n"
        "There is still a rope hanging nearby\n"
        "It looks solid despite its age.\n"

        , SYMBOL_VILLAGE_BUILDING
    );
}

struct Location loc_DARKFOREST_WOLFS;
void l_DARKFOREST_WOLFS() {
    locationInitWithName(&loc_DARKFOREST_WOLFS,
        "Forest",
        "You arrive at an empty plain.\n"
        "Your sixth sense tells you that wild animals like this place.\n"
        "You do not feel safe here.\n"
        , SYMBOL_DARKFOREST_PATH
    );
}

void boxOnPaladinsBag(struct Box* this) {
    if (this->inventory) {
        printMsg("You search the bag. There are a few items inside.");

        TakeAllFromBox(this);
        printMsgHigh("You found", "Gold Coins x100");
        printMsg("You took it all.");
        money += 100;
    }
    else {
        printMsg("Bag is empty\n");
    }
}

enum {
    LOCATION_TRAP = 1
};

void placePackOfWolfs(struct ListItem** monsters, int wolfsCount) {
    appendToList(monsters, newMonster(&MONSTERDEF_AlphaWolf));
    for (int i = 0; i < wolfsCount; i++) {
        appendToList(monsters, newMonster(&MONSTERDEF_Wolf));
    }
}

struct Box* newPaladinBagBox() {
    return newBox(IDENTITY_BOXPALADINSBAG, "Paladin's bag", &boxOnPaladinsBag);
}

struct Location loc_DARKFOREST_PALADINCORPSE;
void l_DARKFOREST_PALLADINCORPSE() {
    locationInitWithName(&loc_DARKFOREST_PALADINCORPSE,
        "Forest",
        "Someone was here, you see footsteps.\n"
        "There is something between bushes.\n"
        "You see a campfire, tent and blood stains.\n"
        "No sign of a body. There is also a bag laying nearby\n"
        , SYMBOL_DARKFOREST_PATH
    );

    struct Box* box = newPaladinBagBox();
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_WEAPON_PaladinsDarkSword, 1));
    appendToList(&box->inventory, newInventoryItemCount(&ITEMD_HEALING_HealingPotion, 2));

    appendToList(&loc_DARKFOREST_PALADINCORPSE.boxes, box);

    placePackOfWolfs(&loc_DARKFOREST_PALADINCORPSE.monsters, 2);
}

struct Location loc_WORLD_ABANDONED_TEMPLE;
void l_WORLD_ABANDONED_TEMPLE() {
    locationInitWithName(&loc_WORLD_ABANDONED_TEMPLE,
        "Forest",
        "Between trees there is a small building.\n"
        "You come closer and recognize that it is a sacred place.\n"
        "Temple perhaps. It is very old, but you see a cross.\n"
        "You sense evil coming from this place.\n"
        , SYMBOL_STAIRSDOWN
    );
}

struct Location loc_FARMS_COTTAGE;
void l_FARMS_COTTAGE() {
    locationInitWithName(&loc_FARMS_COTTAGE,
        "Small farmhouse",
        "You see a small house. It has clay walls and wooden roof.\n"
        "It looks brand new, white walls seems very clean.\n"
        "A young farmer works nearby a house. He looks frustrated."
        , SYMBOL_FARMS_COTTAGE
    );
}

struct Location loc_FARMS_STABLES;
void l_FARMS_STABLES() {
    locationInitWithName(&loc_FARMS_STABLES,
        "Barn",
        "This building looks like an ordinary barn.\n"
        "Brown walls, red roof, yellow gate.\n"
        "For some reason there is no cattle inside.\n"
        , SYMBOL_FARMS_STABLES
    );

    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_FARMS_STABLES.monsters, newMonster(&MONSTERDEF_RatKing));
}

struct Location loc_WORLD_SECRETENTRY;
void l_WORLD_SECRETENTRY() {
    locationInitWithName(&loc_WORLD_SECRETENTRY,
        "Dead end",
        "You reached an end of a road.\n"
        "There is nothing but trees and bushes here.\n"
        "Something still is odd with a road leading to nowhere.\n"
        "Why would anyone lead a path here?\n"
        , SYMBOL_DOORIN
    );
}

struct Location loc_TOWN_CHURCH;
void l_TOWN_CHURCH() {
    locationInitWithName(&loc_TOWN_CHURCH,
        "Church",
        "The biggest building in the town. Looks very monumental and old.\n"
        "It is built on a hill, surrounded by cliff from a side.\n"
        "God no longer visits this place. Evil took it and held.\n"
        "For some reason, yet unknown, it cannot leave this sacred place.\n",
        SYMBOL_TOWN_CHURCH
    );
}

struct Location loc_TOWN_GATE;
void l_TOWN_GATE() {
    locationInitWithName(&loc_TOWN_GATE,
        "Town gate",
        "You stand in front of a stone arch, wooden gates are opened.\n"
        "Around the town there is an old stone wall. It is full of holes.\n"
        "Once you come closer you smell the civilisation.\n"
        "\n"
        "There is a bulletin board here:\n"
        "> Don't walk alone after dark\n"
        "> Smith: I have best swords\n"
        "> Best booze in the tavern\n"
        "> Church is closed\n"
        , SYMBOL_TOWN_PATH
    );
}

struct Location loc_TOWN_SMITH;
void l_TOWN_SMITH() {
    locationInitWithName(&loc_TOWN_SMITH,
        "Smith",
        "You stand in from of a smith workshop.\n"
        "Apprentices help their master.\n"
        "Master smith shows some merchandise to a customer.\n"
        "You see a fine collection of swords, shields\n"
        "household tools and iron made decorations\n"
        , SYMBOL_TOWN_BUILDING
    );
}

struct Location loc_TOWN_TAVERN;
void l_TOWN_TAVERN() {
    locationInitWithName(&loc_TOWN_TAVERN,
        "Tavern",
        "You enter the tavern. Place full of red faces.\n"
        "If you intend to get robbed or beaten, it is the right place.\n"
        "Source of gossips, alcohol and whores. All in one.\n"
        , SYMBOL_TOWN_BUILDING
    );
}

struct Location loc_TOWN_ARMORER;
void l_TOWN_ARMORER() {
    locationInitWithName(&loc_TOWN_ARMORER,
        "Armorer",
        "Smith of different kind.\n"
        "This master of armor craft specializes in steel plates.\n"
        "Rumor says that lords and even pope's guards relay on his work.\n"
        "You would not even notice this place if not sign with\n"
        "a gold-plated armor hanging over the entrance.\n"
        , SYMBOL_TOWN_BUILDING
    );
}

struct Location loc_TOWN_HEALER;
void l_TOWN_HEALER() {
    locationInitWithName(&loc_TOWN_HEALER,
        "Hospital",
        "It is not a butcher. Entrance to hospital welcomes you\n"
        "with a big stain of blood. It is town hospital.\n"
        "Nearby entrance, sick people crawl with hope to get medicine.\n"
        "Others covered with bandages try to rest.\n"
        "Only few healers are around, and they are all busy\n"
        "except once, who is willing to talk to peasants.\n"
        , SYMBOL_TOWN_BUILDING
    );
}

struct Location loc_TOWN_CENTER;
void l_TOWN_CENTER() {
    locationInitWithName(&loc_TOWN_CENTER,
        "Town center",
        "Pretty much a center of a town.\n"
        "Here most of announcements are shouted out loud.\n"
        "Once holiday, a place of party and celebration.\n"
        "In these grim times, just an empty square.\n"
        , SYMBOL_TOWN_PATH
    );
}

struct Location loc_TOWN_CHAPEL;
void l_TOWN_CHAPEL() {
    locationInitWithName(&loc_TOWN_CHAPEL,
        "Chapel",
        "House with the single big room.\n"
        "Few lines of benches next to walls.\n"
        "An altar with a bronze cross.\n"
        "Chapel is empty today, doors stay opened.\n"
        "A priest stands in front. He seems to be waiting for someone.\n"
        , SYMBOL_TOWN_BUILDING
    );
}

struct Location* createChurchInterior() {
    return newLocation("Church hallway",
        "You move slow in here. Looking over your shoulder\n"
        "as if something was looking at you.\n"
        "Your only companion is the perfect silence\n"
        , SYMBOL_CHURCH_INTERIOR);
}

struct Location* createVillageDungeonPath() {
    return newLocation("Cave",
        "Wet, dark and slippery.\n"
        "You barely see anything.\n"
        "You hear droplets of water hitting ground.\n"
        , SYMBOL_VILLAGE_DUNGEONPATH);
}

struct Location* createCatacombs2Trap() {
    struct Location* loc = newLocation("Riddle room",
        "Every step you do here\n"
        "you feel that floor is moving up and down\n"
        "as if you were pushing a button.\n\n"
        , SYMBOL_CATACOMBS_TRAP);
    loc->identity = LOCATION_TRAP;
    return loc;
}

struct Location* createCatacombs2Path() {
    return newLocation("Riddle room",
        "Every step you do here seems safe.\n"
        "This room hides no secrets, but what is the next step?\n"
        "\n\n"
        , SYMBOL_CATACOMBS_PATH);
}

struct Location* createWorldPath() {
    return newLocation(
        "Road",
        "It is a muddy road between trees\n"
        "Barely a small carrage could pass here\n"
        "Cold wind strikes from sides and pushes you\n"
        "towards your goal.\n"
        "There is nothing interesting here besides a wild nature\n"

        , SYMBOL_WORLD_PATH);
}

struct Location* createVillagePath() {
    return newLocation(
        "Village path",
        "Road between fields.\n"
        "You hear dogs barking, hen and goats are running here and there.\n"
        "Few cottages are here but looks abandoned.\n"
        "Peasants are working on the field.\n"
        "A smell of house animals is in the air."
        , SYMBOL_VILLAGE_PATH);
}

struct Location* createDarkForestPath() {
    return newLocation(
        "Forest",
        "You enter deeper into forest.\n"
        "Woods and plants are dense here, it is also a lot darker.\n"
        "No wind, but a lot of noises. Animal perhaps.\n"
        "You have a feeling of being watched.\n"
        , SYMBOL_DARKFOREST_PATH);
}

struct Location* createFarmsFields() {
    return newLocation("Farm fields",
        "You see a green field, crops are growing.\n"
        "Paths are curvy.\n"
        "Wind is blowing strong.\n"
        "\n"
        , SYMBOL_FARMS_FIELD);
}

struct Location* createFarmsAbandonedCottage() {
    return newLocation("Empty farmhouse",
        "You see a house. It has no door, part of a wall collapsed.\n"
        "Whoever lived here is already gone.\n"
        "Through a hole you see pack of crows.\n"
        , SYMBOL_FARMS_ABANDONED_COTTAGE);
}

struct Location* createTownPath() {
    return newLocation("Streets",
        "Narrow, dirty, beggers on the street,\n"
        "kids running around between horses.\n"
        "Strange people watching you as you walk by.\n"
        , SYMBOL_TOWN_PATH);
}

int isVisited(struct LocationTransfer* route) {
    return route && route->destination->visited;
}

void showLocationHints() {
    changeColor(C_TRACK_HINT);

    if (isVisited(here->moves[NORTH])) printf("On the "CC(C_TRACK_HIGHHINT)"north"CC(C_TRACK_HINT)" you see %s\n", here->moves[NORTH]->closed ? ClosedDoorText : here->moves[NORTH]->destination->name);
    if (isVisited(here->moves[EAST]))  printf("On the "CC(C_TRACK_HIGHHINT)"east"CC(C_TRACK_HINT)" you see %s\n", here->moves[EAST]->closed ? ClosedDoorText : here->moves[EAST]->destination->name);
    if (isVisited(here->moves[SOUTH])) printf("On the "CC(C_TRACK_HIGHHINT)"south"CC(C_TRACK_HINT)" you see %s\n", here->moves[SOUTH]->closed ? ClosedDoorText : here->moves[SOUTH]->destination->name);
    if (isVisited(here->moves[WEST]))  printf("On the "CC(C_TRACK_HIGHHINT)"west"CC(C_TRACK_HINT)" you see %s\n", here->moves[WEST]->closed ? ClosedDoorText : here->moves[WEST]->destination->name);
    if (isVisited(here->moves[TRAVEL])) {
        if (here->moves[TRAVEL]->closed) {
            printf("You cannot "CC(C_TRACK_HIGHHINT)"travel"CC(C_TRACK_HINT)". Path is blocked\n");
        }
        else {
            printf("You can "CC(C_TRACK_HIGHHINT)"travel"CC(C_TRACK_HINT)" to %s\n", here->moves[TRAVEL]->destination->name);
        }
    }
    changeColor(C_RST);
}

void nextPage() {
    printMsg("[ENTER] to continue");
    fillInput();
    cursorMoveOffset(0, -2);
    printf("                                                                    \n");
    cursorMoveOffset(0, -1);
    printLine();
}

void look() {
    changeColor(C_LOC_SND);
    printf("\n~ -~~~~~~~~~~~~~- ~\n");
    changeColor(C_LOC_PRM);
    printf("%s\n", here->name);
    changeColor(C_LOC_SND);
    printf("%s\n", here->description);

    printLine();
    showLocationHints();

    if (here->inventory) {
        changeColor(C_LOC_ITEMS);
        printLine();
        printf("Here lies:\n");
        int idx = 1;
        for (struct ListItem* item = here->inventory; item != NULL; item = item->next, ++idx) {
            struct InventoryItem* invItem = item->payload;
            if (invItem->count > 1) {
                printf("#%i %s x%i\n", idx, invItem->def->name, invItem->count);
            }
            else {
                printf("#%i %s\n", idx, invItem->def->name);
            }
        }
    }

    if (here->monsters) {
        changeColor(C_LOC_MONSTERS);
        printLine();
        printf("Monsters are here:\n");
        int idx = 1;
        for (struct ListItem* monsterItem = here->monsters; monsterItem != NULL; monsterItem = monsterItem->next, ++idx) {
            struct Monster* monster = monsterItem->payload;
            printf("#%i %s\n", idx, monster->def->name);
        }
    }

    if (here->npcs) {
        changeColor(C_LOC_NPCS_TLT);
        printLine();
        printf("People you see here:\n");
        int idx = 1;
        for (struct ListItem* npcItem = here->npcs; npcItem != NULL; npcItem = npcItem->next, ++idx) {
            struct NPC* npc = npcItem->payload;
            printf(CC(C_LOC_NPCS)"#%i %s\n", idx, npc->name);
            printf(CC(C_LOC_NPCS_DESC)"  %s\n"CC(C_RST), npc->description);
        }
    }

    if (here->boxes) {
        printf(CC(C_LOC_BOXES)"\nThere are containers: \n");
        int idx = 1;
        for (struct ListItem* boxItem = here->boxes; boxItem != NULL; boxItem = boxItem->next, ++idx) {
            struct Box* box = boxItem->payload;
            printf("#%i %s\n", idx, box->name);
        }
    }

    changeColor(C_LOC_SND);
    printf("~ -~~~~~~~~~~~~~- ~\n");
    changeColor(C_RST);
}

void placeInLocation(struct Location* loc) {
    prevHere = here;
    here = loc;
    look();
    showMap();
}

void showOutro();
void enterLocation(int direction) {
    if (GameState.goodEnding) {
        showOutro();
        exit(0);
    }

    struct LocationTransfer* move = here->moves[direction];
    if (move == NULL) {
        printMsg("You cannot go there");
        return;
    }

    if (move->closed) {
        move->onClosedCallback(direction);
        return;
    }
    cursorMap(move->cursorOffset, move->destination->mapMark);
    placeInLocation(move->destination);
    fleeDirection = reverseDirection(direction);
    globalOnEnterLocation();
    here->visited = 1;
}

void showEquipedElement(struct ListItem* listItem, char* foundPrefix, char* notFoundMessage) {
    if (listItem) {
        struct InventoryItem* item = listItem->payload;
        printf(CC(C_EQUIP)"%s "CC(C_MSGHIGH)"%s\n"CC(C_RST), foundPrefix, item->def->name);
        printf(CC(C_DISABLED)"  %s\n"CC(C_RST), item->def->description);
    }
    else {
        printf(CC(C_EQUIP)"%s\n"CC(C_RST), notFoundMessage);
    }
}

void showEquipStatus() {
    showEquipedElement(Equipped.armor, "You wear", "You wear no armor");
    showEquipedElement(Equipped.weapon, "You wield", "You wield no weapon");
    printf("\n");
}

void showInventory() {
    showMoneyStatus();
    showEquipStatus();
    if (inventory == NULL) {
        printf("Your inventory is empty\n");
    }
    else {
        indexedListInventory(inventory);
    }
}

void showJournal() {
    printMsg("You read your journal");
    if (journal == NULL) {
        printMsgDisabled("The page is blank");
        return;
    }

    for (struct ListItem* head = journal; head != NULL; head = head->next) {
        struct JournalEntry* payload = head->payload;

        printLine();

        if (payload->completed) {
            printf(CC(C_QUEST_DONE)"[ Completed ] "CC(C_RST));
        }

        printf(CC(C_QUEST_TITLE)"%s"CC(C_RST)"\n  %s\n", payload->title, payload->description);
    }
    printLine();
}

void jmpTo(struct Location* target, int newCursorPos) {
    initDrawMap();
    DrawMapCursor = newCursorPos != -1 ? newCursorPos : DrawMapHeight / 2 * DrawMapWidth + DrawMapWidth / 2;

    cursorMap(0, target->mapMark);
    placeInLocation(target);
}

// Quest_MissingPaladinInDarkForest //
enum {
    QUEST_MissingPaladin_start_identity,
    QUEST_MissingPaladin_whereIsForest,
    QUEST_MissingPaladin_sellsword_identity,
    QUEST_MissingPaladin_dontsellsword_identity,
};

struct JournalEntry QUEST_MissingPaladin = {
    .identity = QuestMissingPaladinInDarkForest,
    .title = "Paladin has gone missing",
    .description = "Blacksmith told me that he had seen another paladin in the nearby forest to the east.\n"
            "  He is very interested in his sword and offers 200 coins for it.",
    .completed = 0
};

// Quest_MissingCat //
enum {
    QUEST_MissingCat_start_identity,
    QUEST_MissingCat_stillLooking,
    QUEST_MissingCat_deliverDaughterIdentity,
    QUEST_MissingCat_deliverShopKeeperIdentity,
};

struct JournalEntry QUEST_MissingCat = {
    .identity = QuestMissingCat,
    .title = "The kid's cat is missing",
    .description = "The little girl is missing her cat, let's help her and find it.",
    .completed = 0
};

// Quest_Ghost //
enum {
    QUEST_Ghost_start_identity,
    QUEST_Ghost_deliverAmulet_identity,
    QUEST_Ghost_keepAmulet_identity
};

struct JournalEntry QUEST_Ghost = {
    .identity = QuestReleaseGhost,
    .title = "Where is my amulet",
    .description = "I've met a ghost, who lost his amulet. He asked me to bring it back.",
    .completed = 0
};

// Quest_Farmer //
enum {
    QUEST_Farmer_start_identity,
    QUEST_Farmer_whereIsBarn,
    QUEST_Farmer_remindSecret,
    QUEST_Farmer_done_identity
};

struct JournalEntry QUEST_Farmer = {
    .identity = QuestFarmerRatsProblem,
    .title = "Farmer needs his barn to be wiped out of beasts",
    .description = "The farmer offered me a gift if I took care of his problem",
    .completed = 0
};

// Quest_Main //
struct JournalEntry QUEST_Main = {
    .identity = QuestMain,
    .title = "Wipe out evil from the town church",
    .description = "I was set on the quest to remove the evil from the church",
    .completed = 0
};

struct TalkTree* TalkToShopMode(struct TalkOption* this) {
    shopMode(talker);
    return &TalkSelf;
}

struct TalkOption TALKOptions_VillageElder[] = {
    {.message = "Where am I?",
        .response = "You are in our village, sir.",
        .next = &TalkSelf,
    },
    {.message = "Tell me about this place.",
        .response =
            "I have lived here for over 50 years. The good people of our village made me an elder.\n"
            "You can find a few services here. My son is a blacksmith, he likes swords a lot.\n"
            "My wife has a small shop nearby, you can get there the best ale.\n"
            "A herbalist has a hut here as well. She brews some good booze,\n"
            "but watch for it as sometimes people don't remember how they got back home after it.\n"
            "Behind the forest, there is a small city. Bad things happen there for a while.\n"
            "It might be not the best place to visit for such a young lad like you.\n"
        ,
        .next = &TalkSelf,
    },
    {.message = "I'm a paladin, I'm here to find a church.",
        .response =
            "Paladin, you say. Well then, it is not the village you seek.\n"
            "South from here, there is a city. They have a church there, but we don't visit it anymore.\n"
            "The priest shut it and forbade anybody to enter it. They say that dead people came alive.\n"
            "But you know what happens to a body after lying for years in the grave? It rottens.\n"
            "I cannot imagine how painful it must be to wake up alive in a rotten body, can you?\n"
            "Anyway Palandin, the city is where you need to head.\n"
        ,
        .next = &TalkSelf,
    },
    {.message = "Farewell", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_VillageElder = {
    .welcome = "Hello Sir. You are not from here I assume. What can I help you with?",
    .options = TALKOptions_VillageElder
};


void TALK_VillageElder(struct NPC* this) {
    printMsg("He looks at you with curiosity.");
    talkMode(&TALKTree_VillageElder);
}

struct TalkTree* QUEST_MissingCat_deliverShopKeeper(struct TalkOption* this) {
    GameState.catQuestEnding = CATQUEST_ENDING_MOTHER;
    struct InventoryItem* corpse = FindInventoryEntry(&ITEMD_QUEST_CatCorpse);
    removeFromList(&inventory, corpse);
    printMsgHigh("You hand over", ITEMD_QUEST_CatCorpse.name);
    printf(
        "Thank you.\n"
        "My daughter must not see this poor cat like that.\n"
        "It will be better for her to think that she just ran away.\n"
    );

    nextPage();
    printf("Take this little compenstation for your time.\n");
    printMsgHigh("You receive", "50 gold coins");
    money += 50;

    free(corpse);

    struct JournalEntry* quest = findJournalEntry(QuestMissingCat);
    completeQuest(quest);

    return NULL;
}

int QUEST_MissingCat_vis(struct TalkOption* this);
struct TalkOption TALKOptions_VillageShopKeeper_1[] = {
    {.message = "Can you tell me about this place?",
        .response =
            "Not much I can say. I have lived here my whole life. Forest gives us protection.\n"
            "Recently strangers started to arrive, so the business started to grow a bit.\n"
            "They all head to the church to fight the evil or something.\n"
            "Overall it sounds like a good place to start fighting evil right?\n"
            "I mean church, yuk, yuk, but they haven't come back so far.\n"
            "My older one wanted to go there as well, but I made him drop this silly idea.\n"
            "He runs a blacksmith here in a village, why don't you visit him?\n"
        ,
        .next = &TalkSelf},
    {.message = "Do you hire children?",
        .response =
            "This one? She's my younger one. Helps me to run this business.\n"
            "But recently her cat gone missing. She cannot stop talking about it.\n"
        ,
        .next = &TalkSelf},


    {.message = "I'd like to buy something",  .action = TalkToShopMode},
    {.message = "I found this poor kitty on the bottom of a well", .action = QUEST_MissingCat_deliverShopKeeper, .next = NULL, .vis = QUEST_MissingCat_vis, .identity = QUEST_MissingCat_deliverShopKeeperIdentity},
    {.message = "Bye", .response = "Bye stranger.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_VillageShopKeeper_1 = {
    .welcome = "What can I get you?",
    .options = TALKOptions_VillageShopKeeper_1
};

void TALK_VillageShopKeeper(struct NPC* this) {
    printMsg("The woman nods at you and shares the most honest smile.");
    talkMode(&TALKTree_VillageShopKeeper_1);
}

struct TalkTree* QUEST_MissingCat_start(struct TalkOption* this) {
    if (findJournalEntry(QuestMissingCat)) {
        printf("QUEST_MissingCat_start quest already taken\n");
    }
    else {
        addQuest(&QUEST_MissingCat);
    }
    return NULL;
}

struct TalkTree* QUEST_MissingCat_deliverDaughter(struct TalkOption* this) {
    GameState.catQuestEnding = CATQUEST_ENDING_GIRL;
    struct InventoryItem* corpse = FindInventoryEntry(&ITEMD_QUEST_CatCorpse);
    removeFromList(&inventory, corpse);
    printMsgHigh("You hand over", ITEMD_QUEST_CatCorpse.name);


    printf("Ow nooo. My little kitty. Why??\n");
    nextPage();
    printMsg("After a moment she handles herself and stops crying");
    printf(
        "I need to bury her...  please take this as a token of my gratitude.\n"
        "You are very good paladin sir.\n"
    );
    nextPage();
    printMsgHigh("You receive ", ITEMD_QUEST_WoodenRing.name);
    free(corpse);

    struct JournalEntry* quest = findJournalEntry(QuestMissingCat);
    completeQuest(quest);

    appendToList(&inventory, newInventoryItem(&ITEMD_QUEST_WoodenRing));

    return NULL;
}

int QUEST_MissingCat_vis(struct TalkOption* this) {
    struct JournalEntry* quest = findJournalEntry(QuestMissingCat);

    if (this->identity == QUEST_MissingCat_stillLooking && quest && !quest->completed) {
        return 1;
    }

    if ((this->identity == QUEST_MissingCat_deliverDaughterIdentity || this->identity == QUEST_MissingCat_deliverShopKeeperIdentity) && quest) {
        return !quest->completed && NULL != FindInventoryEntry(&ITEMD_QUEST_CatCorpse);
    }

    return (this->identity == QUEST_MissingCat_start_identity && NULL == quest);
}

struct TalkOption TALKOptions_VillageEldersDaughter_2[] = {
    {.message = "I am a paladin my child. I will find your kitty.", .action = QUEST_MissingCat_start, .next = NULL},
    {.message = "I have no time now.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_VillageEldersDaughter_2 = {
    .welcome =
        "My kitty has gone missing. She always was coming to me in the morning.\n"
        "And today she didn't. I was looking for her everywhere but I could not find her.\n"
        "Have you seen her sir? You are taller than me, you can see further.\n"
    ,
    .options = TALKOptions_VillageEldersDaughter_2
};

struct TalkOption TALKOptions_VillageEldersDaughter_1[] = {
    {.message = "Calm down child. Tell me what happened.",  .next = &TALKTree_VillageEldersDaughter_2, .vis = QUEST_MissingCat_vis, .identity = QUEST_MissingCat_start_identity},
    {.message = "I'm still looking for it.",  .next = NULL, .response = "I hope she is alright.\n", .vis = QUEST_MissingCat_vis, .identity = QUEST_MissingCat_stillLooking},
    {.message = "I'm afraid I found your cat", .action = QUEST_MissingCat_deliverDaughter, .next = NULL, .vis = QUEST_MissingCat_vis, .identity = QUEST_MissingCat_deliverDaughterIdentity},
    {.message = "Bye",  .next = NULL},
    {.message = NULL }
};

struct TalkTree TALKTree_VillageEldersDaughter_1 = {
    .welcome = "Where is my kitty!? Where did she go!?",
    .options = TALKOptions_VillageEldersDaughter_1
};

void TALK_VillageEldersDaughter(struct NPC* this) {
    if (GameState.catQuestEnding == CATQUEST_ENDING_MOTHER) {
        printMsg("A little girl is sad");
        printf(
            "Mommy said that farmer saw my kitty, she must have just ran away.\n"
            "Maybe she will visit me some day.\n"
        );
    }
    else if (GameState.catQuestEnding == CATQUEST_ENDING_GIRL) {
        printMsg("A little girl is calm");
        printf(
            "I made her a beautiful funeral. Daddy helped me with digging a grave.\n"
            "I've planted flowers there, she will be in peace now.\n"
            "Thank you again sir for finding her.\n"
        );
    }
    else {
        printMsg("A little girl has eyes full of tears.");
        talkMode(&TALKTree_VillageEldersDaughter_1);
    }
}

int QUEST_MissingPaladin_vis(struct TalkOption* this) {
    struct JournalEntry* quest = findJournalEntry(QuestMissingPaladinInDarkForest);
    struct InventoryItem* sword = FindInventoryEntry(&ITEMD_WEAPON_PaladinsDarkSword);

    if (quest && !quest->completed && this->identity == QUEST_MissingPaladin_whereIsForest) {
        return 1;
    }

    return (this->identity == QUEST_MissingPaladin_start_identity && NULL == quest) ||
        ((this->identity == QUEST_MissingPaladin_sellsword_identity || this->identity == QUEST_MissingPaladin_dontsellsword_identity) && quest && quest->completed == 0 && sword);
}

struct TalkTree* QUEST_MissingPaladin_start(struct TalkOption* this) {
    printMsg("Blacksmith gives you a pathetic look");
    printf(
        "Ok mister.\n"
        "I get your kind. Seen a lot of you recently here.\n"
        "There was one of you with a pretty nice sword recently here.\n"
        "As all the others, he has gone missing. But this one went to visit dark forest.\n"
        "Maybe you can go and look for him?\n"
        "Dark forest is to the east from the village.\n"
        "If you by any chance find his sword, I can offer 200 gold coins for it.\n"
    );
    addQuest(&QUEST_MissingPaladin);

    return NULL;
}

struct TalkTree* QUEST_MissingPaladin_complete(struct TalkOption* this) {
    struct JournalEntry* quest = findJournalEntry(QuestMissingPaladinInDarkForest);
    struct InventoryItem* sword = FindInventoryEntry(&ITEMD_WEAPON_PaladinsDarkSword);

    completeQuest(quest);

    if (this->identity == QUEST_MissingPaladin_sellsword_identity) {
        printMsgHigh("You hand over", sword->def->name);
        printMsgHigh("You get", "200 gold coins");
        money += 200;
        printf("Thank you dear sir. Now I can truly study some well crafted blade.\n");

        struct ListItem* removeMe = NULL;
        transferItem(&inventory, &removeMe, sword);
        if (removeMe) {
            freeListItem(removeMe);
        }
    }
    else {
        printMsg("You decided to keep the sword. The blacksmith is not happy.");
        printf("If this is what you want, fine. After all I think it is not worth my 200 coins.\n");
    }

    return NULL;
}

struct TalkOption TALKOptions_VillageBlacksmith_1[] = {
        {.message = "Tell me about yourself",
            .response =
                "You're like a stalker or something?\n"
                "I work with iron, need anything with iron, I can help.\n"
                "That is all you need to know.\n"
                "Need more, go talk to my father, he is a village elder.\n"
            , .next = &TalkSelf
        },
        {.message = "I'm a paladin. I came here to fight with evil forces", .action = QUEST_MissingPaladin_start, .next = NULL, .vis = QUEST_MissingPaladin_vis, .identity = QUEST_MissingPaladin_start_identity},
        {.message = "Where is the dark forest?",                 .response = "East from the village, past the herbalist hut.", .next = &TalkSelf, .vis = QUEST_MissingPaladin_vis, .identity = QUEST_MissingPaladin_whereIsForest},
        {.message = "I found a sword. He won't need it anymore. You can have it.",       .action = QUEST_MissingPaladin_complete,  .next = NULL, .vis = QUEST_MissingPaladin_vis, .identity = QUEST_MissingPaladin_sellsword_identity},
        {.message = "He camped in the forest. Only the sword was left. I will keep it for my order.",   .action = QUEST_MissingPaladin_complete,  .next = NULL, .vis = QUEST_MissingPaladin_vis, .identity = QUEST_MissingPaladin_dontsellsword_identity},
        {.message = "What can I buy here?", .action = &TalkToShopMode },
        {.message = "That's all", .response = "Have a great hunt.",  .next = NULL},
        {.message = NULL }
};


struct TalkTree TALKTree_VillageBlacksmith_1 = {
    .welcome = "Howdy Mister Knight. There is a lot of you here recently. What's your need?",
    .options = TALKOptions_VillageBlacksmith_1
};

void TALK_VillageBlacksmith(struct NPC* this) {
    printMsg("The blacksmith stops his work and looks at you.");
    talkMode(&TALKTree_VillageBlacksmith_1);

}

struct TalkOption TALKOptions_VillageHerbalist[] = {
        {.message = "What do you do here?", .response =
            "I have herbs, my love. Lots of herbs. They help me to brew medicine.\n"
            "Everyone in the village loves to visit me from time to time.\n"
            "My medicine is good. It makes them all happier.\n"
            ,.next = &TalkSelf},
        {.message = "I'd like to taste your medicine", .action = TalkToShopMode },
        {.message = "I need to go.", .response = "Care to visit me again someday!", .next = NULL},
        {.message = NULL }
};

struct TalkTree TALKTree_VillageHerbalist = {
    .welcome = "Greetings my love. What can such an old fart, like me, do for you?",
    .options = TALKOptions_VillageHerbalist
};

void TALK_VillageHerbalist(struct NPC* this) {
    printMsg("She looks at you with blurry sight. She looks drunk, or maybe she is just so old.");
    talkMode(&TALKTree_VillageHerbalist);
}

struct TalkTree* QUEST_Ghost_start(struct TalkOption* this) {
    printMsg("Ghost floats happily to the top of a hall and flies back with cheer on its face");
    printf(
        "Thank you sir.\n"
        "You are my only hope.\n"
        "Please bring be an amulet.\n"
    );
    addQuest(&QUEST_Ghost);

    return NULL;
}

int QUEST_Ghost_vis(struct TalkOption* this) {
    struct JournalEntry* quest = findJournalEntry(QuestReleaseGhost);

    return (this->identity == QUEST_Ghost_start_identity && NULL == quest);
}

void ghostQuestComplete() {
    struct JournalEntry* quest = findJournalEntry(QuestReleaseGhost);
    if (!quest) {
        addQuest(&QUEST_Ghost);
        quest = &QUEST_Ghost;
    }
    completeQuest(quest);
}

void onSlayGhost() {
    printf("Ghost disappeared in a mist\n");
    ghostQuestComplete();
}

void triggerGhostFight(int playerAttacks) {
    removeFromList(&here->npcs, &NPCGhost);
    struct Monster* ghostEnemy = newMonster(&MONSTERDEF_Ghost);
    appendToList(&here->monsters, ghostEnemy);

    if (!playerAttacks) {
        printf("Ghost suddently attack you\n");
    }
    fightMode(ghostEnemy, playerAttacks);
    if (here != &loc_ABANDONED_TEMPLE_ENTRY) {
        // in case of player fleeing from a fight
        appendToList(&loc_ABANDONED_TEMPLE_ENTRY.npcs, &NPCGhost);
        removeFromList(&loc_ABANDONED_TEMPLE_ENTRY.monsters, ghostEnemy);
        freeMonster(ghostEnemy);
        ghostEnemy = NULL;
    }
    else {
        onSlayGhost();
    }
}

struct TalkTree* QUEST_Ghost_attack(struct TalkOption* this) {
    triggerGhostFight(1);
    return NULL;
}

struct TalkOption TALKOptions_Ghost_1[] = {
    {.message = "I will help you damned soul.", .action = QUEST_Ghost_start, .next = NULL, .vis = QUEST_Ghost_vis, .identity = QUEST_Ghost_start_identity},
    {.message = "I'm a paladin. My heart is pristine, I fight evil ghosts like you! <attack him>", .action = QUEST_Ghost_attack, .next = NULL },
    {.message = "I need to go.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_Ghost_1 = {
    .welcome =
        "Welcome stranger.\n"
        "Spend a moment for a lost soul, trapped in this damned place.\n"
        "Devilish forces keep me here. I have sworn to protect an amulet, but evil forces took it.\n"
        "I won't be released until I take an amulet with me.\n"
        "Please help me fulfill my destiny. I've been waiting for so long.\n"
        ,
    .options = TALKOptions_Ghost_1
};

struct TalkTree* QUEST_Ghost_handover_amulet(struct TalkOption* this) {
    printMsg("You give the amulet to Ghost and see how it vanishes.");
    printMsgHigh("From the void, you hear only", "Thank you, dear paladin");

    ghostQuestComplete();
    struct InventoryItem* questKey = FindInventoryEntry(&ITEMD_QUEST_GhostAmulet);
    removeFromList(&inventory, questKey);
    free(questKey);

    removeFromList(&here->npcs, &NPCGhost);
    return NULL;
}

struct TalkTree* QUEST_Ghost_keep_amulet(struct TalkOption* this) {
    triggerGhostFight(0);
    return NULL;
}

struct TalkOption TALKOptions_Ghost_2[] = {
    {.message = "Here it is. Take it Ghost and rest in peace.", .action = QUEST_Ghost_handover_amulet, .next = NULL},
    {.message = "I will keep it for myself.", .action = QUEST_Ghost_keep_amulet, .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_Ghost_2 = {
    .welcome = "I see you have my amulet. Wonderful. Give it back to me!",
    .options = TALKOptions_Ghost_2
};

void TALK_Ghost(struct NPC* this) {
    struct InventoryItem* amulet = FindInventoryEntry(&ITEMD_QUEST_GhostAmulet);
    if (amulet) {
        talkMode(&TALKTree_Ghost_2);
    }
    else {
        printMsg("Ghost approaches you");
        talkMode(&TALKTree_Ghost_1);
    }
}

int QUEST_Farmer_vis(struct TalkOption* this) {
    struct JournalEntry* quest = findJournalEntry(QuestFarmerRatsProblem);
    int ratsAreGone = loc_FARMS_STABLES.monsters == NULL;
    return
        (this->identity == QUEST_Farmer_remindSecret && quest && quest->completed) ||
        (this->identity == QUEST_Farmer_whereIsBarn && quest != NULL && quest->completed == 0 && !ratsAreGone) ||
        (this->identity == QUEST_Farmer_start_identity && NULL == quest) ||
        (this->identity == QUEST_Farmer_done_identity && quest && quest->completed == 0 && ratsAreGone);
}

struct TalkTree* QUEST_Farmer_start(struct TalkOption* this) {
    printf(
        "My barn is to the west.\n"
        "Deal with the beasts and come back to me once you're done\n"
        "You shall be well rewarded.\n"
    );
    addQuest(&QUEST_Farmer);

    return NULL;
}

void openSecretPathFromCatacomb() {
    GameState.secretPathOpenedFromCatacomb = 1;
    joinLocationOneWay(&loc_CATACOMBS_1_ENTRY_SECRET, &loc_WORLD_SECRETENTRY, -1 * DRAWMAPOFFSET(6, -2));
}

void openSecretPath() {
    GameState.secretPathOpened = 1;
    joinLocationOneWay(&loc_WORLD_SECRETENTRY, &loc_CATACOMBS_1_ENTRY_SECRET, DRAWMAPOFFSET(6, -2));
}

struct TalkTree* QUEST_Farmer_done(struct TalkOption* this) {
    printMsg("The farmer is very happy");
    printf(
        "Thank you my boy.\n"
        "Now I can get back to work without worries.\n"
        "I can tell you a secret as a reward for your trouble.\n"
    );
    nextPage();
    printf(
        "South from here, there is a path.\n"
        "At the end of it, there is a small cave.\n"
    );

    nextPage();
    printf(
        "Inside you can find a secret passage, it leads to church undergrounds.\n"
        "I've played there as a child, maybe it is still opened.\n"
    );
    nextPage();
    printf("Good luck.\n");

    struct JournalEntry* quest = findJournalEntry(QuestFarmerRatsProblem);
    completeQuest(quest);
    openSecretPath();
    return NULL;
}

struct TalkOption TALKOptions_Farmer_2[] = {
    {.message = "I'm a paladin and I shall fight the evil. Where is your barn peasant?!", .vis = QUEST_Farmer_vis, .action = QUEST_Farmer_start, .identity = QUEST_Farmer_start_identity, .next = NULL},
    {.message = "I'm busy now", .next = NULL, .response = "Yeah. Go. Don't worry about my common problems." },
    {.message = NULL }
};

struct TalkTree TALKTree_Farmer_2 = {
    .welcome = "What happened!!?\n"
            "These red-eyed beasts have taken over my barn.\n"
            "They are everywhere. Attack my cattle, eat my crops.\n"
            "God must be having fun by sending these imps here.\n"
            "I tried to get them out, but they still came back and became aggressive.\n"
            "I don't know what to do now. Seems my barn became a devil imp's nest.\n",
    .options = TALKOptions_Farmer_2
};

struct TalkOption TALKOptions_Farmer_1[] = {
    {.message = "What happened?", .vis = QUEST_Farmer_vis, .identity = QUEST_Farmer_start_identity,  .next = &TALKTree_Farmer_2},
    {.message = "Where is your barn?", .vis = QUEST_Farmer_vis, .identity = QUEST_Farmer_whereIsBarn, .response = "Head west, you can't miss it.", .next = &TalkSelf},
    {.message = "Beasts are gone. Your barn is freed.",   .vis = QUEST_Farmer_vis, .action = QUEST_Farmer_done,    .identity = QUEST_Farmer_done_identity,   .next = NULL},
    {.message = "What was the secret again?", .vis = QUEST_Farmer_vis,
        .response =
            "South from here, there is a path to the church underground.\n"
            "You can travel there from a dead-end path.\n"
            "Stay safe hero.\n",
        .identity = QUEST_Farmer_remindSecret, .next = &TalkSelf},
    {.message = "Bye", .response = "Watch your steps, it is a farm after all.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_Farmer_1 = {
    .welcome = "What do you want? Can't you see I'm busy!",
    .options = TALKOptions_Farmer_1
};

void TALK_Farmer(struct NPC* this) {
    printMsg("The farmer is tired and angry.");
    talkMode(&TALKTree_Farmer_1);
}

struct TalkOption TALKOption_TownSmith[] = {
    {
        .message = "I'm a paladin, I came to wipe evil from this city!",
        .response =
            "Hello paladin. You are not the first one who try this quest.\n"
            "There was not a single week that one of you came here for a long time.\n"
            "We really appreciate your presence.\n"
            "You make our city prosper. I hope you won't be the last one to visit us.\n"
            , .next = &TalkSelf
    },
    {
        .message = "I need a fine sword to wipe out the evil!",
        .response =
            "I have the best swords in town, my dear sir.\n"
            "I don't work for free though. In the beginning, I was keen to donate my iron.\n"
            "But after all, you take my iron swords, go to church, and never come back.\n"
            "Evil still lives in church and people are scared to walk after dark.\n"
            "Not to mention that selling silver swords become very profitable recently.\n"
            "So don't count on any discounts.\n"
            , .next = &TalkSelf
    },
    {.message = "Show me what you offer", .action = TalkToShopMode },
    {.message = "Farewell", .response = "You too sir.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_TownSmith = {
    .welcome = "Welcome to my workshop traveler. How can I be of service?",
    .options = TALKOption_TownSmith
};

void TALK_TownSmith(struct NPC* this) {
    printMsg("Smith approaches you slowly");
    talkMode(&TALKTree_TownSmith);
}

struct TalkOption TALKOption_TownArmorer[] = {
    {
        .message = "What do you sell?",
        .response =
            "Jackets, sweaters, blouses, t-shirts.\n"
            "They are kind of special, as they can block blades.\n"
            "ha ha ha, you're funny lad.\n"
            ,.next = &TalkSelf
    },
    {
        .message = "I'm a paladin, I need you to make me armor!",
        .response =
            "And I'm a pope, I need you to bring me a mountain of gold!\n"
            "ha ha ha, you're so funny, lad.\n"
            "I cannot stop crying, sorry,\n"
            "ha ha ha ha.\n"
            "So what did you want?\n"
            , .next = &TalkSelf
    },
    {.message = "Show me what you have", .action = TalkToShopMode },
    {.message = "I leave", .response = "I live as well. Ha ha ha ha", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_TownArmorer = {
    .welcome = "Are you lost Master? You seem to be confused with my selection of goods.",
    .options = TALKOption_TownArmorer
};

void TALK_TownArmorer(struct NPC* this) {
    printMsg("Once you come closer, he looks at you with his deep dark eyes");
    talkMode(&TALKTree_TownArmorer);
}

enum {
    HURT_HEAL10 = 1,
    HURT_HEAL20,
    HURT_HEAL50,
};

int HurtHelp_vis(struct TalkOption* this) {
    if (this->identity == HURT_HEAL10) {
        return health < 100;
    }

    if (this->identity == HURT_HEAL20) {
        return health < 90;
    }

    if (this->identity == HURT_HEAL50) {
        return health < 70;
    }

    return 0;
}

struct TalkTree* HealByHealer(struct TalkOption* this) {
    int cost = 0;
    int healGain = 0;
    if (this->identity == HURT_HEAL10) {
        cost = 3;
        healGain = 10;
    }

    if (this->identity == HURT_HEAL20) {
        cost = 5;
        healGain = 20;
    }

    if (this->identity == HURT_HEAL50) {
        cost = 10;
        healGain = 50;
    }

    if (money < cost) {
        printf("Sorry, you cannot afford such medical care.\n");
        return &TalkSelf;
    }
    else {
        money -= cost;

        int wasHP = health;
        health += healGain;
        if (health > 100) health = 100;
        int healed = health - wasHP;

        printf(CC(C_MSG) "~~ You are charged " CC(C_MSGHIGH)"%i gold"CC(C_MSG)" and healed "CC(C_MSGHIGH)"%i "CC(C_MSG)"HP. You have "CC(C_MSGHIGH)"%i "CC(C_MSG)"HP ~~\n"CC(C_RST), cost, healed, health);
        return &TalkSelf;
    }
}

struct TalkOption TALKOption_HealAtHealer[] = {
    {.message = "Heal 10 HP for  3 gold coins", .vis = HurtHelp_vis, .action = HealByHealer, .identity = HURT_HEAL10},
    {.message = "Heal 20 HP for  5 gold coins", .vis = HurtHelp_vis, .action = HealByHealer, .identity = HURT_HEAL20},
    {.message = "Heal 50 HP for 10 gold coins", .vis = HurtHelp_vis, .action = HealByHealer, .identity = HURT_HEAL50},
    {.message = "I'm fine.", .next = NULL},
    {.message = NULL},
};

struct TalkTree TALKTree_HealAtHealer = {
    .welcome = "I can offer various healing packages",
    .options = TALKOption_HealAtHealer
};

struct TalkOption TALKOption_TownHealer[] = {
    {.message = "I'm hurt, I need help", .vis = HurtHelp_vis, .next = &TALKTree_HealAtHealer, .identity = HURT_HEAL10},
    {
        .message = "What is this place?",
        .response =
            "Hos-Pi-Tal. Clinic, a place where sick people can get some help.\n"
            "Recently people have been getting sick more often. Maybe it is because of autumn.\n"
            "Or maybe because of the great evil which haunts our church.\n"
            "Pick which suits you best."
            ,.next = &TalkSelf
    },
    {
        .message = "I'm a paladin! I need medicine",
        .response =
            "For your condition, I would visit the tavern. They have plenty of medicine there."
            , .next = &TalkSelf
    },
    {.message = "Show me what you have", .action = TalkToShopMode },
    {.message = "Thank you for your service", .response = "Right, bye paladin.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_TownHealer = {
    .welcome = "I run hospital here, are you hurt?",
    .options = TALKOption_TownHealer
};


void TALK_Healer(struct NPC* this) {
    printMsg("A tired man looks at you. His clothes are covered with blood, not likely his.");
    talkMode(&TALKTree_TownHealer);
}

enum {
    PRIEST_GetKey = 1,
    PRIEST_WhereIsChurch
};

int TALK_Priest_vis(struct TalkOption* this) {
    return (this->identity == PRIEST_GetKey && !GameState.priestGaveKey) ||
        (this->identity == PRIEST_WhereIsChurch && GameState.priestGaveKey);
}

void openChurchDoors() {
    loc_CHURCH_ENTRY.moves[TRAVEL]->closed = 0;
    loc_TOWN_CHURCH.moves[TRAVEL]->closed = 0;
    GameState.churchDoorOpened = 1;
}

void showIntro() {
    printMsg("The elder of your order approaches you");
    printf(
        "> Hello my son.. it has been a while since we last spoke.\n"
        "  There is a grave danger and a time comes when you can prove your loyalty.\n"
        "> God must have heard your prey because it blessed you with a quest.\n"
        "  You need to pack your things, take food for a few days, and leave now.\n"
        "> Carrage is waiting for you by the entrance to the tower.\n"
        "  The driver knows where to take you.\n"
        "> You need to find a church and ask the priest about the events.\n"
        "  He will introduce you to a quest. But be aware.\n"
        "  Evil forces are already present and you are not the first who was sent there.\n"
        "  Be humble, watch your back, and pray to God so he gives you his blessing.\n"
    );

    nextPage();
    printMsg("The elder comes closer and puts his hand on your shoulder");
    printf("> Stay safe my child, remember your training, you are the hand of God now.\n");
    nextPage();
    printMsg("The elder leaves the room, leaving you with your thoughts.");
    nextPage();
    printMsg("You pack your belongings, run down the tower, and meet a driver");
    printf("> Sir, the carrage is waiting. We need to rush.\n");
    nextPage();
    printMsg("You enter the carrage and await where it takes you. The quest begins...");
    printLine();
    printLine();
    nextPage();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~ THE PALADIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    nextPage();
}

void showOutro() {
    printMsg("After a tough fight, you managed to escape the catacombs.");
    nextPage();
    printf(CC(C_CYAN)   "You stand in front of the church door.\n");
    printf(CC(C_BCYAN)  "You are tired. Your hands are shaking, you barely stand straight.\n");
    nextPage();
    printf(CC(C_MAGENTA)    "With all your strength, you push the doors.\n");
    printf(CC(C_BMAGENTA)   "The priest welcomes you with tears in his eyes.\n");
    printf(CC(C_MAGENTA)    "There are others around him, all full of hope.\n");
    printf(CC(C_BMAGENTA)   "Nobody dares to say anything.\n");
    nextPage();
    printf(CC(C_BWHITE)     "It is done.\n");
    nextPage();
    printf(CC(C_GREEN)      "You barely recognize your voice.\n");
    printf(CC(C_BGREEN)     "People are laughing, crying out loud, shouting in joy.\n");
    printf(CC(C_GREEN)      "Evil is gone.\n");
    nextPage();
    printf(CC(C_WHITE)      "Once the joy was over and people got back to their daily work,\n");
    printf(CC(C_BWHITE)     "a hero entered the carrage and drove back to The Order.\n");
    printf(CC(C_WHITE)      "Always on duty, always ready to spread good and fight evil forces.\n");
    nextPage();
    printf(CC(C_CYAN)       "In the name of God, in the name of The Order, one never stops to be\n");
    nextPage();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~ THE PALADIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    nextPage();
}

void showEvilOutro() {
    printMsg("Once you get out of the church, you smell a smoke");
    printf(
        CC(C_RED)   "Fire spreads across town.\n"
        CC(C_BRED)  "The Chapel's roof collapsed. Flames covered every surrounding building.\n"
        CC(C_RED)   "Bodies lay on the ground. You recognize a priest. He tries to reach you.\n"
    );
    nextPage();
    printf(
        CC(C_BBLACK)    "You run covered by smoke and terror towards him.\n"
        CC(C_WHITE)     "Once near, you hold his hand, his face is black from the smoke.\n"
        CC(C_BBLACK)    "He wants to say something but has no strength. You lower your head closer to him\n"
    );
    nextPage();
    printf(
        CC(C_CYAN)      "He only manages to whisper:\n"
        CC(C_MAGENTA)   "> Traitor\n"
        CC(C_BMAGENTA)  "> I curse you traitor\n"
    );
    nextPage();
    printf(
        CC(C_BBLACK)    "He dies in your arms.\n"
        CC(C_RED)       "Fire is closer every second.\n"
        CC(C_BRED)      "You are trapped, a wall of fire is coming.\n"
        CC(C_BBLACK)    "Smoke brings tears and takes your breath.\n"
    );
    nextPage();
    printf(
        CC(C_CYAN)      "In despair, you try to get back to Church.\n"
        CC(C_BCYAN)     "You try to open it, but the doors are so heavy.\n"
    );
    nextPage();
    printf(
        CC(C_CYAN)      "You try harder and harder, only to realize\n"
        CC(C_BCYAN)     "that the doors are shut.\n"
    );
    nextPage();
    printf(
        CC(C_BBLACK)    "Another cloud of smoke reaches you and you feel that you collapse.\n"
        CC(C_WHITE)     "The shame, the despair, the anger. You want to pray.\n"
        CC(C_CYAN)      "But the only thing that you can remember is the face of the vampire.\n"
    );
    nextPage();
    printf(
        CC(C_MAGENTA)   "His smiling face is your companion in your final moments.\n"
        CC(C_BMAGENTA)  "The regret is a pain which is the last feeling in the heart of...\n"
    );

    printf(CC(C_RST)"~~~~~~~~~~~~~~~~~~~~~~~~~~~ THE PALADIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    nextPage();
}

struct TalkTree* TALK_Priest_GetKey(struct TalkOption* this) {
    GameState.priestGaveKey = 1;
    printf(
        "Church is closed. Here is a key.\n"
        "But be aware. Catacombs are very deep and ancient.\n"
        "We've never went deeper than just below church itself.\n"
        "Old books mention a labiryth.\n"
        "\n"
        "Church is to the west.\n"
        "May the God be with you.\n"
    );

    printMsgHigh("Priest give you a", ITEMD_KEY_KeyToChurch.name);
    appendToList(&inventory, newInventoryItem(&ITEMD_KEY_KeyToChurch));

    return &TalkSelf;
}

struct TalkOption TALKOptions_Priest_1[] = {
    {.message = "I am a paladin! I came to the rescue!", .response = "I know who you are. I'm grateful you managed to arrive so fast.", .next = &TalkSelf},
    {.message = "How can I be of service?",
        .response =
            "We struggle with great evil.\n"
            "It came from nowhere.\n"
            "One day priests started to hear noises from church cellars.\n"
            "They came down to the catacombs and found that coffins were opened.\n"
            "One of the priests went to the bottom and found stairs that were not there before.\n"
            "He got back here so scared that he ran away and never returned.\n"
            "Then an expedition was sent. But they never returned.\n"
            "Since then we sealed the church and sent a message to the Bishop.\n"
        ,.next = &TalkSelf},
    {.message = "What might be down there?",
        .response =
            "Bishop sent here an expert.\n"
            "He went to church and made an investigation.\n"
            "He was sure. A Vampire.\n"
            "Since then, a lot of paladins have come here, none returned.\n"
            "One day a Vampire slayer also arrived.\n"
            "He said that only silver can slay the monster.\n"
            "Once slayed, a wooden peg must pass through its heart to send it back to hell.\n"
            "He never got back as well.\n"
        ,.next = &TalkSelf},
    {.message = "How to enter the church?", .vis = TALK_Priest_vis, .action = TALK_Priest_GetKey, .identity = PRIEST_GetKey},
    {.message = "Where is the church?", .vis = TALK_Priest_vis, .response = "To the west. Use the key to open the door.", .identity = PRIEST_WhereIsChurch, .next = &TalkSelf},
    {.message = "Farewell", .response = "May God guide you through the valley of darkness.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_Priest_1 = {
    .welcome = "Hello dear paladin, I was waiting for you.",
    .options = TALKOptions_Priest_1
};

void TALK_Priest(struct NPC* this) {
    printMsg("Priest greets you warmly, he was waiting for you.");
    talkMode(&TALKTree_Priest_1);
}

struct TalkOption TALKOptions_Bartender[] = {
    {.message = "What is this place?", .response = "Tavern? You come here to drink. Are you retarded?", .next = &TalkSelf},
    {.message = "Any news from around?",
        .response =
            "Nothing new here. Same customers, same forbidden faces.\n"
            "Idiots come here to get themselves killed in church.\n"
            "Sometimes new girls come, but not too often.\n"
            "Same old booze, same old shit.\n"
        , .next = &TalkSelf},
    {.message = "Do you make your liquor?",
        .response =
            "Naaah. I got it from this crazy lady from the nearby village.\n"
            "She comes here and sells it for a bargain. I think she likes to make people drunk."
        , .next = &TalkSelf},
    {.message = "What about church?",
        .response =
            "What about it?\n"
            "Priest closed it months ago. Said evil came and whatever.\n"
            "It brought a lot of knights here, so good for business.\n"
            "But guess what? Nobody gets back. There must be some good party there.\n"
            "Hue hue hue.\n"
            "But it is a bit scary to walk after dark. People sometimes go missing.\n"
        , .next = &TalkSelf},
    {.message = "Show me what you've got", .action = &TalkToShopMode },
    {.message = "Bye", .response = "Stay safe.", .next = NULL },
    {.message = NULL }
};

struct TalkTree TALKTree_Bartender = {
    .welcome = "Hi lad, what can I get you?",
    .options = TALKOptions_Bartender
};

void TALK_TownTavernBartender(struct NPC* this) {
    printMsg("A fat old bartender comes by and looks at you.");
    talkMode(&TALKTree_Bartender);
}

struct TalkOption TALKOptions_VampireSlayer[] = {
    {.message = "I'm a paladin! I came here to wipe out the evil.",
        .response =
            "Well, hello there!\n"
            "I'm world known Vampire Slayer.\n"
            "I came here to bring an end to the abomination that lives in these dungeons.\n"

        , .next = &TalkSelf},
    {.message = "What are you doing here?",
        .response =
            "I came here to slay a Vampire.\n"
            "So far I failed. I managed to find only remains of paladins all over the place.\n"
            "Actually, I've managed to gather a lot of silver swords."

        , .next = &TalkSelf},

    {.message = "Buy sword? Here?", .action = TalkToShopMode},
    {.message = "Farewell", .response = "Farewell brave paladin.", .next = NULL},
    {.message = NULL }
};

struct TalkTree TALKTree_VampireSlayer = {
    .welcome = "Greetings stranger. What brings you to this damned place? Do you want to buy a silver sword?",
    .options = TALKOptions_VampireSlayer
};

void TALK_VamireSlayer(struct NPC* this) {
    printMsg("The stranger is surprised that he sees you here");
    talkMode(&TALKTree_VampireSlayer);
}


struct {
    int showMessage;
} VampireTalkState;

enum {
    VAMPIRE_INTRODUCE = 0,
    VAMPIRE_OFFER,
    VAMPIRE_PICK_SCENARIO,
};

struct TalkTree* VampireOnIntroduce(struct TalkOption* this) {
    printMsg("Vampire opens his hands in a friendly gesture");

    printf("Calm down my dear.\n"
        "How about I make you an offer?\n"
        "I'm sure you don't want to become another nameless fool.\n"
        "Don't you wonder why nobody returned from here?\n"
        "Who do you think these sceletons are?\n");

    VampireTalkState.showMessage = VAMPIRE_OFFER;
    return &TalkSelf;
}

struct TalkTree* VampireShowOffer(struct TalkOption* this) {
    printMsg("Vampire has a wide smile, he is eager to share his brilliant idea.");

    printf(
        "I'm stuck here for centuries\n"
        "You cannot even imagine how boring it is to stay here.\n"
        "Only dead people as a company and old books.\n"
        "I used to keep it clean here. Tried to polish all coffins.\n"
        "Write books, poems, I even tried to sleep. No chance.\n"
        "Boredom is horrible.\n"
    );

    nextPage();
    printf(
        "There is a curse cast on me a long time ago.\n"
        "I cannot leave this church until someone invites me outside of a church.\n"
        "Few months ago I almost succeed. One of a priests wanted to help me.\n"
        "He invited me upstairs.\n"
    );
    nextPage();
    printf(
        "It almost worked, but holy crossed casted light upon me\n"
        "Priest realised who I am and run away, inviting me only to a church.\n"
    );
    nextPage();
    printf(
        "Then all these fools started to crawl here, so I had some company.\n"
        "Poor souls started to join me as sceletons and ghouls, but still nobody invited me outside.\n"
        "And I need to leave. I cannot be damned here for ever. It is too much to suffer.\n"
    );
    nextPage();
    printf(
        "I don't want your poor soul. I will spare you, in exchange for a simple invitation.\n"
        "Ask me to join you in a walk through the city streets.\n"
        "Your quest will be fulfilled.\n"
        "Evil will be gone from this holy church.\n"
        "I will never again visit this city.\n"
    );
    nextPage();
    printf("What say you?\n");
    nextPage();
    VampireTalkState.showMessage = VAMPIRE_PICK_SCENARIO;
    return &TalkSelf;
}

void setupEvilEnding() {
    GameState.evilEnding = 1;

    removeTravelFromLocation(&loc_CATACOMBS_1_ENTRY_SECRET);
    loc_CATACOMBS_3_FINAL.description = "You see an empty vampire's coffin.\n"
        "There is nobody around. Vampire no longer haunts this place\n\n\n";

    removeAllFromList(&loc_CATACOMBS_3_FINAL.monsters);
    completeQuest(&QUEST_Main);
    openChurchDoors();
}

struct TalkTree* VampireOnInvite(struct TalkOption* this) {
    printMsg("Once you finished your sentence, the vampire transformed into a bat and flew upstairs.");
    setupEvilEnding();

    return NULL;
}

struct TalkTree* VampireOnSlay(struct TalkOption* this) {
    printf("You fool. There is nothing waiting for you here, except death.\n");
    printMsg("The vampire changes into a bat a flies towards his coffin at the end of the corridor.");
    return NULL;
}

int VampireTalkVis(struct TalkOption* this) {
    return VampireTalkState.showMessage == this->identity;
}

struct TalkOption TALKOptions_Vampire[] = {
    {.message = "I'm a paladin. I came here to slay you!.", .action = &VampireOnIntroduce, .identity = VAMPIRE_INTRODUCE, .vis = VampireTalkVis },
    {.message = "What offer do you mean?", .action = &VampireShowOffer, .identity = VAMPIRE_OFFER, .vis = VampireTalkVis },
    {.message = "I need to think about it.", .vis = VampireTalkVis,  .response = "You must decide now. I've waited for too long.", .identity = VAMPIRE_PICK_SCENARIO, .next = &TalkSelf },
    {.message = "Would you join me for a walk through the city streets?", .vis = VampireTalkVis, .action = VampireOnInvite, .identity = VAMPIRE_PICK_SCENARIO },
    {.message = "You won't trick me, dark lord. I will send you to the void.", .vis = VampireTalkVis, .action = VampireOnSlay, .identity = VAMPIRE_PICK_SCENARIO },
    {.message = "Haven't we met before? The stranger... vampire slayer?",
        .response =
            "Please forgive me for the masquerade.\n"
            "I always seek to get to know another hero.\n"
            "You should thank me, I offered you your predecessors' swords for the best price."
        , .next = &TalkSelf},

    {.message = "Farewell", .response = "Not so fast my puppet. We need to talk about matters of high importance.", .next = &TalkSelf},
    {.message = NULL }
};

struct TalkTree TALKTree_Vampire = {
    .welcome = "",
    .options = TALKOptions_Vampire
};

void TALK_Vampire(struct NPC* this) {
    printMsg("Lord of the darkness approaches you.");
    talkMode(&TALKTree_Vampire);
}

void onPickCatCorpse(struct InventoryItem* item) {
    if (GameState.catCorpsePicked) {
        return;
    }

    GameState.catCorpsePicked = 1;
    struct JournalEntry* quest = findJournalEntry(QuestMissingCat);
    changeColor(COLOR_CYAN);
    if (quest)  printf("It must be a poor cat, which little girl is looking for.\n");
    else        printf("I wonder if anyone is looking for this cat.\n");

    changeColor(COLOR_RESET);
}

void globalOnPickItem(struct InventoryItem* item) {
    if (item->def == &ITEMD_QUEST_CatCorpse) onPickCatCorpse(item);
}

void pickItem(int idx) {
    struct ListItem* found = numItemFromInventory(idx, here->inventory, "Cannot find item to pick");
    if (!found) {
        return;
    }

    struct InventoryItem* item = (struct InventoryItem*)found->payload;
    printMsgHigh("You pick", item->def->name);
    transferItem(&here->inventory, &inventory, item);
    globalOnPickItem(item);
}

void dropItem(int idx) {
    struct ListItem* found = numItemFromInventory(idx, inventory, "Cannot find item in inventory");
    if (!found) {
        return;
    }

    void* payload = found->payload;
    struct InventoryItem* item = (struct InventoryItem*)payload;
    printMsgHigh("You drop", item->def->name);
    transferItem(&inventory, &here->inventory, payload);
}

void openBox(int idx) {
    struct ListItem* found = numItemFromInventory(idx, here->boxes, "Cannot find such container top open");
    if (!found) {
        return;
    }

    struct Box* box = found->payload;
    box->onOpen(box);
}


void talk(int idx) {
    struct ListItem* found = numItemFromInventory(idx, here->npcs, "Cannot find person with this number");
    if (!found) {
        return;
    }

    struct NPC* npc = found->payload;
    talker = npc;
    npc->onTalk(npc);
    talker = NULL;
}

void defaultOnClosedCallback(int direction) {
    printf("You cannot move %s, path is blocked\n", DIRECTIONNAMES[direction]);
}

void playerDied() {
    printLine();
    printMsg("You have fallen");
    snprintf(input, inputSize, "exit");
}

void prompt() {
    printHealth();
    printMoves();
    printf("$ ");
    if (health > 0) {
        fillInput();
    }
    else {
        playerDied();
    }
}

void onEnterCatacombsEntrySecret() {
    if (GameState.secretPathOpened && prevHere == &loc_WORLD_SECRETENTRY) {
        openSecretPathFromCatacomb();
    }
}

void onEnterAbandonedTempleEntry() {
    if (findPayload(loc_ABANDONED_TEMPLE_ENTRY.npcs, &NPCGhost) && findInventoryItemLike(inventory, &ITEMD_QUEST_GhostAmulet)) {
        printMsg("Ghost stands in your way");
        talk(1);
    }
}

void onEnterTrap() {
    if (!here->visited) {
        printMsg("You make a step and suddenly feel a pain. Your step released a trap");
        printMsg("You are hit for 20 hp\n");
        health -= 20;
    }
}

void onEnterGolem() {
    if (!GameState.golemAttacks) {
        printMsg("Golem sees you, but you come from catacombs, so you are ignored");
    }
}

void onEnterCatacombs3() {
    if (loc_CATACOMBS_1_ENTRY_SECRET.npcs) {
        removeFromList(&loc_CATACOMBS_1_ENTRY_SECRET.npcs, &NPCVampireSlayer);
    }

    if (here->npcs) {
        talk(1);
        removeFromList(&here->npcs, &NPCVampire);
    }
}

void onEnterTownChurch() {
    if (GameState.evilEnding) {
        showEvilOutro();
        exit(0);
    }
}

void triggerSpecialOnEnterLocations() {
    if (here == &loc_TOWN_CHURCH) onEnterTownChurch();
    if (here == &loc_CATACOMBS_3_ENTRY) onEnterCatacombs3();
    if (here == &loc_CHURCH_GOLEM) onEnterGolem();
    if (here == &loc_CATACOMBS_1_ENTRY_SECRET) onEnterCatacombsEntrySecret();
    if (here == &loc_ABANDONED_TEMPLE_ENTRY) onEnterAbandonedTempleEntry();
    if (here->identity == LOCATION_TRAP) onEnterTrap();
}

void globalOnEnterLocation() {
    hostilesAttack();
    triggerSpecialOnEnterLocations();
}

#ifdef WITHDEBUG
// SECTION:DEBUG FUNCTIONS

char* RoadLocationName = "Plain old road";
char* RoadLocationDesc = "It is an old road, has a long history and seen a lot of adventurers";
struct Location* createRoadLocation() {
    return newLocation(RoadLocationName, RoadLocationDesc, '+');
}

// DEMO LOCATIONS
struct Location loc_main;
void l_main() {
    locationInitWithName(&loc_main,
        "Main location",
        "It is a place of where you can head to different places\n"
        "There is nothing interresting here...\n"
        "NORTH: monster playground, watch for rats\n"
        "EAST: items playground, find some helloworlds\n",
        'X'
    );
}

struct Location loc_north;
void l_north() {
    locationInitWithName(&loc_north,
        "North location",
        "",
        'n'
    );
}

struct Location loc_south;
void l_south() {
    locationInitWithName(&loc_south,
        "South location",
        "",
        's'
    );
}

struct Location loc_east;
void l_east() {
    locationInitWithName(&loc_east,
        "East location",
        "",
        'e'
    );
}

struct Location loc_west;
void l_west() {
    locationInitWithName(&loc_west,
        "West location",
        "",
        'w'
    );
}

struct NPC* newNPC(int identity, char* name, char* description, void (*onTalkCallback)(struct NPC* this)) {
    struct NPC* npc = malloc(sizeof(struct NPC));
    memset(npc, 0, sizeof(struct NPC));
    npc->identity = identity;
    npc->name = name;
    npc->description = description;
    npc->onTalk = onTalkCallback;
    return npc;
}

void strangerOnTalk(struct NPC* this) {
    printf("Stranger only nods ... he does not seem to like to talk to strangers\n");
}

int talkerCanSeeThirdOption(struct TalkOption* this) {
    return inventory != NULL;
}

struct TalkTree talkerTalkTree;
struct TalkTree* talkWithAction(struct TalkOption* this) {
    if (inventory == NULL) {
        printf("I see you have nothing of interrest..\n");
        return &talkerTalkTree;
    }
    else {
        printf("Hmm a lot of handy stuff, get out here before I get it all!!\n");
        return NULL;
    }
}

enum {
    TalkerIdentityStartQuest,
    TalkerIdentityEndQuest
};

void cmd_completequest(char* cmd) {
    for (struct ListItem* head = journal; head != NULL; head = head->next) {
        struct JournalEntry* payload = head->payload;

        if (payload->completed == 0) {
            payload->completed = 1;
            return;
        }
    }
}

void cmd_setmoney(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: setmoney N");
    if (num != -1) {
        money = num;
    }
}

void cmd_sethp(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: sethp N");
    if (num != -1) {
        health = num;
    }
}

void cmd_sethostile(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: sethostile 0|1");
    if (num != -1) {
        globalHostile = num;
    }
}

void cmd_briefing(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: briefing [1|2|3]");
    if (num != -1) {
        if (num == 1) {
            showIntro();
        }
        if (num == 2) {
            showOutro();
        }
        if (num == 3) {
            showEvilOutro();
        }
    }
}

int doorOpenClose(int direction, int newState) {
    if (here->moves[direction]) {
        here->moves[direction]->closed = newState;
        return 1;
    }
    else {
        return 0;
    }
}

// helper for JMPS
struct NameToLocation {
    char* key;
    struct Location* value;
    int cursorPos;
};

struct NameToLocation JMPS[] = {
    { "DEMO",           &loc_main, -1 },
    { "CHURCH",         &loc_CHURCH_ENTRY, DRAWMAPOFFSET(5,9) },
    { "C1",             &loc_CATACOMBS_1_ENTRY_MAIN, DRAWMAPOFFSET(10, 4) },
    { "C2",             &loc_CATACOMBS_2_ENTRY, DRAWMAPOFFSET(27,7) },
    { "C3",             &loc_CATACOMBS_3_ENTRY, DRAWMAPOFFSET(19,0) },
    { "AT",             &loc_ABANDONED_TEMPLE_ENTRY, DRAWMAPOFFSET(15,3) },
    { "VD",             &loc_VILLAGE_DUNGEON_ENTRY, DRAWMAPOFFSET(17,0) },
    { "W",              &loc_WORLD_START, DRAWMAPOFFSET(0,0) },
    { "VH",             &loc_VILLAGE_HEALER, DRAWMAPOFFSET(5,1) },
    { "VB",             &loc_VILLAGE_BLACKSMITH, DRAWMAPOFFSET(3,2) },
    { "TC",             &loc_TOWN_CHURCH, DRAWMAPOFFSET(5,7) },
    { "CENTER",         &loc_TOWN_CENTER, DRAWMAPOFFSET(7,5) },
    { NULL, NULL}
};

void cmd_jmpto(char* cmd) {
    char* argIdx = findArg(cmd, 1);
    if (argIdx) {
        if (findArg(argIdx, 1)) {
            printf("Expected 1 argument, got more...\n");
            return;
        }

        struct NameToLocation* each = JMPS;
        struct Location* target = NULL;
        for (;;) {
            if (each->key == NULL) {
                break;
            }
            else if (0 == strcmp(argIdx, each->key)) {
                target = each->value;
                break;
            }
            else {
                ++each;
            }
        }

        if (!target) {
            printf("Cannot find location %s\n", argIdx);
        }
        else {
            jmpTo(target, each->cursorPos);
        }

    }
    else {
        printf("use: jmpto <LOCATION>\n");
        printf("Locations: \n");
        struct NameToLocation* each = JMPS;
        for (;;) {
            if (each->key) {
                printf("%s\n", each->key);
                ++each;
            }
            else break;
        }
    }
}

void BOT_run(char* src[]) {
    for (int i = 0; src[i] != NULL; i++) {
        appendToList(&BOTInput, src[i]);
    }
}

void cmd_bot(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: bot NUM");
    if (num != -1) {
        if (num == 1) BOT_run(BOTSequence_ShowHelp);
        else if (num == 2) BOT_run(BOTSequence_WalkDemo);
        else if (num == 3) BOT_run(BOTSequence_WalkMain);
        else if (num == 4) BOT_run(BOTSequence_FinalFight);
        else {
            printf("Not such program\n");
        }
    }
    else {
        printf("Supported bots:\n");
        printf("1) ShowHelp\n");
        printf("2) WalkDemo\n");
        printf("3) WalkMain\n");
        printf("4) FinalFight\n");
    }
}

void constructLocDemo() {
    l_main();
    l_north();
    l_east();
    l_south();
    l_west();
}

void initDemoLocations() {
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_WEAPON_SwordLvl1));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_WEAPON_SwordLvl2));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_WEAPON_SwordLvl3));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_WEAPON_SwordLvl4));

    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl1));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl2));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl3));
    appendToList(&loc_main.inventory, newInventoryItem(&ITEMD_ARMOR_ArmorLvl4));

    appendToList(&loc_south.monsters, newMonster(&MONSTERDEF_SmallRat));
    appendToList(&loc_south.monsters, newMonster(&MONSTERDEF_Rat));
    appendToList(&loc_south.monsters, newMonster(&MONSTERDEF_Wolf));
    appendToList(&loc_south.monsters, newMonster(&MONSTERDEF_AlphaWolf));
}
#endif

#define GLOBALLOCATIONLISTSIZE 150
struct Location* globalLocationList[GLOBALLOCATIONLISTSIZE];
int locationIdx = 0;


void initGlobalLocationList() {
    memset(globalLocationList, 0, GLOBALLOCATIONLISTSIZE * sizeof(struct Location*));
}

struct ListItem* globalItemDefList;
void initGlobalItemDefList() {
    globalItemDefList = 0;
    appendToList(&globalItemDefList, &ITEMD_WEAPON_SwordLvl1);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_SwordLvl2);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_SwordLvl3);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_SwordLvl4);
    appendToList(&globalItemDefList, &ITEMD_ARMOR_ArmorLvl1);
    appendToList(&globalItemDefList, &ITEMD_ARMOR_ArmorLvl2);
    appendToList(&globalItemDefList, &ITEMD_ARMOR_ArmorLvl3);
    appendToList(&globalItemDefList, &ITEMD_ARMOR_ArmorLvl4);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_FakeSilverSword);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_SilverSword);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_WoodenPeg);
    appendToList(&globalItemDefList, &ITEMD_KEY_KeyToChurch);
    appendToList(&globalItemDefList, &ITEMD_QUEST_CatCorpse);
    appendToList(&globalItemDefList, &ITEMD_QUEST_WoodenRing);
    appendToList(&globalItemDefList, &ITEMD_WEAPON_PaladinsDarkSword);
    appendToList(&globalItemDefList, &ITEMD_HEALING_Ale);
    appendToList(&globalItemDefList, &ITEMD_HEALING_Wine);
    appendToList(&globalItemDefList, &ITEMD_HEALING_Lagger);
    appendToList(&globalItemDefList, &ITEMD_HEALING_HealingPotion);
    appendToList(&globalItemDefList, &ITEMD_BOMB_SparklingBooze);
    appendToList(&globalItemDefList, &ITEMD_BOMB_FlameLizard);
    appendToList(&globalItemDefList, &ITEMD_BOMB_RatShot);
    appendToList(&globalItemDefList, &ITEMD_QUEST_GhostAmulet);
    appendToList(&globalItemDefList, &ITEMD_ITEM_GoldenGoblet);
    appendToList(&globalItemDefList, &ITEMD_ITEM_GoldenRing);
    appendToList(&globalItemDefList, &ITEMD_ITEM_Ruby);
    appendToList(&globalItemDefList, &ITEMD_ITEM_SilverNeckless);
    appendToList(&globalItemDefList, &ITEMD_ITEM_VeryOldBook);
    appendToList(&globalItemDefList, &ITEMD_ITEM_Wine);
}

struct ListItem* globalMonsterDefList;
void initGlobalMonsterDefList() {
    globalMonsterDefList = 0;
    appendToList(&globalMonsterDefList, &MONSTERDEF_Vampire);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Ghost);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Golem);

    appendToList(&globalMonsterDefList, &MONSTERDEF_SmallRat);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Rat);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Wolf);
    appendToList(&globalMonsterDefList, &MONSTERDEF_AlphaWolf);
    appendToList(&globalMonsterDefList, &MONSTERDEF_RatKing);
    appendToList(&globalMonsterDefList, &MONSTERDEF_LesserSceleton);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Sceleton);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Ghoul);
    appendToList(&globalMonsterDefList, &MONSTERDEF_Zombie);
}

#define GLOBALJOURNALENTRYLISTSIZE 10
struct JournalEntry* globalJournalEntryList[GLOBALJOURNALENTRYLISTSIZE];
void initGlobalJournalEntryList() {
    memset(globalJournalEntryList, 0, sizeof(struct JournalEntry*) * GLOBALJOURNALENTRYLISTSIZE);
    globalJournalEntryList[QuestMain] = &QUEST_Main;
    globalJournalEntryList[QuestMissingCat] = &QUEST_MissingCat;
    globalJournalEntryList[QuestMissingPaladinInDarkForest] = &QUEST_MissingPaladin;
    globalJournalEntryList[QuestReleaseGhost] = &QUEST_Ghost;
    globalJournalEntryList[QuestFarmerRatsProblem] = &QUEST_Farmer;
}

#define GLOBALNPCSLISTSIZE 20
struct NPC* globalNPCList[GLOBALNPCSLISTSIZE];
void initGlobalNPCList() {
    memset(globalNPCList, 0, sizeof(struct NPC*) * GLOBALNPCSLISTSIZE);

    globalNPCList[Identity_NPCFarmer] = &NPCFarmer;
    globalNPCList[Identity_NPCGhost] = &NPCGhost;
    globalNPCList[Identity_NPCTownArmorer] = &NPCTownArmorer;
    globalNPCList[Identity_NPCTownHealer] = &NPCTownHealer;
    globalNPCList[Identity_NPCTownPriest] = &NPCTownPriest;
    globalNPCList[Identity_NPCTownSmith] = &NPCTownSmith;
    globalNPCList[Identity_NPCTownTavernBartender] = &NPCTownTavernBartender;
    globalNPCList[Identity_NPCVampire] = &NPCVampire;
    globalNPCList[Identity_NPCVampireSlayer] = &NPCVampireSlayer;
    globalNPCList[Identity_NPCVillageBlacksmith] = &NPCVillageBlacksmith;
    globalNPCList[Identity_NPCVillageElder] = &NPCVillageElder;
    globalNPCList[Identity_NPCVillageEldersDaughter] = &NPCVillageEldersDaughter;
    globalNPCList[Identity_NPCVillageHerbalist] = &NPCVillageHerbalist;
    globalNPCList[Identity_NPCVillageShopkeeper] = &NPCVillageShopkeeper;
}


//#ifdef WITHDEBUG

void printTalkOption(struct TalkOption* ptr) {
    for (;ptr->message;ptr++) {
        printf("%s\n", ptr->message);
        if (ptr->response) {
            printf("%s\n", ptr->response);
        }
    }
}

void cmd_alltexts(char* cmd) {
    printf("LOCATIONS\n");
    for (int i = 0; i < GLOBALLOCATIONLISTSIZE; i++) {
        struct Location* loc = globalLocationList[i];
        if (loc) {
            printf("%s\n%s\n\n", loc->name, loc->description);
        }
        else break;
    }

    printf("ITEMS\n");
    for (struct ListItem* head = globalItemDefList; head != NULL; head = head->next) {
        struct InventoryItemDefinition* each = head->payload;
        printf("%s\n%s\n\n", each->name, each->description);
    }

    printf("MONSTERS\n");
    for (struct ListItem* head = globalMonsterDefList; head != NULL; head = head->next) {
        struct MonsterDefinition* each = head->payload;
        printf("%s\n\n", each->name);
    }

    printf("NPCs\n");
    for (int i = 0; i < GLOBALNPCSLISTSIZE; i++) {
        struct NPC* npc = globalNPCList[i];
        if (npc) {
            printf("%s\n%s\n\n", npc->name, npc->description);
        }
        else break;
    }

    printf("JOURNAL\n");
    for (int i = 0; i < GLOBALJOURNALENTRYLISTSIZE; i++) {
        struct JournalEntry* journal = globalJournalEntryList[i];
        if (journal) {
            printf("%s\n%s\n\n", journal->title, journal->description);
        }
        else break;
    }

    struct TalkOption* talkOptions[] = {
        TALKOptions_VillageElder,
        TALKOptions_VillageShopKeeper_1,
        TALKOptions_VillageEldersDaughter_2,
        TALKOptions_VillageEldersDaughter_1,
        TALKOptions_VillageBlacksmith_1,
        TALKOptions_VillageHerbalist,
        TALKOptions_Ghost_1,
        TALKOptions_Ghost_2,
        TALKOptions_Farmer_2,
        TALKOptions_Farmer_1,
        TALKOption_TownSmith,
        TALKOption_TownArmorer,
        TALKOption_HealAtHealer,
        TALKOption_TownHealer,
        TALKOptions_Priest_1,
        TALKOptions_Bartender,
        TALKOptions_VampireSlayer,
        TALKOptions_Vampire,
        NULL
    };

    printf("TALK OPTIONS\n");
    for (int i=0;;i++) {
        struct TalkOption* each = talkOptions[i];
        if (each) {
            printTalkOption(each);
        }
        else {
            break;
        }
    }

    struct TalkTree* talkTrees[] = {
        &TALKTree_VillageElder,
        &TALKTree_VillageShopKeeper_1,
        &TALKTree_VillageEldersDaughter_2,
        &TALKTree_VillageEldersDaughter_1,
        &TALKTree_VillageBlacksmith_1,
        &TALKTree_VillageHerbalist,
        &TALKTree_Ghost_1,
        &TALKTree_Ghost_2,
        &TALKTree_Farmer_2,
        &TALKTree_Farmer_1,
        &TALKTree_TownSmith,
        &TALKTree_TownArmorer,
        &TALKTree_HealAtHealer,
        &TALKTree_TownHealer,
        &TALKTree_Priest_1,
        &TALKTree_Bartender,
        &TALKTree_VampireSlayer,
        &TALKTree_Vampire,
        NULL
    };

    printf("TALK TREE\n");
    for (int i=0;;i++) {
        struct TalkTree* each = talkTrees[i];
        if (each) {
            printf("%s\n", each->welcome);
        }
        else {
            break;
        }
    }
}
//#endif

struct Location* symbolToLocation(char symbol) {
#ifdef WITHDEBUG
    //DEMO
    if (symbol == 'x') return &loc_main;
    else if (symbol == 'N') return &loc_north;
    else if (symbol == 'E') return &loc_east;
    else if (symbol == 'S') return &loc_south;
    else if (symbol == 'W') return &loc_west;
    else if (symbol == 'r') return createRoadLocation();
#endif

    //Church
    if (symbol == 'Q') return &loc_CHURCH_ENTRY;
    else if (symbol == 'q') return createChurchInterior();
    else if (symbol == 'G') return &loc_CHURCH_GOLEM;

    //Catacombs_1
    else if (symbol == 'A') return &loc_CATACOMBS_1_ENTRY_MAIN;
    else if (symbol == 'a') return &loc_CATACOMBS_1_ENTRY_SECRET;
    else if (symbol == '$') return createCatacombs1Path();
    else if (symbol == 'z') return createCatacombs1Sceleton();
    else if (symbol == 'Z') return createCatacombs1Rat();
    else if (symbol == '4') return &loc_CATACOMBS_1_CHEST_1;
    else if (symbol == '5') return &loc_CATACOMBS_1_CHEST_2;
    else if (symbol == '6') return &loc_CATACOMBS_1_CHEST_3;
    else if (symbol == 'B') return &loc_CATACOMBS_1_STAIRSDOWN;

    //Catacombs_2
    else if (symbol == 'Y') return createCatacombs2Trap();
    else if (symbol == ',') return createCatacombs2Path();
    else if (symbol == ':') return &loc_CATACOMBS_2_ENTRY;
    else if (symbol == ';') return &loc_CATACOMBS_2_STAIRSDOWN;

    //Catacombs_3
    else if (symbol == '[') return &loc_CATACOMBS_3_ENTRY;
    else if (symbol == '(') return &loc_CATACOMBS_3_PART1;
    else if (symbol == ')') return &loc_CATACOMBS_3_PART2;
    else if (symbol == '^') return &loc_CATACOMBS_3_PART3;
    else if (symbol == '&') return &loc_CATACOMBS_3_FINAL;

    // Abandoned Temple
    else if (symbol == 'T') return &loc_ABANDONED_TEMPLE_ENTRY;
    else if (symbol == 't') return &loc_ABANDONED_TEMPLE_PATH;
    else if (symbol == '1') return &loc_ABANDONED_TEMPLE_SCELETON_1;
    else if (symbol == '2') return &loc_ABANDONED_TEMPLE_SCELETON_2;
    else if (symbol == '3') return &loc_ABANDONED_TEMPLE_SCELETON_3;
    else if (symbol == 'g') return &loc_ABANDONED_TEMPLE_GOAL;

    // Village Dungeon
    else if (symbol == 'V') return &loc_VILLAGE_DUNGEON_ENTRY;
    else if (symbol == 'v') return createVillageDungeonPath();
    else if (symbol == 'D') return &loc_VILLAGE_DUNGEON_CAT;

    // World
    else if (symbol == 's') return &loc_WORLD_START;
    else if (symbol == '!') return createVillagePath();
    else if (symbol == '-') return createWorldPath();
    else if (symbol == 'e') return &loc_VILLAGE_ELDER;
    else if (symbol == 'u') return &loc_VILLAGE_SHOP;
    else if (symbol == 'b') return &loc_VILLAGE_BLACKSMITH;
    else if (symbol == 'h') return &loc_VILLAGE_HEALER;
    else if (symbol == 'w') return &loc_VILLAGE_OLDWELL;

    else if (symbol == 'd') return createDarkForestPath();
    else if (symbol == 'R') return &loc_DARKFOREST_WOLFS;
    else if (symbol == 'C') return &loc_DARKFOREST_PALADINCORPSE;

    else if (symbol == 'l') return &loc_WORLD_ABANDONED_TEMPLE;
    else if (symbol == 'J') return &loc_FARMS_COTTAGE;
    else if (symbol == 'j') return &loc_FARMS_STABLES;
    else if (symbol == 'k') return createFarmsFields();
    else if (symbol == 'K') return createFarmsAbandonedCottage();

    else if (symbol == 'm') return &loc_WORLD_SECRETENTRY;

    else if (symbol == 'M') return &loc_TOWN_CHURCH;
    else if (symbol == '.') return createTownPath();

    else if (symbol == 'O') return &loc_TOWN_GATE;
    else if (symbol == 'o') return &loc_TOWN_SMITH;
    else if (symbol == 'p') return &loc_TOWN_TAVERN;
    else if (symbol == 'U') return &loc_TOWN_ARMORER;
    else if (symbol == 'P') return &loc_TOWN_HEALER;
    else if (symbol == 'X') return &loc_TOWN_CENTER;
    else if (symbol == 'y') return &loc_TOWN_CHAPEL;

    else if (symbol == ' ') return NULL;

    else {
        printf("FATAL ERROR: cannot resolve symbol %c\n", symbol);
        exit(3);
        return NULL;
    }
}

int validateLocationidx(int locationIdx) {
    if (locationIdx >= GLOBALLOCATIONLISTSIZE) {
        printf("PANIC! exceeded global location list\n");
        exit(1);
    }

    return locationIdx;
}

struct Location* loadIdxLocation(int idx) {
    struct Location* loc = globalLocationList[validateLocationidx(idx)];
    if (!loc) {
        printf("PANIC!! pointed to not existing location\n");
        exit(1);
    }

    return loc;
}

struct Location* symbolToLocationIndexed(char symbol) {
    struct Location* l = symbolToLocation(symbol);
    if (l == NULL) {
        return NULL;
    }
    if (l->id == -1) {
        l->id = locationIdx;
        ++locationIdx;
        validateLocationidx(locationIdx);

        globalLocationList[l->id] = l;
    }

    return l;
}

//SECTION: commands

void cmd_n(char* cmd) {
    saveGame();
    enterLocation(NORTH);
}

void cmd_e(char* cmd) {
    saveGame();
    enterLocation(EAST);
}

void cmd_s(char* cmd) {
    saveGame();
    enterLocation(SOUTH);
}

void cmd_w(char* cmd) {
    saveGame();
    enterLocation(WEST);
}

void cmd_t(char* cmd) {
    saveGame();
    enterLocation(TRAVEL);
}

void cmd_inv(char* cmd) {
    showInventory();
}

void cmd_look(char* cmd) {
    look();
    showMap();
}

void cmd_journal(char* cmd) {
    showJournal();
}

void cmd_equip(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: equip [# from inventory]");
    if (num != -1) {
        equipItem(num);
    }
}

void cmd_drink(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: drink [# from inventory]");
    if (num != -1) {
        drinkItem(num);
    }
}

void cmd_unequip(char* cmd) {
    char* argIdx = findArg(cmd, 1);
    char* expected = "use: unequip [weapon|armor]";

    if (argIdx == NULL) {
        printf("%s\n", expected);
        return;
    }

    if (0 == strcmp("armor", argIdx)) {
        unequipItem(ITYPE_ARMOR);
    }
    else if (0 == strcmp("weapon", argIdx)) {
        unequipItem(ITYPE_WEAPON);
    }
    else {
        printf("%s\n", expected);
        return;
    }
}

void cmd_pick(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: pick [# from inventory]");
    if (num != -1) {
        pickItem(num);
    }
}

void cmd_drop(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: drop [# from inventory]");
    if (num != -1) {
        dropItem(num);
    }
}

void cmd_open(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: open [# of container]");
    if (num != -1) {
        openBox(num);
    }
}

void cmd_attack(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: attack [# of monster]");
    if (num != -1) {
        attack(num);
    }
}

void cmd_talk(char* cmd) {
    int num = argToNumOrFail(cmd, 1, "use: talk [# of person]");
    if (num != -1) {
        talk(num);
    }
}

void cmd_credits(char* cmd) {
    printf("Thank you for checking out the game.\n");
    printf("https://github.com/macvek/PaladinTheGame\n\n");
    printf("MIT License\n\nCopyright (c) 2023 Maciej Aleksandrowicz\n");
    printMsg("With Regards, Maciej Aleksandrowicz");
}

void cmd_exit(char* cmd) {
    saveGame();
    printf("bye bye\n");
    parseInputRetCode = 0;
}

void cmd_help(char* cmd);
void printHelpMessage(struct CommandHandler* commandList) {
    for (int i = 0; commandList[i].exactMatch != END_OF_LIST_VALUE; i++) {
        struct CommandHandler* cmd = &commandList[i];
        char* desc = cmd->desc;
        if (!desc) {
            desc = "TODO!";
        }
        if (cmd->testString[1]) {
            printf("%s, %s "CC(C_BBLACK)"%s"CC(C_RST)"\n", cmd->testString[0], cmd->testString[1], desc);
        }
        else {
            printf("%s "CC(C_BBLACK)"%s"CC(C_RST)"\n", cmd->testString[0], desc);
        }
    }
}

void cmd_opendoor() {
    if (here == &loc_CHURCH_ENTRY || here == &loc_TOWN_CHURCH) {
        if (FindInventoryEntry(&ITEMD_KEY_KeyToChurch)) {
            openChurchDoors();
            printMsg("You open church doors.");
        }
        else {
            printMsg("You don't have a key to open church doors.");
        }
    }
    else {
        printMsg("There is no door to open.");
    }
}

struct CommandHandler commands[] = {
    #ifdef WITHDEBUG
    {.testString = {"completequest",NULL},      .callback = cmd_completequest,    .exactMatch = 1, .desc = "DEV" },
    {.testString = {"alltexts",NULL},           .callback = cmd_alltexts,         .exactMatch = 1, .desc = "DEV" },
    {.testString = {"setmoney",NULL},           .callback = cmd_setmoney,         .exactMatch = 0, .desc = "DEV" },
    {.testString = {"sethp",NULL},              .callback = cmd_sethp,            .exactMatch = 0, .desc = "DEV" },
    {.testString = {"sethostile",NULL},         .callback = cmd_sethostile,       .exactMatch = 0, .desc = "DEV" },
    {.testString = {"jmpto",NULL},              .callback = cmd_jmpto,            .exactMatch = 0, .desc = "DEV" },
    {.testString = {"bot",NULL},                .callback = cmd_bot,              .exactMatch = 0, .desc = "DEV" },
    {.testString = {"briefing",NULL},           .callback = cmd_briefing,         .exactMatch = 0, .desc = "DEV" },
    {.testString = {"save",NULL},               .callback = saveGame,             .exactMatch = 1, .desc = "DEV" },
    {.testString = {"load",NULL},               .callback = loadGame,             .exactMatch = 1, .desc = "DEV" },
    #endif

    {.testString = {"?","help"},                .callback = cmd_help,             .exactMatch = 1, .desc = "Show this message"},
    {.testString = {"n","north"},               .callback = cmd_n,                .exactMatch = 1, .desc = "Go north" },
    {.testString = {"e","east"},                .callback = cmd_e,                .exactMatch = 1, .desc = "Go east" },
    {.testString = {"s","south"},               .callback = cmd_s,                .exactMatch = 1, .desc = "Go south" },
    {.testString = {"w","west"},                .callback = cmd_w,                .exactMatch = 1, .desc = "Go west" },
    {.testString = {"t","travel"},              .callback = cmd_t,                .exactMatch = 1, .desc = "Go through special route" },
    {.testString = {"i","inv"},                 .callback = cmd_inv,              .exactMatch = 1, .desc = "Show inventory" },
    {.testString = {"l","look"},                .callback = cmd_look,             .exactMatch = 1, .desc = "Show current location" },
    {.testString = {"j","journal"},             .callback = cmd_journal,          .exactMatch = 1, .desc = "Show journal" },
    {.testString = {"x","exit"},                .callback = cmd_exit,             .exactMatch = 1, .desc = "Save and exit game" },
    {.testString = {"opendoor",NULL},           .callback = cmd_opendoor,         .exactMatch = 1, .desc = "Open closed door" },
    //...//
    {.testString = {"drink"},                   .callback = cmd_drink,            .exactMatch = 0, .desc = "Drink [# from inventory]" },
    {.testString = {"eq","equip"},              .callback = cmd_equip,            .exactMatch = 0, .desc = "Equip item [# from inventory]" },
    {.testString = {"ueq","unequip"},           .callback = cmd_unequip,          .exactMatch = 0, .desc = "Unequip [weapon or armor]" },
    {.testString = {"p","pick"},                .callback = cmd_pick,             .exactMatch = 0, .desc = "Pick [# item from location]" },
    {.testString = {"d","drop"},                .callback = cmd_drop,             .exactMatch = 0, .desc = "Drop [# item from inventory]" },
    {.testString = {"o","open"},                .callback = cmd_open,             .exactMatch = 0, .desc = "Open [# of container]" },
    {.testString = {"a","attack"},              .callback = cmd_attack,           .exactMatch = 0, .desc = "Attack [# of monster]" },
    {.testString = {"t","talk"},                .callback = cmd_talk,             .exactMatch = 0, .desc = "Talk to [# of person]" },
    {.testString = {"credits",NULL},            .callback = cmd_credits,          .exactMatch = 0, .desc = "Credits" },

    //...//
    {.exactMatch = END_OF_LIST_VALUE, .testString = {"END_OF_LIST"} }
};

void cmd_help(char* cmd) {
    printf("General help:\n");
    printHelpMessage(commands);
}

// SECTION:ENGINE fight
int calcHit(int min, int max, int ac) {
    int randVal = randOf(1 + max - min);
    int ret = min + randVal - ac;
    ret = ret < 1 ? 1 : ret;
#ifdef SHOWCALC
    printf("calc: min + randVal(1+max-min) - ac = %i + %i - %i = %i\n", min, randVal, ac, ret);
#endif
    return ret;
}

int equippedAC() {
    return Equipped.armor ? ((struct InventoryItem*)Equipped.armor->payload)->def->armorClass : 0;
}

struct InventoryItemDefinition* equipedWeapon() {
    return Equipped.weapon ? ((struct InventoryItem*)Equipped.weapon->payload)->def : &ITEMD_WEAPON_BareFists;
}

void endFight() {
    removeAllFromList(&enemies);
    enemy = NULL;
}

void nextEnemy() {
    if (enemies && enemy == NULL) {
        enemy = enemies->payload;
        printf("You now fight with "CC(C_FIGHT_MSGHIGH)"%s\n"CC(C_RST), enemy->def->name);
    }
}

void fightKillEnemy() {
    removeFromList(&here->monsters, enemy);
    removeFromList(&enemies, enemy);
    freeMonster(enemy);
    enemy = NULL;
    nextEnemy();
}

void joinOtherMonsters() {
    for (struct ListItem* head = here->monsters; head != NULL; head = head->next) {
        struct Monster* locationMonster = head->payload;
        if (locationMonster->def->joinsFight && !findPayload(enemies, locationMonster)) {
            appendToList(&enemies, locationMonster);
            printf(CC(C_RST)"Monster "CC(C_FIGHT_MSGHIGH)"%s"CC(C_FIGHT_MSG)" joins fight\n", locationMonster->def->name);
        }
    }
    nextEnemy();
}

void fightEnemiesRound() {
    joinOtherMonsters();
    for (struct ListItem* head = enemies; head != NULL; head = head->next) {
        struct Monster* currentEnemy = head->payload;
        if (currentEnemy->def == &MONSTERDEF_Vampire && currentEnemy->hp == 1) {
            printf("Vampire's body lays unconcious on the floor\n");
            return;
        }

        if (currentEnemy->stun) {
            printf("~~ %s is stunned ~~\n", currentEnemy->def->name);
            --currentEnemy->stun;
        }
        else {
            int hit = calcHit(currentEnemy->def->minAttack, currentEnemy->def->maxAttack, equippedAC());
            health -= hit;
            printf("~~ You are hit by "CC(C_BRED)"%s"CC(C_RED)"[%i/%i]"CC(C_RST)" for "CC(C_BRED)"%i"CC(C_RST)" hp ~~\n",
                currentEnemy->def->name,
                currentEnemy->hp, currentEnemy->def->maxHp
                , hit);
        }

        if (health <= 0) {
            endFight();
            return;
        }
    }
}


void onVampireLaying() {
    printMsg("Vampire lays on the ground. He looks dead. Does not breath.");
    printMsgHigh("But he is not dead yet.", "One last step remains to release him.");
}

void onVampireImmutable() {
    printMsg("You hit vampire. He looks almost dead, but for some reason, your attacks do not affect him. He still attacks.");
}

void onVampireFinish() {
    GameState.goodEnding = 1;
    printMsg("You hit a vampire with a wooden peg. Straight to the heart.");
    nextPage();
    printf(
        CC(C_BCYAN)"He releases his last breath.\n"
        "His body in an instance changes into a pile of ash.\n");
    nextPage();
    printf(
        CC(C_BWHITE)"Blast of wind comes and ash disappears.\n"
        "His misery is over...\n"
        CC(C_RST));
    nextPage();
    completeQuest(&QUEST_Main);
}

void commitAttack(struct InventoryItemDefinition* weapon) {
    int hit = calcHit(weapon->minAttack, weapon->maxAttack, enemy->def->armorClass);
    if (enemy->def != &MONSTERDEF_Vampire || enemy->hp > 5) {
        enemy->hp -= hit;
        if (enemy->def == &MONSTERDEF_Vampire && enemy->hp < 5) {
            enemy->hp = 5;  // hp=5 for vampire is an entry to a special mode
        }
        printf(CC(C_FIGHT_YOU)"> You hit "CC(C_FIGHT_MSGHIGH)"%s"CC(C_FIGHT_YOU)" using "CC(C_MSGHIGH)"%s"CC(C_FIGHT_YOU)" for "CC(C_FIGHT_MSGHIGH)"%i"CC(C_FIGHT_YOU)" hp\n"CC(C_RST), enemy->def->name, weapon->name, hit);
    }
    else if (enemy->def == &MONSTERDEF_Vampire && enemy->hp <= 5) {
        if (weapon == &ITEMD_WEAPON_WoodenPeg && enemy->hp == 1) {
            onVampireFinish();
            enemy->hp = 0;
        }
        else if (enemy->hp == 1 || weapon == &ITEMD_WEAPON_SilverSword) {
            onVampireLaying();
            enemy->hp = 1;
        }
        else {
            onVampireImmutable();
        }
    }

    if (enemy->hp <= 0) {
        printf(CC(C_FIGHT_MSG)"~~ "CC(C_FIGHT_MSGHIGH)"%s"CC(C_FIGHT_MSG)" is slayed ~~\n"CC(C_RST), enemy->def->name);
        fightKillEnemy();
    }
}

void onBomb(struct InventoryItemDefinition* bomb) {
    if (bomb == &ITEMD_BOMB_SparklingBooze || bomb == &ITEMD_BOMB_FlameLizard) {
        commitAttack(bomb);
    }
    else if (bomb == &ITEMD_BOMB_RatShot) {
        if (enemy->def == &MONSTERDEF_Rat || enemy->def == &MONSTERDEF_RatKing || enemy->def == &MONSTERDEF_SmallRat) {
            enemy->stun = bomb->minAttack + randOf(bomb->maxAttack - bomb->minAttack);
            printf(CC(C_FIGHT_YOU)"> "CC(C_FIGHT_MSGHIGH)"%s"CC(C_FIGHT_YOU)" is stunned for "CC(C_MSGHIGH)"%i"CC(C_FIGHT_YOU)" rounds\n"CC(C_RST), enemy->def->name, enemy->stun);
        }
        else {
            printf(CC(C_DISABLED)"> %s is not affected by %s\n"CC(C_RST), enemy->def->name, bomb->name);
        }
    }
    else {
        printf("!!! PANIC. Running onBomb on unsupported bomb: %s, aborting\n", bomb->saveName);
        exit(1);
    }
}

void useBomb(struct InventoryItem* item) {
    printf(CC(C_MSG)"~~ You throw "CC(C_MSGHIGH)"%s"CC(C_MSG)" at your enemies ~~\n", item->def->name);

    struct InventoryItemDefinition* bomb = item->def;

    struct Monster* currentEnemy = enemy;
    struct ListItem* nextEnemy = enemies;
    while (nextEnemy) {
        // pick here, as enemy can be killed and the removed from list.
        enemy = nextEnemy->payload;
        nextEnemy = nextEnemy->next;

        onBomb(bomb);
    }

    if (findPayload(enemies, currentEnemy)) {
        enemy = currentEnemy;
    }

    struct ListItem* removeMe = NULL;
    transferItem(&inventory, &removeMe, item);

    if (removeMe) {
        freeListItem(removeMe);
    }
}

void fightActionUse() {
    for (;;) {
        showEquipStatus();
        printf("> What do you want to use?\n"CC(C_TALK_EXIT)"#0 back\n"CC(C_RST));
        indexedListInventoryWithMode(inventory, SHOW_USE);
        printf("use #?> ");
        fillInput();
        int num = readInt(input);
        if (num <= 0) {
            return;
        }

        struct InventoryItem* itemPayload = findInventoryIndexedItem(inventory, num);
        if (itemPayload == NULL) {
            continue;
        }

        if (itemPayload->def->type == ITYPE_WEAPON || itemPayload->def->type == ITYPE_ARMOR) {
            equipItem(num);
        }
        else if (itemPayload->def->type == ITYPE_HEALING) {
            drinkItem(num);
        }
        else if (itemPayload->def->type == ITYPE_BOMB) {
            useBomb(itemPayload);
        }
        else {
            printMsg("You cannot use it during a fight");
            continue;
        }

        fightEnemiesRound();
        return;
    }
}

void fightPrompt() {
    printHealth();
    printFightMoves();
    printf("@ ");
    fillInput();
}

void fightActionAttack() {
    struct InventoryItemDefinition* weapon = equipedWeapon();

    if (weapon == &ITEMD_WEAPON_FakeSilverSword) {
        struct InventoryItem* toRemove = Equipped.weapon->payload;
        removeFromList(&Equipped.weapon, toRemove);
        freeInventoryItem(toRemove);
        printMsg("You try to use the silver sword, but it breaks into thousands of small shiny pieces.");
    }
    else {
        commitAttack(weapon);
    }
    fightEnemiesRound();
}

void fightActionFlee() {
    printFightMsg("You managed to flee from a fight to your previous location");
    endFight();
    enterLocation(fleeDirection);
}

void fightActionTarget() {
    printMsg("Pick your target");

    int idx = 0;
    for (struct ListItem* head = enemies; head != NULL; head = head->next) {
        struct Monster* monster = head->payload;
        ++idx;
        printf("%i) %s [%i/%i]\n", idx, monster->def->name, monster->hp, monster->def->maxHp);
    }

    for (;;) {
        printf("target #?> ");
        fillInput();
        int num = readInt(input);
        if (num <= 0 || num > idx) {
            printf("Cannot find such monster. Try again.\n");
            continue;
        }
        struct ListItem* picked = enemies;
        for (int i = 1; i < num; i++, picked = picked->next);

        enemy = picked->payload;
        break;
    }

    printMsgHigh("Your target is", enemy->def->name);
}

void cmd_fight_attack(char* cmd) {
    fightActionAttack();
}

void cmd_fight_use(char* cmd) {
    fightActionUse();
}

void cmd_fight_flee(char* cmd) {
    fightActionFlee();
}

void cmd_fight_target(char* cmd) {
    fightActionTarget();
}

void cmd_fight_help(char* cmd);
struct CommandHandler fightCommands[] = {
    {.testString = {"?", "help"},   .callback = cmd_fight_help,   .exactMatch = 1, .desc = "Show this message" },
    {.testString = {"a", "attack"}, .callback = cmd_fight_attack, .exactMatch = 1, .desc = "Attack current target" },
    {.testString = {"u", "use"},    .callback = cmd_fight_use,    .exactMatch = 1, .desc = "Use or equip item" },
    {.testString = {"t", "target"}, .callback = cmd_fight_target, .exactMatch = 1, .desc = "Select current target" },
    {.testString = {"f", "flee"},   .callback = cmd_fight_flee,   .exactMatch = 1, .desc = "Flee from a fight" },
    //...//
    {.exactMatch = END_OF_LIST_VALUE, .testString = {"END_OF_LIST"} }
};

void cmd_fight_help(char* cmd) {
    printf("Fight mode help:\n");
    printHelpMessage(fightCommands);
}

int parseFightInput() {
    return parseInputAsCommands(fightCommands);
}

void fightMode(struct Monster* attackIt, int userAttacks) {
    enemy = attackIt;
    appendToList(&enemies, enemy);
    if (userAttacks) {
        printf("You enter fight against "ASCOLOR(COLOR_RED)"%s"ASCOLOR(COLOR_RESET)"\n", enemy->def->name);
    }
    else {
        printf("You are attacked by "ASCOLOR(COLOR_RED)"%s"ASCOLOR(COLOR_RESET)"\n", enemy->def->name);
        fightEnemiesRound();
    }

    while (enemy) {
        fightPrompt();
        if (!parseFightInput()) {
            printf("%s\n", HelpPrompt);
        }
    }
}


void attack(int idx) {
    struct ListItem* found = numItemFromInventory(idx, here->monsters, "Cannot find such monster");
    if (!found) {
        return;
    }

    struct Monster* monster = found->payload;
    fightMode(monster, 1);
}

int GolemWouldAttack() {
    GameState.golemAttacks = GameState.golemAttacks && prevHere != &loc_CATACOMBS_1_ENTRY_MAIN;
    return GameState.golemAttacks;

}

void hostilesAttack() {
    struct Monster* attacker = NULL;
    for (struct ListItem* each = here->monsters; each != NULL; each = each->next) {
        struct Monster* monster = each->payload;
        if (monster->def->hostile || (monster->def == &MONSTERDEF_Golem && GolemWouldAttack())) {
            attacker = monster;
            break;
        }
    }

#ifdef WITHDEBUG
    if (globalHostile)

#endif
        if (attacker) {
            fightMode(attacker, 0);
        }



}

// SECTION: setups
void constructLocChurch() {
    l_CHURCH_ENTRY();
    l_CHURCH_GOLEM();
}

void constructLocCatacombs_1() {
    l_CATACOMBS_1_ENTRY_MAIN();
    l_CATACOMBS_1_ENTRY_SECRET();
    l_CATACOMBS_1_CHEST_1();
    l_CATACOMBS_1_CHEST_2();
    l_CATACOMBS_1_CHEST_3();
    l_CATACOMBS_1_CHEST_STAIRSDOWN();
}


void constructLocCatacombs_2() {
    l_CATACOMBS_2_ENTRY();
    l_CATACOMBS_2_STAIRSDOWN();
}

void constructLocCatacombs_3() {
    l_CATACOMBS_3_ENTRY();
    l_CATACOMBS_3_PART1();
    l_CATACOMBS_3_PART2();
    l_CATACOMBS_3_PART3();
    l_CATACOMBS_3_FINAL();
}

void constructAbandonedTemple() {
    l_ABANDONED_TEMPLE_ENTRY();
    l_ABANDONED_TEMPLE_PATH();
    l_ABANDONED_TEMPLE_SCELETON_1();
    l_ABANDONED_TEMPLE_SCELETON_2();
    l_ABANDONED_TEMPLE_SCELETON_3();
    l_ABANDONED_TEMPLE_GOAL();
}

void constructVillageDungeon() {
    l_VILLAGE_DUNGEON_ENTRY();
    l_VILLAGE_DUNGEON_CAT();
}

void constructWorldMap() {
    l_WORLD_START();

    l_VILLAGE_ELDER();
    l_VILLAGE_SHOP();
    l_VILLAGE_BLACKSMITH();
    l_VILLAGE_HEALER();
    l_VILLAGE_OLDWELL();

    l_DARKFOREST_PALLADINCORPSE();
    l_DARKFOREST_WOLFS();

    l_WORLD_ABANDONED_TEMPLE();
    l_FARMS_COTTAGE();
    l_FARMS_STABLES();

    l_WORLD_SECRETENTRY();

    l_TOWN_CHURCH();
    l_TOWN_CHAPEL();
    l_TOWN_GATE();
    l_TOWN_SMITH();
    l_TOWN_HEALER();
    l_TOWN_ARMORER();
    l_TOWN_TAVERN();
    l_TOWN_CENTER();
}

void addTravelJoints() {
    joinLocations(&loc_WORLD_ABANDONED_TEMPLE, &loc_ABANDONED_TEMPLE_ENTRY, DRAWMAPOFFSET(2, 0));
    joinLocations(&loc_VILLAGE_OLDWELL, &loc_VILLAGE_DUNGEON_ENTRY, DRAWMAPOFFSET(12, -2));
    joinLocations(&loc_TOWN_CHURCH, &loc_CHURCH_ENTRY, DRAWMAPOFFSET(0, 2));
    joinLocations(&loc_CHURCH_GOLEM, &loc_CATACOMBS_1_ENTRY_MAIN, DRAWMAPOFFSET(3, -5));
    joinLocations(&loc_CATACOMBS_1_STAIRSDOWN, &loc_CATACOMBS_2_ENTRY, DRAWMAPOFFSET(13, -1));
    joinLocations(&loc_CATACOMBS_2_STAIRSDOWN, &loc_CATACOMBS_3_ENTRY, DRAWMAPOFFSET(-9, -9));
}

void churchDoorsAreClosed(int direction) {
    printMsg("Doors are closed. You need a key to open it.");
}

void applyTravelLocks() {
    loc_CHURCH_ENTRY.moves[TRAVEL]->closed = 1;
    loc_CHURCH_ENTRY.moves[TRAVEL]->onClosedCallback = churchDoorsAreClosed;
    loc_TOWN_CHURCH.moves[TRAVEL]->closed = 1;
    loc_TOWN_CHURCH.moves[TRAVEL]->onClosedCallback = churchDoorsAreClosed;

}

void deleteTravelJoints() {
    unjoinLocations(&loc_WORLD_SECRETENTRY, EAST, WEST);
}

void constructLocations() {
    constructLocChurch();
    constructLocCatacombs_1();
    constructLocCatacombs_2();
    constructLocCatacombs_3();
    constructAbandonedTemple();
    constructVillageDungeon();
    constructWorldMap();

#ifdef WITHDEBUG
    constructLocDemo();
#endif
}

void initNPCs() {
    initNPC(Identity_NPCVillageElder, &NPCVillageElder, "Village elder", "An old man, he is a host of the village", TALK_VillageElder, &loc_VILLAGE_ELDER);
    initNPC(Identity_NPCVillageShopkeeper, &NPCVillageShopkeeper, "Shop keeper", "She is elder's wife", TALK_VillageShopKeeper, &loc_VILLAGE_SHOP);
    initNPC(Identity_NPCVillageEldersDaughter, &NPCVillageEldersDaughter, "Little girl", "Elder's daughter", TALK_VillageEldersDaughter, &loc_VILLAGE_SHOP);
    initNPC(Identity_NPCVillageBlacksmith, &NPCVillageBlacksmith, "Blacksmith", "Elder's son, he is a blacksmith", TALK_VillageBlacksmith, &loc_VILLAGE_BLACKSMITH);

    initNPC(Identity_NPCVillageHerbalist, &NPCVillageHerbalist, "Herbalist", "He has a cottage and is surrounded by herbs", TALK_VillageHerbalist, &loc_VILLAGE_HEALER);
    initNPC(Identity_NPCGhost, &NPCGhost, "Ghost", "Poor soul trapped in dungeons", TALK_Ghost, &loc_ABANDONED_TEMPLE_ENTRY);
    initNPC(Identity_NPCFarmer, &NPCFarmer, "Farmer", "He works on a farm", TALK_Farmer, &loc_FARMS_COTTAGE);

    initNPC(Identity_NPCTownSmith, &NPCTownSmith, "Smith", "Short, but well built man, his face is full of dark beard.", TALK_TownSmith, &loc_TOWN_SMITH);
    initNPC(Identity_NPCTownArmorer, &NPCTownArmorer, "Armorer", "He makes best armours", TALK_TownArmorer, &loc_TOWN_ARMORER);
    initNPC(Identity_NPCTownHealer, &NPCTownHealer, "Healer", "He works in a hospital", TALK_Healer, &loc_TOWN_HEALER);
    initNPC(Identity_NPCTownPriest, &NPCTownPriest, "Priest", "The servant of God. Humble and silent.", TALK_Priest, &loc_TOWN_CHAPEL);
    initNPC(Identity_NPCTownTavernBartender, &NPCTownTavernBartender, "Bartender", "The soul of local tavern", TALK_TownTavernBartender, &loc_TOWN_TAVERN);

    initNPC(Identity_NPCVampireSlayer, &NPCVampireSlayer, "Stranger", "wears black hood and black outfit", TALK_VamireSlayer, &loc_CATACOMBS_1_ENTRY_SECRET);
    initNPC(Identity_NPCVampire, &NPCVampire, "Vampire", "Lord of the darkness", TALK_Vampire, &loc_CATACOMBS_3_ENTRY);
}

void initMainQuest() {
    addQuest(&QUEST_Main);
}

int parseInput() {
    return parseInputAsCommands(commands);
}

//SECTION: SAVE UTILS
struct {
    FILE* f;
    struct Location* location;
    struct ListItem** itemList;
    struct ListItem** monsters;
    struct ListItem** npcs;
    struct ListItem** boxes;

} SaveState;

void mapToSaveable() {
    for (int i = 0; i < DrawMapSize; i++) {
        if (DrawMap[i] == 0) {
            DrawMap[i] = '\n';
        }
    }
}

void mapFromSaveable() {
    for (int i = 0; i < DrawMapSize; i++) {
        if (DrawMap[i] == '\n') {
            DrawMap[i] = 0;
        }
    }
}

enum {
    SECTION_CURSOR = 1,
    SECTION_MAP
};

#define SECTIONBUFFERSIZE 80
char sectionBuffer[SECTIONBUFFERSIZE + 1];
char* sectionStack = 0;

char* sectionPop() {
    char* ret = sectionStack;
    char* end = &sectionBuffer[SECTIONBUFFERSIZE];

    if (*ret == 0) {
        char* ptr = sectionBuffer;
        for (; ptr < ret; ++ptr) {
            if (*ptr == 0) *ptr = ' ';
        }
        printf("PANIC!! trying to pop from empty stack: %s\n", sectionBuffer);
        exit(1);
    }

    for (; sectionStack < end; ++sectionStack) {
        if (*sectionStack == ' ') {
            *sectionStack = 0;
            ++sectionStack;
            break;
        }
    }

    return ret;
}

int sectionPopInt() {
    return atoi(sectionPop());
}

void readSaveFileLine() {
    sectionStack = sectionBuffer;
    int i = 0;

    for (; i < SECTIONBUFFERSIZE; i++) {
        char c = fgetc(SaveState.f);
        if (c == '\n') break;
        if (c == 0) {
            printf("PANIC!! FOUND EOF in save file; which is never expected");
            exit(1);
        }
        sectionBuffer[i] = c;
    }

    if (i == SECTIONBUFFERSIZE) {
        sectionBuffer[SECTIONBUFFERSIZE] = 0;
        printf("PANIC!!; reached end of readSection buffer, aborting: %s buffer\n", sectionBuffer);
        exit(1);
    }

    sectionBuffer[i] = 0;
}

void loadMap() {
    if (1 != fread(DrawMap, DrawMapSize, 1, SaveState.f)) {
        printf("PANIC! Unexpected end of file while reading map.. aborting\n");
        exit(1);
    }

    if (fgetc(SaveState.f) != '\n') {
        printf("PANIC! EXPECTED newline after map.. aborting\n");
        exit(1);
    }

    readSaveFileLine();

    if (strcmp("MAPEND", sectionPop())) {
        printf("PANIC! EXPECTED MAPEND.. aborting\n");
        exit(1);
    }

    mapFromSaveable();

}

void saveMap() {
    mapToSaveable();
    fprintf(SaveState.f, "MAP\n%s\n", DrawMap);
    mapFromSaveable();
    fprintf(SaveState.f, "MAPEND\n");
}

void saveHere() {
    fprintf(SaveState.f, "HERE %i %i %i\n", here->id, prevHere->id, fleeDirection);
}

void loadHere() {
    here = loadIdxLocation(sectionPopInt());
    prevHere = loadIdxLocation(sectionPopInt());
    fleeDirection = sectionPopInt();
}

struct InventoryItemDefinition* loadGlobalItemDef(char* itemSaveName) {
    for (struct ListItem* head = globalItemDefList; head != NULL; head = head->next) {
        struct InventoryItemDefinition* def = head->payload;
        if (!strcmp(itemSaveName, def->saveName)) {
            return def;
        }
    }

    printf("PANIC!! Unknown item definition, aborting: %s\n", itemSaveName);
    exit(1);
}

void loadItem() {
    char* itemSaveName = sectionPop();
    int count = sectionPopInt();
    struct InventoryItemDefinition* def = loadGlobalItemDef(itemSaveName);
    appendToList(SaveState.itemList, newInventoryItemCount(def, count));
}

struct MonsterDefinition* loadGlobalMonsterDef(char* monsterSaveName) {
    for (struct ListItem* head = globalMonsterDefList; head != NULL; head = head->next) {
        struct MonsterDefinition* def = head->payload;
        if (!strcmp(monsterSaveName, def->saveName)) {
            return def;
        }
    }

    printf("PANIC!! Unknown monster definition, aborting: %s\n", monsterSaveName);
    exit(1);
}

void loadMonster() {
    char* monsterSaveName = sectionPop();
    int monsterHp = sectionPopInt();
    struct Monster* monster = newMonster(loadGlobalMonsterDef(monsterSaveName));
    monster->hp = monsterHp;

    appendToList(SaveState.monsters, monster);
}

void loadWeapon() {
    SaveState.itemList = &Equipped.weapon;
}

void loadArmor() {
    SaveState.itemList = &Equipped.armor;
}

void loadNPC() {
    int npcIdentifier = sectionPopInt();

    if (npcIdentifier < 0 || npcIdentifier >= GLOBALNPCSLISTSIZE || !globalNPCList[npcIdentifier]) {
        printf("PANIC!! Unsupported NPC %i, aborting\n", npcIdentifier);
        exit(1);
    }

    struct NPC* npc = globalNPCList[npcIdentifier];
    removeAllFromList(&npc->inventory);

    appendToList(SaveState.npcs, npc);
    SaveState.itemList = &npc->inventory;
}

void loadBox() {
    int boxIdentifier = sectionPopInt();

    struct Box* box = NULL;
    if (boxIdentifier == IDENTITY_BOXCHEST) {
        box = newChestBox();
    }
    else if (boxIdentifier == IDENTITY_BOXPALADINSBAG) {
        box = newPaladinBagBox();
    }
    else if (boxIdentifier == IDENTITY_BOXCATACOMBS) {
        box = newCatacombsBox();
    }
    else if (boxIdentifier == IDENTITY_BOXFINAL) {
        box = newFinalPathBox();
    }

    if (box == NULL) {
        printf("PANIC!! Unsupported Box id %i, aborting\n", boxIdentifier);
        exit(1);
    }

    appendToList(SaveState.boxes, box);
    SaveState.itemList = &box->inventory;
}

void loadJournal() {
    removeAllFromList(&journal);
}

void loadJournalEntry() {
    int idx = sectionPopInt();
    int state = sectionPopInt();
    if (idx < 0 || idx >= GLOBALJOURNALENTRYLISTSIZE) {
        printf("PANIC! Journal entry out of range %i\n", idx);
        exit(1);
    }

    struct JournalEntry* entry = globalJournalEntryList[idx];
    if (!entry) {
        printf("PANIC! Journal entry not found %i\n", idx);
        exit(1);
    }

    entry->completed = state;
    appendToList(&journal, entry);
}

void saveJournal() {
    fprintf(SaveState.f, "JOURNAL\n");
    for (int i = 0; i < GLOBALJOURNALENTRYLISTSIZE; i++) {
        struct JournalEntry* entry = globalJournalEntryList[i];
        if (entry && findJournalEntry(entry->identity)) {
            fprintf(SaveState.f, "JOURNALENTRY %i %i\n", entry->identity, entry->completed);
        }
    }
}

void saveItem(struct InventoryItem* item) {
    fprintf(SaveState.f, "ITEM %s %i\n", item->def->saveName, item->count);
}

void saveEquippedWeapon() {
    if (Equipped.weapon) {
        fprintf(SaveState.f, "WEAPON\n");
        saveItem(Equipped.weapon->payload);
    }
}

void saveEquippedArmor() {
    if (Equipped.armor) {
        fprintf(SaveState.f, "ARMOR\n");
        saveItem(Equipped.armor->payload);
    }
}

void saveMonster(struct Monster* monster) {
    fprintf(SaveState.f, "MONSTER %s %i\n", monster->def->saveName, monster->hp);
}

void saveItemContainer(struct ListItem* items) {
    if (items == NULL) {
        return;
    }
    for (struct ListItem* head = items; head != NULL; head = head->next) {
        saveItem(head->payload);
    }
}

void saveNPC(struct NPC* npc) {
    fprintf(SaveState.f, "NPC %i\n", npc->identity);
    saveItemContainer(npc->inventory);
}

void saveBox(struct Box* box) {
    fprintf(SaveState.f, "BOX %i\n", box->identity);
    saveItemContainer(box->inventory);
}

void saveMonsterContainer(struct ListItem* monsters) {
    if (monsters == NULL) {
        return;
    }
    for (struct ListItem* head = monsters; head != NULL; head = head->next) {
        saveMonster(head->payload);
    }
}

void saveNPCContainer(struct ListItem* npcs) {
    if (npcs == NULL) {
        return;
    }
    for (struct ListItem* head = npcs; head != NULL; head = head->next) {
        saveNPC(head->payload);
    }
}

void saveBoxContainer(struct ListItem* npcs) {
    if (npcs == NULL) {
        return;
    }
    for (struct ListItem* head = npcs; head != NULL; head = head->next) {
        saveBox(head->payload);
    }
}

void saveInventory() {
    fprintf(SaveState.f, "INVENTORY\n");
    saveItemContainer(inventory);
}

void loadInventory() {
    removeAllFromList(&inventory);
    SaveState.itemList = &inventory;
}

void saveEnd() {
    fprintf(SaveState.f, "END\n");
}

//SECTION: LOAD/SAVE

void loadCursor() {
    DrawMapCursor = sectionPopInt();
}

void saveCursor() {
    fprintf(SaveState.f, "CURSOR %i\n", DrawMapCursor);
}

void loadGameState() {
    GameState.catCorpsePicked = sectionPopInt();
    GameState.catQuestEnding = sectionPopInt();
    GameState.churchDoorOpened = sectionPopInt();
    GameState.priestGaveKey = sectionPopInt();
    GameState.secretPathOpened = sectionPopInt();
    GameState.secretPathOpenedFromCatacomb = sectionPopInt();
    GameState.golemAttacks = sectionPopInt();
}

void saveGameState() {
    fprintf(SaveState.f, "GAMESTATE %i %i %i %i %i %i %i\n",
        GameState.catCorpsePicked,
        GameState.catQuestEnding,
        GameState.churchDoorOpened,
        GameState.priestGaveKey,
        GameState.secretPathOpened,
        GameState.secretPathOpenedFromCatacomb,
        GameState.golemAttacks
    );
}

void loadHpAndGold() {
    health = sectionPopInt();
    money = sectionPopInt();
}

void saveHpAndGold() {
    fprintf(SaveState.f, "HPANDGOLD %i %i\n", health, money);
}

void saveLocation(struct Location* loc) {
    fprintf(SaveState.f, "LOCATION %i %i\n", loc->id, loc->visited);
    saveItemContainer(loc->inventory);
    saveMonsterContainer(loc->monsters);
    saveNPCContainer(loc->npcs);
    saveBoxContainer(loc->boxes);
}

void loadLocation() {
    struct Location* l = loadIdxLocation(sectionPopInt());
    l->visited = sectionPopInt();
    SaveState.location = l;

    SaveState.monsters = &l->monsters;
    SaveState.itemList = &l->inventory;
    SaveState.npcs = &l->npcs;
    SaveState.boxes = &l->boxes;

    removeAllFromList(SaveState.monsters);
    removeAllFromList(SaveState.itemList);
    removeAllFromList(SaveState.npcs);
    removeAllFromList(SaveState.boxes);
}

// SECTION: load handlers

struct OnSection {
    char* name;
    void (*callback)();
};

struct OnSection loadHandlers[] = {
    {.name = "MAP", .callback = loadMap },
    {.name = "CURSOR", .callback = loadCursor },
    {.name = "LOCATION", .callback = loadLocation },
    {.name = "GAMESTATE", .callback = loadGameState },
    {.name = "HPANDGOLD", .callback = loadHpAndGold },
    {.name = "HERE", .callback = loadHere},
    {.name = "INVENTORY", .callback = loadInventory},
    {.name = "ITEM", .callback = loadItem},
    {.name = "MONSTER", .callback = loadMonster},
    {.name = "NPC", .callback = loadNPC},
    {.name = "BOX", .callback = loadBox},
    {.name = "JOURNAL", .callback = loadJournal},
    {.name = "JOURNALENTRY", .callback = loadJournalEntry},
    {.name = "WEAPON", .callback = loadWeapon},
    {.name = "ARMOR", .callback = loadArmor},
    {.name = NULL}
};

void callOnSection(char* sectionName) {
    struct OnSection* section = loadHandlers;
    for (;;) {
        if (!section->name) {
            printf("PANIC!! Unsupported section %s, aborting\n", sectionName);
            exit(1);
        }

        if (!strcmp(sectionName, section->name)) {
            section->callback();
            break;
        }

        ++section;
    }
}

void saveAllLocations() {
    for (int i = 0; i < GLOBALLOCATIONLISTSIZE; i++) {
        struct Location* loc = globalLocationList[i];
        if (loc) {
            saveLocation(loc);
        }
        else break;
    }
}

FILE* openFile(char* filename, char* mode) {
#if BUILDONWINDOWS
    FILE* ret;
    int errNo = fopen_s(&ret, filename, mode);
    if (errNo) {
        return NULL;
    }
    else {
        return ret;
    }
#else
    return fopen(filename, mode);
#endif

}

void saveGame() {
    if (health <= 0 || GameState.evilEnding || GameState.goodEnding) {
        return;
    }
    SaveState.f = openFile("paladin.save", "w");
    saveMap();
    saveCursor();
    saveGameState();
    saveHpAndGold();
    saveEquippedWeapon();
    saveEquippedArmor();
    saveHere();
    saveJournal();
    saveInventory();
    saveAllLocations();
    saveEnd();
    fclose(SaveState.f);
}

void applyGameState() {
    loc_CHURCH_ENTRY.moves[TRAVEL]->closed = 1;
    loc_TOWN_CHURCH.moves[TRAVEL]->closed = 1;

    removeTravelFromLocation(&loc_CATACOMBS_1_ENTRY_SECRET);
    removeTravelFromLocation(&loc_WORLD_SECRETENTRY);

    if (GameState.churchDoorOpened) {
        openChurchDoors();
    }

    if (GameState.secretPathOpened) {
        openSecretPath();
    }

    if (GameState.secretPathOpenedFromCatacomb) {
        openSecretPathFromCatacomb();
    }
}

void loadGame() {
    SaveState.f = openFile("paladin.save", "r");
    for (;;) {
        readSaveFileLine();
        char* section = sectionPop();
        if (!strcmp("END", section)) {
            break;
        }
        callOnSection(section);
    }
    fclose(SaveState.f);
    applyGameState();

    printMsg("Game loaded");
    cmd_look("");
}

int startScreen() {
    FILE* file = openFile("paladin.save", "r");
    if (file != NULL) {
        fclose(file);
        printf("Would you like to continue previous game?\n");
        printf("1) Yes - load previous game\n");
        printf("2) No - start New game\n");
        for (;;) {
            printf("?> ");
            fillInput();
            int num = readInt(input);
            if (num != 1 && num != 2) {
                printf("Please type 1 or 2...\n");
                continue;
            }

            return num == 2;
        }
    }

    return 1;
}

void initInventory() {
    appendToList(&Equipped.weapon, newInventoryItem(&ITEMD_WEAPON_SwordLvl1));
    appendToList(&Equipped.armor, newInventoryItem(&ITEMD_ARMOR_ArmorLvl1));

    appendToList(&inventory, newInventoryItemCount(&ITEMD_HEALING_HealingPotion, 5));
    money = 42;
}

void initDynamicMonsters() {
    struct Location* roadPastStart = loc_WORLD_START.moves[EAST]->destination->moves[EAST]->destination;
    appendToList(&roadPastStart->monsters, newMonster(&MONSTERDEF_SmallRat));

    struct Location* westFromPaladinBag = loc_DARKFOREST_PALADINCORPSE.moves[WEST]->destination;
    appendToList(&westFromPaladinBag->monsters, newMonster(&MONSTERDEF_Wolf));
    appendToList(&westFromPaladinBag->monsters, newMonster(&MONSTERDEF_Wolf));

    struct Location* northFromPaladinBag = loc_DARKFOREST_PALADINCORPSE.moves[NORTH]->destination;
    appendToList(&northFromPaladinBag->monsters, newMonster(&MONSTERDEF_Wolf));

}

void initNPCsInventory() {
    appendToList(&NPCVillageHerbalist.inventory, newInventoryItemCount(&ITEMD_BOMB_SparklingBooze, 10));
    appendToList(&NPCVillageHerbalist.inventory, newInventoryItemCount(&ITEMD_BOMB_RatShot, 10));
    appendToList(&NPCVillageHerbalist.inventory, newInventoryItemCount(&ITEMD_HEALING_HealingPotion, 30));

    appendToList(&NPCVillageBlacksmith.inventory, newInventoryItemCount(&ITEMD_WEAPON_SwordLvl2, 1));
    appendToList(&NPCVillageBlacksmith.inventory, newInventoryItemCount(&ITEMD_ARMOR_ArmorLvl2, 1));

    appendToList(&NPCTownSmith.inventory, newInventoryItemCount(&ITEMD_WEAPON_SilverSword, 1));
    appendToList(&NPCTownSmith.inventory, newInventoryItemCount(&ITEMD_WEAPON_WoodenPeg, 1));
    appendToList(&NPCTownSmith.inventory, newInventoryItemCount(&ITEMD_WEAPON_SwordLvl3, 1));
    appendToList(&NPCTownSmith.inventory, newInventoryItemCount(&ITEMD_WEAPON_SwordLvl4, 1));

    appendToList(&NPCTownArmorer.inventory, newInventoryItemCount(&ITEMD_ARMOR_ArmorLvl3, 1));
    appendToList(&NPCTownArmorer.inventory, newInventoryItemCount(&ITEMD_ARMOR_ArmorLvl4, 1));
    appendToList(&NPCTownHealer.inventory, newInventoryItemCount(&ITEMD_HEALING_HealingPotion, 30));

    appendToList(&NPCVampireSlayer.inventory, newInventoryItemCount(&ITEMD_WEAPON_FakeSilverSword, 10));

    appendToList(&NPCVillageShopkeeper.inventory, newInventoryItemCount(&ITEMD_HEALING_Ale, 10));

    appendToList(&NPCTownTavernBartender.inventory, newInventoryItemCount(&ITEMD_HEALING_Ale, 100));
    appendToList(&NPCTownTavernBartender.inventory, newInventoryItemCount(&ITEMD_HEALING_Lagger, 100));
    appendToList(&NPCTownTavernBartender.inventory, newInventoryItemCount(&ITEMD_HEALING_Wine, 100));
}

int main(int argc, char** argv) {
    srand(0);
    initGlobalLocationList();
    initGlobalItemDefList();
    initGlobalMonsterDefList();
    initGlobalJournalEntryList();
    initGlobalNPCList();
    constructLocations();

    initMap(MAP_CHURCH);
    initMap(MAP_CATACOMBS_1);
    initMap(MAP_CATACOMBS_2);
    initMap(MAP_CATACOMBS_3);
    initMap(MAP_ABANDONED_TEMPLE);
    initMap(MAP_VILLAGE_DUNGEON);
    initMap(MAP_WORLD);

#ifdef WITHDEBUG
    initMap(DEMO_MAP);
#endif

    addTravelJoints();
    deleteTravelJoints();

    applyTravelLocks();

    initNPCs();

#ifdef WITHDEBUG
    initDemoLocations();
#endif
    GameState.golemAttacks = 1;

    input = (char*)malloc(inputSize);

    if (startScreen()) {
        showIntro();
        initMainQuest();
        initInventory();
        initNPCsInventory();
        initDynamicMonsters();
        jmpTo(&loc_WORLD_START, 0);
        prevHere = here;
    }
    else {
        loadGame();
    }

    for (;;) {
        prompt();
        if (parseInput() == 0) {
            printf("%s\n", HelpPrompt);
        }
        if (0 == parseInputRetCode) {
            break;
        }
    }

    free(input);
    return 0;
}
