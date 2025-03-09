/*
#define RS_0 = 19;
#define RS_1 = 20;
#define RS_2 = 21;
*/
#include <vector>
#include <numeric>

#define unit1 {1,2,3}
#define unit2 {1,2,3}
#define unit3 {1,2,3}

// SwitchAntennaGivenGravity : 
void switchAntennaGivenGravity(sensors_event_t& gravity_event) {
  if (gravity_event.type == SENSOR_TYPE_GRAVITY) {

    std::vector<double> gravity_vector = {gravity_event.acceleration.x, gravity_event.acceleration.y, gravity_event.acceleration.z};
    std::vector<double> antenna_1_vector = unit1;
    std::vector<double> antenna_2_vector = unit2;
    std::vector<double> antenna_3_vector = unit3;

    float dot1 = std::inner_product(gravity_vector.begin(), gravity_vector.end(), antenna_1_vector.begin(), 0.0);
    float dot2 = std::inner_product(gravity_vector.begin(), gravity_vector.end(), antenna_2_vector.begin(), 0.0);
    float dot3 = std::inner_product(gravity_vector.begin(), gravity_vector.end(), antenna_1_vector.begin(), 0.0);

    float min_dot = min(dot1, min(dot2, dot3));
    if (min_dot == dot1) {
      switchAntenna(1);
    }
    else if (min_dot == dot2) {
      switchAntenna(2);
    }
    else {
      switchAntenna(3);
    }

  }
}



// SwitchAntenna : num -> void
// takes in a number (1-3) and switches the
// RF switcher to the associated antenna
void switchAntenna(int num) {
  switch (num) {
    case 1:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 2:
      digitalWrite(RS_0, HIGH);
      digitalWrite(RS_1, LOW);
      digitalWrite(RS_2, LOW);
    break;

    case 3:
      digitalWrite(RS_0, LOW);
      digitalWrite(RS_1, HIGH);
      digitalWrite(RS_2, LOW);
    break;
  }

}