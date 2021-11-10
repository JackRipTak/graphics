//Input:
//  q - generalized coordiantes for the mass-spring system
//  qdot - generalized velocity for the mass spring system
//  dt - the time step in seconds
//  mass - the mass
//  force(q, qdot) - a function that computes the force acting on the mass as a function. This takes q and qdot as parameters.
//Output:
//  q - set q to the updated generalized coordinate using Runge-Kutta time integration
//  qdot - set qdot to the updated generalized velocity using Runge-Kutta time integration

//#include<forward_euler.h>

template<typename FORCE> 
inline void runge_kutta(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double dt, double mass,  FORCE &force) {
/*	double k=100.0;
	Eigen::VectorXd k1=-k/mass*q;
	Eigen::VectorXd k2=
  */  
	Eigen::VectorXd v=qdot;
	Eigen::VectorXd f,tq,tv;
	tq=q,tv=qdot;
	force(f,q,qdot);
	Eigen::VectorXd k1=f/mass,kq1=tv;
	tq=q+kq1*dt/2; tv=v+k1*dt/2;
	force(f,tq,tv);
	Eigen::VectorXd k2=f/mass,kq2=tv;
	tq=q+kq2*dt/2; tv=v+k2*dt/2;
	force(f,tq,tv);
	Eigen::VectorXd k3=f/mass,kq3=tv;
	tq=q+kq2*dt; tv=v+k3*dt;
	force(f,tq,tv);
	Eigen::VectorXd k4=f/mass,kq4=tv;
	qdot=qdot+dt/6*(k1+2*k2+2*k3+k4);
	q=q+dt/6*(kq1+2*kq2+2*kq3+kq4);
	
}	
