// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>

#define _USE_MATH_DEFINES // for C
#include <math.h>

#define _USE_MATH_DEFINES // for C++
#include <cmath>

#define M_SQRT_PI sqrt(M_PI)

using namespace std;


float gbm(float sp, float mu, int delta_t, float sigma, float epsilon)
{
	return (float) (sp + (sp * mu * delta_t) + (sp * sigma * epsilon * sqrt(delta_t)));
}

float erfinv(float z)
{
	// half of the sqaure root of pi
	float half_sqrt_pi = 0.88622692545;

	// pi/12
	float co_pi_1 = 0.26179938779;

	// 7pi^2 / 480
	float co_pi_2 = 0.14393173084;

	// 127pi^3 / 40320
	float co_pi_3 = 0.0976636195;

	// 4369pi^4 / 5806080
	float co_pi_4 = 0.07329907936;

	// 34807pi^5 / 182476800
	float co_pi_5 = 0.05837250087;

	return half_sqrt_pi * (

		// only really need up to four to be precise,
		// but it will mess up the precision of further functions (e.g., probit)
		z + 
		(co_pi_1 * pow(z, 3)) + 
		(co_pi_2 * pow(z, 5)) +
		(co_pi_3 * pow(z, 7)) +
		(co_pi_4 * pow(z, 9)) +
		(co_pi_5 * pow(z, 11))
	);
}

float probit(float p)
{
	return M_SQRT2 * erfinv(2 * p - 1);
}

void table_header(int width)
{
	std::cout << std::setw(width) << "t |";
	std::cout << std::setw(width) << "s, t+1 |";
	std::cout << std::setw(width) << "s, t |";
	std::cout << std::setw(width) << "mu |";
	std::cout << std::setw(width) << "delta t |";
	std::cout << std::setw(width) << "sigma |";
	std::cout << std::setw(width) << "epsilon, t |";
	std::cout << std::setw(width) << "sqrt of dt";
	std::cout << std::endl;
}

void table_row(int width, int i, float s, float sp, float mu, int t, float sigma, float epsilon)
{
	std::cout << std::setw(width) << i << " |";
	std::cout << std::setw(width) << s << " |";
	std::cout << std::setw(width) << sp << " |";
	std::cout << std::setw(width) << mu << " |";
	std::cout << std::setw(width) << t << " |";
	std::cout << std::setw(width) << sigma << " |";
	std::cout << std::setw(width) << epsilon << " |";
	std::cout << std::setw(width) << sqrt(t);
	std::cout << std::endl;
}

int main()
{
	int cmd = 0;

	// these variables have to be set as outside of scope,
	// so they are not reset everytime the loop starts
	// changeable by user
	float s0 = 100;
	float mu = 0;
	int t = 10;
	int delta_t = 1;
	float sigma = 0;
	float epsilon = 0;

	std::cout << "Input 0 to see help." << std::endl;

	while (true)
	{
		// these need to be recreated everytime
		// not changeable by user
		float *s;
		float sp = 0;

		int subcmd = 0;

		switch(cmd)
		{
		case 3:
			std::cout << "query:";
			scanf_s(" %d", &subcmd);

			switch (subcmd)
			{
			case 0:
				cmd = 0;
				break;
			case 1:
				std::cout << "t=" << t << std::endl;
				std::cout << "mu=" << mu << std::endl;
				std::cout << "sigma=" << sigma << std::endl;
				std::cout << "s0=" << s0 << std::endl;
				break;
			case 2:
				std::cout << "t=" << t << std::endl;
				break;
			case 3:
				std::cout << "mu=" << mu << std::endl;
				break;
			case 4:
				std::cout << "sigma=" << sigma << std::endl;
				break;
			case 5:
				std::cout << "s0=" << s0 << std::endl;
				break;
			}

			continue;
		case 4:
			std::cout << "math: ";
			scanf_s(" %d", &subcmd);

			switch (subcmd)
			{
			case 0:
				cmd = 0;
				break;
				// this is the block for setting length
			case 2:
				std::cout << "t=";
				scanf_s("%d", &t);
				std::cout << "t=" << t << std::endl;
				break;

				// this is the block for mu (mean)
			case 3:
				std::cout << "mu=";
				scanf_s("%f", &mu);
				std::cout << "mu=" << mu << std::endl;
				break;

				// this is the block for sigma (volatility)
			case 4:
				std::cout << "sigma=";
				scanf_s("%f", &sigma);
				std::cout << "sigma=" << sigma << std::endl;
				break;

				// this is the block for s[0]
			case 5:
				std::cout << "s0=";
				scanf_s("%f", &s0);
				std::cout << "s[0]=" << s0 << std::endl;
				break;
			}

			continue;
		}

		std::cout << "cmd:";
		scanf_s(" %d", &cmd);

		/*
		 * this should be constant among all programs
		 * 0 = help
		 * 1 = run program
		 * 2 = query variable
		 */
		switch (cmd)
		{

		// this is the block for help
		case 0:
			std::cout << "Commands for cmd:" << std::endl;
			std::cout << "0=help" << std::endl;
			std::cout << "1=run" << std::endl;
			std::cout << "2=clear" << std::endl;
			std::cout << "3=query variable" << std::endl;
			std::cout << "4=change variable" << std::endl << std::endl;

			std::cout << "Commands for query:" << std::endl;
			std::cout << "0=exit" << std::endl;
			std::cout << "1=all" << std::endl;
			std::cout << "2=t" << std::endl;
			std::cout << "3=mu" << std::endl;
			std::cout << "4=sigma" << std::endl;
			std::cout << "5=0 of s (s0)" << std::endl << std::endl;

			std::cout << "Commands for math:" << std::endl;
			std::cout << "0=exit" << std::endl;
			std::cout << "2+: see commands for query" << std::endl;
			break;

		// this is the block for running the program
		case 1:

			// allocate memory for the set
			s = new float[t];

			// print the tabl headers
			table_header(10);
			
			// make sure the precision is set to six
			std::setprecision(6);

			// print the first element of s
			s[0] = s0;
			table_row(10, 0, s[0], sp, mu, delta_t, sigma, epsilon);

			// do the rest of the elements within s
			for (int i = 1; i < t; i++)
			{
				sp = s[i - 1];
				epsilon = probit((float)rand() / (float)((unsigned)RAND_MAX + 1));
				s[i] = gbm(sp, mu, delta_t, sigma, epsilon);

				table_row(10, i, s[i], sp, mu, delta_t, sigma, epsilon);
			}

			delete[] s;
			
			break;

		// this block is to clear the screen
		case 2:

			system("cls");

			break;
		}
	}

    return 0;
}

