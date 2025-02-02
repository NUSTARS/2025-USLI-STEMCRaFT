bool openFlaps(sensors_event_t* event)
{
  if(event->type == SENSOR_TYPE_LINEAR_ACCELERATION)
  {
    double accel = event->acceleration.x;
    if (accel < 0)
    {
      Serial.println("Burnout reached!");
      SetDesiredAreaPercent(100);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}