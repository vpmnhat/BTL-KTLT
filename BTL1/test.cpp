#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue) {
    cout << "HP=" << HP
        << ", level=" << level
        << ", remedy=" << remedy
        << ", maidenkiss=" << maidenkiss
        << ", phoenixdown=" << phoenixdown
        << ", rescue=" << rescue << endl;
}

bool findchar(string s, char c) {
    if (s.find(c) != string::npos || s.find(c - 32) != string::npos) return true;
    return false;
}

bool prime(int n) {
    if (n == 1) return false;
    bool x = true;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) {
            x = false;
            break;
        }
    return x;
}

void updatetiny(int& tinyint, bool& tinybool, int& HP, int& MaxHP) {
    if (tinybool && tinyint == 0) {
        tinybool = false;
        HP = (HP * 5 >= MaxHP) ? MaxHP : HP * 5;
    }
    if (tinyint > 0) tinyint--;
}

void updatefrog(int& frogint, bool& frogbool, int& level, int& oldlevel) {
    if (frogbool && frogint == 0) {
        frogbool = false;
        level = oldlevel;
    }
    if (frogint > 0) frogint--;
}

void adventureToKoopa(string file_input, int& HP, int& level, int& remedy, int& maidenkiss, int& phoenixdown, int& rescue) {
    int x, levelO, MaxHP, damage, indexevent = 0, b, tinyint = 0, oldlevel, frogint = 0, n2, arr13[100], medicine[100][100];
    int max13, min13, maxi, mini, mtx, mti, max1, min1, maxi2, mini2, max2_3x, max2_3i;
    bool tinybool = false, frogbool = false, asclepius = false, merlin = false, mushghost = false;
    double baseDamage;
    string s, event, filename, str = "", file_mush_ghost, file_asclepius_pack, file_merlin_pack;

    ifstream ifs;
    ifs.open(file_input);
    ifs >> HP >> level >> remedy >> maidenkiss >> phoenixdown;
    MaxHP = HP;
    getline(ifs, event);
    getline(ifs, event);
    getline(ifs, filename);
    ifs.close();

    filename += ",";
    for (int i = 0; i < filename.length(); i++) {
        if (filename[i] != ',') str += filename[i];
        else {
            file_mush_ghost = str;
            str = "";
            x = i;
            break;
        }
    }
    for (int i = x + 1; i < filename.length(); i++) {
        if (filename[i] != ',') str += filename[i];
        else {
            file_asclepius_pack = str;
            str = "";
            x = i;
            break;
        }
    }
    for (int i = x + 1; i < filename.length(); i++) {
        if (filename[i] != ',') str += filename[i];
        else {
            file_merlin_pack = str;
            str = "";
            break;
        }
    }

    if (event[event.length() - 1] == '\r') event.erase(event.length() - 1, 1);
    event += " ";
    int len = event.length();
    str = "";

    for (int z = 0; z < len; z++) {
        if (event[z] != ' ') str += event[z];
        else {
            indexevent++;
            if (str == "0") {
                rescue = 1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                goto esc;
            }
            else if (str == "1" || str == "2" || str == "3" || str == "4" || str == "5") {
                b = indexevent % 10;
                levelO = indexevent > 6 ? (b > 5 ? b : 5) : b;
                if (level > levelO || MaxHP == 999 || prime(MaxHP)) level = (level < 10) ? level + 1 : 10;
                else if (level < levelO) {
                    if (str == "1") baseDamage = 1;
                    else if (str == "2") baseDamage = 1.5;
                    else if (str == "3") baseDamage = 4.5;
                    else if (str == "4") baseDamage = 7.5;
                    else baseDamage = 9.5;
                    damage = baseDamage * levelO * 10;
                    HP -= damage;
                }
                if (HP <= 0) {
                    if (phoenixdown > 0) {
                        HP = MaxHP;
                        phoenixdown--;
                        tinybool = false;
                        tinyint = 0;
                        if (frogbool) level = oldlevel;
                        frogbool = false;
                        frogint = 0;                      
                    }
                    else {
                        rescue = 0;
                        display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                        break;
                    }
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "6") {
                bool tiny = false;
                b = indexevent % 10;
                levelO = indexevent > 6 ? (b > 5 ? b : 5) : b;
                if (tinybool || frogbool);
                else if (level > levelO || MaxHP == 999 || prime(MaxHP)) level = (level < 9) ? level + 2 : 10;
                else if (level < levelO) {
                    if (remedy >= 1) remedy--;
                    else {
                        HP = (HP < 5) ? 1 : HP / 5;
                        tinybool = true;
                        tinyint = 2;
                        tiny = true;
                    }
                }
                rescue = (z == len - 1) ? 1 : -1;
                if (!tiny) updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "7") {
                bool frog = false;
                b = indexevent % 10;
                levelO = indexevent > 6 ? (b > 5 ? b : 5) : b;
                if (frogbool || tinybool);
                else if (level > levelO || MaxHP == 999 || prime(MaxHP)) level = (level < 9) ? level + 2 : 10;
                else if (level < levelO) {
                    if (maidenkiss > 0) maidenkiss--;
                    else {
                        oldlevel = level;
                        frogbool = true;
                        frogint = 2;
                        level = 1;
                        frog = true;
                    }
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                if (!frog) updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "11") {
                int n1 = ((level + phoenixdown) % 5 + 1)*3, s1 = 0;
                for (int i = 0; i < n1; i++) s1 += (99 - 2 * i);
                HP += s1 % 100;
                do HP++;
                while (!prime(HP));
                HP = (HP >= MaxHP) ? MaxHP : HP;
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "12") {
                if (HP > 1) {
                    int fibo1 = 1, fibo2 = 1, fibo3;
                    do {
                        fibo3 = fibo1 + fibo2;
                        fibo1 = fibo2;
                        fibo2 = fibo3;
                    } while (fibo3 < HP);
                    HP = fibo1;
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str.length() > 1 && str[0] == '1' && str[1] == '3') {
                if (!mushghost) {
                    ifstream ifs;
                    ifs.open(file_mush_ghost);
                    string data = "", num = "";
                    ifs >> n2;
                    x = 0;
                    getline(ifs, data);
                    getline(ifs, data);
                    for (int i = 0; i < data.length(); i++) {
                        if (data[i] != ',') num += data[i];
                        else {
                            arr13[x] = stoi(num);
                            x++;
                            num = "";
                        }
                    }
                    arr13[x] = stoi(num);
                    max13 = arr13[0], min13 = arr13[0], maxi = 0, mini = 0;
                    for (int i = 1; i < n2; i++) {
                        if (arr13[i] >= max13) {
                            max13 = arr13[i];
                            maxi = i;
                        }
                        if (arr13[i] <= min13) {
                            min13 = arr13[i];
                            mini = i;
                        }
                    }

                    bool mountain = true, decrease = false;
                    if (n2 == 1) {
                        mtx = -2;
                        mti = -3;
                    }
                    else {
                        for (int i = 0; i < n2 - 1; i++) {
                            if (arr13[i] == arr13[i + 1]) {
                                mountain = false;
                                break;
                            }
                            else if (arr13[i] > arr13[i + 1]) {
                                decrease = true;
                                x = i;
                                break;
                            }
                        }
                        if (decrease) {
                            for (int i = x + 1; i < n2 - 1; i++) {
                                if (arr13[i] <= arr13[i + 1]) {
                                    mountain = false;
                                    break;
                                }
                            }
                        }
                        if (mountain) {
                            mti = x;
                            mtx = arr13[x];
                        }
                        else {
                            mtx = -2;
                            mti = -3;
                        }
                    }

                    for (int i = 0; i < n2; i++) {
                        arr13[i] = (arr13[i] < 0) ? -arr13[i] : arr13[i];
                        arr13[i] = (17 * arr13[i] + 9) % 257;
                    }
                    max1 = arr13[0], min1 = arr13[0], maxi2 = 0, mini2 = 0;
                    for (int i = 1; i < n2; i++) {
                        if (arr13[i] > max1) {
                            max1 = arr13[i];
                            maxi2 = i;
                        }
                        if (arr13[i] < min1) {
                            min1 = arr13[i];
                            mini2 = i;
                        }
                    }

                    if (n2 == 1 || (n2 == 2 && arr13[0] == arr13[1])) {
                        max2_3x = -5;
                        max2_3i = -7;
                    }
                    else if (n2 == 2) {
                        max2_3x = min(arr13[0], arr13[1]);
                        max2_3i = (arr13[0] < arr13[1]) ? 0 : 1;
                    }
                    else if (arr13[0] == arr13[1] && arr13[1] == arr13[2]) {
                        max2_3x = -5;
                        max2_3i = -7;
                    }
                    else {
                        int max1_3x = max(arr13[0], max(arr13[1], arr13[2]));
                        max2_3x = min(arr13[0], min(arr13[1], arr13[2]));
                        for (int i = 0; i < 3; i++) {
                            if (arr13[i] < max1_3x && arr13[i] >= max2_3x) {
                                max2_3x = arr13[i];
                                max2_3i = i;
                            }
                        }
                    }
                    ifs.close();
                    mushghost = true;
                }

                for (int i = 2; i < str.length(); i++) {
                    if (str[i] == '1') {
                        HP -= (maxi + mini);
                        HP = (HP > MaxHP) ? MaxHP : HP;
                        if (HP <= 0) {
                            if (phoenixdown > 0) {
                                phoenixdown--;
                                HP = MaxHP;
                                if (frogbool) level = oldlevel;
                                frogbool = false;
                                frogint = 0;
                                tinybool = false;
                                tinyint = 0;
                            }
                            else {
                                rescue = 0;
                                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                                goto esc;
                            }
                        }
                    }
                    else if (str[i] == '2') {
                        HP -= mtx + mti;
                        HP = (HP > MaxHP) ? MaxHP : HP;
                        if (HP <= 0) {
                            if (phoenixdown > 0) {
                                phoenixdown--;
                                HP = MaxHP;
                                if (frogbool) level = oldlevel;
                                frogbool = false;
                                frogint = 0;
                                tinybool = false;
                                tinyint = 0;
                            }
                            else {
                                rescue = 0;
                                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                                goto esc;
                            }
                        }
                    }
                    else if (str[i] == '3') {
                        HP -= maxi2 + mini2;
                        if (HP <= 0) {
                            if (phoenixdown > 0) {
                                phoenixdown--;
                                HP = MaxHP;
                                if (frogbool) level = oldlevel;
                                frogbool = false;
                                frogint = 0;
                                tinybool = false;
                                tinyint = 0;
                            }
                            else {
                                rescue = 0;
                                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                                goto esc;
                            }
                        }
                    }
                    else {
                        HP -= max2_3x + max2_3i;
                        HP = min(HP, MaxHP);
                        if (HP <= 0) {
                            if (phoenixdown > 0) {
                                phoenixdown--;
                                HP = MaxHP;
                                if (frogbool) level = oldlevel;
                                frogbool = false;
                                frogint = 0;
                                tinybool = false;
                                tinyint = 0;
                            }
                            else {
                                rescue = 0;
                                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                                goto esc;
                            }
                        }
                    }
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "15") {
                remedy = (remedy < 99) ? remedy + 1 : remedy;
                if (tinybool) {
                    remedy--;
                    tinybool = false;
                    tinyint = 0;
                    HP = (HP * 5 >= MaxHP) ? MaxHP : HP * 5;
                }

                rescue = (z == len - 1) ? 1 : -1;
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "16") {
                maidenkiss = (maidenkiss < 99) ? maidenkiss + 1 : maidenkiss;

                if (frogbool) {
                    maidenkiss--;
                    frogbool = false;
                    frogint = 0;
                    level = oldlevel;
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "17") {
                phoenixdown = (phoenixdown < 99) ? phoenixdown + 1 : phoenixdown;


                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "19") {
                if (!asclepius) {
                    ifstream ifs;
                    ifs.open(file_asclepius_pack);
                    int r1, c1, remmed = 0, maimed = 0, phomed = 0, med = 0;
                    ifs >> r1 >> c1;
                    for (int i = 0; i < r1; i++)
                        for (int j = 0; j < c1; j++) ifs >> medicine[i][j];
                    for (int i = 0; i < r1; i++) {
                        med = 0;
                        for (int j = 0; j < c1; j++) {
                            if (med == 3) break;
                            else {
                                if (medicine[i][j] == 16) {
                                    remmed++;
                                    med++;
                                }
                                else if (medicine[i][j] == 17) {
                                    maimed++;
                                    med++;
                                }
                                else if (medicine[i][j] == 18) {
                                    phomed++;
                                    med++;
                                }
                            }
                        }
                    }
                    ifs.close();
                    remedy = min(remedy + remmed, 99);
                    maidenkiss = min(maidenkiss + maimed, 99);
                    phoenixdown = min(phoenixdown + phomed, 99);

                    if (tinybool && remedy > 0) {
                        remedy--;
                        tinybool = false;
                        tinyint = 0;
                        HP = (HP * 5 >= MaxHP) ? MaxHP : HP * 5;
                    }
                    if (frogbool && maidenkiss > 0) {
                        maidenkiss--;
                        frogbool = false;
                        frogint = 0;
                        level = oldlevel;
                    }
                    asclepius = true;
                }

                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "18") {
                if (!merlin) {
                    ifstream ifs;
                    int n9;
                    string item;
                    ifs.open(file_merlin_pack);
                    ifs >> n9;
                    for (int i = 0; i < n9; i++) {
                        ifs >> item;
                        if (item.find("Merlin") != string::npos || item.find("merlin") != string::npos)
                            HP = (HP + 3 > MaxHP) ? MaxHP : HP + 3;
                        else if (findchar(item, 'm') && findchar(item, 'e') && findchar(item, 'r') && findchar(item, 'l') && findchar(item, 'i') && findchar(item, 'n'))
                            HP = (HP + 2 > MaxHP) ? MaxHP : HP + 2;
                    }
                    ifs.close();
                    merlin = true;
                }
                rescue = (z == len - 1) ? 1 : -1;
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            else if (str == "99") {


                if (MaxHP == 999 || (prime(MaxHP) && level >= 8) || level == 10) level = 10;
                else {
                    rescue = 0;
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                    goto esc;
                }
                rescue = (z == len - 1) ? 1 : -1;         
                updatetiny(tinyint, tinybool, HP, MaxHP);
                updatefrog(frogint, frogbool, level, oldlevel);
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            }
            str = "";
        }
    }
esc: {}
}

int main(int argc, char** argv) {
    string file_input = "tc1_input";// file_mush_ghost = "tc1_mush_ghost", file_merlin_pack = "tc1_merlin_pack", file_asclepius_pack = "tc1_asclepius_pack";

    int HP, level, remedy, maidenkiss, phoenixdown, rescue;
    adventureToKoopa(file_input, HP, level, remedy, maidenkiss, phoenixdown, rescue);

    return 0;
}
