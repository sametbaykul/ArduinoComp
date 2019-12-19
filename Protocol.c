// --------------------------------------------------
// Protocol Class
// v.1.0.0
// by Samet Baykul
// --------------------------------------------------

// Fine Tunnings
const int Protocol_Capacity = 2;

class Protocol
{
  public:
  	// Constructor
  	Protocol(bool Active, byte Command_Byte, byte End_Byte, byte New_Parameter, byte New_Operation);
    // Methods
    void UPDATE(int Step_Interval);
    void SET_OPERATION(int Protocol_No, void (*Operation)(int, int), byte Start_Byte);
    void SET_SYNTAX(int Protocol_No, byte Start_Byte, byte Command_Byte, byte End_Byte, byte New_Parameter, byte New_Operation);
    void SET_TIMEOUT(int Protocol_No, int Time_Limit);
  private:
  	// Objects
    typedef struct Str_Protocol
    {
      // Main
      bool Active = false;
      void (*Operation)(int, int);
      // Syntax
      int Protocol_No;
      byte Byte_Start;
      // Timeout
      int Time_Limit = 0;
      int Execution_Time = 0;
      int Timeout = 0;
    };
  private:
    // Warning Codes
    const int _status_ok = 0;
    const int _status_category_not_defined = 1;
    const int _status_protocol_not_defined = 2;
    const int _status_missing_command_input = 3;
    const int _status_unfinished_command = 4;
    const int _status_timeout = 5;
    const int _status_no_more_available_serial_data_error = 6;
    const int _status_unknown_error = 7;
    const int _status_success = 8;
    // Data
  	Str_Protocol PROL[Protocol_Capacity];						
    // Dynamics
    bool _active = false;
    bool _category_found = false;
    bool _protocol_found = false;
    int _step_interval = 10;
    unsigned long _last_update;
    int _no = 0;
    byte _current_byte;
    int _current_dec;
    int _current_operation;
    int _current_param;
    int _status = 0;
    // General Syntax
    byte _byte_command = 32;						    // space
    byte _byte_end = 59;							      // ;
    byte _byte_new_parameter = 58;					// :
    byte _byte_new_operation = 44;					// ,
	  // Functions
    void update_protocols(Str_Protocol PRO);
    void read_serial(Str_Protocol PRO);
    void handle_status();
  	void reset();
}; 
Protocol::Protocol(bool Active = true, byte Command_Byte = 32, byte End_Byte = 59, byte New_Parameter = 58, byte New_Operation = 44)
{
  _active = Active;
  _byte_command = Command_Byte;
  _byte_end = End_Byte;
  _byte_new_parameter = New_Parameter;
  _byte_new_operation = New_Operation;
}
void Protocol::SET_OPERATION(int Protocol_No, void (*Operation)(int, int), byte Start_Byte = 65)
{
  _no = Protocol_No - 1;
  
  PROL[_no].Active = true;
  PROL[_no].Protocol_No = Protocol_No;
  PROL[_no].Byte_Start = Start_Byte;
  PROL[_no].Operation = (*Operation);
}
void Protocol::SET_TIMEOUT(int Protocol_No, int Time_Limit = 15000)
{
  _no = Protocol_No - 1;
  
  PROL[_no].Time_Limit = Time_Limit;
}
void Protocol::UPDATE(int Step_Interval = 10)
{ 
  if (_step_interval != Step_Interval)
  {
    _step_interval = Step_Interval;
  }
  
  if (_active && millis() - _last_update > _step_interval) 
  {  
    if (Serial.available()) 
    { 
      _category_found = false;
      _protocol_found = false;
      _current_byte = Serial.read();
      _current_dec = Serial.parseInt();
       
      for (_no = 0; _no < Protocol_Capacity; _no++)
      {  
        if (PROL[_no].Byte_Start == _current_byte)
        {
          _category_found = true;
           
          if (PROL[_no].Protocol_No == _current_dec)
          {
            _protocol_found = true;
            
			if (PROL[_no].Active && _status == _status_ok)
            {
			  update_protocols(PROL[_no]);
            }
          }
        }
      }
      
      if (!_category_found)
      {
        _status = _status_category_not_defined;
      }
      else if (!_protocol_found)
      {
        _status = _status_protocol_not_defined;
      }
      
      if (_status != _status_ok)
      {
        handle_status();
    	reset();
      }
    }
  }
}
void Protocol::update_protocols(Str_Protocol PRO)
{
  if (_byte_command)
  {
    _current_byte = Serial.read();
    
    if (_current_byte != _byte_command)
    { 
      _status = _status_missing_command_input;
    }
  }
  
  if (PRO.Time_Limit)
  {
    PRO.Execution_Time = millis();
  }

  if (_status == _status_ok)
  {
    read_serial(PRO);
  }
  else
  {
    handle_status();
    reset();
  }
}
void Protocol::read_serial(Str_Protocol PRO)
{
  _current_dec = Serial.parseInt();
  _current_byte = Serial.read();
  
  if (PRO.Time_Limit && millis() - PRO.Execution_Time > PRO.Time_Limit)
  {
    _status = _status_timeout;
  }
  else if (_current_byte == _byte_new_parameter)
  {
    PRO.Operation(_current_param, _current_dec);
    
    _current_param ++;
    
    read_serial(PRO);
  }
  else if (_current_byte == _byte_new_operation)
  {
    PRO.Operation(_current_param, _current_dec);
    
    _current_param = 0;
    
    read_serial(PRO);
  }
  else if (_current_byte == _byte_end)
  {
    PRO.Operation(_current_param, _current_dec);
    
    _status = _status_success;
  }
  else if (_current_byte != _byte_end && Serial.available() == 0)
  {
    _status = _status_unfinished_command;
  }
  else if (Serial.available() == 0)
  {
    _status = _status_no_more_available_serial_data_error;
  }
  else
  {
    _status = _status_unknown_error;
  }
  
  if (_status != _status_ok && _status != _status_success)
  {
    handle_status();
    reset();
  }
}
void Protocol::handle_status()
{
  if (_status == _status_success)
  {
    Serial.println("DONE!");
  }
  else if (_status > 0)
  {
    Serial.print("- WANING " + String(_status) + " > ");
    
    switch (_status)
    {
      case 1:
        Serial.println("Missing Category!");
      	break;
      case 2:
        Serial.println("Missing Protocol!");
      	break;
      case 3:
        Serial.println("Syntax Error!");
      	break;
      case 4:
        Serial.println("Unfinished Command!");
      	break;
      case 5:
        Serial.println("Timeout Error!");
      	break;
      case 6:
        Serial.println("No More Available Serial Data Error!");
      	break;
      case 7:
      default:
      	Serial.println("Unkown Error!");
      	break;
    }
  } 
}
void Protocol::reset()
{
  _status = _status_ok;
  _current_byte = 0;
  _current_dec = 0;
  _current_param = 0;

  Serial.flush();
  
  while (Serial.available() > 0)
  {
    _current_byte = Serial.read();
    delay(1);
  }
}
