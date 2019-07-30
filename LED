// --------------------------------------------------
// Led Class
// v.1.0.1
// by Samet Baykul
// --------------------------------------------------

class Led
{
  public:
  	// Constructor
  	Led(int Pin);
  	// Methods
  	void LIGHT(bool State);
    bool GET_STATE();
    void BLINK(int OpeningDuration, int ClosingDuration); 
  private:
    // Main
    int _pin;
    bool _state;
    // Properties
    unsigned long _last_update;
};

Led::Led(int Pin)
{
  _pin = Pin;
  
  pinMode(Pin, OUTPUT);
}
void Led::LIGHT(bool State)
{
  _state = State;
  
  digitalWrite(_pin, _state);
  
  _last_update = millis();
}
bool Led::GET_STATE()
{
  return _state;
}
void Led::BLINK(int OpeningDuration = 200, int ClosingDuration = 200)
{
  if (!_state && millis() - _last_update > OpeningDuration) 
  {
    LIGHT(true);
  }
  if (_state && millis() - _last_update > ClosingDuration) 
  {
    LIGHT(false);
  }
}
