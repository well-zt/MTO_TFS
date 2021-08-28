Info<< "Initialization\n" << endl;
std::remove("Voluse.txt");
std::remove("Time.txt");
std::remove("MeanT.txt"); 
std::remove("PowerDiss.txt"); 
std::remove("Compliance.txt"); 

int i=0,opt=0;
double del=0;
scalar n(0),N(0),eta0,eta5,eta1,y0,y5;
n=x.size();//n:length of x in the i-th core
N=x.size();//N:design freedom
reduce(N, sumOp<scalar>()); 

scalar C(0),V(0),area=gSum(mesh.V()),MeanT(0),PowerDiss(0);
double len=std::pow(area/N, 1.0/3);
b.value()=1.0/(filterR*len/3.464)/(filterR*len/3.464);
//***********************************//Petsc
PetscInt n_dof=n,N_dof=N,m=3;//m:number of constraints
PetscScalar Xmin=0; // Min. value of design variables
PetscScalar Xmax=1; // Max. value of design variables

Vec xmma,xold,xmin,xmax;
PetscScalar *xmma_array;
VecCreate(PETSC_COMM_WORLD,&xmma);
VecSetSizes(xmma,n_dof,N_dof);
VecSetFromOptions(xmma);
VecGetArray(xmma,&xmma_array);
for(i=0;i<n;i++)
{
  xmma_array[i]=x[i];
}
VecRestoreArray(xmma,&xmma_array);

VecDuplicate(xmma,&xold); 
VecDuplicate(xmma,&xmin);
VecDuplicate(xmma,&xmax); 

PetscScalar aMMA[m];
PetscScalar cMMA[m];
PetscScalar dMMA[m];

for (PetscInt i=0;i<m;i++)
{
  aMMA[i]=0.0;
  dMMA[i]=1.0;
  cMMA[i]=1000.0;
}

MMA *mma;
mma=new MMA(N_dof,m,xmma, aMMA, cMMA, dMMA,mma_init,mma_dec,mma_inc);

Vec dfdx; // Sensitivities of objective
VecDuplicate(xmma,&dfdx); 
PetscScalar *dfdx_array;
VecGetArray(dfdx,&dfdx_array);
VecRestoreArray(dfdx,&dfdx_array); 

PetscScalar *gx= new PetscScalar[m]; // Array with constraint values
Vec *dgdx; // Sensitivities of constraints
VecDuplicateVecs(xmma,m, &dgdx); 
PetscScalar **dgdx_array;
VecGetArrays(dgdx,m,&dgdx_array);
VecRestoreArrays(dgdx,m,&dgdx_array);

