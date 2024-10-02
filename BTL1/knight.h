#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "main.h"

using namespace std;

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue);
void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue);
bool findchar(string s, char c);
bool prime(int n);
void updatetiny(int& tinyint, bool& tinybool, int& HP, int& MaxHP);
void updatefrog(int& frogint, bool& frogbool, int& level, int& oldlevel);

#endif // __KNIGHT_H__