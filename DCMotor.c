// --------------------------------------------------
// DCMotor Class
// v.1.0.0
// by Samet Baykul
// --------------------------------------------------

class DCMotor
{
  public:
  	// Constructor
    DCMotor(bool Activate);
  	// Methods
    void ADD_HALF_BRIDGE(int Half_Bridge_No, int Pin_Speed, int Pin_Half_Bridge); // Pin_Speed must be one of the PWM pins to control Speed
    void ADD_FULL_BRIDGE(int Full_Bridge_No, int Pin_Speed, int Pin_First_Dir, int Pin_Second_Dir); // Pin_Speed must be one of the PWM pins to control Speed
    void SET_SPEED(int Section_No, int Speed); // Speed values from 0 to 255
  	void SET_DIRECTION(int Full_Bridge_No, bool CW_or_CCW);
  	int GET_SPEED(int Section_No);
    int GET_DIRECTION(int Section_No);
  private:
  	// Main
    bool _active;
  	// Data
    int _pin_list[6] = {-1, -1, -1, -1, -1, -1};
  	// Functions
    void update_pins();
    // Properties
    int SPEED_1;
    int SPEED_2;
    int DIR_1;
    int DIR_2;
};
DCMotor::DCMotor(bool Activate)
{
  _active = Activate;
}
void DCMotor::ADD_HALF_BRIDGE(int Half_Bridge_No, int Pin_Speed, int Pin_Half_Bridge)
{
  switch (Half_Bridge_No)
  {
    case 1:
      _pin_list[4] = Pin_Speed;
      _pin_list[0] = Pin_Half_Bridge;
      break;
    case 2:
      _pin_list[4] = Pin_Speed;
      _pin_list[1] = Pin_Half_Bridge;
      break;
    case 3:
      _pin_list[5] = Pin_Speed;
      _pin_list[2] = Pin_Half_Bridge;
      break;
    case 4:
      _pin_list[5] = Pin_Speed;
      _pin_list[3] = Pin_Half_Bridge;
      break;
  }
  
  update_pins();
}
void DCMotor::ADD_FULL_BRIDGE(int Full_Bridge_No, int Pin_Speed, int Pin_First_Dir, int Pin_Second_Dir)
{
  switch (Full_Bridge_No)
  {
    case 1:
      _pin_list[4] = Pin_Speed;
      _pin_list[0] = Pin_First_Dir;
      _pin_list[1] = Pin_Second_Dir;
      break;
    case 2:
      _pin_list[5] = Pin_Speed;
      _pin_list[2] = Pin_First_Dir;
      _pin_list[3] = Pin_Second_Dir;
      break;
  }
  
  update_pins();
}
void DCMotor::SET_SPEED(int Section_No, int Speed)
{ 
  switch (Section_No)
  {
    case 1:
      analogWrite(_pin_list[4], Speed);
      SPEED_1 = Speed;
      break;
    case 2:
	  analogWrite(_pin_list[5], Speed);
      SPEED_2 = Speed;
      break;
  }
}
void DCMotor::SET_DIRECTION(int Full_Bridge_No, bool From_1_to_2)
{ 
  if (Full_Bridge_No == 1)
  {
    if (From_1_to_2)
    {
      digitalWrite(_pin_list[0], 1);
      digitalWrite(_pin_list[1], 0);
    }
    else
    {
      digitalWrite(_pin_list[0], 0);
      digitalWrite(_pin_list[1], 1);
    }
    
    DIR_1 = From_1_to_2;
  }
  else if (Full_Bridge_No == 2)
  {
    if (From_1_to_2)
    {
      digitalWrite(_pin_list[2], 1);
      digitalWrite(_pin_list[3], 0);
    }
    else
    {
      digitalWrite(_pin_list[2], 0);
      digitalWrite(_pin_list[3], 1);
    }
    
    DIR_2 = From_1_to_2;
  }
}
int DCMotor::GET_SPEED(int Section_No)
{
  if (Section_No == 1)
  {
    return SPEED_1;
  }
  else if (Section_No == 2)
  {
    return SPEED_2;
  }
}
int DCMotor::GET_DIRECTION(int Section_No)
{
  if (Section_No == 1)
  {
    return DIR_1;
  }
  else if (Section_No == 2)
  {
    return DIR_2;
  }
}
void DCMotor::update_pins()
{ 
  for (int i = 0; i <= 5 ; i ++)
  {
    if (_pin_list[i] >= 0)
    {
      pinMode(_pin_list[i], OUTPUT);
    }
  }
}
