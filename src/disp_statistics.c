#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "iio.h"


int main (int argc, char **argv)
{
   /* ppatameter parsing - parameters*/
   if(argc<4) 
   {
      fprintf (stderr, "too few parameters\n");
      fprintf (stderr, "   usage: %s DISP GT [precision=1 [DIFF]]\n\n",argv[0] );
      fprintf (stderr, "Computes statistics about the disparity map(DISP), given the ground truth (GT) and a precision.");
      fprintf (stderr, "\n");
      fprintf (stderr, "Writes the statistics in the standard output and optionally generates a difference image (DIFF).\n");
      fprintf (stderr, "The statistics are:  precision(nondense), precision(dense), RMSE(nondense), L1(nondense)\n");
      fprintf (stderr, "\n");
      fprintf (stderr, "GT may contain NaN and INFS, these pixels will not be considered in the statistics.\n");
      fprintf (stderr, "DISP may contain NaNs, for indicating that the algorithm was unable to compute a disparity (ie. non-dense).\n");
      fprintf (stderr, "these pixels will not be considered in the nondense statistics.\n");
      fprintf (stderr, "\n");
      fprintf (stderr, "In case the inputs are multi-channel only the first channel is used.\n");
      return 1;
   }

   int nc,nr,nch,nc2,nr2,nch2;
   int N;
   float *disp,*gt,*out_diff;
   float precision = 1;

   disp  = iio_read_image_float_split(argv[1], &nc, &nr, &nch);
   gt = iio_read_image_float_split(argv[2], &nc2, &nr2, &nch2);
   if (nc!=nc2 || nr!=nr2) abort(); 
   if(argc>=4) precision = atof (argv[3]);

   N = nc*nr;

   // allocate false positive/negative masks
   out_diff = malloc(N*sizeof*out_diff);
   for(int i=0;i<N;i++) out_diff[i]=0;

   // only use the first channel for computing the statistics
   float M_nondense=0, M=0, correct=0, l1=0, l2=0;
   for(int i=0;i<N;i++)
   {
      float d = disp[i];
      float g = gt[i];

      // Do not consider the point if it is NaN or infinity in the GT 
      // NaN in GT indicate that there is no data there to compare with.
      // Inf in GT usually is used to signal occlusion, and we are not interested in occlusions.
      //
      // Nan in the disparity map indicate that the algorithm was unable to compute a disparity
      // almost all the statistics are computed without considering these points.
      // The "correct" statistic also evaluates the completeness so the nan count as incorrect if the area is not occluded.
      if( !isnan(g) )
      {
         float diff = fabs(d-g);
         if (isinf(g) && (isnan(d) || isinf(d))) diff=0;   // case of correctly detected occlusion

         M++;
         if(!isnan(d)) {
            M_nondense++;
            if(diff <= precision) correct++;
            l2 += diff*diff;
            l1 += diff;

            out_diff[i] = g-d;
         } 

      }

   }
   // compute statistics
   float ratio_correct_nondense = correct/M_nondense;
   float ratio_correct_dense    = correct/M;
   float RMSE                   = sqrt((float)l2 /M_nondense);
   float L1                     = (float)l1 /M_nondense;

   printf("%f \t %f \t %f \t %f\n", ratio_correct_nondense, ratio_correct_dense, RMSE, L1);
   printf("#precision\t precision complete\t RMSE\t L1\n");

   // save the FP and FN masks
   if(argc>=5) iio_save_image_float_split(argv[4],out_diff,nc,nr,1);

   free(gt);
   free(disp);
   free(out_diff);

   return 0;
}



