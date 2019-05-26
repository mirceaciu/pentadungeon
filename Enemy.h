#include "Arduino.h"

class Enemy
{
  public:
    void Spawn(int pos, int gate_start, int gate_end, int dir, int speed_factor = 1);
    void Move();
    void GetHit();
    void Revive(int speed_factor = 1);
    void Unloot();
    boolean Alive();
    int _pos;
    int _loot_dropped;
    int _loot_pos;
  private:
    int _origin;
    int _alive;
    int _dir;
    int _start;
    int _end;
    int _life;
    float _pace;
    float _sp;
};

void Enemy::Spawn(int pos, int gate_start, int gate_end, int dir, int speed_factor){
    _pos = pos;
    _origin = pos;
    _alive = 1;
    _dir=dir;
    _life = 1;
    _loot_dropped = 0;
    _loot_pos = pos;
    
    _start = gate_start;
    _end = gate_end;
    _sp = 0.25*speed_factor;
    _pace = _sp;
}

void Enemy::Move(){ 
  if(_alive){
    if(_pos <= _start){
      _dir = 1;
    }
    
    if(_pos >= _end) {
      _dir = -1;
    }
    
    _pace += _sp; 
    
    if(_pace >= 2){
     _pos += _dir;
     _pace = 0;
    }
  }
}

void Enemy::GetHit(){
  _life-=1;
  _dir=1;
  
  if(_life<=0){
    _loot_dropped = random(0,2);  // random decide if loot should be dropped
    _loot_pos = random(_pos, _end+1);
    _alive=0;
    _pos=_end;
  }
}

boolean Enemy::Alive(){
  return _alive;
}

void Enemy::Revive(int speed_factor){
  _sp = 0.25*speed_factor;
  _alive=1;
  _loot_dropped = 0;
}

void Enemy::Unloot(){
  _loot_dropped = 0;
}
