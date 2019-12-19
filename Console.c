// --------------------------------------------------
// Console Class
// v.1.0.0
// by Samet Baykul
// --------------------------------------------------

// Fine Tunnings
const int Max_Request_Chain = 3;

class Console
{
  public:
  	// Constructor
    Console(bool Activate);
  	// Methods
    void INIT(int Freq);
    void UPDATE(int Step_Interval);
    void ADD_REQUEST(void (*Test_Function)(int Choise), int Timeout);
    void NEXT();
    void NEW_CHAIN();
    void HEADER(String Message);
    void LINE(bool With_Mark);
    void PRINT(String Message);
  private:
  	// Objects
    typedef struct RO
    {
      // Main
      bool Active;
      int Request_No;
      void (*Operation)(int);
      // Properties
      int Timeout;
    };
  private:
  	// Main
    bool _active;
    int _choise;
    int _target_test = 0;
  	// Dynamics
    int _request_current_time;
    int _current_test = 0;
    int _last_used_request = 0;
  	// Data
  	typedef void (*TypeFunction) (int Choise);
    TypeFunction _test_list[5];
    RO ROL[Max_Request_Chain];
    // Update
    int _step_interval = 1000;
    unsigned long _last_update;
};
Console::Console(bool Activate)
{
  _active = Activate;
}
void Console::INIT(int Freq = 9600)
{
  _active = true;
  
  Serial.begin(Freq); 
}
void Console::UPDATE(int Step_Interval = 1000)
{ 
  if (_step_interval != Step_Interval)
  {
    _step_interval = Step_Interval;
  }
  
  if (_active && millis() - _last_update > _step_interval) 
  {    
    if (Serial.available() && _current_test < _target_test) 
    {
      _choise = Serial.parseInt();

      Serial.print("\n-> ");
      PRINT(String(_choise));
      LINE(false);
      
      if(_current_test == _target_test)
      {
        NEW_CHAIN();
      }
      else
      {
        ROL[_current_test].Operation(_choise);
        
        _last_used_request ++;
      }
    }
    else if (_request_current_time < ROL[_current_test].Timeout)
    {
      Serial.print("."); 
      
      _request_current_time += _step_interval;
      
      delay(_step_interval);
    }
    else if (ROL[_current_test].Timeout > 0)
    {
      _last_update = millis();
        
      if(_current_test == _target_test)
      {
        NEW_CHAIN();
      }
      else
      {
        PRINT("\n"); 
        
        ROL[_current_test].Operation(-2);
        
        _request_current_time = 0;
      }
    }
  }
}
void Console::ADD_REQUEST(void (*Test_Function)(int Choise), int Timeout = 0)
{
  ROL[_last_used_request].Active = true;
  ROL[_last_used_request].Timeout = Timeout;
  ROL[_last_used_request].Operation = (*Test_Function);

  _request_current_time = 0;
  _last_used_request ++;
  _target_test ++;
}
void Console::NEXT()
{
  _current_test ++;
  
  if (ROL[_current_test].Active)
  {
    _request_current_time = 0;
    
    ROL[_current_test].Operation(-1);
  }
  else
  {
    NEW_CHAIN();
  }
}
void Console::NEW_CHAIN()
{
  _last_used_request = 0;
  _current_test = 0;
  _target_test = 0;
  _request_current_time = 0;
  
  for (int i = 0; i < Max_Request_Chain; i ++)
  {
    ROL[i].Active = false;
    ROL[i].Timeout = 0;
  }
}
void Console::HEADER(String Message)
{
  LINE(true);
  PRINT(Message);
  LINE(true);
  LINE(false);
}
void Console::LINE(bool With_Mark = false)
{
  if (With_Mark)
  {
    for (int i = 0; i <= 50; i ++)
    {
      Serial.print("-");
    }
  }

  Serial.println();
}
void Console::PRINT(String Message)
{
  Serial.println(Message);
}
