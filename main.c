#include <stdio.h>
#include <stdlib.h>
double Isd;
double Isq;
double Ia= 0.0;
double Ib= 0.0;
double Ic= 0.0;
float rho = 50.0;
double imr = 0.0001;
float sineFreq = 50.0;
float tau_r = 0.3958;
long int currTime = 0;
float segma_r = 0.0;
float Lm = 0.0;
int Rs = 0;
double tau_s = 0.0;
double segma = 0.0;
int Omega = 0;
int Omega_r = 0;
int Omega_ref=0;
double V_dc = 0.0;
long int prevTime1= 0;
long int prevTime2= 0;
long int prevTime3= 0;
long int prevTime4= 0;
long int prevTime5= 0;


int main()
{

//protyping the functions
void dqTransformation(double Ia,double Ib,double Ic, double rho);
void fluxObserver();
double getIsdref(double imr_ref, double Kp_lamda, double Ki_lamda);
double getIsqref(int Omega_r, int Omega_ref, double Kp_q, double Ki_q);
double getmd(double Kp_md, double Ki_md, double Isdref);
double getmq(double Kp_mq, double Ki_mq, double Isqref);



void setup() {
  // put your setup code here, to run once:
   pinMode(A0, INPUT);
   pinMode(A1, INPUT);
   pinMode(A2, INPUT);
   pinMode(A3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Ia=analogRead(A0);
  Ib=analogRead(A1);
  Ic=analogRead(A2);
  Omega_r = analogRead(A3);
  dqTransformation(Ia, Ib, Ic,rho);
  currTime = micros();
  fluxObserver();
  prevTime1= currTime;
  currTime = micros();
  double Isdref = getIsdref(50.2, 1.2, 1.2);
  prevTime2= currTime;
  currTime= micros();
  double Isqref = getIsqref(Omega_r, Omega_ref,1.2, 1.2);
  prevTime3= currTime;
  currTime = micros();
  double md = getmd(1.2, 1.2, Isdref);
  prevTime4 = currTime;
  currTime = micros();
  double mq = getmq(1.2, 1.2, Isqref);
  prevTime5 = currTime;
}







//ABC to dq transformation function
void dqTransformation (double Ia,double Ib,double Ic, double rho){
  //alpha beta frame

  double Ialpha=(2/3)*(cos(0)*Ia+cos(2*3.14159265/3)*Ib+cos(4*3.14159265/3)*Ic);
  double Ibeta=(2/3)*(sin(0)*Ia+sin(2*3.14159265/3)*Ib+sin(4*3.14159265/3)*Ic);

  //dq frame
    Isd= Ialpha*sin(rho)+Ibeta*cos(rho);
    Isq= Ialpha*sin(-rho)+Ibeta*cos(rho);
}

//Flux observer function
void fluxObserver(){

  double error= (Isd-imr)/tau_r;
  imr += error * (currTime-prevTime1);
  // saturation
   if (imr < 0.0){
      imr = 0.0;
      }
   if (imr > 25.0){
      imr = 25.0;
   }

  Omega = Omega_r + Isd/(imr * tau_r);
  rho += Omega*(currTime-prevTime1);
}

//Getting I_dsref function
double getIsdref(double imr_ref, double Kp_lamda, double Ki_lamda){

  double error = imr_ref-imr;
  double accError = accError+ error * (currTime-prevTime2);
  double Isdref = Kp_lamda*error + Ki_lamda*accError;
  return Isdref;
}

//Getting I_qref function
double getIsqref(int Omega_r, int Omega_ref, double Kp_q, double Ki_q){
  double error = Omega_ref - Omega_r;
  double accError =  accError + error *(currTime-prevTime3);
  double Isqref = Kp_q*error + Ki_q* accError;
  return Isqref;
}

//Getting md
double getmd(double Kp_md, double Ki_md, double Isdref){
  double error = Isdref - Isd;
  double accError = accError +  error * (currTime-prevTime4);
  double ud = Kp_md*error + Ki_md*accError;
  double md = (Rs* (ud - Isq*segma*tau_s*Omega))/V_dc;
  return md;
  }

//Getting mq
double getmq(double Kp_mq, double Ki_mq, double Isqref){
  double error = Isqref - Isq;
  double accError = accError +  error * (currTime-prevTime5);
  double uq = Kp_mq*error + Ki_mq*accError;
  double mq = (Rs* (uq + Isd*segma*tau_s*Omega + (imr*Omega)*(1- segma)*tau_s))/V_dc;
  return mq;
  }
}
