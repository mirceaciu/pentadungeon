#include "Arduino.h"

class Hero 
{
  public:
    void Spawn(int pos);
    void Attack();
    void LevelUp();
    void Die();
    void SaveCheckpoint(int checkpoint);
    int _pos;
    int _is_attacking;
    int _a_down;
    int _a_up;
  private:
    int _level;
    int _checkPoint;
    int _alive;
    int _attack_resonance;
};

void Hero::Spawn(int pos){
  _pos=pos;
  _checkPoint=pos;
  _alive=1;
  _is_attacking=0;
  _a_down=0;
  _a_up=0;
  _level=1;
  _attack_resonance=10;
}

void Hero::Attack(){
  _is_attacking=1;
  _attack_resonance -= 1;
  
  int weapon_pow;
  if(_attack_resonance<=10 && _level >= 1) weapon_pow = 1;
  if(_attack_resonance<=7 && _level >= 2) weapon_pow = 2;
  if(_attack_resonance<=4 && _level == 3) weapon_pow = 3;
  
  _a_down = _pos - weapon_pow;
  _a_up = _pos + weapon_pow;
  
  if(_attack_resonance<=0){
    _is_attacking=0;
    _attack_resonance=10;
  }
}

void Hero::LevelUp(){
  _level += 1;
  
  if(_level > 3) _level = 3;
}

void Hero::SaveCheckpoint(int checkpoint){
  _checkPoint = checkpoint;
}

void Hero::Die(){
  _level = 1;
  _pos = _checkPoint;
}

