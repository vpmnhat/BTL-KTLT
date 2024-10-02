#include "knight2.h"

bool isprime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

/* * * BEGIN implementation of class BaseBag * * */
bool BaseBag::insertFirst(BaseItem* item) {
    if (numberOfItems >= maxItem || (item->itemType == ANTIDOTE && knight->getKnightType() == DRAGON)) {
        return false;
    }
    else {
        item->next = head;
        head = item;
        numberOfItems++;
        return true;
    }
}

BaseItem* BaseBag::get(ItemType type) {
    BaseItem* p = head;
    while (p != nullptr && p->itemType != type) p = p->next;
    if (p == nullptr) return nullptr;
    if (p == head) {
        head = p->next;
    }
    else if (p == head->next) {
        head->next = p->next;
    }
    else {
        BaseItem* prev = head, * tmp = head->next;
        while (prev->next != p) prev = prev->next;
        prev->next = head;
        head->next = p->next;
        head = tmp;
    }
    numberOfItems--;
    return p;
}

string BaseBag::toString() const {
    if (numberOfItems == 0) return "Bag[count=0;]";
    string s;
    s += "Bag[count=";
    s += to_string(numberOfItems);
    s += ";";
    BaseItem* p = head;
    while (p != nullptr) {
        if (p->itemType == ANTIDOTE) s += "Antidote";
        else if (p->itemType == PHOENIXDOWNI) s += "PhoenixI";
        else if (p->itemType == PHOENIXDOWNII) s += "PhoenixII";
        else if (p->itemType == PHOENIXDOWNIII) s += "PhoenixIII";
        else s += "PhoenixIV";

        if (p->next == nullptr) {
            s += "]";
            return s;
        }
        s += ",";
        p = p->next;
    }
    s += "]";
    return s;
}

BaseBag::BaseBag(BaseKnight* knight, int phoenixdownI, int antidote) {
    this->knight = knight;
    if (knight->getKnightType() == DRAGON) maxItem = 14;
    else if (knight->getKnightType() == LANCELOT) maxItem = 16;
    else if (knight->getKnightType() == NORMAL) maxItem = 19;
    else maxItem = int(2e9);
    if (phoenixdownI + antidote == 0) {
        numberOfItems = 0;
        head = nullptr;
    }
    else if (phoenixdownI > 0) {
        head = new PhoenixDownI();
        head->next = nullptr;
        numberOfItems = 1;
        for (int i = 0; i < phoenixdownI - 1; i++) {
            BaseItem* item = new PhoenixDownI();
            if (!insertFirst(item)) delete item;
        }
        for (int i = 0; i < antidote; i++) {
            BaseItem* item = new Antidote();
            if (!insertFirst(item)) delete item;
        }
    }
    else {
        head = new Antidote();
        head->next = nullptr;
        numberOfItems = 1;
        for (int i = 0; i < antidote - 1; i++) {
            BaseItem* item = new Antidote();
            if (!insertFirst(item)) delete item;
        }
    }
}

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
    BaseKnight* baseKnight = new BaseKnight;
    baseKnight->id = id;
    baseKnight->maxhp = maxhp;
    baseKnight->hp = maxhp;
    baseKnight->level = level;
    baseKnight->gil = gil;
    baseKnight->dead = false;
    baseKnight->poisoned = false;
    if (isprime(maxhp)) baseKnight->knightType = PALADIN;
    else if (maxhp == 888) baseKnight->knightType = LANCELOT;
    else if (maxhp == 345 || maxhp == 354 || maxhp == 435 || maxhp == 453 || maxhp == 534 || maxhp == 543) baseKnight->knightType = DRAGON;
    else baseKnight->knightType = NORMAL;
    if (baseKnight->knightType == PALADIN) baseKnight->bag = new PaladinBag(baseKnight, phoenixdownI, antidote);
    else if (baseKnight->knightType == LANCELOT) baseKnight->bag = new LancelotBag(baseKnight, phoenixdownI, antidote);
    else if (baseKnight->knightType == DRAGON) baseKnight->bag = new DragonBag(baseKnight, phoenixdownI, antidote);
    else baseKnight->bag = new NormalBag(baseKnight, phoenixdownI, antidote);

    return baseKnight;
}

string BaseKnight::toString() const {
    string typeString[4] = { "PALADIN", "LANCELOT", "DRAGON", "NORMAL" };
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id)
        + ",hp:" + to_string(hp)
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";
    return s;
}

/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
ArmyKnights::ArmyKnights(const string& file_armyknights) {
    ifsArmyKnights.open(file_armyknights);
    ifsArmyKnights >> numberOfKnights;
    originalNumberOfKnights = numberOfKnights;
    arrBaseKnights = new BaseKnight * [numberOfKnights];
    for (int i = 0; i < numberOfKnights; i++) {
        int maxhp, level, phoenixdownI, gil, antidote;
        ifsArmyKnights >> maxhp >> level >> phoenixdownI >> gil >> antidote;
        arrBaseKnights[i] = BaseKnight::create(i + 1, maxhp, level, gil, antidote, phoenixdownI);
    }
    winOmegaWeapon = winHades = paladinShield = lancelotSpear = guinevereHair = excaliburSword = false;
}

ArmyKnights::~ArmyKnights() {
    ifsArmyKnights.close();
    for (int i = 0; i < originalNumberOfKnights; i++) {
        BaseItem* p = arrBaseKnights[i]->getBag()->head;
        while (p != nullptr) {
            BaseItem* tmp = p;
            p = p->next;
            delete tmp;
        }
        delete arrBaseKnights[i]->getBag();
        delete arrBaseKnights[i];
    }
    delete[] arrBaseKnights;
}

bool ArmyKnights::fight(BaseOpponent* opponent) {
    if (opponent->eventCode <= 5) {
        bool fightResult = true;
            if ((lastKnight()->getLevel() >= opponent->levelO) || (lastKnight()->getKnightType() == LANCELOT) || (lastKnight()->getKnightType() == PALADIN)) {
                lastKnight()->setGil(lastKnight()->getGil() + opponent->winningGil);
                if (lastKnight()->getGil() > 999) {
                    int s = lastKnight()->getGil() - 999, i = 2;
                    lastKnight()->setGil(999);
                    while (s > 0 && i <= numberOfKnights) {
                        if (arrBaseKnights[numberOfKnights - i]->getGil() + s <= 999) {
                            arrBaseKnights[numberOfKnights - i]->setGil(arrBaseKnights[numberOfKnights - i]->getGil() + s);
                            break;
                        }
                        else {
                            s = arrBaseKnights[numberOfKnights - i]->getGil() + s - 999;
                            arrBaseKnights[numberOfKnights - i]->setGil(999);
                            i++;
                        }
                    }
                }
            }
            else {
                lastKnight()->setHp(lastKnight()->getHp() - opponent->baseDamage * (opponent->levelO - lastKnight()->getLevel()));

                BaseItem* p = lastKnight()->getBag()->head;
                while (p != nullptr) {
                    if (p->itemType == ANTIDOTE) {
                        p = p->next;
                    }
                    else if (!p->canUse(lastKnight())) {
                        p = p->next;
                    }
                    else {
                        lastKnight()->getBag()->get(p->itemType)->use(lastKnight());
                        break;
                    }
                }

                if (lastKnight()->getHp() > 0) ;
                else {
                    if (lastKnight()->getGil() >= 100) {
                        lastKnight()->setGil(lastKnight()->getGil() - 100);
                        lastKnight()->setHp(lastKnight()->getMaxhp() / 2);
                    }
                    else {
                        numberOfKnights--;
                        fightResult = false;
                    }
                }
            }
        return fightResult;
    }
    else if (opponent->eventCode == 6) {
        bool fightResult = true;
            if ((lastKnight()->getLevel() >= opponent->levelO)) {
                lastKnight()->setLevel(min(lastKnight()->getLevel() + 1, 10));
            }
            else if (lastKnight()->getKnightType() == DRAGON) ;
            else {
                lastKnight()->poisoned = true;
                BaseItem* ptr = lastKnight()->getBag()->get(ANTIDOTE);
                if (ptr != nullptr) {
                    ptr->use(lastKnight());
                }
                else {
                    if (lastKnight()->getBag()->numberOfItems <= 3) {
                        BaseItem* p = lastKnight()->getBag()->head;
                        for (int i = 0; i < lastKnight()->getBag()->numberOfItems; i++) {
                            BaseItem* tmp = p;
                            p = p->next;
                            delete tmp; 
                        }
                        lastKnight()->getBag()->numberOfItems = 0;
                        lastKnight()->getBag()->head = nullptr;
                    }
                    else {
                        BaseItem* p = lastKnight()->getBag()->head;
                        for (int i = 0; i < 3; i++) {
                            BaseItem* tmp = p;
                            p = p->next;
                            delete tmp;
                        }
                        lastKnight()->getBag()->head = p;
                        lastKnight()->getBag()->numberOfItems -= 3;
                    }

                    lastKnight()->setHp(lastKnight()->getHp() - 10);
                    BaseItem* p = lastKnight()->getBag()->head;
                    while (p != nullptr) {
                        if (p->itemType == ANTIDOTE) p = p->next;
                        else if (!p->canUse(lastKnight())) p = p->next;
                        else {
                            lastKnight()->getBag()->get(p->itemType)->use(lastKnight());
                            break;
                        }
                    }

                    if (lastKnight()->getHp() > 0) ;
                    else {
                        if (lastKnight()->getGil() >= 100) {
                            lastKnight()->setGil(lastKnight()->getGil() - 100);
                            lastKnight()->setHp(lastKnight()->getMaxhp() / 2);
                        }
                        else {
                            numberOfKnights--;
                            fightResult = false;
                        }
                    }
                }
            }
        return fightResult;
    }
    else if (opponent->eventCode == 7) {
        if (lastKnight()->getLevel() < opponent->levelO) {
            if (lastKnight()->getKnightType() != PALADIN) lastKnight()->setGil(lastKnight()->getGil() / 2);
        }
        else {
            if (lastKnight()->getGil() < 500) lastKnight()->setGil(lastKnight()->getGil() * 2);
            else {
                int s = lastKnight()->getGil() * 2 - 999, i = 2;
                lastKnight()->setGil(999);
                while (s > 0 && i <= numberOfKnights) {
                    if (arrBaseKnights[numberOfKnights - i]->getGil() + s <= 999) {
                        arrBaseKnights[numberOfKnights - i]->setGil(arrBaseKnights[numberOfKnights - i]->getGil() + s);
                        break;
                    }
                    else {
                        s = arrBaseKnights[numberOfKnights - i]->getGil() + s - 999;
                        arrBaseKnights[numberOfKnights - i]->setGil(999);
                        i++;
                    }
                }
            }
        }
        return true;
    }
    else if (opponent->eventCode == 8) {
        if (lastKnight()->getKnightType() == PALADIN) {
            if (lastKnight()->getHp() < lastKnight()->getMaxhp() / 3) lastKnight()->setHp(lastKnight()->getHp() + lastKnight()->getMaxhp() / 5);
        }
        else if (lastKnight()->getGil() >= 50 && lastKnight()->getHp() < lastKnight()->getMaxhp() / 3) {
            lastKnight()->setGil(lastKnight()->getGil() - 50);
            lastKnight()->setHp(lastKnight()->getHp() + lastKnight()->getMaxhp() / 5);
        }
        return true;
    }
    else if (opponent->eventCode == 9) {
        lastKnight()->setHp(lastKnight()->getMaxhp());
        return true;
    }
    else if (opponent->eventCode == 10) {
        bool fightResult = true;
            if ((lastKnight()->getLevel() == 10 && lastKnight()->getHp() == lastKnight()->getMaxhp()) || lastKnight()->getKnightType() == DRAGON) {
                lastKnight()->setLevel(10);
                lastKnight()->setGil(999);
                winOmegaWeapon = true;
            }
            else {
                lastKnight()->setHp(0);
                BaseItem* p = lastKnight()->getBag()->head;
                while (p != nullptr) {
                    if (p->itemType != ANTIDOTE) break;
                    else p = p->next;
                }
                if (p != nullptr) {
                    lastKnight()->getBag()->get(p->itemType)->use(lastKnight());
                }

                if (lastKnight()->getHp() > 0) ;
                else {
                    if (lastKnight()->getGil() >= 100) {
                        lastKnight()->setGil(lastKnight()->getGil() - 100);
                        lastKnight()->setHp(lastKnight()->getMaxhp() / 2);
                    }
                    else {
                        numberOfKnights--;
                        fightResult = false;
                    }
                }
            }
        return fightResult;
    }
    else if (opponent->eventCode == 11) {
        bool fightResult = true;
            if (lastKnight()->getLevel() == 10 || (lastKnight()->getKnightType() == PALADIN && lastKnight()->getLevel() >= 8)) {
                paladinShield = true;
                winHades = true;
            }
            else {
                lastKnight()->setHp(0);
                BaseItem* p = lastKnight()->getBag()->head;
                while (p != nullptr) {
                    if (p->itemType != ANTIDOTE) break;
                    else p = p->next;
                }
                if (p != nullptr) {
                    lastKnight()->getBag()->get(p->itemType)->use(lastKnight());
                }

                if (lastKnight()->getHp() > 0) ;
                else {
                    if (lastKnight()->getGil() >= 100) {
                        lastKnight()->setGil(lastKnight()->getGil() - 100);
                        lastKnight()->setHp(lastKnight()->getMaxhp() / 2);
                    }
                    else {
                        numberOfKnights--;
                        fightResult = false;
                    }
                }
            }
        return fightResult;
    }
}

bool ArmyKnights::adventure(Events* events) {
    lastEvent = false;
    for (int eventIndex = 0; eventIndex < events->count(); eventIndex++) {
        if (events->get(eventIndex) == 1) {
            BaseOpponent* opponent = new MadBear;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        if (events->get(eventIndex) == 2) {
            BaseOpponent* opponent = new Bandit;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        if (events->get(eventIndex) == 3) {
            BaseOpponent* opponent = new LordLupin;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        if (events->get(eventIndex) == 4) {
            BaseOpponent* opponent = new Elf;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        if (events->get(eventIndex) == 5) {
            BaseOpponent* opponent = new Troll;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 6) {
            BaseOpponent* opponent = new Tornbery;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 7) {
            BaseOpponent* opponent = new QueenOfCards;
            opponent->levelO = (eventIndex + opponent->eventCode) % 10 + 1;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 8) {
            BaseOpponent* opponent = new NinaDeRings;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 9) {
            BaseOpponent* opponent = new DurianGarden;
            fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 10) {
            BaseOpponent* opponent = new OmegaWeapon;
            if (!winOmegaWeapon) fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 11) {
            BaseOpponent* opponent = new Hades;
            if (!winHades) fight(opponent);
            printInfo();
            delete opponent;
            if (numberOfKnights <= 0) return false;
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 112) {
            BaseItem* item = new PhoenixDownII;
            bool receive = false;
            for (int i = numberOfKnights - 1; i >= 0; i--) {
                if (arrBaseKnights[i]->getBag()->insertFirst(item)) {
                    receive = true;
                    break;
                }
            }
            if (!receive) delete item;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 113) {
            BaseItem* item = new PhoenixDownIII;
            bool receive = false;
            for (int i = numberOfKnights - 1; i >= 0; i--) {
                if (arrBaseKnights[i]->getBag()->insertFirst(item)) {
                    receive = true;
                    break;
                }
            }
            if (!receive) delete item;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 114) {
            BaseItem* item = new PhoenixDownIV;
            bool receive = false;
            for (int i = numberOfKnights - 1; i >= 0; i--) {
                if (arrBaseKnights[i]->getBag()->insertFirst(item)) {
                    receive = true;
                    break;
                }
            }
            if (!receive) delete item;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 95) {
            paladinShield = true;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 96) {
            lancelotSpear = true;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 97) {
            guinevereHair = true;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 98) {
            if (hasGuinevereHair() && hasLancelotSpear() && hasPaladinShield()) excaliburSword = true;
            else excaliburSword = false;
            printInfo();
            if (eventIndex == events->count() - 1) return numberOfKnights > 0;
        }
        else if (events->get(eventIndex) == 99) {
            if (hasExcaliburSword()) {
                printInfo();
                return true;
            }
            else if (hasGuinevereHair() && hasLancelotSpear() && hasPaladinShield()) {
                int totalDamage = 0;
                totalDead = 0;
                for (int i = numberOfKnights - 1; i >= 0; i--) {
                    if (arrBaseKnights[i]->getKnightType() == NORMAL) continue;
                    int damage, HP = arrBaseKnights[i]->getHp(), level = arrBaseKnights[i]->getLevel();
                    double knightBaseDamage;
                    if (arrBaseKnights[i]->getKnightType() == LANCELOT) knightBaseDamage = 0.05;
                    else if (arrBaseKnights[i]->getKnightType() == PALADIN) knightBaseDamage = 0.06;
                    else if (arrBaseKnights[i]->getKnightType() == DRAGON) knightBaseDamage = 0.075;
                    damage = HP * level * knightBaseDamage;
                    totalDamage += damage;
                    if (totalDamage >= 5000) {
                        lastEvent = true;
                        for (int j = numberOfKnights - 1; j >= 0; j--) {
                            if (!arrBaseKnights[j]->dead) {
                                lastAlive = j;
                                break;
                            }
                        }
                        printInfo();
                        return true;
                    }
                    else {
                        arrBaseKnights[i]->dead = true;
                        totalDead++;
                    }
                }
                if (totalDamage < 5000) {
                    numberOfKnights = 0;
                    printInfo();
                    return false;
                }
            }
            else {
                numberOfKnights = 0;
                printInfo();
                return false;
            }
        }
    }
}

int ArmyKnights::count() const {
    if (!lastEvent) return numberOfKnights;
    return numberOfKnights - totalDead;
}

BaseKnight* ArmyKnights::lastKnight() const {
    if (numberOfKnights > 0) {
        if (!lastEvent) {
            return arrBaseKnights[numberOfKnights - 1];
        }
        else {
            return arrBaseKnights[lastAlive];
        }
    }
    return nullptr;
}

bool ArmyKnights::hasPaladinShield() const {
    return paladinShield;
}

bool ArmyKnights::hasLancelotSpear() const {
    return lancelotSpear;
}

bool ArmyKnights::hasGuinevereHair() const {
    return guinevereHair;
}

bool ArmyKnights::hasExcaliburSword() const {
    return excaliburSword;
}

void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight* lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure() {}

KnightAdventure::~KnightAdventure() {
    delete armyKnights;
    delete events;
}

void KnightAdventure::loadArmyKnights(const string& file_armyknights) {
    armyKnights = new ArmyKnights(file_armyknights);
}

void KnightAdventure::loadEvents(const string& file_events) {
    events = new Events(file_events);
}

void KnightAdventure::run() {
    armyKnights->printResult(armyKnights->adventure(events));
}

/* * * END implementation of class KnightAdventure * * */

/* * * BEGIN implementation of class Events * * */
Events::Events(const string& file_events) {
    ifsEvents.open(file_events);
    ifsEvents >> numberOfEvents;
    eventsId = new int[numberOfEvents];
    for (int i = 0; i < numberOfEvents; i++) ifsEvents >> eventsId[i];
}

int Events::count() const {
    return numberOfEvents;
}

int Events::get(int i) const {
    return eventsId[i];
}

Events::~Events() {
    ifsEvents.close();
    delete[] eventsId;
}

/* * * END implementation of class Events * * */

/* * * BEGIN implementation of classes inherited from BaseItem * * */
bool Antidote::canUse(BaseKnight* knight) {
    if (knight->getKnightType() == DRAGON || !knight->poisoned) return false;
    BaseItem* p = knight->getBag()->head;
    while (p != nullptr && p->itemType != ANTIDOTE) p = p->next;
    if (p == nullptr) return false;
    return true;
}

void Antidote::use(BaseKnight* knight) {
    knight->poisoned = false;
    delete this;
}

bool PhoenixDownI::canUse(BaseKnight* knight) {
    BaseItem* p = knight->getBag()->head;
    while (p != nullptr && p->itemType != PHOENIXDOWNI) p = p->next;
    if (p != nullptr && knight->getHp() <= 0) return true;
    return false;
}

void PhoenixDownI::use(BaseKnight* knight) {
    knight->setHp(knight->getMaxhp());
    delete this;
}

bool PhoenixDownII::canUse(BaseKnight* knight) {
    BaseItem* p = knight->getBag()->head;
    while (p != nullptr && p->itemType != PHOENIXDOWNII) p = p->next;
    if (p != nullptr && knight->getHp() < knight->getMaxhp() / 4) return true;
    return false;
}

void PhoenixDownII::use(BaseKnight* knight) {
    knight->setHp(knight->getMaxhp());
    delete this;
}

bool PhoenixDownIII::canUse(BaseKnight* knight) {
    BaseItem* p = knight->getBag()->head;
    while (p != nullptr && p->itemType != PHOENIXDOWNIII) p = p->next;
    if (p != nullptr && knight->getHp() < knight->getMaxhp() / 3) return true;
    return false;
}

void PhoenixDownIII::use(BaseKnight* knight) {
    if (knight->getHp() <= 0) knight->setHp(knight->getMaxhp() / 3);
    else knight->setHp(knight->getHp() + knight->getMaxhp() / 4);
    delete this;
}

bool PhoenixDownIV::canUse(BaseKnight* knight) {
    BaseItem* p = knight->getBag()->head;
    while (p != nullptr && p->itemType != PHOENIXDOWNIV) p = p->next;
    if (p != nullptr && knight->getHp() < knight->getMaxhp() / 2) return true;
    return false;
}

void PhoenixDownIV::use(BaseKnight* knight) {
    if (knight->getHp() <= 0) knight->setHp(knight->getMaxhp() / 2);
    else knight->setHp(knight->getHp() + knight->getMaxhp() / 5);
    delete this;
}

/* * * END implementation of classes inherited from BaseItem * * */
