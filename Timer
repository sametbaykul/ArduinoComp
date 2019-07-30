// --------------------------------------------------
// Timer Class
// v.1.0.1
// by Samet Baykul
// --------------------------------------------------

class Timer
{
  public:
  	// Constructor
    Timer(int StepInterval);
  	// Methods
  	void UPDATE(void (*CallBack)());
    void START();
    void STOP();
    void RESUME();
    void NEXT(void (*CallBack)());
  private:
  	// Main
  	bool _active;
    // Dynamics
  	int _step_interval;
    unsigned long _last_update;
};
Timer::Timer(int StepInterval)
{
  _step_interval = StepInterval;
  _active = true;
}
void Timer::UPDATE(void (*CallBack)())
{ 
  if (_active && millis() - _last_update > _step_interval) 
  {
    (*CallBack)();
    
    _last_update = millis();
  }
}
void Timer::START()
{
  _active = true;
}
void Timer::STOP()
{
  _active = false;
}
void Timer::RESUME()
{
  _active = !_active;
}
void Timer::NEXT(void (*CallBack)())
{
  (*CallBack)();
  
  _last_update = millis();
}
