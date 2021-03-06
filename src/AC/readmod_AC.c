/*------------------------------------------------------------------------
 *  Read acoustic model properties (vp,density) from files  
 *
 *  D. Koehn
 *  Kiel, 10.06.2017
 *  ----------------------------------------------------------------------*/


#include "fd.h"

void readmod_AC(float  **  rho, float **  pi){

	extern int NX, NY, NXG, NYG,  POS[3], MYID, INVMAT1;
	extern char  MFILE[STRING_SIZE];	
	extern FILE *FP;

		
	/* local variables */
	float rhov, piv, vp;
	int i, j, ii, jj;
	FILE *fp_vp, *fp_rho;
	char filename[STRING_SIZE];


	fprintf(FP,"\n...reading model information from model-files...\n");
           
	/* read density and seismic velocities */
	/* ----------------------------------- */
	if(INVMAT1==1){ 

	   fprintf(FP,"\t Vp:\n\t %s.vp\n\n",MFILE);
	   sprintf(filename,"%s.vp",MFILE);
	   fp_vp=fopen(filename,"r");
	   if (fp_vp==NULL) errf(" Could not open model file '%s' for Vp ! ", filename);

	   fprintf(FP,"\t Density:\n\t %s.rho\n\n",MFILE);
	   sprintf(filename,"%s.rho",MFILE);
	   fp_rho=fopen(filename,"r");
	   if (fp_rho==NULL) errf(" Could not open model file '%s' for densities ! ", filename);
        }
	   
	/* read density and Lame parameters */
	/* ----------------------------------- */
	if(INVMAT1==3){ 
	   fprintf(FP,"\t Lame parameter lambda:\n\t %s.lam\n\n",MFILE);
	   sprintf(filename,"%s.lam",MFILE);
	   fp_vp=fopen(filename,"r");
	   if (fp_vp==NULL) errf(" Could not open model file '%s' for Lame parameter lambda ! ", filename);

	   fprintf(FP,"\t Density:\n\t %s.rho\n\n",MFILE);
	   sprintf(filename,"%s.rho",MFILE);
	   fp_rho=fopen(filename,"r");
	   if (fp_rho==NULL) errf(" Could not open model file '%s' for densities ! ", filename);
        }
	   

	/* loop over global grid */
		for (i=1;i<=NXG;i++){
			for (j=1;j<=NYG;j++){
			fread(&vp, sizeof(float), 1, fp_vp);
			fread(&rhov, sizeof(float), 1, fp_rho);
				
			/* only the PE which belongs to the current global gridpoint 
			is saving model parameters in his local arrays */
				if ((POS[1]==((i-1)/NX)) && 
				    (POS[2]==((j-1)/NY))){
					ii=i-POS[1]*NX;
					jj=j-POS[2]*NY;
                                
                                rho[jj][ii]=rhov;
                                pi[jj][ii]=vp;
				
				}
			}
		}
	

	fclose(fp_vp);
	fclose(fp_rho);
		
	/* each PE writes his model to disk */
	sprintf(filename,"%s.fdveps.pi",MFILE);
	writemod(filename,pi,3);
	MPI_Barrier(MPI_COMM_WORLD);

	if (MYID==0) mergemod(filename,3);	
	
	sprintf(filename,"%s.fdveps.rho",MFILE);
	writemod(filename,rho,3);
	MPI_Barrier(MPI_COMM_WORLD);
	                        
	if (MYID==0) mergemod(filename,3);

}




