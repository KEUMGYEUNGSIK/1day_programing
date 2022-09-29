#define TRIG 2
#define ECHO 3

#define FILTERSIZE 10
float ar[FILTERSIZE+1] = {0,};
float pre_avg;
float new_value;

float mov_filter (float pre_avg, float new_value)
{
  float average = 0;
  
  ar[FILTERSIZE] = new_value;
  
  average = pre_avg + ar[FILTERSIZE]/FILTERSIZE - ar[0]/FILTERSIZE;
  
  for(int i = 1 ; i < FILTERSIZE+1 ; i++)
  {
    ar[i-1] = ar[i];
  }

  ar[FILTERSIZE] = 0.0;
  
  return average;
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);  // 초음파 출력부 설정
  pinMode(ECHO, INPUT);   // 초음파 입력부 설정
  Serial.println("mm,이동평균");
}


void loop() { 
  long duration, distance;
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);  
  
  duration = pulseIn(ECHO, HIGH);    // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2; 

  pre_avg = mov_filter (pre_avg, distance);
  
  Serial.print(distance);
  Serial.print(",");
  Serial.println(pre_avg);
  
  delay(200);
}
