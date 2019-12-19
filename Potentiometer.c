// --------------------------------------------------
// Potentiometer Class
// v.1.0.0
// by Samet Baykul
// --------------------------------------------------

class Potentiometer
{
  public:
  	// Constructor
  	Potentiometer(int Pin, void (*CallBack)());
    // Methods
    void UPDATE(int StepInterval, int update_tolerance, int Min_Value, int Max_Value);
    int GET_VALUE();
  private:
    // Main
    bool _active;
    int _pin;
    int _read;
    int _value;
    int _last_value;
    void (*_call_back)();
    // Update
    int _step_interval;
    unsigned long _last_update; 
}; 
Potentiometer::Potentiometer(int Pin, void (*CallBack)())
{
  _active = true;
  _pin = Pin;
  _call_back = (*CallBack);
}
void Potentiometer::UPDATE(int Step_Interval = 15, int update_tolerance = 0, int Min_Value = 0, int Max_Value = 1023)
{ 
  if (_step_interval != Step_Interval)
  {
    _step_interval = Step_Interval;
  }
  
  if (_active && millis() - _last_update > _step_interval) 
  {
    _read = analogRead(_pin);
    
    if (_read >= _last_value + update_tolerance || _read <= _last_value - update_tolerance)
    {
      _last_value = _read;
      _value = map(_read, 0, 1023, Min_Value, Max_Value);

      _call_back();

      _last_update = millis();
    }
  }
}
int Potentiometer::GET_VALUE()
{
  return _value;
}
