#include<math.h>
#include<stm32_vga.h>

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  uint16_t* disp=vga_setup();
    for(int i=0;i<30000;i++){
    	  		disp[i]=0;
    	  		  	}
    HAL_Delay(4000);

// creating 1200 3d points
    float points[3][1200];

    // drawing edges using 100 points per edge

      for(int i=0;i<100;i++){

    	  points[0][i]=i-50;
    	  points[1][i]=50;
    	  points[2][i]=50;

    	  points[0][i+200]=i-50;
    	  points[1][i+200]=-50;
    	  points[2][i+200]=50;
      }
      for(int i=0;i<100;i++){

      	  points[0][i+100]=50;
      	  points[1][i+100]=i-50;
      	  points[2][i+100]=50;

      	  points[0][i+300]=-50;
      	  points[1][i+300]=i-50;
      	  points[2][i+300]=50;
        }

      for(int i=0;i<400;i++){

          points[0][i+400]=points[0][i];
       	  points[1][i+400]=points[1][i];
       	  points[2][i+400]=-50;

          }

      for(int i=0;i<100;i++){

           points[0][i+800]=50;
         	points[1][i+800]=50;
            points[2][i+800]=50-i;


            points[0][i+900]=-50;
         	points[1][i+900]=50;
            points[2][i+900]=50-i;


            points[0][i+1000]=50;
         	points[1][i+1000]=-50;
            points[2][i+1000]=50-i;


            points[0][i+1100]=-50;
         	points[1][i+1100]=-50;
            points[2][i+1100]=50-i;

            }


      float n[3]={0.33333,0.66666,0.66666};// unit vector axis of rotation (keep |n|=1)
      float v[3];
      float w=0.01;// angle rotated per iteration in radians
      int x,y;
      float cosv,sinv;


      cosv=cos(w);
      sinv=sin(w);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  for(int i=0;i<1200;i++){

	  		v[0]=points[0][i];
	  		v[1]=points[1][i];
	  		v[2]=points[2][i];

	  	  points[0][i] = v[0]*cosv
	  		           + (n[1]*v[2] - n[2]*v[1])*sinv
	  		           + n[0]*(n[0]*v[0] + n[1]*v[1] + n[2]*v[2])*(1.0f - cosv);

	  	  points[1][i] = v[1]*cosv
	  		           + (n[2]*v[0] - n[0]*v[2])*sinv
	  		           + n[1]*(n[0]*v[0] + n[1]*v[1] + n[2]*v[2])*(1.0f - cosv);

	  	  points[2][i] = v[2]*cosv
	  		           + (n[0]*v[1] - n[1]*v[0])*sinv
	  		           + n[2]*(n[0]*v[0] + n[1]*v[1] + n[2]*v[2])*(1.0f - cosv);



	  	 }


	  	  for(int i=15;i<35;i++){
	  		  for(int j=120;j<480;j++){
	  		  disp[i+50*j]=0;
	  		  }
	  	  }


	  	  for(int i=0;i<1200;i++){
	  		  x= (int)(points[0][i]*1.5)+400;
	  		  y= (int)(points[1][i]*2)+300;
	  		  disp[(x/16)+(y*50)]|=(1<<(15-(x%16)));
	  	  }

	  	 HAL_Delay(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

