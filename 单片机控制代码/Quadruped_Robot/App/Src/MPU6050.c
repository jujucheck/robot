#include "MPU6050.h"
#include "string.h"
#include "math.h"
#include "Inverse_K.h"


/*******USART3��������ز���*******/
extern unsigned char Re_buf[11];
extern unsigned char sign;
float a[3]={0},w[3]={0},angle[3]={0};
//float T;

float acc_offset[3]={0};
float gyro_offset[3]={0};
int Gyro_calibration_times=0;
int Gyro_calibration_times2=0;

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation  ��Ԫ��
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error  
struct floatRPY Q_ANGLE;

extern void LegAngle_to_ServoPos(struct floatXYZ pos,struct floatRPY angle);

/*****���������ݽ��պ���******/
void Gyroscopetest(void)
{
	
   unsigned char Temp[11];
   if(sign)
   {  
      memcpy(Temp,Re_buf,11);
      sign=0;
      if(Re_buf[0]==0x55)       //���֡ͷ
      {  	
				switch(Re_buf[1])
            {
               case 0x51: //���ٶȰ�
                  a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16*9.8;      //X����ٶ�
                  a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16*9.8;      //Y����ٶ�
                  a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16*9.8;      //Z����ٶ�
							 /***
									if(Gyro_calibration_times<50)
							    {
										 acc_offset[0]+=a[0];
										 acc_offset[1]+=a[1];
										 acc_offset[2]+=a[2];
										 Gyro_calibration_times++;
									}
									else
									{	
										 a[0]=a[0]-acc_offset[0]/50;
										 a[1]=a[1]-acc_offset[1]/50;
										 a[2]=9.8+a[2]-acc_offset[2]/50;	
									}		
							 ***/
									// T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //�¶�
                  break;
               case 0x52: //���ٶȰ�
                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X����ٶ�
                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y����ٶ�
                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z����ٶ�
							 /****
							 		if(Gyro_calibration_times2<50)
									{
										gyro_offset[0]+=w[0];
										gyro_offset[1]+=w[1];
										gyro_offset[2]+=w[2];
										Gyro_calibration_times2++;
									}
									else
									{	
										w[0]=w[0]-gyro_offset[0]/50;
										w[1]=w[1]-gyro_offset[1]/50;
										w[2]=w[2]-gyro_offset[2]/50;	
									}
							 ****/
                 // T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //�¶�
                  break;
               case 0x53: //�ǶȰ�
                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X���ת��
                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y�ḩ����
                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z��ƫ����
	
                 // T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //�¶�
                  break;
               default:  break;
            }
						//if(Gyro_calibration_times<50||Gyro_calibration_times2<50)
						//	printf("Gyro_calibration_times=%d\r\n",Gyro_calibration_times);
						//else
							printf("X���ٶ�:%.2f  Y���ٶ�:%.2f  Z���ٶ�:%.2f X���ٶ�:%.2f  Y���ٶ�:%.2f  Z���ٶ�:%.2f   X�Ƕ�:%.2f  Y�Ƕ�:%.2f  Z�Ƕ�:%.2f\r\n",
						 				a[0],a[1],a[2],w[0],w[1],w[2],angle[0],angle[1],angle[2]);
																//���������
      }  			
   } 

  
}

/*******���������ݴ�����******/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float temp[4];
  float norm;
  //float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;
 
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  //float q0q3 = q0*q3;
  float q1q1 = q1*q1;
  //float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
 
  norm = sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
	if(norm==0)  return;
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;
 
  // estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
  vx = 2*(q1q3 - q0q2);                                             //��Ԫ����xyz�ı�ʾ
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
 
  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                                             //�����������õ���֣��������
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;
 
  exInt = exInt + ex * Ki * 2 * halfT;                                //��������
  eyInt = eyInt + ey * Ki * 2 * halfT;
  ezInt = ezInt + ez * Ki * 2 * halfT;
 
  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;                                              //�����PI�󲹳��������ǣ����������Ư��
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;                                          //�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�
 
  // integrate quaternion rate and normalise                           //��Ԫ��΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;
	

 
  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
 
  //Q_ANGLE.Y = atan2(2 * q1 * q2 + 2 * q0 * q3,q0*q0+q1*q1-q2*q2-q3*q3)* 57.3; // yaw
  Q_ANGLE.Y = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1); // yaw
  Q_ANGLE.P = asin(-2 * q1 * q3 + 2 * q0* q2); // pitch
  Q_ANGLE.R = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1); // roll
}

/*****��������غ���*****/
struct floatRPY MUP6050_Data_Process(void)
{
	Gyroscopetest();
	//IMUupdate(w[0],w[1],w[2],a[0],a[1],a[2]);
	//if(Gyro_calibration_times>=50&&Gyro_calibration_times2>=50)
	//	printf("P=%.2f,R=%.2f,Y=%.2f\r\n \r\n",Q_ANGLE.P,Q_ANGLE.R,Q_ANGLE.Y);
	return Q_ANGLE;
}

void IMU_Attitude_Control(void)
{
	float KP=-1;
	struct floatRPY euler_angle;
	struct floatXYZ pos;
	pos.x=0;
	pos.y=0;
	pos.z=150;
	euler_angle=MUP6050_Data_Process();
	
	//euler_angle.P=KP*euler_angle.P;
	//euler_angle.R=KP*euler_angle.R;
	//euler_angle.Y=KP*euler_angle.Y;
	euler_angle.P=KP*angle[1];
	euler_angle.R=KP*angle[0];
	euler_angle.Y=KP*angle[2];
	LegAngle_to_ServoPos(pos,euler_angle);  
	//delay_ms(20);
}
