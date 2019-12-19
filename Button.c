// --------------------------------------------------
// Button Class
// v.1.1.0
// by Samet Baykul
// --------------------------------------------------

class Button
{
  public:
  	// Constructor
  	Button(int Pin, void (*CallBack)(), int Mode, bool is_Interrupt, bool Release_Mode);
  	// Methods
  	void UPDATE(int Debounce_Delay);
    void SET_MODE(int Mode, bool ReleaseMode);
    bool GET_STATE();
  private:
    // Main
    int _pin;
    int _mode;
    void (*_call_back)();
    // Dynamics
    bool _state;
    bool _reading;
  	bool _prev_state;
    // Properties
    bool _release_mode;
    // Update
  	unsigned long _last_debounce_time = 0;
	unsigned long _debounce_delay = 10;
  	// Interrupts
    static void call_back_interrupt_1();
    static void call_back_interrupt_2();
    static Button * _interrupt_btn_instance_1;
    static Button * _interrupt_btn_instance_2;   
};  
// Reference
Button * Button::_interrupt_btn_instance_1;
Button * Button::_interrupt_btn_instance_2;
// Enumerators
enum ButtonModes 
{
  CONT = 0,
  TOGGLE = 1,
  SWITCH = 2,     
};
Button::Button(int Pin, void (*CallBack)(), int Mode, bool is_Interrupt = false, bool Release_Mode = false)
{
  _pin = Pin;
  _mode = Mode;
  _call_back = (*CallBack);
  _release_mode = Release_Mode;
  
  if (!is_Interrupt)
  {
    pinMode(_pin, INPUT);
  }
  else
  {
    if (_pin == 2)
    {
      _interrupt_btn_instance_1 = this;
      attachInterrupt(digitalPinToInterrupt(2), call_back_interrupt_1, CHANGE);
    }
    else if (_pin == 3)
    {
      _interrupt_btn_instance_2 = this;
      attachInterrupt(digitalPinToInterrupt(3), call_back_interrupt_2, CHANGE);
    }
  }
}
void Button::UPDATE(int Debounce_Delay = 10)
{
  _reading = digitalRead(_pin);
  
  if (_debounce_delay != Debounce_Delay)
  {
    _debounce_delay = Debounce_Delay;
  }
  
  if (millis() - _last_debounce_time > _debounce_delay) 
  {
    _last_debounce_time = millis(); 
    
    switch (_mode)
    {
      case 1:

        _state = _reading;
      
        _call_back();

        break;
      
      case 2:
		
        if (_reading != _state) 
        {
          _state = _reading;
          
          if ((!_release_mode && _state) || (_release_mode && !_state)) 
          {
            _call_back();
          }
        }

        break;
      
      case 3:
		
      	if ((!_release_mode && _reading && !_prev_state) || (_release_mode && !_reading && _prev_state))
        {
          if (_state)
          {
            _state = false;
          }
          else
          {
            _state = true;
          }

          _call_back();
        }

        break;
    }
    
    _prev_state = _reading;
  }
}
void Button::SET_MODE(int Mode, bool ReleaseMode = false)
{
   _mode = Mode;
   _release_mode = ReleaseMode;
}
bool Button::GET_STATE()
{
  return _state;
}
void Button::call_back_interrupt_1()
{
  _interrupt_btn_instance_1 -> UPDATE();
}
void Button::call_back_interrupt_2()
{
  _interrupt_btn_instance_2 -> UPDATE();
}
