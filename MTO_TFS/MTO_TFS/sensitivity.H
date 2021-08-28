
Info<<"sensitivity analysis"<<endl;
//计算灵敏度
fsenshMeanT=-alphaMax*(1+qu)*qu/((qu+xh)*(qu+xh))*(U&Ub)+b1*(kf-ks)*(1+qu)*qu/((qu+xh)*(qu+xh))*(fvc::grad(T)&fvc::grad(Tb))/rhoc;
gsenshPowerDiss=-alphaMax*(1+qu)*qu/((qu+xh)*(qu+xh))*(U&Ua);
mu=-(1+qd)*qd/((qd+xh)*(qd+xh))*(E-Emin)/(2.0*(1.0+Po));
lambda=-(1+qd)*qd/((qd+xh)*(qd+xh))*Po*(E-Emin)/((1.0+Po)*(1.0-2.0*Po));
gsenshCompliance=-gradD && (rho*(mu*twoSymm(gradD)+ (lambda*I)*tr(gradD)));

//链式求导计算对物理密度的灵敏度
#include "filter_sens.H"

//往MMA中传入数据
gx[0]=(PowerDiss/Foam::max(PowerDissRelax-opt*0.2,PowerDissMax)-1.0);
gx[1]=(C/Foam::max(CRelax-opt*0.2,CMax)-1.0);
gx[2]=V;

VecGetArray(dfdx,&dfdx_array);
VecGetArrays(dgdx,m,&dgdx_array);
for(i=0;i<n;i++)
{
 dfdx_array[i]=fsensMeanT[i]/N;//sensitivities of objective function
 dgdx_array[0][i]=gsensPowerDiss[i]/N;//sensitivities of constraint function
 dgdx_array[1][i]=gsensCompliance[i]/N;
 dgdx_array[2][i]=gsensVol[i]/N;
}
VecRestoreArray(dfdx,&dfdx_array);
VecRestoreArrays(dgdx,m,&dgdx_array);

Info<< "\n run MMA \n" << endl;
mma->SetOuterMovelimit(Xmin,Xmax,movlim,xmma,xmin,xmax);
mma->Update(xmma,dfdx,gx,dgdx,xmin,xmax,raa0,mma_dec,mma_inc);

//PDE过滤和Heaviside过滤计算物理密度
#include "filter_x.H"
