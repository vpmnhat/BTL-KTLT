#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

bool isprime(int n);

enum ItemType { ANTIDOTE, PHOENIXDOWNI, PHOENIXDOWNII, PHOENIXDOWNIII, PHOENIXDOWNIV };

class BaseKnight;
class BaseItem;
class Events;

class BaseBag {
public:
    BaseKnight* knight;
    BaseItem* head;
    int maxItem;
    int numberOfItems;
    virtual bool insertFirst(BaseItem* item);
    virtual BaseItem* get(ItemType type);
    virtual string toString() const;
    BaseBag(BaseKnight* knight, int phoenixdownI, int antidote);
};

class DragonBag : public BaseBag {
public:
    DragonBag(BaseKnight* knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote) {}
};

class PaladinBag : public BaseBag {
public:
    PaladinBag(BaseKnight* knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote){}
};

class LancelotBag : public BaseBag {
public:
    LancelotBag(BaseKnight* knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote){}
};

class NormalBag : public BaseBag {
public:
    NormalBag(BaseKnight* knight, int phoenixdownI, int antidote) : BaseBag(knight, phoenixdownI, antidote){}
};

class BaseOpponent {
public:
    int winningGil;
    int levelO;
    int baseDamage;
    int eventCode;
};

class MadBear : public BaseOpponent {
public:
    MadBear() {
        eventCode = 1;
        winningGil = 100;
        baseDamage = 10;
    }
};

class Bandit : public BaseOpponent {
public:
    Bandit() {
        eventCode = 2;
        winningGil = 150;
        baseDamage = 15;
    }
};

class LordLupin : public BaseOpponent {
public:
    LordLupin() {
        eventCode = 3;
        winningGil = 450;
        baseDamage = 45;
    }
};

class Elf : public BaseOpponent {
public:
    Elf() {
        eventCode = 4;
        winningGil = 750;
        baseDamage = 75;
    }
};

class Troll : public BaseOpponent {
public:
    Troll() {
        eventCode = 5;
        winningGil = 800;
        baseDamage = 95;
    }
};

class Tornbery : public BaseOpponent {
public:
    Tornbery() {
        eventCode = 6;
    }
};

class QueenOfCards : public BaseOpponent {
public:
    QueenOfCards() {
        eventCode = 7;
    }
};

class NinaDeRings : public BaseOpponent {
public:
    NinaDeRings() {
        eventCode = 8;
    }
};

class DurianGarden : public BaseOpponent {
public:
    DurianGarden() {
        eventCode = 9;
    }
};

class OmegaWeapon : public BaseOpponent {
public:
    OmegaWeapon() {
        eventCode = 10;
    }
};

class Hades : public BaseOpponent {
public:
    Hades() {
        eventCode = 11;
    }
};

enum KnightType { PALADIN = 0, LANCELOT, DRAGON, NORMAL };

class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    BaseBag* bag;
    KnightType knightType;

public:
    bool poisoned;
    bool dead;
    static BaseKnight* create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    void setId(int n) {
        id = n;
    }
    void setHp(int n) {
        hp = n;
    }
    void setLevel(int n) {
        level = n;
    }
    void setGil(int n) {
        gil = n;
    }
    void setKnightType(KnightType type) {
        knightType = type;
    }
    int getId() {
        return id;
    }
    int getHp() {
        return hp;
    }
    int getMaxhp() {
        return maxhp;
    }
    int getLevel() {
        return level;
    }
    int getGil() {
        return gil;
    }
    int getKnightType() {
        return knightType;
    }
    BaseBag* getBag() {
        return bag;
    }
};

class ArmyKnights {
public:
    ifstream ifsArmyKnights;
    int numberOfKnights;
    BaseKnight** arrBaseKnights;
    bool winOmegaWeapon;
    bool winHades;
    bool paladinShield;
    bool lancelotSpear;
    bool guinevereHair;
    bool excaliburSword;
    bool lastEvent;
    int totalDead;
    int lastAlive;
    int originalNumberOfKnights;

    ArmyKnights(const string& file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent* opponent);
    bool adventure(Events* events);
    int count() const;
    BaseKnight* lastKnight() const;

    bool hasPaladinShield() const;
    bool hasLancelotSpear() const;
    bool hasGuinevereHair() const;
    bool hasExcaliburSword() const;

    void printInfo() const;
    void printResult(bool win) const;
};

class BaseItem {
public:
    BaseItem* next;
    ItemType itemType;
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;
};

class Antidote : public BaseItem {
public:
    Antidote() {
        itemType = ANTIDOTE;
    }
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};

class PhoenixDownI : public BaseItem {
public:
    PhoenixDownI() {
        itemType = PHOENIXDOWNI;
    }
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};

class PhoenixDownII : public BaseItem {
public:
    PhoenixDownII() {
        itemType = PHOENIXDOWNII;
    }
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};

class  PhoenixDownIII : public BaseItem {
public:
    PhoenixDownIII() {
        itemType = PHOENIXDOWNIII;
    }
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};

class PhoenixDownIV : public BaseItem {
public:
    PhoenixDownIV() {
        itemType = PHOENIXDOWNIV;
    }
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};

class Events {
public:
    ifstream ifsEvents;
    int numberOfEvents;
    int* eventsId;
    Events(const string& file_events);
    int count() const;
    int get(int i) const;
    ~Events();
};

class KnightAdventure {
private:
    ArmyKnights* armyKnights;
    Events* events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string& file_armyknights);
    void loadEvents(const string& file_events);
    void run();
};

#endif // __KNIGHT2_H__