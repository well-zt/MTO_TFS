//灵敏度归一化
fsenshMeanT=fsenshMeanT/gMax(mag(fsenshMeanT.primitiveField()));
gsenshPowerDiss=gsenshPowerDiss/gMax(mag(gsenshPowerDiss.primitiveField()));
gsenshCompliance=gsenshCompliance/gMax(mag(gsenshCompliance.primitiveField()));
gsenshVol.primitiveFieldRef()=mesh.V()/gMax(mesh.V());

//不可设计域的灵敏度置零
if(solid_area)
{
   forAll(cells_solid, i)
   {
      fsenshMeanT[cells_solid[i]]=0;
      gsenshPowerDiss[cells_solid[i]]=0;
      gsenshCompliance[cells_solid[i]]=0;
      gsenshVol[cells_solid[i]]=0;
   }
}
if(fluid_area)
{
   forAll(cells_fluid, i)
   {
      fsenshMeanT[cells_fluid[i]]=0;
      gsenshPowerDiss[cells_fluid[i]]=0;
      gsenshCompliance[cells_fluid[i]]=0;
      gsenshVol[cells_fluid[i]]=0;
   }
}

//Heaviside过滤的链式求导
if(opt>1)
{
  for(i=0;i<n;i++)
  {
     if(xp[i]<=eta5)
     {
       drho[i]=del*Foam::exp(-del*(1-xp[i]/eta5))+Foam::exp(-del); 
     }
     else
     {
       drho[i]=del*Foam::exp(-del*(xp[i]-eta5)/(1-eta5))+Foam::exp(-del); 
     } 
  }
  fsenshMeanT=fsenshMeanT*drho;
  gsenshPowerDiss=gsenshPowerDiss*drho;
  gsenshCompliance=gsenshCompliance*drho;
  gsenshVol=gsenshVol*drho;
}

//PDE 过滤的链式求导
fvScalarMatrix Eqn1(fvm::laplacian(fsensMeanT)-fvm::Sp(b,fsensMeanT)+fsenshMeanT*b);
Eqn1.solve();

fvScalarMatrix Eqn2(fvm::laplacian(gsensPowerDiss)-fvm::Sp(b,gsensPowerDiss)+gsenshPowerDiss*b);
Eqn2.solve();

fvScalarMatrix Eqn3(fvm::laplacian(gsensVol)-fvm::Sp(b,gsensVol)+gsenshVol*b);
Eqn3.solve();

fvScalarMatrix Eqn5(fvm::laplacian(gsensCompliance)-fvm::Sp(b,gsensCompliance)+gsenshCompliance*b);
Eqn5.solve();
